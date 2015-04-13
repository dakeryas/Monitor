#include "Point.hpp"

ClassImp(Point)

using namespace std;

ostream& operator<<(ostream& output, const Point& p){
  
  output<<"x = "<<p.x<<"\ty = "<<p.y<<"\tz = "<<p.z;
  return output;
  
}

Point::Point():x(0),y(0),z(0){
  
}

Point::Point(double x, double y, double z):x(x),y(y),z(z){
  
}

Point::Point(TRandom3& ran, double R, double h){ //generates a point randomly into a cylinder of radius r and height h
  
  double theta = ran.Uniform(0,2*3.14159); //the elementary volume is r*d_theta*d_r*dz = d(r²/2)dz
  double r = sqrt(ran.Uniform(0,pow(R,2))); //r²/2 has to be chosen uniformly between 0 and R²/2
  x = r*cos(theta);
  y = r*sin(theta);
  z = ran.Uniform(-h/2, h/2);
  
}

Point::Point(TRandom3& ran, double R1, double R2, double h1, double h2){ //generates a point randomly between two cylinders of respective radii R1 and R2 and of respective height h1 and h2
  
  double r, theta;
  
  if(R1>R2){ // if the values we given in the wrong order, ie, cylinder1 is larger than cylinder2 swap the radii
    
    double Rtemp = R2;
    R2 = R1;
    R1 = Rtemp;
    
  }
  
  if(h1>h2){
    
    double htemp = h2; //if the values we given in the wrong order, ie cylinder1 is larger than cylinder2 swap the heights
    h2 = h1;
    h1 = htemp;
    
  }
  
  z = ran.Uniform(-h2/2, h2/2);
  if((h1/2<z && z <h2/2) ||(-h2/2<z && z<-h1/2)){ // in the upper and lower parts of the volume cylinder2-cylinder1, there is no restriction of the value of r, it can be as small as 0
    
      r = sqrt(ran.Uniform(0,pow(R2,2))); //r²/2 has to be chosen uniformly between 0 and R²/2
      
  }
  else r = sqrt(ran.Uniform(pow(R1,2),pow(R2,2))); // as for the hollow part of the volume C2-C1, r cannot be infinitely small
  
  theta = ran.Uniform(0,2*3.14159); //the elementary volume is r*d_theta*d_r*dz = d(r²/2)dz
  x = r*cos(theta);
  y = r*sin(theta);
  
}

Point::~Point()
{}

void Point::cylinder(TRandom3& ran, double R, double h){ //moves a point randomly into a cylinder of radius r and height h
  
  double theta = ran.Uniform(0,2*3.14159); //the elementary volume is r*d_theta*d_r*dz = d(r²/2)dz
  double r = sqrt(ran.Uniform(0,pow(R,2))); //r²/2 has to be chosen uniformly between 0 and R²/2
  x = r*cos(theta);
  y = r*sin(theta);
  z = ran.Uniform(-h/2, h/2);
  
}

void Point::HollowCylinder(TRandom3& ran, double R1, double R2, double h1, double h2){//moves a point randomly between two cylinders of respective radii R1 and R2 and of respective height h1 and h2
  
  double r, theta;
  
  if(R1>R2){ // if the values we given in the wrong order, ie, cylinder1 is larger than cylinder2 swap the radii
    
    double Rtemp = R2;
    R2 = R1;
    R1 = Rtemp;
    
  }
  
  if(h1>h2){
    
    double htemp = h2; //if the values we given in the wrong order, ie cylinder1 is larger than cylinder2 swap the heights
    h2 = h1;
    h1 = htemp;
    
  }
  
  z = ran.Uniform(-h2/2, h2/2);
  if((h1/2<z && z <h2/2) ||(-h2/2<z && z<-h1/2)){ // in the upper and lower parts of the volume cylinder2-cylinder1, there is no restriction of the value of r, it can be as small as 0
    
      r = sqrt(ran.Uniform(0,pow(R2,2))); //r²/2 has to be chosen uniformly between 0 and R²/2
      
  }
  else r = sqrt(ran.Uniform(pow(R1,2),pow(R2,2))); // as for the hollow part of the volume C2-C1, r cannot be infinitely small
  
  theta = ran.Uniform(0,2*3.14159); //the elementary volume is r*d_theta*d_r*dz = d(r²/2)dz
  x = r*cos(theta);
  y = r*sin(theta);
    
}