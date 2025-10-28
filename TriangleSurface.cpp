#include "TriangleSurface.h"
#include <fstream>
#include <QDebug>
TriangleSurface::TriangleSurface() : VisualObject()
{

    float s = 3; //Skalering av objektet

    // Fire hjørner + midtpunkt
    Vertex v1{ 0.0f * s,  .6f * s,  0.0f * s,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f }; // bunn venstre
    Vertex v2{ 1.0f * s,  0.0f * s,  0.0f * s,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f }; // bunn høyre
    Vertex v3{ 1.0f * s,  0.0f * s,  1.0f * s,   0.0f, 0.0f, 1.0f,  1.0f, 1.0f }; // top høyre
    Vertex v4{ 0.0f * s,  0.0f * s,  1.0f * s,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f }; // top venstre
    Vertex v5{ 0.5f * s,  0.0f * s,  0.5f * s,   1.0f, 0.0f, 1.0f,  0.5f, 0.5f }; // senter




    mVertices.push_back(v1); // 0
    mVertices.push_back(v2); // 1
    mVertices.push_back(v3); // 2
    mVertices.push_back(v4); // 3
    mVertices.push_back(v5); // 4




    // Indekser for fire trekanter
    mIndices = {
        0,1,4, // T1
        1,2,4, // T2
        2,3,4, // T3
        3,0,4  // T4
    };

    // Lagre trekantene til fysikk
    mTriangles.resize(4);

    // T1
    mTriangles[0].v[0] = 0; mTriangles[0].v[1] = 1; mTriangles[0].v[2] = 4;
    mTriangles[0].neighbors[0] = 1; // deler kant (1,4) med T2
    mTriangles[0].neighbors[1] = 3; // deler kant (0,4) med T4
    mTriangles[0].neighbors[2] = -1; // ytre kant (0,1)

    // T2
    mTriangles[1].v[0] = 1; mTriangles[1].v[1] = 2; mTriangles[1].v[2] = 4;
    mTriangles[1].neighbors[0] = 0; // nabo T1
    mTriangles[1].neighbors[1] = 2; // nabo T3
    mTriangles[1].neighbors[2] = -1; // ytre kant (1,2)

    // T3
    mTriangles[2].v[0] = 2; mTriangles[2].v[1] = 3; mTriangles[2].v[2] = 4;
    mTriangles[2].neighbors[0] = 1; // nabo T2
    mTriangles[2].neighbors[1] = 3; // nabo T4
    mTriangles[2].neighbors[2] = -1; // ytre kant (2,3)

    // T4
    mTriangles[3].v[0] = 3; mTriangles[3].v[1] = 0; mTriangles[3].v[2] = 4;
    mTriangles[3].neighbors[0] = 2; // nabo T3
    mTriangles[3].neighbors[1] = 0; // nabo T1
    mTriangles[3].neighbors[2] = -1; // ytre kant (3,0)

    drawType = 0;

}


