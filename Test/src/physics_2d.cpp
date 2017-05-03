#include "physics_vertex.h"

namespace Physics_2D {

	void Collision(std::vector<Drawable*>* list)
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
					//Are they at least moving
					if (list->at(j)->Velocity() == 0 && list->at(i)->Velocity() == 0)
						continue;

					//Are they at the same z
					if (list->at(j)->Position().z != list->at(i)->Position().z)
						continue;

					//Can both objects even collide
					if (!list->at(i)->mustCollide || !list->at(j)->mustCollide)
						continue;

					//Is there any point in time in the frame where they could POSSIBLY have collided
					if (Physics::Intersect(list->at(i)->GetMoveBoundingBox(), list->at(j)->GetMoveBoundingBox()))
						continue;

					//Lets make sure they did and find out where...
					list->at(i)->SetTranslatedVertices();
					list->at(j)->SetTranslatedVertices();
					CollisionObject co1 = CollisionObject(list->at(i)->GetTranslatedVertices(), list->at(i)->GetIndices(),
						list->at(i)->Velocity());
					CollisionObject co2 = CollisionObject(list->at(j)->GetTranslatedVertices(), list->at(j)->GetIndices(),
						list->at(j)->Velocity());


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
