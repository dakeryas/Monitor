#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <iostream>
#include <TRandom3.h>

class Point {
  
public:
  double x, y, z;
  
  Point();
  Point(double x, double y, double z);
  Point(TRandom3& ran, double R, double h); //generates a point randomly into a cylinder of radius r and height h
  Point(TRandom3& ran, double R1, double R2, double h1, double h2); //generates a point randomly between two cylinders of respective radii R1 and R2 and of respective height h1 and h2
  virtual ~Point();
  void cylinder(TRandom3& ran, double R, double h); //moves a point randomly into a cylinder of radius r and height h
  void HollowCylinder(TRandom3& ran, double R1, double R2, double h1, double h2); //moves a point randomly between two cylinders of respective radii R1 and R2 and of respective height h1 and h2
  
};

std::ostream& operator<<(std::ostream& output, const Point& p);

#endif
