#include "objects.h"
#include "scene.h"

bool Sphere::IntersectRay(const Ray& ray, HitInfo& hInfo, int hitSide) const {
    float a = ray.dir.LengthSquared();
    float b = ray.dir.Dot(ray.p);
    float c = ray.p.LengthSquared() - 1.0f;
    float det = b * b - a * c;
    if (det < 0) {
        return false;
    }
    else {
        float t1 = (-b - sqrt(det)) / a;
        float t2 = (-b + sqrt(det)) / a;
        if (t1 > t2) {
            std::swap(t1, t2);
        }
        float tmin = min(t1, t2);
        float tmax = max(t1, t2);

        if (hitSide != HIT_FRONT) {
			if (hInfo.z > tmax && tmax > EPS) {
				hInfo.z = tmax;
				hInfo.p = ray.p + tmax * ray.dir;
				hInfo.N = hInfo.p;
                hInfo.front = false;
			}
		}
		if (hitSide != HIT_NONE) {
			if (hInfo.z > tmin && tmin > EPS) {
				hInfo.z = tmin;
				hInfo.p = ray.p + tmin * ray.dir;
				hInfo.N = hInfo.p;
                hInfo.front = true;
			}
		}
		return true;
    }
}

bool Plane::IntersectRay( const Ray &ray, HitInfo &hInfo, int hitSide) const {
    Point3 N(0, 0, 1);
    float det = ray.dir.Dot(N);
    if (det < 0 && hitSide == HIT_BACK) {
        return false;
    }
    if (det >= 0 && hitSide == HIT_FRONT) {
        return false;
    }
    float t = -ray.p.z / ray.dir.z;
    Point3 p = ray.p + t * ray.dir;
    if (abs(p.x) > 1.0f || abs(p.y) > 1.0f) {
        return false;
    }

    if (t > EPS && hInfo.z > t) {
        hInfo.front = (det < 0);
        hInfo.N = N;
        hInfo.p = p;
        hInfo.z = t;
        return true;
    }
    else {
        return false;
    }
}

bool TriObj::IntersectRay( const Ray &ray, HitInfo &hInfo, int hitSide) const {
    bool res = false;
    for (int fid = 0; fid < nf; ++fid) {
        res |= IntersectTriangle(ray, hInfo, hitSide, fid);
    }
    return res;
}

bool TriObj::IntersectTriangle( const Ray &ray, HitInfo &hInfo, int hitSide, unsigned int faceID ) const {
    TriFace face = f[faceID];
    Point3 e1 = v[face.v[1]] - v[face.v[0]];
    Point3 e2 = v[face.v[2]] - v[face.v[0]];
    Point3 p = ray.dir.Cross(e2);
    float a = e1.Dot(p);
    if (a == 0.0) {
        return false;
    }
    if (hitSide == HIT_FRONT && a < 0) {
        return false;
    }
    if (hitSide == HIT_BACK && a > 0) {
        return false;
    }
    float f = 1.0 / a;
    Point3 s = ray.p - v[face.v[0]];
    float u = f * (s.Dot(p));
    if (u < 0.0 || u > 1.0) {
        return false;
    }
    Point3 q = s.Cross(e1);
    float v = f * (ray.dir.Dot(q));
    if (v < 0.0 || u + v > 1.0) {
        return false;
    }
    float t = f * (e2.Dot(q));
    Point3 bc(1.0 - (u + v), u, v);

    if (hInfo.z > t && t > EPS) {
        hInfo.N = GetNormal(faceID, bc).GetNormalized();
        hInfo.p = ray.p + t * ray.dir;
        hInfo.z = t;
        hInfo.front = (a > 0.0f);
        return true;
    }

    return false;
}

bool Box::IntersectRay(const Ray& ray, float t_max) const {
    float t0 = EPS;
    float t1 = t_max;
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    float divx = 1.0f / ray.dir.x;
    float divy = 1.0f / ray.dir.y;
    float divz = 1.0f / ray.dir.z;
    int sx = (divx < 0);
    int sy = (divy < 0);
    int sz = (divz < 0);
    Point3 bounds[2] = {pmin, pmax};

    tmin = (bounds[sx].x - ray.p.x) * divx;
    tmax = (bounds[1 - sx].x - ray.p.x) * divx;
    tymin = (bounds[sy].y - ray.p.y) * divy;
    tymax = (bounds[1 - sy].y - ray.p.y) * divy;
    if ( (tmin > tymax) || (tymin > tmax) )
        return false;
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;
    tzmin = (bounds[sz].z - ray.p.z) * divz;
    tzmax = (bounds[1 - sz].z - ray.p.z) * divz;
    if ( (tmin > tzmax) || (tzmin > tmax) )
        return false;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;
    return ( (tmin < t1) && (tmax > t0) );
}