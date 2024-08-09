#include <stdio.h>
#include <vector>

#include "ogldev_math_3d.h"
#include "quad_list.h"
#include "terrain.h"


QuadList::QuadList()
{
}


QuadList::~QuadList()
{
    Destroy();
}

void QuadList::Destroy()
{
    if (m_vao > 0) {
        glDeleteVertexArrays(1, &m_vao);
    }
    if (m_vb > 0) {
        glDeleteBuffers(1, &m_vb);
    }
    if (m_ib > 0) {
        glDeleteBuffers(1, &m_ib);
    }
}

void QuadList::CreateQuadList(int Width, int Depth, const BaseTerrain* pTerrain) {
    m_width = Width;
    m_depth = Depth;
    CreateGLState();
    PopulateBuffers();
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void QuadList::CreateGLState()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vb);
    glBindBuffer(GL_ARRAY_BUFFER, m_vb);
    glGenBuffers(1, &m_ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);

    int POS_LOC = 0;
    int TEX_LOC = 1;
    size_t NumFloats = 0;

    glEnableVertexAttribArray(POS_LOC);
    glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
    NumFloats += 3;

    glEnableVertexAttribArray(TEX_LOC);
    glVertexAttribPointer(TEX_LOC, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
    NumFloats += 2;
}
void QuadList::PopulateBuffers(const BaseTerrain* pTerrain)
{
    std::vector<Vertex> Vertices;
    Vertices.resize(m_width * m_depth);
    InitVertices(pTerrain, Vertices);
    std::vector<unsigned int> Indices;
    int NumQuads = (m_width - 1) * (m_depth - 1);
    Indices.resize(NumQuads * 4);
    InitIndices(Indices);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices[0]) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

void QuadList::Vertex::InitVertex(const BaseTerrain* pTerrain, int Width, int Depth, int x, int z)
{
    float WorldScale = pTerrain->GetWorldScale();
    Pos = Vector3f(x * WorldScale, 0.0f, z * WorldScale);

    float TextureScale = pTerrain->GetTextureScale();
    Tex = Vector2f(TextureScale * (float)x / (float)Width, TextureScale * (float)z / (float)Depth);
}


void QuadList::InitVertices(const BaseTerrain* pTerrain, std::vector<Vertex>& Vertices)
{
    int Index = 0;
    for (int z = 0; z < m_depth; z++) {
        for (int x = 0; x < m_width; x++) {
            assert(Index < Vertices.size());
            Vertices[Index].InitVertex(pTerrain, m_width, m_depth, x, z);
            Index++;
        }
    }
    assert(Index == Vertices.size());
}

void QuadList::InitIndices(std::vector<unsigned int>& Indices)
{
    int Index = 0;

    for (int z = 0; z < m_depth - 1; z++) {
        for (int x = 0; x < m_width - 1; x++) {            // Adds one quad
            assert(Index < Indices.size());
            unsigned int IndexBottomLeft = z * m_width + x;
            Indices[Index++] = IndexBottomLeft;

            assert(Index < Indices.size());
            unsigned int IndexBottomRight = z * m_width + x + 1;
            Indices[Index++] = IndexBottomRight;

            assert(Index < Indices.size());
            unsigned int IndexTopLeft = (z + 1) * m_width + x;
            Indices[Index++] = IndexTopLeft;

            assert(Index < Indices.size());
            unsigned int IndexTopRight = (z + 1) * m_width + x + 1;
            Indices[Index++] = IndexTopRight;
        }
    }

    assert(Index == Indices.size());
}
void QuadList::Render()
{
    glBindVertexArray(m_vao);
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glDrawElements(GL_PATCHES, (m_depth - 1) * (m_width - 1) * 4, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}