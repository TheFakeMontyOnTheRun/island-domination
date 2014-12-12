#include <cstdlib>
#include <time.h> //required by MinGW32

#ifndef PLAT_game_H
#define PLAT_game_H

extern void showScreen( SDL_Surface *video, SDL_Surface *screen );

class Plane {
 public:
  
  Vec3 p0;
  Vec3 p1;
  int r;
  int g;
  int b;
  int id;

  Plane() {
    id = 0;
    r = g = b = 0;
    p0.set( 0, 0, 0 );
    p1.set( p0 );
  }
  
  bool hit( Vec3 &point, float delta = 0 ) {
    
    if ( p0.x <= point.x && point.x <= p1.x )  {
      if ( p0.z <= point.z && point.z <= p1.z )  {
	if ( fabs( point.y - p0.y ) <= 50 || fabs( point.y - p1.y ) <= 50 )  {
	  return true;
	}
      }
    }
    
    return false;
  }
};

class Particle {
	public:

	int size;
	Vec3 position;
	Vec3 speed;
	Vec3 accel;

	Particle() {
		size = 64;
	}

	void update() {
		--size;
		position.add( speed );
		speed.add( accel );

		if ( position.y <= 0 ) {
			speed.y = accel.y = position.y = 0;
		}
	}
};

class ParticleEmitter {
	public:
	int intensity;
	bool active;
	Particle origin;
	std::vector< Particle* >particles;
	
	ParticleEmitter() {
		active = true;
		intensity = 64;
	}

	void update() {
		Particle *p;
	
		if ( active ) {
			for ( int c = 0; c < intensity; ++c ) {
				p = new Particle();
				p->position.set( origin.position );
				p->accel.set( -0.01f * RAND_MAX + 0.02f * rand(), -2.0f * RAND_MAX, 0.0f );
				p->accel.scale( 1.0f / RAND_MAX );
//				p->accel.set( 0.0f, -0.25f, 0.0f );
				particles.push_back( p );
								
			}	
		}

		for ( int c = 0; c < particles.size(); ++c ) {
			p = particles[ c ];
			p->update();
		}
	
	
		bool changed = true;
		while ( changed ) {
			changed = false; 
			for ( int c = 0; c < particles.size(); ++c ) {
				p = particles[ c ];
	
				if ( p->size <= 2 ) {
					particles.erase( particles.begin() + c );
					changed = true;
	
					break;
				}
			}
		}
	
	}
};

class Body {
	public:
	Particle bodyRep;
	ParticleEmitter jetpack;
	
	Body() {
		jetpack.active = true;
	}
	
	void update() {
		jetpack.update();
		bodyRep.update();
		jetpack.origin.position.set( bodyRep.position );
		if ( jetpack.active ) {
			jetpack.active = false;
		}
	}
};

class Level {

  int initialTime;

 public:
  int nextId;
  int jumps;
  int timeLeft;
  Body player;
   Vec3 camera;
  Vec3 cameraSpeed;
  std::vector< Plane *> planes;

  Level() {
    timeLeft = DEFAULTTIME;
    nextId = 0;
    jumps = DEFAULTJUMPS;
    initialTime = time( 0 );
  }

  void updateGame( Mix_Chunk *gotIt, Mix_Chunk *step, SDL_Surface *video ) {
    
    timeLeft = DEFAULTTIME - ( time( 0 ) - initialTime );

    if ( timeLeft < 0 ) {

      std::cout << std::endl << "Time up!" << std::endl;      
      SDL_Surface *outcome = IMG_Load( "res/timeup.png" );
      showScreen( video, outcome );
      SDL_FreeSurface( video );
      SDL_FreeSurface( outcome );
      SDL_Quit();
      exit( 0 ); 
    }
    
    Vec3 gravity( 0, -9.8, 0 );
    
    camera.add( cameraSpeed );
    player.update();
    
    cameraSpeed.scale( 0.75f );
    player.bodyRep.speed.scale( 0.125f, 1.0f, 0.125f );
    
    if ( camera.y > player.bodyRep.position.y + 10 ) {
      cameraSpeed.y = -1;
    }
    
    
    if ( player.bodyRep.position.z <= 1.0f ) {
      player.bodyRep.position.z = 1.0f;
    }
    

    if ( player.bodyRep.position.z >= 10.0f ) {
      player.bodyRep.position.z = 10.0f;
    }
    
    if ( player.bodyRep.position.x <= -600.0f ) {
      player.bodyRep.position.x = -600.0f;
    }
    
    if ( player.bodyRep.position.x >= 600.0f ) {
      player.bodyRep.position.x = 600.0f;
    }
    

    Plane *plane;
    Particle *p;

    for ( int c = 0; c < planes.size(); ++c ) {
      plane = planes[ c ];
      
	
	for ( int d = 0; d < player.jetpack.particles.size(); ++d ) {
		p = player.jetpack.particles[ d ];

		if ( plane->hit( p->position, p->speed.y ) ) {
			p->accel.y = 0;
			p->speed.y = 0;
		}
	}

	if ( plane->hit( player.bodyRep.position, 50) ) {
	  
	  player.bodyRep.position.y = plane->p0.y;
	  
	  if ( player.bodyRep.speed.y < 0 ) {
	    player.bodyRep.speed.y = 0;
	  }
	  
	  player.jetpack.active = false;
	  
	  if ( c == nextId ) {
	    ++nextId;
	    
	    if ( gotIt != nullptr ) {
	      Mix_PlayChannel( -1, gotIt, 0 );
	    }



	  if ( nextId == planes.size() ) {
	    std::cout << std::endl << "You won!" << std::endl;
	    SDL_Surface *outcome = IMG_Load( "res/won.png" );
	    showScreen( video, outcome );
	    SDL_FreeSurface( video );
	    SDL_FreeSurface( outcome );
	    SDL_Quit();
	    exit( 0 );
	  }
	} else {

	  if ( step != nullptr ) {
	    //	    Mix_PlayChannel( -1, step, 0 );
	  }
	}
	
	return;
      }
    }

    player.bodyRep.speed.add( gravity );

    if ( player.bodyRep.position.y <= 0.1f ) {
      player.bodyRep.position.y = 0.1f;
 	player.jetpack.active = false; 
	  } else {
		player.jetpack.active = true;
	}
  }
};




#endif
