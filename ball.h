#ifndef BALL_H
#define BALL_H
#include "VisualObject.h"
#include <vector>


class Ball : public VisualObject
{
public:
    Ball(int sectors = 16, int stacks = 16, float radius = 0.1f);
    ~Ball() {}

    void update(float dt,
                const std::vector<Vertex>& vertices,
                const std::vector<Triangle>& triangles);

    void setVelocity(const QVector3D &v) { mVelocity = v; }
    QVector3D getPosition() const { return QVector3D(mMatrix.column(3)); }

    void setCurrentTriangle(int t) { mCurrentTriangle = t; }

private:
    QVector3D mVelocity{0,0,0};
    QVector3D mAcceleration{0,0,0};
    float mMass{1.f};
    int mCurrentTriangle{0};

    QVector3D projectGravityOnTriangle(const QVector3D &g,
                                       const QVector3D &v0,
                                       const QVector3D &v1,
                                       const QVector3D &v2);
    bool isInsideTriangle(const QVector3D &p,
                          const QVector3D &v0,
                          const QVector3D &v1,
                          const QVector3D &v2);
};

#endif // BALL_H
