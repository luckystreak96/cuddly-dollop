#include "physics.h"

namespace Physics
{
	//co1/co2 - Struct containing necessary info to calculate stuff
	//collisionConfirmed - actual vertex/surface collision
	//percentMovement - percentage of distance travelled before collision
	//triangleNormal - the normal vector of collided surface
	//first - did v1 collide with i2 or the other way around?
	void FindClosestCollision(CollisionObject co1, CollisionObject co2, bool* collisionConfirmed,
		float* percentMovement, Vector3f* triangleNormal, bool* first, Triangle* tri, Vector3f* traject, int* vertIndex, Vector3f* poc)
	{
		static bool firstIteration = true;

		int counter = -1;
		for(auto v : co1.verts)
		{
			counter++;

			if (co1.velocity == 0)
				break;

			Ray r = Ray();
			r.P0 = v.vertex - co1.velocity * (float)ElapsedTime::GetInstance().GetElapsedTime();
			r.P1 = v.vertex;


			for (int u = 0; u < co2.inds.size(); u += 3)
			{
				Triangle t = Triangle();
				t.V0 = co2.verts[co2.inds[u]].vertex;
				t.V1 = co2.verts[co2.inds[u + 1]].vertex;
				t.V2 = co2.verts[co2.inds[u + 2]].vertex;

				Vector3f V = t.V1 - t.V0;
				Vector3f W = t.V2 - t.V0;
				Vector3f normal = V.Cross(W).Normalize();

				if (Physics::RespectsNormal(co1.velocity, normal))
					continue;

				Vector3f i = Vector3f();
				int result = Physics::intersect3D_RayTriangle(r, t, &i);

				//No success
				if (result == 0)
					continue;
				else if (result == 2)//line is embedded in triangle
					continue;
				else//SUCCESS
				{
					Vector3f perc1, perc2;
					perc1 = (r.P0 - i).Abs();
					perc2 = (r.P0 - r.P1).Abs();
					float percent = (perc1).PercentDistance(perc2);
					//std::cout << "Percent: " << percent << std::endl;
					if (*percentMovement > percent)
					{
						*triangleNormal = normal;
						*tri = t;
						*traject = i - r.P0;
						*vertIndex = counter;
						*poc = i;
						*first = firstIteration;
						*percentMovement = percent;
						*collisionConfirmed = true;
					}
				}
			}
		}

		if (firstIteration)
		{
			firstIteration = !firstIteration;
			FindClosestCollision(co2, co1, collisionConfirmed, percentMovement, triangleNormal, first, tri, traject, vertIndex, poc);
		}
		else
		{
			firstIteration = !firstIteration;
			return;
		}
	}

	bool RespectsNormal(Vector3f vel, Vector3f normal)
	{
		float result = abs(vel.AngleBetween(normal));
		return result < (M_PI / 2);
	}

	//Two cubic bounding boxes intersect?
	bool Intersect3D(std::array<float, 6>& local, std::array<float, 6>& other)
	{
		//float x1 = local.Get(AABB::Left, 0);
		//float x2 = other.Get(AABB::Left, 0);
		//float x3 = local.Get(AABB::Right, 0);
		//float x4 = other.Get(AABB::Right, 0);
		//float y1 = local.Get(AABB::Up, 0);
		//float y2 = other.Get(AABB::Up, 0);
		//float y3 = local.Get(AABB::Down, 0);
		//float y4 = other.Get(AABB::Down, 0);
		float z1 = local[Close];
		float z2 = other[Close];
		float z3 = local[Far];
		float z4 = other[Far];


		//Left-Right
		if (!(local[Left] <= other[Right] && local[Right] >= other[Left]))
			return false;

		//Up-Down
		if (!(local[Up] >= other[Down] && local[Down] <= other[Up]))
			return false;

		//Close-Far
		if (!(local[Close] <= other[Far] && local[Far] >= other[Close]))
			return false;

		return true;
	}

	//Two 2D square bounding boxes intersect?
	bool Intersect2D(std::array<float, 6>& local, std::array<float, 6>& other)
	{
		//Left-Right
		if (!(local[Left] < other[Right] && local[Right] > other[Left]))
			return false;

		//Up-Down
		if (!(local[Up] > other[Down] && local[Down] < other[Up]))
			return false;

		return true;
	}

	//A box and a dot intersect?
	bool Intersect2D(std::array<float, 6>& bb, Vector3f point)
	{
		//Up-Down
		if (!(abs(bb[Close] - point.z) <= 0.5f || abs(bb[Far] - point.z) <= 0.5f))
			return false;

		//Left-Right
		if (!(bb[Left] < point.x && bb[Right] > point.x))
			return false;

		//Up-Down
		if (!(bb[Up] > point.y && bb[Down] < point.y))
			return false;

		return true;
	}

	// intersect3D_RayTriangle(): find the 3D intersection of a ray with a triangle
	//    Input:  a ray R, and a triangle T
	//    Output: *I = intersection point (when it exists)
	//    Return: -1 = triangle is degenerate (a segment or point)
	//             0 =  disjoint (no intersect)
	//             1 =  intersect in unique point I1
	//             2 =  are in the same plane
	int intersect3D_RayTriangle(Ray R, Triangle T, Vector3f* I)
	{
		Vector3f    u, v, n;              // triangle vectors
		Vector3f    dir, w0, w;           // ray vectors
		float     r, a, b;              // params to calc ray-plane intersect

										// get triangle edge vectors and plane normal
		u = T.V1 - T.V0;
		v = T.V2 - T.V0;
		n = u.Cross(v);              // cross product
		if (n == Vector3f())             // triangle is degenerate
			return -1;                  // do not deal with this case

		dir = R.P1 - R.P0;              // ray direction vector
		w0 = R.P0 - T.V0;
		a = -dot(n, w0);
		b = dot(n, dir);
		if (fabs(b) < SMALL_NUM) {     // ray is  parallel to triangle plane
			if (a == 0)                 // ray lies in triangle plane
				return 2;
			else return 0;              // ray disjoint from plane
		}

		// get intersect point of ray with triangle plane
		r = a / b;
		if (r < 0.0)                    // ray goes away from triangle
			return 0;                   // => no intersect
										// for a segment, also test if (r > 1.0) => no intersect
		if (r > 1.0)
			return 0;

		*I = R.P0 + dir * r;            // intersect point of ray and plane

										// is I inside T?
		float    uu, uv, vv, wu, wv, D;
		uu = dot(u, u);
		uv = dot(u, v);
		vv = dot(v, v);
		w = *I - T.V0;
		wu = dot(w, u);
		wv = dot(w, v);
		D = uv * uv - uu * vv;

		// get and test parametric coords
		float s, t;
		s = (uv * wv - vv * wu) / D;
		if (s < 0.0 || s > 1.0)         // I is outside T
			return 0;
		t = (uv * wu - uu * wv) / D;
		if (t < 0.0 || (s + t) > 1.0)  // I is outside T
			return 0;

		return 1;                       // I is in T
	}
}