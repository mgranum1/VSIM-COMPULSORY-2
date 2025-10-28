#include "Ball.h"
#include <cmath>
#include <QDebug>

// Konstruktør lager en kule-mesh
Ball::Ball(int sectors, int stacks, float radius)
{
    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = M_PI/2 - i * M_PI / stacks;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * 2 * M_PI / sectors;

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            Vertex v;
            v.x = x; v.y = y; v.z = z;
            v.r = 1.0f; v.g = 1.0f; v.b = 1.0f;
            v.u = (float)j / sectors;
            v.v = (float)i / stacks;
            mVertices.push_back(v);
        }
    }

    for (int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                mIndices.push_back(k1);
                mIndices.push_back(k2);
                mIndices.push_back(k1+1);
            }
            if (i != (stacks-1)) {
                mIndices.push_back(k1+1);
                mIndices.push_back(k2);
                mIndices.push_back(k2+1);
            }
        }
    }

    // Startposisjon
    mMatrix.translate(0.5f, 0.5f, 0.5f);
}


// Projiserer tyngdekraft på planet definert av trekanten
QVector3D Ball::projectGravityOnTriangle(const QVector3D &g,
                                         const QVector3D &v0,
                                         const QVector3D &v1,
                                         const QVector3D &v2)
{
    QVector3D n = QVector3D::crossProduct(v1-v0, v2-v0).normalized();
    return g - n * QVector3D::dotProduct(g, n);
}


// Sjekk om et punkt p er inni trekant (barysentriske koordinater)
bool Ball::isInsideTriangle(const QVector3D &p,
                            const QVector3D &v0,
                            const QVector3D &v1,
                            const QVector3D &v2)
{
    QVector3D u = v1 - v0;
    QVector3D v = v2 - v0;
    QVector3D w = p - v0;

    float uu = QVector3D::dotProduct(u,u);
    float uv = QVector3D::dotProduct(u,v);
    float vv = QVector3D::dotProduct(v,v);
    float wu = QVector3D::dotProduct(w,u);
    float wv = QVector3D::dotProduct(w,v);
    float D = uv * uv - uu * vv;

    if (std::fabs(D) < 1e-6f) {
        // Trekanten er negativ, returner false
        return false;
    }

    float s = (uv * wv - vv * wu) / D;
    float t = (uv * wu - uu * wv) / D;

    return (s >= 0 && t >= 0 && (s+t) <= 1);
}


// Fysikk oppdatering
void Ball::update(float dt,
                  const std::vector<Vertex>& vertices,
                  const std::vector<Triangle>& triangles)
{
    if (triangles.empty()) return;

    // Gjeldende trekant
    const Triangle& tri = triangles[mCurrentTriangle];
    QVector3D v0(vertices[tri.v[0]].x, vertices[tri.v[0]].y, vertices[tri.v[0]].z);
    QVector3D v1(vertices[tri.v[1]].x, vertices[tri.v[1]].y, vertices[tri.v[1]].z);
    QVector3D v2(vertices[tri.v[2]].x, vertices[tri.v[2]].y, vertices[tri.v[2]].z);

    // Normal og plan
    QVector3D normal = QVector3D::crossProduct(v1 - v0, v2 - v0).normalized();
    float d = QVector3D::dotProduct(normal, v0);

    //  F = m * g
    QVector3D gravity(0, -9.81f, 0);
    QVector3D gravityForce = gravity * mMass;

    // Projiser kraften på trekantens plan
    QVector3D projectedForce = gravityForce - normal * QVector3D::dotProduct(gravityForce, normal);

    // Newtons andre lov: a = F / m
    mAcceleration = projectedForce / mMass;

    // Integrasjon: v = v + a*dt
    mVelocity += mAcceleration * dt;

    // Friksjon/demping
    mVelocity *= 0.98f;

    // Ny posisjon: p = p + v*dt
    QVector3D pos = QVector3D(mMatrix.column(3));
    pos += mVelocity * dt;

    // Snap posisjon til planet
    float dist = QVector3D::dotProduct(normal, pos) - d;
    pos -= normal * dist;

    // Sjekk om ballen sklir ut av trekanten
    if (!isInsideTriangle(pos, v0, v1, v2)) {
        bool moved = false;
        for (int edge = 0; edge < 3; ++edge) {
            if (tri.neighbors[edge] != -1) {
                mCurrentTriangle = tri.neighbors[edge];

                // Reprojiser hastighet inn i nytt plan
                const Triangle& newTri = triangles[mCurrentTriangle];
                QVector3D nv0(vertices[newTri.v[0]].x, vertices[newTri.v[0]].y, vertices[newTri.v[0]].z);
                QVector3D nv1(vertices[newTri.v[1]].x, vertices[newTri.v[1]].y, vertices[newTri.v[1]].z);
                QVector3D nv2(vertices[newTri.v[2]].x, vertices[newTri.v[2]].y, vertices[newTri.v[2]].z);
                QVector3D nNormal = QVector3D::crossProduct(nv1 - nv0, nv2 - nv0).normalized();

                mVelocity -= nNormal * QVector3D::dotProduct(mVelocity, nNormal);
                moved = true;
                break;
            }
        }
        if (!moved) {
            // Ingen nabo? da stopper ballen helt
            mVelocity = QVector3D(0,0,0);
            mMatrix.setColumn(3, QVector4D(pos, 1.0f));
            return;
        }
    }

    // Oppdater transform
    mMatrix.setColumn(3, QVector4D(pos, 1.0f));
}

