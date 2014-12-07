#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <vector>

#include "base.h"
#include "game.h"

#include "video.h"

bool running = true;



void handleEvents( SDL_Event &event, Level &level ) {

  switch ( event.key.keysym.sym ) {
  case SDLK_q:
    running = false;
  case SDLK_a:
    level.playerSpeed.x -= 50.0f;
  case SDLK_LEFT:
    level.cameraSpeed.x -= 10.0f;
    break;
  case SDLK_w:
    //    speed.y += 10.0f;
    level.playerSpeed.z += 0.75f;
    break;
  case SDLK_s:
    level.playerSpeed.x += 50.0f;
  case SDLK_RIGHT:
    level.cameraSpeed.x += 10.0f;
    break;
  case SDLK_z:
    // speed.y -= 10.0f;
    level.playerSpeed.z -= 0.75f;
    break;

  case SDLK_e:
    if ( level.jumps > 0 ) {
      level.playerSpeed.y += 140.0f;
      level.cameraSpeed.y += 5.0f;
      --level.jumps;
    }
    break;
  }
}

void initLevel( Level &level ) {
  Plane *plane;

  plane = new Plane();
  plane->p0.set( -500, 400, 1 );
  plane->p1.set( -300, 400, 5 );  
  plane->r = 255;
  plane->g = 0;
  plane->b = 0;
  plane->id = 1;
  level.planes.push_back( plane );

  
  plane = new Plane();
  plane->p0.set( 100, 250, 5 );
  plane->p1.set( 300, 250, 7 );  
  plane->r = 255;
  plane->g = 255;
  plane->b = 0;
  plane->id = 2;
  level.planes.push_back( plane );

  plane = new Plane();
  plane->p0.set( 400, 150, 1 );
  plane->p1.set( 500, 150, 3 );  
  plane->r = 255;
  plane->g = 255;
  plane->b = 255;
  plane->id = 3;
  level.planes.push_back( plane );

  plane = new Plane();
  plane->p0.set( -300, 300, 8 );
  plane->p1.set( 0, 300, 10 );  
  plane->r = 255;
  plane->g = 128;
  plane->b = 0;
  plane->id = 4;
  level.planes.push_back( plane );


  plane = new Plane();
  plane->p0.set( -600, 400, 8 );
  plane->p1.set( -400, 400, 10 );  
  plane->r = 64;
  plane->g = 64;
  plane->b = 64;
  plane->id = 5;

  level.planes.push_back( plane );

  level.nextId = 0;
  level.jumps = DEFAULTJUMPS;
}


int main ( int argc, char **argv ) {
  
  SDL_Surface *video;
  SDL_Event event;
  
  Level level;
  level.camera.set( 0, 200, 0 );
  level.cameraSpeed.set( 0, 0, 0 );
  level.player.set( 6, 2, 1 );
  level.playerSpeed.set( 0, 0, 0 );


  initLevel( level );


   
  SDL_Init( SDL_INIT_EVERYTHING );
  
  video = SDL_SetVideoMode( 640, 480, 0, 0 );
  
  while ( running ) {

    level.updateGame();
    refreshScreen( video, level );

    
    if ( SDL_PollEvent( &event ) ) {

      if( event.type == SDL_QUIT ) {
	running = false;
      }
  
      handleEvents( event, level );
    }
  }
  
  SDL_Quit();
  
  return 0;
}
