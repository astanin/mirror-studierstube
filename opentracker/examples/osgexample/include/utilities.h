#ifndef UTILITIES
#define UTILITIES

void	randInit( void ) ;
float	RandFloat(void);
int		RandInt(int i);

#ifdef WIN32
#define drand48() (((float) rand())/((float) RAND_MAX))
#define srand48(x) (srand((x)))
#else
extern double drand48(void);
extern void srand48(long seedval);
#endif

#define PI 3.14159265359
#define PIOVER180 0.01745329252

#define dRadToDeg(x) (((float)x)*57.29577951308f)
#define MIN(x, y)     ((x) < (y) ? (x) : (y))
#define MAX(x, y)     ((x) > (y) ? (x) : (y))
#define ABS(x)        ((x) >= 0 ? (x) : -(x))
#define SIGN(x)       ((x) >= 0 ? 1 : -1)
#define SQR(x)        ((x)*(x))
#define LEN(x, y, z) (sqrt(SQR(x) + SQR(y) + SQR(z)))
#define DIST(x1, y1, z1, x2, y2, z2) LEN(((x1)-(x2)),((y1)-(y2)), ((z1)-(z2)))
#define DOT(x1, y1, z1, x2, y2, z2) ((x1)*(x2)+(y1)*(y2)+(z1)*(z2))



#endif
