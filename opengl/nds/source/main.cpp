#include <iostream>
#include <nds.h>
#include <vector>
#include <math.h>
#include "base.h"
#include "game.h"
#include "video.h"


bool running = true;
bool playSounds = true;

void handleEvents(  Level &level ) {
  scanKeys();

  if ( keysHeld() & KEY_LEFT ) {
    level.player.bodyRep.speed.x -= 50.0f;
    level.cameraSpeed.x -= 10.0f;
  } else if ( keysHeld() & KEY_UP ) {
    level.player.bodyRep.speed.z += 0.5f;
  } else if ( keysHeld() & KEY_RIGHT ) {
    level.player.bodyRep.speed.x += 50.0f;
    level.cameraSpeed.x += 10.0f;
  } else if ( keysHeld() & KEY_DOWN ) {
    level.player.bodyRep.speed.z -= 0.5f;
  } else {
    // if ( level.jumps > 0 ) {
      level.player.bodyRep.speed.y += 50.0f;
      level.cameraSpeed.y += 5.0f;
      //      --level.jumps;
      //    }
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
    glClearColor( 0.0f, 0.0f, 0.0f, 31 );
    glClearDepth( 0x7FFFF );
    glColor3f( 1.0f, 1.0f, 1.0f);
}

int main ( int argc, char **argv ) {
    
  Level level;
  level.camera.set( 0, 200, 0 );
  level.cameraSpeed.set( 0, 0, 0 );
  level.player.bodyRep.position.set( 6, 2, 1 );
  level.player.bodyRep.speed.set( 0, 0, 0 );
  
  videoSetMode(MODE_0_3D);

  glInit();
  initGL();
  resizeWindow( 255, 191 );
  initLevel( level );

  
  clock_t t0;
  clock_t t1;
  float delta;
  
  


  while ( 1 ) {
    glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);  
    //    t0 = clock();
    level.updateGame();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    refreshScreen( level );
    /*    
    glColor3f( 1, 1, 1 );

    glBegin(GL_TRIANGLES);
    glVertex3f( 0.0f, 10.0f, -10.0f);                                  // Top                   
    glVertex3f(-10.0f,-10.0f, -10.0f);                                  // Bottom Left           
    glVertex3f( 10.0f,-10.0f, -10.0f);                                  // Bottom Right          
    glEnd();  
    */
    glPopMatrix(1);    
    swiWaitForVBlank();
    glFlush(0);
    handleEvents( level );
    //    t1 = clock();
    //    delta = ((((float) t1 ) - ((float)t0)) / CLOCKS_PER_SEC );
  }
  
  return 0;
}