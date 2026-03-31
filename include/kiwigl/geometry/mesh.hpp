#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../core/constants.hpp"
#include "../geometry/face.hpp"
#include "../geometry/vector.hpp"
#include "../graphics/color.hpp"
#include "../texture/texture.hpp"

namespace kiwigl {

class Mesh {
  public:
    Mesh() = default;
    ~Mesh() = default;

    // Load mesh data from a file.
    void loadMesh(const std::string& filename) {
        loadOBJ(filename);
        // loadTexture(filename);
    }

    // Load geometry and texture coordinates from a Wavefront OBJ file.
    bool loadOBJ(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open mesh file: " << filename << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.rfind("vt ", 0) == 0) {
                Texture2D texture;
                if (sscanf(line.c_str(), "vt %lf %lf", &texture.u, &texture.v) == 2) {
                    addTexture(texture);
                }
            } else if (line.rfind("v ", 0) == 0) {
                Vector3D vertex;
                if (sscanf(line.c_str(), "v %lf %lf %lf", &vertex.x, &vertex.y, &vertex.z) == 3) {
                    addVertex(vertex);
                }
            } else if (line.rfind("f ", 0) == 0) {
                parseFace(line.substr(2));
            }
        }

        file.close();
        centerMesh();
        return true;
    }

    // Load textures from a file.
    bool loadTextures(const std::string& filename);

    // Accessors.
    const std::vector<Vector3D>& getVertices() const { return vertices; }
    const std::vector<Texture2D>& getTextures() const { return textures; }

    // Mutators.
    void addVertex(const Vector3D& vertex) { vertices.push_back(vertex); }
    void addTexture(const Texture2D& texture) { textures.push_back(texture); }

    void addFace(int v1, int v2, int v3,
                 const Texture2D& t1,
                 const Texture2D& t2,
                 const Texture2D& t3,
                 const Color& color) {
        faces.emplace_back(v1, v2, v3, t1, t2, t3, color);
    }

    std::vector<Vector3D> vertices;
    std::vector<Face> faces;
    std::vector<Texture2D> textures;
    Vector3D scale;
    Vector3D rotation;
    Vector3D translation;

  private:
    static size_t getAbsoluteIndex(int index, size_t size) {
        if (index > 0) return static_cast<size_t>(index - 1);
        if (index < 0) return size + index;
        return static_cast<size_t>(-1);
    }

    void parseFace(const std::string& face_data) {
        std::stringstream ss(face_data);
        std::string token;
        std::vector<int> vertex_indices;
        std::vector<int> texture_indices;
        std::vector<int> normal_indices;

        while (ss >> token) {
            int v = 0;
            int t = 0;
            int n = 0;

            if (sscanf(token.c_str(), "%d/%d/%d", &v, &t, &n) == 3) {
                vertex_indices.push_back(v);
                texture_indices.push_back(t);
                normal_indices.push_back(n);
            } else if (sscanf(token.c_str(), "%d//%d", &v, &n) == 2) {
                vertex_indices.push_back(v);
                normal_indices.push_back(n);
            } else if (sscanf(token.c_str(), "%d/%d", &v, &t) == 2) {
                vertex_indices.push_back(v);
                texture_indices.push_back(t);
            } else if (sscanf(token.c_str(), "%d", &v) == 1) {
                vertex_indices.push_back(v);
            }
        }

        if (vertex_indices.size() < 3) {
            return;
        }

        for (size_t i = 1; i < vertex_indices.size() - 1; ++i) {
            Texture2D tex[3]{};

            if (texture_indices.size() == vertex_indices.size()) {
                const size_t t0 = getAbsoluteIndex(texture_indices[0], textures.size());
                const size_t t1 = getAbsoluteIndex(texture_indices[i], textures.size());
                const size_t t2 = getAbsoluteIndex(texture_indices[i + 1], textures.size());

                if (t0 < textures.size() && t1 < textures.size() && t2 < textures.size()) {
                    tex[0] = textures[t0];
                    tex[1] = textures[t1];
                    tex[2] = textures[t2];
                }
            }

            addFace(vertex_indices[0],
                    vertex_indices[i],
                    vertex_indices[i + 1],
                    tex[0],
                    tex[1],
                    tex[2],
                    WHITE);
        }
    }

    void centerMesh() {
        if (vertices.empty()) {
            return;
        }

        Vector3D center(0, 0, 0);
        for (const auto& vertex : vertices) {
            center.x += vertex.x;
            center.y += vertex.y;
            center.z += vertex.z;
        }

        center.x /= vertices.size();
        center.y /= vertices.size();
        center.z /= vertices.size();

        for (auto& vertex : vertices) {
            vertex.x -= center.x;
            vertex.y -= center.y;
            vertex.z -= center.z;
        }
    }
};

} // namespace kiwigl
