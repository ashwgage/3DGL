#pragma once

#include "../graphics/color.hpp"
#include "../texture/texture.hpp"

namespace kiwigl {

/// Represents a triangular face of a 3D mesh.
class Face {
  public:
    /// Construct a face from vertex indices, texture coordinates, and color.
    Face(int v1, int v2, int v3,
         const Texture2D& t1,
         const Texture2D& t2,
         const Texture2D& t3,
         const Color& color)
        : vertexIndices{v1, v2, v3},
          textures{t1, t2, t3},
          color(color) {}

    ~Face() = default;

    /// Indices of the vertices forming this face.
    int vertexIndices[3];

    /// Texture coordinates for each vertex.
    Texture2D textures[3];

    /// Face color.
    Color color;
};

} // namespace kiwigl
