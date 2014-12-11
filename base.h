#include <sstream>

#ifndef PLAT_base_H
#define PLAT_base_H

#define XRES 640
#define YRES 480
#define XBOARDSIZE 1000
#define ZBOARDSIZE 20
#define DEFAULTJUMPS 30
#define DEFAULTTIME 40

class Vec2 {
public:
  float x;
  float y;

  Vec2( float _x, float _y) {
    this->x = _x;
    this->y = _y;
  }
};

class Vec3 {
public:
  float x;
  float y;
  float z;

  Vec3( float _x, float _y, float _z ) {
    set( _x, _y, _z );
  }

  Vec3() {
    set( 0, 0, 0 );
  }

  void set( float _x, float _y, float _z ) {
    this->x = _x;
    this->y = _y;
    this->z = _z;
  }

  void set( Vec3 &other ) {
    set( other.x, other.y, other.z );
 }

  void add( Vec3 &other ) {
    x += other.x;
    y += other.y;
    z += other.z;
  }

  void scale( float sx, float sy, float sz ) {
    x *= sx;
    y *= sy;
    z *= sz;
  }

  void scale( float factor ) {
    x *= factor;
    y *= factor;
    z *= factor;
  }

  std::string toString() {
    std::stringstream toReturn;
    toReturn << "( ";
    toReturn << (x);
    toReturn << ", ";
    toReturn << (y);
    toReturn << ", ";
    toReturn << (z);
    toReturn << " )";
    
    return toReturn.str();
  }
};
#endif
