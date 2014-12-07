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
    
    if ( point.x < p0.x || point.x > p1.x )  {
      return false;
    }

    if ( point.z < p0.z || point.z > p1.z )  {
      return false;
    }
        
    if ( ( point.y - p0.y ) > 5 || ( p1.y - point.y ) > 5 )  {
      return false;
    }
    

    return true;
  }
};

class Level {
 public:
  int nextId;
  int jumps;
  Vec3 player;
  Vec3 playerSpeed;
  Vec3 camera;
  Vec3 cameraSpeed;
  std::vector< Plane *> planes;

  void updateGame() {
   
    Vec3 gravity( 0, -2, 0 );
    
    camera.add( cameraSpeed );
    player.add( playerSpeed );
    
    player.add( gravity );
    
    cameraSpeed.scale( 0.75f );
    playerSpeed.scale( 0.125f );
    
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
    

    //    std::cout << player.x << ", " << player.y << ", " << player.z << std::endl;

    Plane *plane;
    
    for ( int c = 0; c < planes.size(); ++c ) {
      plane = planes[ c ];
      
      if ( plane->hit( player ) ) {
	player.y = plane->p0.y;

	if ( c == nextId ) {
	  //	  planes.erase( planes.begin() + c );
	  ++nextId;

	  if ( nextId == planes.size() ) {
	    std::cout << "You won!" << std::endl;
	    exit( 0 );
	  }
	}

	return;
      }
    }

    if ( player.y <= 0.1f ) {
      player.y = 0.1f;
    }    

  }
};




#endif
