#include <vector>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "base.h"
#include "game.h"

#include "video.h"


void draw3DLine( SDL_Surface *video, Vec3 &camera, Vec3 &p0, Vec3 &p1, int r, int g, int b ) {

  glColor4f( r / 255.0f, g / 255.0f, b / 255.0f, 1.0f );
	
  glBegin( GL_LINES );
  
  glVertex3f( p0.x / 50.0f, p0.y / 50.0f, -p0.z * 10.0f );
  glVertex3f( p1.x / 50.0f, p1.y / 50.0f, -p1.z * 10.0f );
  glEnd();

}


void drawXZPlane( SDL_Surface *video, Vec3 &camera, Vec3 &p0, Vec3 &p1, int r, int g, int b ) {

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

  draw3DLine( video, camera, _p0, _p1, r, g, b );
  draw3DLine( video, camera, _p2, _p1, r, g, b );
  draw3DLine( video, camera, _p2, _p3, r, g, b );
  draw3DLine( video, camera, _p0, _p3, r, g, b );
}


void drawCube( SDL_Surface *video, Vec3 &camera, Vec3 &p0, Vec3 &p1, int r, int g, int b ) {
  Vec3 _p0( p0 );
  Vec3 _p1( p1 );

  _p1.y = p0.y;
  drawXZPlane( video, camera, _p0, _p1, r, g, b );

  _p1.y = p1.y;
  _p0.y = p1.y;
  drawXZPlane( video, camera, _p0, _p1, r, g, b );

  _p0.set( p0 );
  _p1.set( p0 );
  _p1.y = p1.y;
  draw3DLine( video, camera, _p0, _p1, r, g, b );

  _p0.set( p1 );
  _p1.set( p1 );
  _p1.y = p0.y;
  draw3DLine( video, camera, _p0, _p1, r, g, b );


  _p0.set( p1 );
  _p1.set( p1 );
  _p1.y = p0.y;
  _p0.z = p0.z;
  _p1.z = p0.z;
  draw3DLine( video, camera, _p0, _p1, r, g, b );



  _p0.set( p0 );
  _p1.set( p0 );
  _p1.y = p1.y;
  _p0.z = p1.z;
  _p1.z = p1.z;
  draw3DLine( video, camera, _p0, _p1, r, g, b );



}

void refreshScreen( SDL_Surface *video, Level &level ) {

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glLoadIdentity( );
	glTranslatef( -level.camera.x / 50.0f, -level.camera.y / 50.0f, level.camera.z * 10.0f );

  Plane *plane;

  SDL_Rect rect;
  Uint32 colour;
  Vec3 p0;
  Vec3 p1;
  
  rect.x = 0;
  rect.y = 0;
  rect.w = XRES;
  rect.h = YRES;  
  colour = SDL_MapRGB( video->format, 0, 0, 0 );
  //  SDL_FillRect( video, &rect, colour );


  rect.x = 0;
  rect.y = YRES - 40;
  rect.w = ( XRES / DEFAULTJUMPS ) * level.jumps;
  rect.h = 20;  
  colour = SDL_MapRGB( video->format, 255, 255, 255 );
  //  SDL_FillRect( video, &rect, colour );

  rect.x = 0;
  rect.y = YRES - 20;
  rect.w = ( XRES / DEFAULTTIME ) * level.timeLeft;
  rect.h = 20;  
  colour = SDL_MapRGB( video->format, 128, 128, 128 );
  //  SDL_FillRect( video, &rect, colour );



  plane = level.planes[ level.nextId ];
  
  rect.x = XRES - 50;
  rect.y = 0;
  rect.w = 50;
  rect.h = 50;  
  colour = SDL_MapRGB( video->format, plane->r, plane->g, plane->b );
  //  SDL_FillRect( video, &rect, colour );

  p0.set( -600, 0, 1 );
  p1.set( 600, 0, 10 );  
  drawXZPlane( video, level.camera, p0, p1, 0, 255, 0 );
  
  for ( int c = 0; c < level.planes.size(); ++c ) {
    plane = level.planes[ c ];
    drawXZPlane( video, level.camera, plane->p0, plane->p1, plane->r, plane->g, plane->b );
  }

  
  Particle *p;
  for ( int c = 0; c < level.player.jetpack.particles.size(); ++c ) {
    p = level.player.jetpack.particles[ c ];
    draw3DLine( video, level.camera, p->position, p->position, 255 - p->size, 0, 0 );
  }
  

  p0.set( level.player.bodyRep.position.x - 20, level.player.bodyRep.position.y, level.player.bodyRep.position.z );
  p1.set( level.player.bodyRep.position.x + 20, level.player.bodyRep.position.y + 100, level.player.bodyRep.position.z + 0.1f );  
  drawCube( video, level.camera, p0, p1, 0, 0, 255 );
	SDL_GL_SwapBuffers( );
  //SDL_UpdateRect( video, 0, 0, 0, 0 );

}

void showScreen( SDL_Surface *video, SDL_Surface *screen ) {

  return ;

  SDL_Event event;                                                                                    

  SDL_BlitSurface( screen, NULL, video, NULL );
  SDL_Flip( video );                                                                                     
  while ( true ) {                                                                                          if ( SDL_PollEvent( &event ) ) {                                                                          if ( event.type == SDL_KEYDOWN ) {                                                                	return;                                                                                               }                                                                                                 
    }                                                                                                      }                                                                                                     } 
