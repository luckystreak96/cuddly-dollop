#ifndef VERTEX_H__
#define VERTEX_H__

#include "vector3f.h"

class Vertex
{
    public:
        Vertex();
        Vertex(Vector3f vert, Vector2f tex);
		Vertex(Vertex* v);

    public:
        Vector3f vertex;
        Vector2f tex;
		Vector3f color;
		float alpha;
};

#endif
