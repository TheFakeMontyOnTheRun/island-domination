#include <vector>
#include <iostream>
#include <nds.h>
#include "base.h"
#include "game.h"

#include "video.h"


void draw3DLine(  Vec3 &camera, Vec3 &p0, Vec3 &p1, int r, int g, int b ) {

  glColor3f( 1, 0, 0 );
	
  glBegin( GL_TRIANGLES );
  glVertex3f( p0.x / 50.0f, p0.y / 50.0f, -10.0f -p0.z * 10.0f );
  glVertex3f( p1.x / 50.0f, p1.y / 50.0f, -10.0f -p1.z * 10.0f );
  glVertex3f( p1.x / 50.0f, p1.y / 50.0f, -10.0f -p1.z * 10.0f );
  glEnd();
}


void drawXZPlane(  Vec3 &camera, Vec3 &p0, Vec3 &p1, int r, int g, int b ) {

  Vec3 _p0;
  Vec3 _p1;
  Vec3 _p2;
  Vec3 _p3;

  _p0.set( p0 );
  _p1.set( p1 );
  
  _p0.y = p0.y + ( ( p1.y - p0.y ) / 2.0f );
  _p1.y = _p0.y;
  _p2.y = _p0.y;
  _p3.y = _p0.y;

  _p1.set( p1.x, _p0.y, p0.z );
  _p2.set( p1.x, _p0.y, p1.z );
  _p3.set( p0.x, _p0.y, p1.z );

  draw3DLine( camera, _p0, _p1, r, g, b );
  draw3DLine( camera, _p2, _p1, r, g, b );
  draw3DLine( camera, _p2, _p3, r, g, b );
  draw3DLine( camera, _p0, _p3, r, g, b );
}


void drawCube( Vec3 &camera, Vec3 &p0, Vec3 &p1, int r, int g, int b ) {



  Vec3 _p0( p0 );
  Vec3 _p1( p1 );

  _p1.y = p0.y;
  drawXZPlane(  camera, _p0, _p1, r, g, b );


  _p1.y = p1.y;
  _p0.y = p1.y;
  drawXZPlane(  camera, _p0, _p1, r, g, b );

  _p0.set( p0 );
  _p1.set( p0 );
  _p1.y = p1.y;
  draw3DLine( camera, _p0, _p1, r, g, b );

  _p0.set( p1 );
  _p1.set( p1 );
  _p1.y = p0.y;
  draw3DLine(  camera, _p0, _p1, r, g, b );


  _p0.set( p1 );
  _p1.set( p1 );
  _p1.y = p0.y;
  _p0.z = p0.z;
  _p1.z = p0.z;
  draw3DLine(  camera, _p0, _p1, r, g, b );



  _p0.set( p0 );
  _p1.set( p0 );
  _p1.y = p1.y;
  _p0.z = p1.z;
  _p1.z = p1.z;
  draw3DLine(  camera, _p0, _p1, r, g, b );
}

void refreshScreen( Level &level ) {

  //    glLoadIdentity( );
  //  glTranslatef( level.camera.x / 300.0f, level.camera.y / 300.0f, level.camera.z * 10.0f );

  glTranslatef( 0.0f, -1.0f, -5.0f );

  Plane *plane;

  Vec3 p0;
  Vec3 p1;
  

  p0.set( -600, 0, 1 );
  p1.set( 600, 0, 10 );  
  drawXZPlane(  level.camera, p0, p1, 0, 255, 0 );



  for ( int c = 0; c < level.planes.size(); ++c ) {
    plane = level.planes[ c ];
    drawXZPlane( level.camera, plane->p0, plane->p1, plane->r, plane->g, plane->b );
  }


  level.player.bodyRep.position.y = 0;

  p0.set( level.player.bodyRep.position.x - 20, level.player.bodyRep.position.y, level.player.bodyRep.position.z );
  p1.set( level.player.bodyRep.position.x + 20, level.player.bodyRep.position.y + 100, level.player.bodyRep.position.z + 0.1f );  
  drawCube( level.camera, p0, p1, 0, 0, 255 );
}
