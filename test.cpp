#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#define XRES 640
#define YRES 480
#define XBOARDSIZE 1000
#define ZBOARDSIZE 20


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
};

bool running = true;
Vec3 camera( 0, 200, 0 );
Vec3 speed( 0, 0, 0 );
Vec3 player( 6, 1000, 6 );
Vec3 playerSpeed( 0, 0, 0 );
Vec3 gravity( 0, -2, 0 );

void updateGame() {
  camera.add( speed );
  player.add( playerSpeed );
  player.add( gravity );

  speed.scale( 0.9 );
  playerSpeed.scale( 0.1f );

  if ( player.z <= 0.1f ) {
    player.z = 0.1f;
  }

  if ( player.y <= 0.1f ) {
    player.y = 0.1f;
  }

}

void draw3DLine( SDL_Surface *video, Vec3 &p0, Vec3 &p1, Uint8 r, Uint8 g, Uint8 b ) {

  float x0 = ( XRES / 2 ) + ( ( p0.x - camera.x ) / ( p0.z - camera.z ) );
  float y0 = ( YRES / 2 ) - ( ( p0.y - camera.y ) / ( p0.z - camera.z ) );
  float x1 = ( XRES / 2 ) + ( ( p1.x - camera.x ) / ( p1.z - camera.z ) );
  float y1 = ( YRES / 2 ) - ( ( p1.y - camera.y ) / ( p1.z - camera.z ) );

  lineRGBA( video, x0, y0, x1, y1, r, g, b, 255 );

}


void drawXZPlane( SDL_Surface *video, Vec3 &p0, Vec3 &p1, Uint8 r, Uint8 g, Uint8 b ) {

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

  draw3DLine( video, _p0, _p1, r, g, b );
  draw3DLine( video, _p2, _p1, r, g, b );
  draw3DLine( video, _p2, _p3, r, g, b );
  draw3DLine( video, _p0, _p3, r, g, b );
}


void drawCube( SDL_Surface *video, Vec3 &p0, Vec3 &p1, Uint8 r, Uint8 g, Uint8 b ) {
  Vec3 _p0( p0 );
  Vec3 _p1( p1 );

  _p1.y = p0.y;
  drawXZPlane( video, _p0, _p1, r, g, b );

  _p1.y = p1.y;
  _p0.y = p0.y;
  drawXZPlane( video, _p0, _p1, r, g, b );

}

void refreshScreen( SDL_Surface *video ) {

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
  
  p0.set( -600, 0, 1 );
  p1.set( 600, 0, 10 );  
  drawXZPlane( video, p0, p1, 0, 255, 0 );

  p0.set( player.x - 20, player.y - 50, player.z );
  p1.set( player.x + 20, player.y + 50, player.z + 0.1f );  


  drawCube( video, p0, p1, 0, 0, 255 );

  SDL_UpdateRect( video, 0, 0, 0, 0 );

}

void handleEvents( SDL_Event &event ) {

  switch ( event.key.keysym.sym ) {
  case SDLK_q:
    running = false;
  case SDLK_a:
    speed.x -= 10.0f;
    playerSpeed.x -= 50.0f;
    break;
  case SDLK_w:
    //    speed.y += 10.0f;
    playerSpeed.z += 1.0f;
    break;
  case SDLK_s:
    speed.x += 10.0f;
    playerSpeed.x += 50.0f;
    break;
  case SDLK_z:
    // speed.y -= 10.0f;
    playerSpeed.z -= 1.0f;
    break;

  case SDLK_e:
    playerSpeed.y += 40.0f;
    break;
  }
}


int main ( int argc, char **argv ) {
  
  SDL_Surface *video;
  SDL_Event event;
   
  SDL_Init( SDL_INIT_EVERYTHING );
  
  video = SDL_SetVideoMode( 640, 480, 0, 0 );
  
  while ( running ) {

    updateGame();
    refreshScreen( video );

    
    if ( SDL_PollEvent( &event ) ) {

      if( event.type == SDL_QUIT ) {
	running = false;
      }
  
      handleEvents( event );
    }
  }
  
  SDL_Quit();
  
  return 0;
}
