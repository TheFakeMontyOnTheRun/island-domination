#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>

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

void resizeWindow( float width, float height )
{
    float ratio = width / height;

    glViewport( 0, 0, width, height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( 45.0f, ratio, 0.1f, 100.0f );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
}

void initGL()
{
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );
}

int main ( int argc, char **argv ) {
  
  SDL_Surface *video;
  SDL_Event event;
  
  Level level;
  level.camera.set( 0, 200, 0 );
  level.cameraSpeed.set( 0, 0, 0 );
  level.player.bodyRep.position.set( 6, 2, 1 );
  level.player.bodyRep.speed.set( 0, 0, 0 );

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
  
  video = SDL_SetVideoMode( 640, 480, 16, SDL_OPENGL );
  
	initGL();
  initLevel( level );
    resizeWindow( 640, 480 );
  
  clock_t t0;
  clock_t t1;
  float delta;

  while ( running ) {
    
    t0 = clock();

    level.updateGame( gotIt, step, video );
    refreshScreen( video, level );
    SDL_GL_SwapBuffers( );    
    
    if ( SDL_PollEvent( &event ) ) {
      
      if( event.type == SDL_QUIT ) {
	running = false;
      }
      
      handleEvents( event, level );
    }

    t1 = clock();
 
    delta = ((((float) t1 ) - ((float)t0)) / CLOCKS_PER_SEC );
  }
  
  SDL_FreeSurface( video );
  Mix_FreeChunk( jetSound );
  
  SDL_Quit();
  
  return 0;
}
