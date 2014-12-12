#include <vector>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include "base.h"
#include "game.h"

#include "video.h"


void draw3DLine( SDL_Surface *video, Vec3 &camera, Vec3 &p0, Vec3 &p1, int r, int g, int b ) {

  float x0 = ( XRES / 2 ) + ( ( p0.x - camera.x ) / ( p0.z - camera.z ) );
  float y0 = ( YRES / 2 ) - ( ( p0.y - camera.y ) / ( p0.z - camera.z ) );
  float x1 = ( XRES / 2 ) + ( ( p1.x - camera.x ) / ( p1.z - camera.z ) );
  float y1 = ( YRES / 2 ) - ( ( p1.y - camera.y ) / ( p1.z - camera.z ) );

  lineRGBA( video, x0, y0, x1, y1, r, g, b, 255 );
  lineRGBA( video, 100 +  p0.x / 10, 100 - p0.z * 10, 100 +  p1.x / 10, 100 - p1.z * 10, r, g, b, 255 );
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
  SDL_FillRect( video, &rect, colour );


  rect.x = 0;
  rect.y = YRES - 40;
  rect.w = ( XRES / DEFAULTJUMPS ) * level.jumps;
  rect.h = 20;  
  colour = SDL_MapRGB( video->format, 255, 255, 255 );
  SDL_FillRect( video, &rect, colour );

  rect.x = 0;
  rect.y = YRES - 20;
  rect.w = ( XRES / DEFAULTTIME ) * level.timeLeft;
  rect.h = 20;  
  colour = SDL_MapRGB( video->format, 128, 128, 128 );
  SDL_FillRect( video, &rect, colour );



  plane = level.planes[ level.nextId ];
  
  rect.x = XRES - 50;
  rect.y = 0;
  rect.w = 50;
  rect.h = 50;  
  colour = SDL_MapRGB( video->format, plane->r, plane->g, plane->b );
  SDL_FillRect( video, &rect, colour );

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

  SDL_UpdateRect( video, 0, 0, 0, 0 );

}

void showScreen( SDL_Surface *video, SDL_Surface *screen ) {
  SDL_Event event;                                                                                    

  SDL_BlitSurface( screen, NULL, video, NULL );
  SDL_Flip( video );                                                                                     
  while ( true ) {                                                                                          if ( SDL_PollEvent( &event ) ) {                                                                          if ( event.type == SDL_KEYDOWN ) {                                                                	return;                                                                                               }                                                                                                 
    }                                                                                                      }                                                                                                     } 
