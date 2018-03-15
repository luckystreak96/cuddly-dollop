#include "physics_2d.h"

namespace Physics_2D {

	bool IsLegalHeight(float tilez, float entityz)
	{
		if (tilez < entityz + STAND_HEIGHT - STEP_HEIGHT || tilez > entityz + STAND_HEIGHT + STEP_HEIGHT)
			return false;

		return true;
	}

	std::vector<PhysicsComponent*> FindDupes(std::vector<PhysicsComponent*>* list, float targetHeight)
	{
		std::vector<PhysicsComponent*> dupes;

		// List of spots to check up on
		std::map<Vector2f, std::vector<PhysicsComponent*>> tiles;
		for (auto& x : *list)
		{
			Vector2f pos(x->PositionRef().x, x->PositionRef().y);
			if (!tiles.count(pos))
				tiles.emplace(pos, std::vector<PhysicsComponent*>());

			tiles[pos].push_back(x);
		}

		// Go through each location and remove the tiles that are irrelevant
		// - Tiles underneath tiles that are legal for the player
		// - Tiles that are too high
		// - Anything below a collision tile (floor under a rock)
		for (auto& x : tiles)
		{
			// - Anything below a collision tile (floor under a rock)
			Vector3f collisionBlock = Vector3f(-1);
			for (auto& y : x.second)
				if (y->PositionRef().z == targetHeight)
				{
					collisionBlock = y->PositionRef();
					break;
				}

			if (collisionBlock != -1)
			{

				for (auto& y : x.second)
					if (y->PositionRef().z > collisionBlock.z)
						dupes.push_back(y);
			}

			// - Tiles underneath tiles that are legal for the player
			else //if(collisionBlock == -1)
			{
				bool stand = false;
				for (auto& y : x.second)
					if (IsLegalHeight(y->PositionRef().z, targetHeight))
					{
						stand = true;
						break;
					}

				for (auto& y : x.second)
					if (y->PositionRef().z < targetHeight + 0.5f && y->PositionRef().z > targetHeight - 0.5f)
					{
						stand = false;
						break;
					}

				if (stand)
				{
					bool playerHeight = false;
					bool higherStep = false;
					for (auto& y : x.second)
					{
						if (y->PositionRef().z == targetHeight + 1)
							playerHeight = true;
						else if (y->PositionRef().z < targetHeight + 1 && y->PositionRef().z > targetHeight)
							higherStep = true;
					}

					for (auto& y : x.second)
					{
						float val = targetHeight + 1 + (playerHeight ? 0 : (higherStep ? -0.5f : 0.5f));
						if (y->PositionRef().z > val)
							dupes.push_back(y);
					}
				}
				else
				{
					PhysicsComponent* low = nullptr;
					for (auto& y : x.second)
						if (y->PositionRef().z > targetHeight + 1)
							if (low == nullptr || low->PositionRef().z > y->PositionRef().z)
								low = y;

					for (auto& y : x.second)
						if (y->PositionRef().z > low->PositionRef().z)
							dupes.push_back(y);
				}
			}

			// - Tiles that are too high
			for (auto& y : x.second)
				if (y->PositionRef().z < targetHeight - 0.5f)
					dupes.push_back(y);
		}


		return dupes;
	}

	int TileTouchCount(std::array<float, 6> bb)
	{
		assert(bb.size() == 6);
		if (bb.size() != 6)
			return 0;

		//Get the borders and floor/ceil + area formula to know how many the bb is touching
		return (int)(ceil(bb[Right]) - floor(bb[Left])) *
			(int)(ceil(bb[Up]) - floor(bb[Down]));
	}



	//MAKE SURE YOU KNOW THE 2 OBJECTS ARE ON THE RIGHT Z BEFORE CALLING THIS FUUNCTION
	//Do these 2 objects collide, and if they do change the colliding velocity axis to 0 and set the move BB
	bool ApplyCollision(PhysicsComponent* a, PhysicsComponent* b)
	{
		//Ensure that they actually collide -- 2D-intersect to allow "collision" with bottom of cliffs and pits
		if (!Physics::Intersect2D(a->GetMoveBoundingBox(), b->GetMoveBoundingBox()))
			return false;

		//How to deal with a collision
		//	1: Use velocity/elapsed time to know what collided first (x or y)
		//	2: Change the velocity of that direction to 0

		//Percentage of movement to collide
		float x_perc = 1;
		float xdirection = a->Velocity().x * (float)ElapsedTime::GetInstance().GetElapsedTime();

		//X-collision check
		if (xdirection < 0 && //Going left...
			a->GetBoundingBox()[Left] > b->GetBoundingBox()[Right])//...and A was to the right of B
			x_perc = (abs(a->GetBoundingBox()[Left] - b->GetBoundingBox()[Right])) / (abs(xdirection));//(distance between 2) / (total length)
		else if (xdirection > 0 && //Going right...
			a->GetBoundingBox()[Right] < b->GetBoundingBox()[Left])
			x_perc = (abs(a->GetBoundingBox()[Right] - b->GetBoundingBox()[Left])) / (abs(xdirection));//(distance between 2) / (total length)
		else//was already in same X...
			x_perc = 10;//...make it always lose condition

						//Percentage of movement until collision
		float y_perc = 1;
		float ydirection = a->Velocity().y * (float)ElapsedTime::GetInstance().GetElapsedTime();

		//Y-collision check
		if (ydirection < 0 && //Going down...
			a->GetBoundingBox()[Down] > b->GetBoundingBox()[Up])//...and A was on top of B
			y_perc = (abs(a->GetBoundingBox()[Down] - b->GetBoundingBox()[Up])) / (abs(ydirection));//(distance between 2) / (total length)
		else if (ydirection > 0 && //Going up...
			a->GetBoundingBox()[Up] < b->GetBoundingBox()[Down])//...and A under B
			y_perc = (abs(a->GetBoundingBox()[Up] - b->GetBoundingBox()[Down])) / (abs(ydirection));//(distance between 2) / (total length)
		else//was already in same Y...
			y_perc = 10;//...make it always lose condition

		if (x_perc == 10 && y_perc == 10)
			return false;//The two were already collided?
		else if (x_perc < y_perc)//x collision
			ApplyCollision(a, b, Axis::X);
		else//if both x and y were equally apart, default to y
			ApplyCollision(a, b, Axis::Y);

		return true;
	}

	//MAKE SURE YOU KNOW THE 2 OBJECTS ARE ON THE RIGHT Z BEFORE CALLING THIS FUUNCTION
	//change the colliding velocity axis to 0 and set the move BB
	void ApplyCollision(PhysicsComponent* a, PhysicsComponent* b, Axis precalculatedAxis)
	{
		if (precalculatedAxis == Axis::X)
		{
			a->XCollide();
			//b->XCollide();
		}
		else if (precalculatedAxis == Axis::Y)
		{
			a->YCollide();
			//b->YCollide();
		}
		//Reset the move BB for future attempted collisions
		a->SetMovedBB();
		//b->SetMovedBB();
	}


	//returns number of tiles that are legally touched
	int TouchCount(std::vector<std::shared_ptr<Entity>>* touching, float targetHeight)
	{
		int touchCounter = 0;
		for (auto ents : *touching)
		{
			std::shared_ptr<PhysicsComponent> x = ents->Physics();

			float z = x->Position().z;

			//Disregard illegal blocks
			if (!IsLegalHeight(z, targetHeight))
				continue;

			touchCounter++;
		}

		return touchCounter;
	}

	//returns number of tiles that are legally touching
	int TouchCount(std::vector<PhysicsComponent*>* touching, float targetHeight, bool excludeMustStand = false)
	{
		std::map<Vector2f, bool> touches;
		for (auto ents : *touching)
		{
			float z = ents->Position().z;
			Vector2f pos = Vector2f(ents->Position().x, ents->Position().y);

			//Disregard illegal blocks
			if ((excludeMustStand && !ents->walkOn) || !IsLegalHeight(z, targetHeight))
			{
				if (!touches.count(pos))
					touches.emplace(pos, false);
				else
					touches[pos] = false;
				continue;
			}

			//add to touches if not in notouches
			if (!touches.count(pos))
			{

				touches.emplace(pos, true);
			}
		}

		int result = 0;
		for (auto x : touches)
			if (x.second)
				result++;

		return result;
	}

	//Finds the min and max legal z for a list of tiles
	void FindMinMaxLegalZ(std::vector<PhysicsComponent*>* touching, float& min, float& max, float targetHeight)
	{
		for (auto ents : *touching)
		{
			float z = ents->Position().z;

			//Disregard illegal blocks
			if (!IsLegalHeight(z, targetHeight) || !ents->walkOn)
				continue;

			//If its the lowest, set min to it
			if (z < min)
				min = z;

			//If its the highest, set max to it
			if (z > max)
				max = z;
		}
	}



	// "Collisions" happens in 1 of 2 cases:
	// 1 - The object is going somewhere that lacks any valid blocks (ex: walking off a cliff -- the other blocks are too far)
	// 2 - The object bumps into something within his z-range (if the z is 4, then any object within the range [4,5[ )
	//		For objects greater than 1.5 higher, the object goes under it.
	std::map<unsigned int, std::shared_ptr<Entity>> CollisionRemastered(std::map<unsigned int, std::shared_ptr<Entity>>* clist, std::vector<std::shared_ptr<MapTile>>* mt)
	{
		std::map<unsigned int, std::shared_ptr<Entity>> result = std::map<unsigned int, std::shared_ptr<Entity>>();

		int playerId = -1;

		// Set player
		if (clist->count(1))
			playerId = 1;

		// What if a dude NOT falling off a cliff causes a direct collision?
		// No way to know right now unless we redo the whole thing twice
		//for (int redo = 0; redo < 1; redo++)
		//{

			//How to deal with a collision
			//	1: Use velocity/elapsed time to know what collided first (x or y)
			//	2: Change the velocity of that direction to 0

			//	2: Ensure that the dude isnt falling off a cliff
			//			a: Check what cooordinates the player is touching
			//			b: Make sure all those tiles are under him (z to z - 0.5, OR z to z + 0.5, he cant go up and down a cliff at the same time -- act as collision)
			//	3: Set the object to the right height
			//			a: What tiles/coordinates is the object touching
			//			b: Which is the highest legal tile (some could be very high) - move object there

			// Touching is used throughout the whole method
		std::map<unsigned int, std::vector<PhysicsComponent*>> touching;
		for (auto& xs : *clist)
		{
			Entity* x = xs.second.get();
			if (x->PhysicsRaw()->Velocity() == 0)
				continue;
			int id = x->GetID();
			touching.emplace(id, std::vector<PhysicsComponent*>());


			//Create list of touching
			auto bb1 = x->PhysicsRaw()->GetMoveBoundingBox();

			float x1 = x->PhysicsRaw()->Position().x;
			float y1 = x->PhysicsRaw()->Position().y;
			for (auto& ts : *mt)
			{
				PhysicsComponent* p = ts->PhysicsRaw();
				if (p->_ethereal)
					continue;

				if (p->Position().x < x1 - 2 ||
					p->Position().y < y1 - 2 ||
					p->Position().y > y1 + 2)
					continue;
				else if (p->Position().x > x1 + 2)
					break;
				auto bb2 = p->GetMoveBoundingBox();

				if (Physics::Intersect2D(bb1, bb2))
					touching.at(id).push_back(p);
			}



			float oz = x->PhysicsRaw()->Position().z;

			//Remove Dupes (Blocks that are at the same height but one is irrelevant)
			std::vector<PhysicsComponent*> dupes = FindDupes(&touching.at(id), oz);

			for (auto x : dupes)
			{
				if (std::find(touching.at(id).begin(), touching[id].end(), x) != touching[id].end())
					touching.at(id).erase(std::remove(touching.at(id).begin(), touching.at(id).end(), x));
			}

			//What is this object touching

			int mustTouch = TileTouchCount(x->PhysicsRaw()->GetMoveBoundingBox());
			//Z-values
			float max = -10000;
			float min = 10000;

			if (id == 1)
				int lol = 69;
			int touchCounter = TouchCount(&touching.at(id), oz, true);

			FindMinMaxLegalZ(&touching.at(id), min, max, oz);

			dupes.clear();

			if (touchCounter < mustTouch)
			{
				// X + Y movement is illegal
				// If moving in only X or Y is legal and only the other illegal, just collide in that axis

				// Here we make bounding boxes that test for one direction at a time
				std::array<float, 6> bbx = std::array<float, 6>(x->PhysicsRaw()->GetMoveBoundingBox());
				bbx[Down] = x->PhysicsRaw()->GetBoundingBox()[Down];// Gets the up down from the pre-move bb
				bbx[Up] = x->PhysicsRaw()->GetBoundingBox()[Up];
				int mustTouchX = TileTouchCount(bbx);

				std::array<float, 6> bby = std::array<float, 6>(x->PhysicsRaw()->GetMoveBoundingBox());
				bby[Left] = x->PhysicsRaw()->GetBoundingBox()[Left];// Gets the left right from the pre-move bb
				bby[Right] = x->PhysicsRaw()->GetBoundingBox()[Right];
				int mustTouchY = TileTouchCount(bby);

				std::vector<PhysicsComponent*> touchX = std::vector<PhysicsComponent*>();
				std::vector<PhysicsComponent*> touchY = std::vector<PhysicsComponent*>();

				//Which in touching is the obj touching
				for (auto x : touching.at(id))
				{
					if (Physics::Intersect2D(bbx, x->GetMoveBoundingBox()))
						touchX.push_back(x);
					if (Physics::Intersect2D(bby, x->GetMoveBoundingBox()))
						touchY.push_back(x);
				}

				int touchingX = TouchCount(&touchX, oz, true);
				int touchingY = TouchCount(&touchY, oz, true);

				//Only 1 of them is true
				if ((touchingX == mustTouchX) != (touchingY == mustTouchY))
				{
					if (touchingX == mustTouchX)
					{
						for (auto tx : touchY)
							if (!IsLegalHeight(tx->Position().z, oz) || !tx->walkOn)
								ApplyCollision(x->PhysicsRaw(), tx, Axis::Y);
						touching.at(id) = touchY;
					}
					else if (touchingY == mustTouchY)
					{
						for (auto tx : touchX)
							if (!IsLegalHeight(tx->Position().z, oz) || !tx->walkOn)
								ApplyCollision(x->PhysicsRaw(), tx, Axis::X);
						touching.at(id) = touchX;
					}
				}
				else

					//...collide with everything not at a legal height
					for (auto tx : touching.at(id))
					{
						float z = tx->Position().z;

						//collide with illegal blocks
						if (!IsLegalHeight(z, oz) || !tx->walkOn)
						{
							if (ApplyCollision(x->PhysicsRaw(), tx))
								dupes.push_back(tx);
						}
					}
			}
			//If youre trying to touch 2 different heights at once or you arent touching enough blocks...
			else if (abs(min - max) > STEP_HEIGHT)
			{
				std::vector<Vector3f> pos;

				//...collide with everything not at your exact height

				// ensure you're on something at your height at least, otherwise go down
				bool onSomething = false;
				int futurStand = -5;
				for (int i = 0; i < touching.at(id).size(); i++)
				{
					auto t = touching.at(id).at(i);

					if (futurStand == i)
						continue;

					// Check if ur on something
					if (!onSomething && t->PositionRef().z == oz + STAND_HEIGHT)
					{
						onSomething = true;
						futurStand = i;
						i = -1;
						continue;
					}

					float z = t->Position().z;
					Vector3f v3f = t->Position();
					v3f.z = 0;

					if (z != oz + STAND_HEIGHT && onSomething || std::find(pos.begin(), pos.end(), v3f) != pos.end())
					{
						ApplyCollision(x->PhysicsRaw(), t);
						dupes.push_back(t);
						pos.push_back(v3f);
					}
					else
					{
						if (!onSomething && t->PositionRef().z >= oz + STAND_HEIGHT && Physics::Intersect2D(t->GetBoundingBox(), x->PhysicsRaw()->GetBoundingBox()))
						{
							futurStand = i;
							onSomething = true;
							i = -1;
						}
					}
				}
			}


			// Remove the invalid blocks we arent colliding with anymore to set the height
			for (auto x : dupes)
				touching.at(id).erase(std::remove(touching.at(id).begin(), touching.at(id).end(), x));

			// Collide with tiles
			TileCollision(x, touching.at(id));

			auto temp = std::vector<PhysicsComponent*>(touching.at(id));
			touching.at(id).clear();
			//Create list of touching
			bb1 = x->PhysicsRaw()->GetMoveBoundingBox();
			for (auto ts : temp)
			{
				auto bb2 = ts->GetMoveBoundingBox();
				if (Physics::Intersect2D(bb1, bb2))
					touching.at(id).push_back(ts);
			}

			//Set height to highest
			min = 1000;

			for (auto tx : touching.at(id))
			{
				float z = tx->Position().z;

				//Disregard illegal blocks
				if (!IsLegalHeight(z, oz) || !tx->walkOn)
					continue;

				//If its the highest, set max to it
				if (z < min)
					min = z;
			}

			//if (min == 1000)
			//	for (auto tx : touching.at(id))
			//		if ((min != 1000 && min < tx->PositionRef().z) || tx->PositionRef().z >= oz + STAND_HEIGHT)
			//			min = tx->PositionRef().z;
			if (min != 1000)
			{
				if (id == 1)
					int lol = 69;
				x->PhysicsRaw()->AbsolutePosition(Vector3f(-1, -1, min - STAND_HEIGHT), Vector3f(0, 0, 1));
			}
		}

		bool redo = false;
		for (int i = 0; i < 2; i++)
		{
			for (auto xs : *clist) {
				auto x = xs.second;

				// Only the entities will be colliding
					// Don't collide against other entities
				for (auto xs2 : *clist)
				{
					auto x2 = xs2.second;
					// We dont want to re-pass the same collision checks
					if (x2 == x || /*x2->PhysicsRaw()->Velocity() == 0 &&*/ x->PhysicsRaw()->Velocity() == 0)
						continue;

					//Are the objects inside each other right now? (nothing will go fast enough to skip this)
					std::array<float, 6> lol1 = x->PhysicsRaw()->GetMoveBoundingBox();
					std::array<float, 6> lol2 = i == 0 ? x2->PhysicsRaw()->GetBoundingBox() : x2->PhysicsRaw()->GetMoveBoundingBox();
					std::array<float, 6> lol3 = x->PhysicsRaw()->GetEtherealMoveBoundingBox();
					std::array<float, 6> lol4 = i == 0 ? x2->PhysicsRaw()->GetEtherealMoveBoundingBox() : x2->PhysicsRaw()->GetEtherealMoveBoundingBox();

					bool absoluteCollision = Physics::Intersect2D(lol1, lol2);
					bool etherealCollision = Physics::Intersect2D(lol3, lol4);

					// This is all assuming that the models are 2d, lets make sure that they are
					assert(lol1.at(AABB::Close) == lol1.at(AABB::Far));
					assert(lol2.at(AABB::Close) == lol2.at(AABB::Far));

					// if no collision, just skip
					if (!(abs(lol1.at(AABB::Close) - lol2.at(AABB::Close)) < STAND_HEIGHT && (absoluteCollision || etherealCollision)))
						continue;

					if (etherealCollision)
					{
						ApplyCollision(x->PhysicsRaw(), x2->PhysicsRaw());
						//redo = true;
					}

					// Shoot out the event cause its touching the player
					if (playerId != -1)
						if (x->GetID() == playerId || x2->GetID() == playerId)
							result.emplace(x->GetID() == playerId ? x2->GetID() : x->GetID(), x->GetID() == playerId ? x2 : x);
				}

			}
			//if (!redo)
			//	break;
		}

		//}
		return result;
	}

	void TileCollision(Entity* x, std::vector<PhysicsComponent*> p)
	{
		//	1: Check if there's any collisions
		//			a: Compare BB to see if instersection
		//			b: Deal with it
		for (auto tou : p)
		{
			//Are the objects inside each other right now? (nothing will go fast enough to skip this)
			std::array<float, 6> lol1 = x->Physics()->GetMoveBoundingBox();
			std::array<float, 6> lol2 = tou->GetMoveBoundingBox();

			// This is all assuming that the models are 2d, lets make sure that they are
			assert(lol1.at(AABB::Close) == lol1.at(AABB::Far));
			assert(lol2.at(AABB::Close) == lol2.at(AABB::Far));
			float dif = lol1.at(AABB::Close) - lol2.at(AABB::Close);

			// If the tile is legal, skip it
			if (!(dif <= STEP_HEIGHT && dif >= 0.0f && Physics::Intersect2D(lol1, lol2)))
				if (tou->walkOn)
					continue;

			ApplyCollision(x->PhysicsRaw(), tou);
		}
	}

}