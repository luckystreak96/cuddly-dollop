#include "physics_vertex.h"

namespace Physics_Vertex {

	void VertexCollision(std::vector<Drawable*>* list)
	{
		bool collided = true;
		bool collisionConfirmed = false;
		double bestPercentMovement = 1;
		Vector3f bestNormal;
		Vector3f traject = Vector3f();
		Vector3f poc = Vector3f();
		int vertIndex = -1;
		int selectedObj = -1;
		int selectedOtherObj = -1;
		Triangle triangle;
		std::vector<CollidedSurfaces> collidedSurfaces = std::vector<CollidedSurfaces>();

		for (auto a : *list)
			a->SetGrounded(false);

		//Loop as long as collisions keep chaining
		while (collided)
		{
			collided = false;
			bestPercentMovement = 1;
			for (int i = 0; i < list->size(); i++) {
				for (int j = i + 1; j < list->size(); j++)
				{
					if (!list->at(i)->mustCollide || !list->at(j)->mustCollide)
						continue;

					//Do they even intersect?
					bool intersect = Physics::Intersect(list->at(i)->GetMoveBoundingBox(), list->at(j)->GetMoveBoundingBox());
					if (!intersect)
						continue;

					//Lets confirm that and find out where
					list->at(i)->SetTranslatedVertices();
					list->at(j)->SetTranslatedVertices();
					CollisionObject co1 = CollisionObject(list->at(i)->GetTranslatedVertices(), list->at(i)->GetIndices(),
						list->at(i)->Velocity());
					CollisionObject co2 = CollisionObject(list->at(j)->GetTranslatedVertices(), list->at(j)->GetIndices(),
						list->at(j)->Velocity());

					if (co1.velocity == 0 && co2.velocity == 0)
						continue;

					double percentMovement = 1;
					Vector3f normal = Vector3f();
					bool first = false;
					collisionConfirmed = false;

					Physics::FindClosestCollision(co1, co2, &collisionConfirmed, &percentMovement,
						&normal, &first, &triangle, &traject, &vertIndex, &poc);

					if (!collisionConfirmed || percentMovement > bestPercentMovement) { continue; }
					else
					{
						collided = true;
						bestPercentMovement = percentMovement;
						bestNormal = normal;
						selectedObj = first ? i : j;
						selectedOtherObj = first ? j : i;
					}
				}
			}

			//Shortest has been determined
			if (collided)
			{
				bool secondCollision = false;
				CollidedSurfaces surface(list->at(selectedObj), list->at(selectedOtherObj), triangle);
				if (std::find(collidedSurfaces.begin(), collidedSurfaces.end(), surface) != collidedSurfaces.end())
					secondCollision = true;

				collidedSurfaces.push_back(surface);
				list->at(selectedObj)->CollisionMovement(bestNormal, bestPercentMovement, secondCollision, traject, vertIndex, poc);
				if (secondCollision)
					break;
			}
		}
		collidedSurfaces.empty();
	}

}
