
#ifndef GEOMIP_GRID_H
#define GEOMIP_GRID_H

#include <GL/glew.h>
#include <vector>

#include "ogldev_math_3d.h"

class BaseTerrain;

class GeomipGrid {
public:
    GeomipGrid();

    ~GeomipGrid();

    void CreateGeomipGrid(int Width, int Depth, int PatchSize, const BaseTerrain* pTerrain);

    void Destroy();

    void Render();

private:

    struct Vertex {
        Vector3f Pos;
        Vector2f Tex;
        Vector3f Normal = Vector3f(0.0f, 0.0f, 0.0f);

        void InitVertex(const BaseTerrain* pTerrain, int x, int z);
    };

    void CreateGLState();

    void PopulateBuffers(const BaseTerrain* pTerrain);
    void InitVertices(const BaseTerrain* pTerrain, std::vector<Vertex>& Vertices);
    void InitIndices(std::vector<uint>& Indices);
    void CalcNormals(std::vector<Vertex>& Vertices, std::vector<uint>& Indices);

    uint AddTriangle(uint Index, std::vector<uint>& Indices, uint v1, uint v2, uint v3);

    int m_width = 0;
    int m_depth = 0;
    int m_patchSize = 0;
    GLuint m_vao = 0;
    GLuint m_vb = 0;
    GLuint m_ib = 0;
};

#endif
