#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include <string>
#include "VisualObject.h"


//Defaults to a quad, but can read a mesh from file
class TriangleSurface : public VisualObject
{
public:
    TriangleSurface();

      std::vector<Triangle> getTriangles() const { return mTriangles; }

private:
     std::vector<Triangle> mTriangles;


};

#endif // TRIANGLESURFACE_H
