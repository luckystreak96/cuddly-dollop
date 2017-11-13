#include "physics_2d.h"

namespace Physics_2D {

	bool IsLegalHeight(float tilez, float otherz)
	{
		if (tilez < otherz + STAND_HEIGHT - STEP_HEIGHT || tilez > otherz + STAND_HEIGHT + STEP_HEIGHT)
			return false;

		return true;
	}

	std::vector<std::shared_ptr<PhysicsComponent>> FindDupes(std::vector<std::shared_ptr<PhysicsComponent>>* list, float targetHeight)
	{
		std::vector<std::shared_ptr<PhysicsComponent>> dupes = std::vector<std::shared_ptr<PhysicsComponent>>();

		for (auto x : *list)
		{
			for (auto y : *list)
			{
				if (x == y)
					continue;

				//they have to be at the same place
				if (x->Position().x != y->Position().x || x->Position().y != y->Position().y)
					continue;

				float z = x->Position().z;

				//Disregard illegal blocks
				if (!IsLegalHeight(z, targetHeight))
					continue;
				//If the other block isnt the right height, we dont discard anything
				if (!IsLegalHeight(y->Position().z, targetHeight))
					continue;

				//remove lowest

				std::shared_ptr<PhysicsComponent> to_remove = x->Position().z > y->Position().z ? x : y;

				if (!(std::find(dupes.begin(), dupes.end(), to_remove) != dupes.end())) {
					/* dupes does not contain to_remove */
					dupes.push_back(to_remove);
				}
			}
		}

		return dupes;
	}

	int TileTouchCount(std::array<float, 6> bb)
	{
		if (bb.size() != 6)
			return -1;

		//Get the borders and floor/ceil + area formula to know how many the bb is touching
		return (int)(ceil(bb[Right]) - floor(bb[Left])) *
			(int)(ceil(bb[Up]) - floor(bb[Down]));
	}



	//MAKE SURE YOU KNOW THE 2 OBJECTS ARE ON THE RIGHT Z BEFORE CALLING THIS FUUNCTION
	//Do these 2 objects collide, and if they do change the colliding velocity axis to 0 and set the move BB
	void ApplyCollision(std::shared_ptr<PhysicsComponent> a, std::shared_ptr<PhysicsComponent> b)
	{
		//Ensure that they actually collide -- 2D-intersect to allow "collision" with bottom of cliffs and pits
		if (!Physics::Intersect2D(a->GetMoveBoundingBox(), b->GetMoveBoundingBox()))
			return;

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
			return;//The two were already collided?
		else if (x_perc < y_perc)//x collision
			ApplyCollision(a, b, Axis::X);
		else//if both x and y were equally apart, default to y
			ApplyCollision(a, b, Axis::Y);
	}

	//MAKE SURE YOU KNOW THE 2 OBJECTS ARE ON THE RIGHT Z BEFORE CALLING THIS FUUNCTION
	//change the colliding velocity axis to 0 and set the move BB
	void ApplyCollision(std::shared_ptr<PhysicsComponent> a, std::shared_ptr<PhysicsComponent> b, Axis precalculatedAxis)
	{
		if (precalculatedAxis == Axis::X)
		{
			a->XCollide();
			b->XCollide();
		}
		else if (precalculatedAxis == Axis::Y)
		{
			a->YCollide();
			b->YCollide();
		}
		//Reset the move BB for future attempted collisions
		a->SetMovedBB();
		b->SetMovedBB();
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

	//returns number of tiles that are legally touched
	int TouchCount(std::vector<std::shared_ptr<PhysicsComponent>>* touching, float targetHeight)
	{
		int touchCounter = 0;
		for (auto ents : *touching)
		{
			float z = ents->Position().z;

			//Disregard illegal blocks
			if (!IsLegalHeight(z, targetHeight))
				continue;

			touchCounter++;
		}

		return touchCounter;
	}

	//Finds the min and max legal z for a list of tiles
	void FindMinMaxLegalZ(std::vector<std::shared_ptr<PhysicsComponent>>* touching, float& min, float& max, float targetHeight)
	{
		for (auto ents : *touching)
		{
			float z = ents->Position().z;

			//Disregard illegal blocks
			if (!IsLegalHeight(z, targetHeight))
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
	//		For objects greater than 1 higher, the object goes under it.
	std::vector<std::shared_ptr<Entity>> Collision(std::map<unsigned int, std::shared_ptr<Entity>>* clist, std::shared_ptr<MapHandler> mh)
	{
		std::vector<std::shared_ptr<Entity>> result = std::vector<std::shared_ptr<Entity>>();
		// What if a dude NOT falling off a cliff causes a direct collision?
		// No way to know right now unless we redo the whole thing twice
		std::vector<std::shared_ptr<PhysicsComponent>> flist = std::vector<std::shared_ptr<PhysicsComponent>>();

		int playerPos = 0;
		for (auto x : *clist)
		{
			if (x.second->GetID() == 1)
				playerPos = flist.size();
			flist.push_back(x.second->Physics());
		}
		//for (unsigned int i = 0; i < clist->size(); i++)
		//	flist.push_back(clist->at(i)->Physics());

		for (unsigned int i = 0; i < mh->Tiles()->size(); i++)
			flist.push_back(mh->Tiles()->at(i)->Physics());

		for (int redo = 0; redo < 2; redo++)
		{

			//How to deal with a collision
			//	1: Use velocity/elapsed time to know what collided first (x or y)
			//	2: Change the velocity of that direction to 0

			//Calculate in 3 runs
			//	1: Check if there's any collisions
			//			a: Compare BB to see if instersection
			//			b: Deal with it
			//	2: Ensure that the dude isnt falling off a cliff
			//			a: Check what cooordinates the player is touching
			//			b: Make sure all those tiles are under him (z to z - 0.5, OR z to z + 0.5, he cant go up and down a cliff at the same time -- act as collision)
			//	3: Set the object to the right height
			//			a: What tiles/coordinates is the object touching
			//			b: Which is the highest legal tile (some could be very high) - move object there

			// 1: Check if there's any straight up collisions
			for (unsigned int i = 0; i < flist.size(); i++) {

				//Possible optimization by skipping all non-moving
				if (flist.at(i)->Velocity() == 0)
					continue;

				//Compare against the rest
				for (unsigned int j = 0; j < flist.size(); j++)
				{
					if (i == j)//dont collide with yourself
						continue;

					////Can both objects even collide
					//if (!flist.at(i)->mustCollide || !flist.at(j)->mustCollide)
					//	continue;

					//Are the objects inside each other right now? (nothing will go fast enough to skip this)
					std::array<float, 6> lol1 = flist.at(i)->GetMoveBoundingBox();
					std::array<float, 6> lol2 = flist.at(j)->GetMoveBoundingBox();
					if (!Physics::Intersect3D(lol1, lol2))
						//if (!Physics::Intersect3D(clist->at(i)->Physics()->GetMoveBoundingBox(), clist->at(j)->Physics()->GetMoveBoundingBox()))
						continue;

					ApplyCollision(flist.at(i), flist.at(j));
					int c = 0;
					if (i < clist->size() && j < clist->size())
					{
						for (auto x : *clist)
						{
							if (c == i && j == playerPos || i == playerPos && c == j)
							{
								if (x.second->GetID() != 1)
									result.push_back(x.second);
							}
							c++;
						}
					}
				}
			}

			//	2: Ensure that the dude isnt falling off a cliff
			//			a: Check what cooordinates the player is touching
			//			b: Make sure all those tiles are under him (z to z - 0.5, OR z to z + 0.5, he cant go up and down a cliff at the same time -- act as collision)
			//	3: Set the object to the right height
			//			a: What tiles/coordinates is the object touching
			//			b: Which is the highest legal tile (some could be very high) - move object there

			//Iterate through all objects
			for (unsigned int i = 0; i < flist.size(); i++) {

				//Possible optimization by skipping all non-moving
				if (flist.at(i)->Velocity() == 0)
					continue;

				//What is this object touching
				std::vector<std::shared_ptr<PhysicsComponent>> touching = std::vector<std::shared_ptr<PhysicsComponent>>();

				int mustTouch = TileTouchCount(flist.at(i)->GetMoveBoundingBox());

				//Create list of touching
				for (unsigned int j = 0; j < flist.size(); j++)
				{
					if (i == j)//dont check against yourself
						continue;

					if (!flist.at(i)->mustCollide || !flist.at(j)->mustCollide)
						continue;

					if (Physics::Intersect2D(flist.at(i)->GetMoveBoundingBox(), flist.at(j)->GetMoveBoundingBox()))
						touching.push_back(flist.at(j));
				}

				float oz = flist.at(i)->Position().z;

				//Remove Dupes (Blocks that are at the same height but one is irrelevant)
				std::vector<std::shared_ptr<PhysicsComponent>> dupes = FindDupes(&touching, oz);

				for (auto x : dupes)
					touching.erase(std::remove(touching.begin(), touching.end(), x));

				//Z-values
				float max = -10000;
				float min = 10000;

				int touchCounter = TouchCount(&touching, oz);

				FindMinMaxLegalZ(&touching, min, max, oz);

				dupes.clear();

				//If youre trying to touch 2 different heights at once or you arent touching enough blocks...
				if (abs(min - max) > STEP_HEIGHT)
				{
					//...collide with everything not at your exact height
					for (auto x : touching)
					{
						float z = x->Position().z;

						if (z != oz + STAND_HEIGHT)
						{
							ApplyCollision(flist.at(i), x);
							dupes.push_back(x);
						}
					}
				}
				else if (touchCounter < mustTouch)
				{
					//X + Y movement is illegal
					//If moving in only X or Y is legal and only the other illegal, just collide in that axis
					std::array<float, 6> bbx = std::array<float, 6>(flist.at(i)->GetMoveBoundingBox());
					bbx[Down] = flist.at(i)->GetBoundingBox()[Down];
					bbx[Up] = flist.at(i)->GetBoundingBox()[Up];
					int mustTouchX = TileTouchCount(bbx);

					std::array<float, 6> bby = std::array<float, 6>(flist.at(i)->GetMoveBoundingBox());
					bby[Left] = flist.at(i)->GetBoundingBox()[Left];
					bby[Right] = flist.at(i)->GetBoundingBox()[Right];
					int mustTouchY = TileTouchCount(bby);

					std::vector<std::shared_ptr<PhysicsComponent>> touchX = std::vector<std::shared_ptr<PhysicsComponent>>();
					std::vector<std::shared_ptr<PhysicsComponent>> touchY = std::vector<std::shared_ptr<PhysicsComponent>>();

					//Which in touching is the obj touching
					for (auto x : touching)
					{
						if (Physics::Intersect2D(bbx, x->GetMoveBoundingBox()))
							touchX.push_back(x);
						if (Physics::Intersect2D(bby, x->GetMoveBoundingBox()))
							touchY.push_back(x);
					}

					int touchingX = TouchCount(&touchX, oz);
					int touchingY = TouchCount(&touchY, oz);

					//Only 1 of them is true
					if ((touchingX == mustTouchX) != (touchingY == mustTouchY))
					{
						if (touchingX == mustTouchX)
						{
							for (auto x : touchX)
								if (!IsLegalHeight(x->Position().z, oz))
									ApplyCollision(flist.at(i), x, Axis::Y);
							touching = touchX;
						}
						else if (touchingY == mustTouchY)
						{
							for (auto x : touchY)
								if (!IsLegalHeight(x->Position().z, oz))
									ApplyCollision(flist.at(i), x, Axis::X);
							touching = touchY;
						}
					}
					else

						//...collide with everything not at a legal height
						for (auto x : touching)
						{
							float z = x->Position().z;

							//collide with illegal blocks
							if (!IsLegalHeight(z, oz))
							{
								ApplyCollision(flist.at(i), x);
								dupes.push_back(x);
							}
						}
				}

				//Dont set height to blocks that you just collided with >:P
				for (auto x : dupes)
					touching.erase(std::remove(touching.begin(), touching.end(), x));

				//Set height to highest
				min = 1000;

				for (auto x : touching)
				{
					float z = x->Position().z;

					//Disregard illegal blocks
					if (!IsLegalHeight(z, oz))
						continue;

					//If its the highest, set max to it
					if (z < min)
						min = z;
				}

				if (min != 1000)
					flist.at(i)->AbsolutePosition(Vector3f(-1, -1, min - STAND_HEIGHT), Vector3f(0, 0, 1));
			}
		}
		return result;
	}
}

