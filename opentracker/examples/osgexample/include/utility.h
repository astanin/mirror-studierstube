#ifndef _CC_VECTORMATH_INCLUDED
#define _CC_VECTORMATH_INCLUDED
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>

#define dRadToDeg(x) (((float)x)*57.29577951308f)
#define MIN(x, y)     ((x) < (y) ? (x) : (y))
#define MAX(x, y)     ((x) > (y) ? (x) : (y))
#define ABS(x)        ((x) >= 0 ? (x) : -(x))
#define SIGN(x)       ((x) >= 0 ? 1 : -1)
#define SQR(x)        ((x)*(x))
#define LEN(x, y, z) (sqrt(SQR(x) + SQR(y) + SQR(z)))
#define DIST(x1, y1, z1, x2, y2, z2) LEN(((x1)-(x2)),((y1)-(y2)), ((z1)-(z2)))
#define DOT(x1, y1, z1, x2, y2, z2) ((x1)*(x2)+(y1)*(y2)+(z1)*(z2))
#define PI 3.14159265359
#define PIdiv180 3.14159265359/180.0
//Generates a random float in the range [0;1]
#define RANDOM_FLOAT (((float)rand())/RAND_MAX)


struct Vector3d  //Float 3d-vect, normally used
{
	GLfloat x,y,z;
};

struct Vector2d
{
	GLfloat x,y;
};

typedef struct Vertex3d
{
    // GL_C3F_V3F
	float r, g, b;
	float x, y, z;
} Vertex3d;

#define NULL_VECTOR F3dVector(0.0f,0.0f,0.0f)


float dot(Vector3d a,Vector3d b);
Vector3d cross(Vector3d a,Vector3d b);
Vector3d operator* (Vector3d v, float r);
Vector3d operator/ (Vector3d v, float r);
float operator* (Vector3d v, Vector3d u);	//Scalar product
Vector3d operator+ (Vector3d v, Vector3d u);
Vector3d operator- (Vector3d v, Vector3d u);
bool operator== (Vector3d v, Vector3d u);
bool operator!= (Vector3d v, Vector3d u);
Vector3d F3dVector ( GLfloat x, GLfloat y, GLfloat z );
void AddF3dVectorToVector ( Vector3d * Dst, Vector3d * V2);
Vector3d AddF3dVectors (Vector3d* u, Vector3d* v);
#endif
