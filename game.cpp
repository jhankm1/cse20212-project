/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include <string>
#include <iostream>
#include "game.h"

//Screen attributes
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;

//The frames per second
const int FRAMES_PER_SECOND = 10;
int going_right = 0;
int going_left = 0;

//The dimenstions of the stick figure
const int FOO_WIDTH = 120;
const int FOO_HEIGHT = 150;
const int JUMP_WIDTH = 120;
const int JUMP_HEIGHT = 150;
int DEFAULT_Y = (SCREEN_HEIGHT - FOO_HEIGHT + 10);
int jump = 0;



//The direction status of the stick figure
const int FOO_RIGHT = 0;
const int FOO_LEFT = 1;
const int FOO_JUMP_RIGHT = 3;
const int FOO_JUMP_LEFT = 4;


// obstacle dimensions
int OBST_WIDTH;
int OBST_HEIGHT;
int OBST_XCOORD;
int OBST_YCOORD;

//The surfaces
SDL_Surface *foo = NULL;
SDL_Surface *background = NULL;
SDL_Surface *obstacle = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *enemy = NULL;
SDL_Surface *boss = NULL;

//Get the keystates
Uint8 *keystates = SDL_GetKeyState( NULL );

//The music
Mix_Music *music = NULL;

//The event structure
SDL_Event event;

//The areas of the sprite sheet
SDL_Rect clipsRight[ 6 ];
SDL_Rect clipsLeft[ 6 ];
SDL_Rect clipsUpRight[ 5 ];
SDL_Rect clipsUpLeft[ 5 ];
SDL_Rect fooRect = {0, DEFAULT_Y, FOO_WIDTH, FOO_HEIGHT};
SDL_Rect enemyRect = { 450, 525, 50, 50};

// create obstacle rectangle objects for collision

SDL_Rect obstacleRect1 = {765, 497, 44, 93 };
SDL_Rect obstacleRect2 = {1194, 542, 26, 53 };
SDL_Rect obstacleRect3 = {1253, 512, 31, 25 };
SDL_Rect obstacleRect4 = {1289, 490, 63, 27 };
SDL_Rect obstacleRect5 = {1361, 519, 39, 40 };
SDL_Rect obstacleRect6 = {1405, 544, 43, 25 };
SDL_Rect obstacleRect7 = {1690, 518, 69, 31 };
SDL_Rect obstacleRect8 = {2145, 540, 95, 36 };
SDL_Rect obstacleRect9 = {2246, 514, 38, 23 };
SDL_Rect obstacleRect10 = {2295, 484, 75, 28 };
SDL_Rect obstacleRect11 = {3199, 538, 23, 30 };
SDL_Rect obstacleRect12 = {3248, 538, 104, 30 };
SDL_Rect obstacleRect13 = {3389, 538, 84, 30 };
SDL_Rect obstacleRect14 = {3498, 538, 56, 30 };


Foo::Foo()
{
    //Initialize movement variables
    offSet = 0;
    velocity = 0;

    //Initialize animation variables
    frame = 0;
    status = FOO_RIGHT;
}


void Foo::handle_events()
{

		
    
    //If a key was pressed
    if( event.type == SDL_KEYDOWN )
    {
        //Set the velocity
        switch( event.key.keysym.sym )
        {
	    case SDLK_UP: DEFAULT_Y -= 50; break;
            //case SDLK_RIGHT: velocity += 10; break;
            //case SDLK_LEFT: velocity -= 10; break;
	    case SDLK_9: 
		 //If there is no music playing
                 if( Mix_PlayingMusic() == 0 )
                 {
                     //Play the music
                     if( Mix_PlayMusic( music, -1 ) == -1 )
                     {
                         //return 1;
                     }    
                 }
		 else
                 {
                     //If the music is paused
                     if( Mix_PausedMusic() == 1 )
                     {
                         //Resume the music
                         Mix_ResumeMusic();
                     }
                     //If the music is playing
                     else
                     {
                         //Pause the music
                         Mix_PauseMusic();
                     }
                 }
		 break;
	   
	   case SDLK_0:
	   	
	   	//stop the music
		Mix_HaltMusic();
		break;

        }
    }
}

void Foo::move()
{

    
    if(DEFAULT_Y > (SCREEN_HEIGHT - FOO_HEIGHT + 10))
	//status = FOO_RIGHT;
    	status = FOO_JUMP_RIGHT;
	//DEFAULT_Y -= 25;
    else
    {
	status = FOO_RIGHT;
	fooRect.x += velocity;
	if (collision(fooRect, enemyRect) != true){
    		offSet += velocity;
	}

    	//Keep the stick figure in bounds
	if( ( offSet < 0 ) || ( offSet + FOO_WIDTH > SCREEN_WIDTH ) )
        {
	   if (collision(fooRect, enemyRect) != true){
	  	offSet -= velocity;
	    	fooRect.x -= velocity;
	   }
	}

	
    }

}

void Foo::show()
{


    

    //Show the stick figure
    if( status == FOO_RIGHT )
    {
	 //Loop the animation
	if( frame >= 5 )
	{
		frame = 0;
      	}
	apply_surface( offSet, DEFAULT_Y, foo, screen, &clipsRight[ frame ] );
	frame++;
	DEFAULT_Y = (SCREEN_HEIGHT - FOO_HEIGHT + 10);	
    }

    else if (status == FOO_JUMP_RIGHT )
    {
	if( frame >= 5 )
	{
		frame = 0;
		DEFAULT_Y = (SCREEN_HEIGHT - FOO_HEIGHT + 10);
		jump = 0;
      	}
	else {
		apply_surface( offSet, DEFAULT_Y, foo, screen, &clipsUpRight[ frame ] );
		frame++;
	}
    }
 

}

Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}

/*void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;
    //Get offsets
    offset.x = x;
    offset.y = y;
    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}*/

