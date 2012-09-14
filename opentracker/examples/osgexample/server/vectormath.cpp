#include "vectormath.h"



void normalise(float *x, float *y, float *z);

void normalise(float *x, float *y, float *z)
{
  double len;

  len = LEN(*x, *y, *z);
  if(len != 0.0) {
    *x /= len;
    *y /= len;
	*z /= len;
  }
 
}

Vector3d F3dVector ( GLfloat x, GLfloat y, GLfloat z )
{
	Vector3d tmp;
	tmp.x = x;
	tmp.y = y;
	tmp.z = z;
	return tmp;
}

Vector3d AddF3dVectors (Vector3d* u, Vector3d* v)
{
	Vector3d result;
	result.x = u->x + v->x;
	result.y = u->y + v->y;
	result.z = u->z + v->z;
	return result;
}
void AddF3dVectorToVector ( Vector3d * Dst, Vector3d * V2)
{
	Dst->x += V2->x;
	Dst->y += V2->y;
	Dst->z += V2->z;
}
float operator* (Vector3d v, Vector3d u)	//Scalar product
{
	return v.x*u.x+v.y*u.y+v.z*u.z;
}

float dot(Vector3d a,Vector3d b)
{
  return    a.x*b.x + a.y*b.y + a.z*b.z;
}

Vector3d cross(Vector3d a,Vector3d b)
{
Vector3d r;
      r.x = a.y*b.z - a.z*b.y;
      r.y = a.z*b.x - a.x*b.z;
      r.z = a.x*b.y - a.y*b.x;
return r;
}


Vector3d operator* (Vector3d v, float r)
{
	Vector3d res;
	res.x = v.x*r;
	res.y = v.y*r;
	res.z = v.z*r;
	return res;
}

Vector3d operator+ (Vector3d v, Vector3d u)
{
	Vector3d res;
	res.x = v.x+u.x;
	res.y = v.y+u.y;
	res.z = v.z+u.z;
	return res;
}
Vector3d operator- (Vector3d v, Vector3d u)
{
	Vector3d res;
	res.x = v.x-u.x;
	res.y = v.y-u.y;
	res.z = v.z-u.z;
	return res;
}
Vector3d operator/ (Vector3d v, float r)
{
	return v*(1/r);
}