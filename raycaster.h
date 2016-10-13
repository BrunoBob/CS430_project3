#ifndef __RAY_CASTER
#define __RAY_CASTER

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ERROR_RAYCAST 2
#define ERROR_WRITING 3

typedef struct object{
  int kind; // 0 = sphere, 1 = plane
  double* color;
  double* position;
  union {
    struct {
      double radius;
    } sphere;
    struct {
      double* normal;
    } plane;
  };
  struct object* next;
} *objectList;

void printObjects(objectList list);

void createScene(char* ppm, unsigned char* data, int width, int height);

double planeIntersection(double* Ro, double* Rd, double* position, double* normal);

double sphereIntersection(double* Ro, double* Rd, double* position, double radius);

static inline double* getVector(double x, double y, double z){
  double* v = malloc(3*sizeof(double));
  v[0] = x;
  v[1] = y;
  v[2] = z;
  return v;
}

static inline double* subVector(double* a, double* b){
  return getVector(a[0]-b[0], a[1]-b[1], a[2]-b[2]);
}

static inline double dotProduct(double* a, double* b){
  return (a[0]*b[0] + a[1]*b[1] + a[2]*b[2]);
}

static inline double sqr(double v) {
  return v*v;
}

static inline void normalize(double* v) {
  double len = sqrt(sqr(v[0]) + sqr(v[1]) + sqr(v[2]));
  v[0] /= len;
  v[1] /= len;
  v[2] /= len;
}


#endif
