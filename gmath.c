#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
  normalize(light[LOCATION]);
  normalize(normal);
  normalize(view);
  color a = calculate_ambient(alight, areflect);
  color d = calculate_diffuse(light, dreflect, normal);
  color s = calculate_specular(light, sreflect, view, normal);
  i.red = a.red + d.red + s.red;
  i.green = a.green + d.green + s.green;
  i.blue = a.blue + d.blue + s.blue;
  limit_color(&i);
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  //A * Ka
  color a;
  //red, green, blue
  a.red = alight.red * areflect[RED];
  a.green = alight.green * areflect[GREEN];
  a.blue = alight.blue * areflect[BLUE];
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  //costheta L(normalized) * n(normalized)
  //PKd(N(normalized * I(normalized)))
  double dot = dot_product(light[LOCATION], normal);
  color d;
  d.red = light[COLOR][RED] * dot * dreflect[RED];
  d.green = light[COLOR][GREEN] * dot * dreflect[GREEN];
  d.blue = light[COLOR][GREEN] * dot * dreflect[BLUE];
  return d;
}

//dot product orders might be messed up
color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s;
  //PKs[2(N * L) x N - L * v]
  double R = 2*dot_product(normal, light[LOCATION]);
  normal[RED] = normal[RED] * R;
  normal[GREEN] = normal[GREEN] * R;
  normal[BLUE] = normal[BLUE] * R;
  //cos between r and v = r * v
  double * reflected = normal;
  double rv = dot_product(reflected, view);
  rv = rv > 0 ? rv : 0;
  rv = pow(rv, SPECULAR_EXP);
  s.red = light[COLOR][RED] * rv * sreflect[RED];
  s.green = light[COLOR][GREEN] * rv * sreflect[GREEN];
  s.red = light[COLOR][BLUE] * rv * sreflect[BLUE];
  return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
  c->red = c->red > 255 ? 255 : c->red;
  c->blue = c->blue > 255 ? 255 : c->blue;
  c->green = c->green > 255 ? 255 : c->green;
  c->red = c->red < 0 ? 0 : c->red;
  c->blue = c->blue < 0 ? 0 : c->blue;
  c->green = c->green < 0 ? 0 : c->green;
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double L = sqrt(vector[RED] * vector[RED] + vector[GREEN] * vector[GREEN] + vector[BLUE] * vector[BLUE]);
  vector[RED] = vector[RED]/L;
  vector[GREEN] = vector[GREEN]/L;
  vector[BLUE] = vector[BLUE]/L;
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
