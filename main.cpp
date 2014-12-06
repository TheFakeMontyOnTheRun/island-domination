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
    level.cameraSpeed.x -= 10.0f;
    level.playerSpeed.x -= 50.0f;
    break;
  case SDLK_w:
    //    speed.y += 10.0f;
    level.playerSpeed.z += 1.0f;
    break;
  case SDLK_s:
    level.cameraSpeed.x += 10.0f;
    level.playerSpeed.x += 50.0f;
    break;
  case SDLK_z:
    // speed.y -= 10.0f;
    level.playerSpeed.z -= 1.0f;
    break;

  case SDLK_e:
    level.playerSpeed.y += 140.0f;
    break;
  }
}

void initLevel( Level &level ) {
  Plane *plane;
  
  plane = new Plane();
  plane->p0.set( 25, 200, 5 );
  plane->p1.set( 100, 200, 7 );  
  plane->r = 255;
  plane->g = 255;
  plane->b = 0;
  level.planes.push_back( plane );

  plane = new Plane();
  plane->p0.set( 400, 300, 1 );
  plane->p1.set( 500, 300, 3 );  
  plane->r = 255;
  plane->g = 255;
  plane->b = 255;
  level.planes.push_back( plane );

  plane = new Plane();
  plane->p0.set( -600, 100, 8 );
  plane->p1.set( -400, 100, 10 );  
  plane->r = 255;
  plane->g = 255;
  plane->b = 0;
  level.planes.push_back( plane );
}


int main ( int argc, char **argv ) {
  
  SDL_Surface *video;
  SDL_Event event;
  
  Level level;
  level.camera.set( 0, 200, 0 );
  level.cameraSpeed.set( 0, 0, 0 );
  level.player.set( 6, 1000, 6 );
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
