#include <stdio.h>
#include "stretchy-buffer.h"

typedef struct {
  uint32_t x;
  uint32_t y;
  uint32_t z;
} Point3d;

void floatExample() {
  float* buf = NULL;

  sb_pushback(buf, 1.2f);
  sb_pushback(buf, 2.4f);
  sb_pushback(buf, 3.14f);
  sb_pushback(buf, -37.1f);
  sb_pushback(buf, 55.0f);

  printf("Capacity: %d Length: %d\n", sb_capacity(buf), sb_length(buf));

  for (size_t i = 0; i < sb_length(buf); i++) {
    printf("%d: %f\n", i, buf[i]);
  }
}

void pointExample() {
  sb_create(Point3d, points);

  Point3d temp = { 1, 2, 3 };

  sb_pushback(points, temp);

  temp.x = 55;
  temp.y = 75;
  sb_pushback(points, temp);

  temp.x = -1;
  temp.y = -2;
  temp.z = -3;
  sb_pushback(points, temp);

  Point3d p = *sb_pop(points);
  printf("popped: %d %d %d\n", p.x, p.y, p.z);

  for (size_t i = 0; i < sb_length(points); i++) {
    printf("p%d: %d %d %d\n", i, points[i].x, points[i].y, points[i].z);
  }

  printf("Capacity: %d, Length = %d\n", sb_capacity(points), sb_length(points));
}

int main() {
  pointExample();

  return 0;
}
