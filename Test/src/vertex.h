#ifndef VERTEX_H__
#define VERTEX_H__

#include "vector3f.h"

class Vertex
{
    public:
        Vertex();
        Vertex(Vector3f vert, Vector2f tex);
		Vertex(Vertex* v);

		template<class InputIterator>
		static void SetColorAll(InputIterator first, InputIterator last, Vector3f color, float alpha = 0);

    public:
        Vector3f vertex;
        Vector2f tex;
		Vector3f color;
		float alpha;
};


template<class InputIterator>
void Vertex::SetColorAll(InputIterator first, InputIterator last, Vector3f color, float alpha)
{
	while (first != last) {
		Vertex* vert = &*first;
		vert->color = color;
		vert->alpha = alpha;
		++first;
	}
}

#endif
