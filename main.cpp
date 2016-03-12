#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <vector>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "base.h"
#include "game.h"

#include "video.h"



bool running = true;
bool playSounds = true;
Mix_Chunk *jetSound = nullptr;
Mix_Chunk *gotIt = nullptr;
Mix_Chunk *step = nullptr;

void handleEvents( SDL_Event &event, Level &level ) {

  switch ( event.key.keysym.sym ) {
  case SDLK_q:
    running = false;
  case SDLK_a:
    level.player.bodyRep.speed.x -= 50.0f;
  //case SDLK_LEFT:
    level.cameraSpeed.x -= 10.0f;
    break;
  case SDLK_w:
    //    speed.y += 10.0f;
    level.player.bodyRep.speed.z += 0.5f;
    break;
  case SDLK_s:
    level.player.bodyRep.speed.x += 50.0f;
  //case SDLK_RIGHT:
    level.cameraSpeed.x += 10.0f;
    break;
  case SDLK_z:
    // speed.y -= 10.0f;
    level.player.bodyRep.speed.z -= 0.5f;
    break;

  case SDLK_e:
    if ( level.jumps > 0 ) {
      level.player.bodyRep.speed.y += 50.0f;
      level.cameraSpeed.y += 5.0f;
      --level.jumps;

      if ( playSounds ) {
	Mix_PlayChannel( -1, jetSound, 0 );
      }
    }
    break;

  default:
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
}

void showTitleScreen( SDL_Surface *video ) {

  SDL_Surface *title;
  title = IMG_Load( "res/title.png" );
  showScreen( video, title );
  SDL_FreeSurface( title );

  SDL_Rect rect;

  rect.x = 0;
  rect.y = 0;
  rect.w = XRES;
  rect.h = YRES;
  SDL_FillRect( video, &rect, SDL_MapRGB( video->format, 0, 0, 0 ) );

  title = IMG_Load( "res/history.png" );
  showScreen( video, title );
  SDL_FreeSurface( title );
}

  SDL_Surface *video;
  SDL_Event event;
  
  Level level;


void mainLoop() {
    level.updateGame( gotIt, step, video );
    refreshScreen( video, level );
    
    
    if ( SDL_PollEvent( &event ) ) {
      
      if( event.type == SDL_QUIT ) {
	running = false;
      }
      
      handleEvents( event, level );
    }
}

int main ( int argc, char **argv ) {
  

  level.camera.set( 0, 200, 0 );
  level.cameraSpeed.set( 0, 0, 0 );
  level.player.bodyRep.position.set( 6, 2, 1 );
  level.player.bodyRep.speed.set( 0, 0, 0 );

  initLevel( level );

  SDL_Init( SDL_INIT_EVERYTHING );

  if ( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) {
      std::cout << "Could not start sound system..." << std::endl;
      playSounds = false;
    } else {
      jetSound = Mix_LoadWAV( "res/jet.wav" );
      gotIt = Mix_LoadWAV( "res/gotIt.wav" );
      //      step = Mix_LoadWAV( "res/step.wav" );
      std::cout << "Sound ready!" << std::endl;
      playSounds = true;
  }
  
  video = SDL_SetVideoMode( 640, 480, 0, 0 );

  //  showTitleScreen( video );

  emscripten_set_main_loop(mainLoop, 30, 1);

  
  return 0;
}
