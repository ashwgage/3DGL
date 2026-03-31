#pragma once

#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include "../core/constants.hpp"
#include "../geometry/face.hpp"
#include "../geometry/vector.hpp"

namespace kiwigl {

__device__ __host__ inline void cudaRotate(Vector3D* vertex, double roll, double pitch, double yaw) {
    // Roll (rotation around X-axis)
    const double cos_roll = cos(roll);
    const double sin_roll = sin(roll);
    double y = vertex->y;
    double z = vertex->z;
    vertex->y = y * cos_roll - z * sin_roll;
    vertex->z = z * cos_roll + y * sin_roll;

    // Pitch (rotation around Y-axis)
    const double cos_pitch = cos(pitch);
    const double sin_pitch = sin(pitch);
    double x = vertex->x;
    z = vertex->z;
    vertex->x = x * cos_pitch + z * sin_pitch;
    vertex->z = z * cos_pitch - x * sin_pitch;

    // Yaw (rotation around Z-axis)
    const double cos_yaw = cos(yaw);
    const double sin_yaw = sin(yaw);
    x = vertex->x;
    y = vertex->y;
    vertex->x = x * cos_yaw - y * sin_yaw;
    vertex->y = y * cos_yaw + x * sin_yaw;
}

__device__ __host__ inline void cudaTranslate3D(Vector3D* vertex, double x, double y, double z) {
    vertex->x += x;
    vertex->y += y;
    vertex->z += z;
}

__device__ __host__ inline void cudaTranslate2D(Vector2D* vertex, double x, double y) {
    vertex->x += x;
    vertex->y += y;
}

__device__ __host__ inline void cudaScale(Vector3D* vertex, double x, double y, double z) {
    vertex->x *= x;
    vertex->y *= y;
    vertex->z *= z;
}

__device__ __host__ inline void cudaProject(const Vector3D* vertex, Vector2D* projected_vertex) {
    projected_vertex->x = (vertex->x * FOV) / vertex->z;
    projected_vertex->y = (vertex->y * FOV) / vertex->z;
}

__global__ void transformVerticesKernel(Face* faces,
                                        Vector3D* vertices,
                                        Triangle* projected_triangles,
                                        Vector3D rotation,
                                        Vector3D camera,
                                        int width,
                                        int height,
                                        int num_faces) {
    const int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= num_faces) {
        return;
    }

    // Process one face per thread.
    const Face face = faces[idx];
    for (int j = 0; j < 3; ++j) {
        Vector3D vertex = vertices[face.vertexIndices[j] - 1];

        cudaRotate(&vertex, rotation.x, rotation.y, rotation.z);
        cudaTranslate3D(&vertex, camera.x, camera.y, -camera.z);
        cudaScale(&vertex, 1.01, 1.01, 1.01);
        cudaProject(&vertex, &projected_triangles[idx].points[j]);
        cudaTranslate2D(&projected_triangles[idx].points[j], width / 2, height / 2);
    }
}

void Display::InitalizeCuda() {
    cudaError_t err = cudaMalloc((void**)&d_faces, mesh.faces.size() * sizeof(Face));
    if (err != cudaSuccess) {
        fprintf(stderr, "%s in %s at line %d\n", cudaGetErrorString(err), __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    err = cudaMalloc((void**)&d_vertices, mesh.vertices.size() * sizeof(Vector3D));
    if (err != cudaSuccess) {
        fprintf(stderr, "%s in %s at line %d\n", cudaGetErrorString(err), __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    err = cudaMalloc((void**)&d_projectedTriangles, mesh.faces.size() * sizeof(Triangle));
    if (err != cudaSuccess) {
        fprintf(stderr, "%s in %s at line %d\n", cudaGetErrorString(err), __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}

void Display::FreeCuda() {
    if (d_faces != nullptr) {
        cudaFree(d_faces);
        d_faces = nullptr;
    }

    if (d_vertices != nullptr) {
        cudaFree(d_vertices);
        d_vertices = nullptr;
    }

    if (d_projectedTriangles != nullptr) {
        cudaFree(d_projectedTriangles);
        d_projectedTriangles = nullptr;
    }
}

void Display::LaunchCuda(int width, int height) {
    cudaMemcpy(d_faces, mesh.faces.data(), mesh.faces.size() * sizeof(Face), cudaMemcpyHostToDevice);
    cudaMemcpy(d_vertices, mesh.vertices.data(), mesh.vertices.size() * sizeof(Vector3D), cudaMemcpyHostToDevice);

    const dim3 threads_per_block(256, 1, 1);
    const dim3 blocks_per_grid((mesh.faces.size() + threads_per_block.x - 1) / threads_per_block.x, 1, 1);

    transformVerticesKernel<<<blocks_per_grid, threads_per_block>>>(
        d_faces,
        d_vertices,
        d_projectedTriangles,
        rotation,
        camera,
        width,
        height,
        mesh.faces.size()
    );

    cudaDeviceSynchronize();

    cudaMemcpy(projectedTriangles.data(),
               d_projectedTriangles,
               mesh.faces.size() * sizeof(Triangle),
               cudaMemcpyDeviceToHost);

    const cudaError_t cuda_status = cudaGetLastError();
    if (cuda_status != cudaSuccess) {
        fprintf(stderr, "%s in %s at line %d\n", cudaGetErrorString(cuda_status), __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}

} // namespace kiwigl
