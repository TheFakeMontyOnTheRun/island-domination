#ifndef PLAT_game_H
#define PLAT_game_H

class Plane {
 public:
  
  Vec3 p0;
  Vec3 p1;
  int r;
  int g;
  int b;
  int id;

  Plane() {
    r = g = b = 0;
    p0.set( 0, 0, 0 );
    p1.set( p0 );
  }
  
  bool hit( Vec3 &point ) {
    
    if ( p0.x <= point.x && point.x <= p1.x )  {
      if ( p0.z <= point.z && point.z <= p1.z )  {
	if ( fabs( p0.y - point.y ) < 0.1f || fabs( point.y - p1.y ) < 0.1f )  {
	  return true;
	}
      }
    }
    
    return false;
  }
};

class Level {

  int initialTime;

 public:
  int nextId;
  int jumps;
  int timeLeft;
  Vec3 player;
  Vec3 playerSpeed;
  Vec3 camera;
  Vec3 cameraSpeed;
  std::vector< Plane *> planes;

  Level() {
    initialTime = time( 0 );
  }

  void updateGame() {
    
    timeLeft = DEFAULTTIME - ( time( 0 ) - initialTime );

    if ( timeLeft < 0 ) {

      std::cout << std::endl << "Time up!" << std::endl;      
      exit( 0 );
    }
    
    Vec3 gravity( 0, -2, 0 );
    
    camera.add( cameraSpeed );
    player.add( playerSpeed );
    
    
    cameraSpeed.scale( 0.75f );
    playerSpeed.scale( 0.125f );
    
    if ( camera.y > player.y + 10 ) {
      cameraSpeed.y = -1;
    }
    
    
    if ( player.z <= 1.0f ) {
      player.z = 1.0f;
    }
    

    if ( player.z >= 10.0f ) {
      player.z = 10.0f;
    }
    
    if ( player.x <= -600.0f ) {
      player.x = -600.0f;
    }
    
    if ( player.x >= 600.0f ) {
      player.x = 600.0f;
    }
    

    Plane *plane;
    
    for ( int c = 0; c < planes.size(); ++c ) {
      plane = planes[ c ];
      

      if ( plane->hit( player ) ) {
	
	player.y = plane->p0.y;
	playerSpeed.y = 0;
      
	if ( c == nextId ) {
	  ++nextId;
	  
	  if ( nextId == planes.size() ) {
	    std::cout << std::endl << "You won!" << std::endl;
	    exit( 0 );
	  }
	}
	
	return;
      }
    }

    player.add( gravity );

    if ( player.y <= 0.1f ) {
      player.y = 0.1f;
    }
  }
};




#endif
