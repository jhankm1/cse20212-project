// headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include <string>
#include <iostream>
#include "game.h"
#include <unistd.h>


// screen attributes
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;

// difficulty level
int level = 1;

// frames per second
const int FRAMES_PER_SECOND = 10;
int going_right = 0;
int going_left = 0;

// the dimenstions of the airman
const int AIRMAN_WIDTH = 120;
const int AIRMAN_HEIGHT = 150;
const int JUMP_WIDTH = 120;
const int JUMP_HEIGHT = 150;
int DEFAULT_Y = (SCREEN_HEIGHT - AIRMAN_HEIGHT + 10);
int jump = 0;



// the direction status of the airman
const int AIRMAN_RIGHT = 0;
const int AIRMAN_LEFT = 1;
const int AIRMAN_JUMP_RIGHT = 3;
const int AIRMAN_JUMP_LEFT = 4;



// the surfaces
SDL_Surface *airman = NULL;
SDL_Surface *background = NULL;
SDL_Surface *obstacle = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *enemy = NULL;
SDL_Surface *boss = NULL;
SDL_Surface *bullet1 = NULL;
SDL_Surface *bullet2 = NULL;
SDL_Surface *bullet3 = NULL;
SDL_Surface *gameover = NULL;
SDL_Surface *start = NULL;
SDL_Surface *winner = NULL;
SDL_Surface *seconds;
SDL_Surface *startStop;

// the music
Mix_Music *music = NULL;

// the event structure
SDL_Event event;

//The areas of the sprite sheet
SDL_Rect clipsRight[ 6 ];
SDL_Rect clipsLeft[ 6 ];
SDL_Rect clipsUpRight[ 5 ];
SDL_Rect clipsUpLeft[ 5 ];
SDL_Rect airmanRect = {0, DEFAULT_Y, AIRMAN_WIDTH, AIRMAN_HEIGHT};
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


Airman::Airman()
{
	// initialize movement variables
	offSet = 50;
	yoff = DEFAULT_Y;
	velocity = 0;
	yvel = 0;

	// initialize animation variables
	frame = 0;
	status = AIRMAN_RIGHT;
}

// event handling function
void Airman::handle_events()
{

		
    
    // if a key was pressed
	if( event.type == SDL_KEYDOWN )
	{
	    // set the velocity
	    switch( event.key.keysym.sym )
	    {
		case SDLK_UP: yvel += 30; break;
		case SDLK_RIGHT:if (velocity < 15){velocity += 15;} break;
            	case SDLK_LEFT: if (velocity > -15){velocity -= 15;} break;
	    	case SDLK_9: 
			// if there is no music playing
                	if( Mix_PlayingMusic() == 0 )
               		{
                     		// play the music
                    		if( Mix_PlayMusic( music, -1 ) == -1 )
                     		{
                         	//return 1;
                     		}    
                 	}
			else
                	{
                     	// if the music is paused
                     		if( Mix_PausedMusic() == 1 )
                     		{
                         		// resume the music
                         		Mix_ResumeMusic();
                     		}
                     		// if the music is playing
                     		else
                     		{
                         		// pause the music
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

	// if key is released
	else if( event.type == SDL_KEYUP )
	{
		// adjust the velocity
		switch( event.key.keysym.sym )
	        {
	        	case SDLK_UP: yvel -= 50; break;
		}
        }
}

// move function
void Airman::move()
{
	// automatically apply velocity to aiman
	airmanRect.x += velocity;
	// check for collision
	//if (collision (airmanRect, enemyRect) != true){
		offSet += velocity;
	//}

	// ensure character stays on screen
	if( ( offSet < 0 ) || ( offSet + AIRMAN_WIDTH > SCREEN_WIDTH ) )
    	{
		//if (collision(airmanRect, enemyRect) != true){
		//}   
		// reset positioning if at end of screen
		// player wins
		if(offSet <= 0 ){
			offSet = 0;
		}
		else {
			if( level < 3){
				offSet = 0;
				level++;
			}
			else{
				apply_surface(0, 0, winner, screen, NULL);
				if( SDL_Flip( screen ) == -1 )
				{
			    		//return 1;
		      		}
				sleep(5);
				exit(1);
			}
		}
		
    	}
	
}

void Airman::show()
{
	// if airman is jumping
	if (yvel > 0 ){
		// set appropriate status
		status = AIRMAN_JUMP_RIGHT;
		// update offset and frame
		yoff -= yvel;
		frame++;
	}
	
	// if airman is running right
	else if ( velocity > 0 ){
		status = AIRMAN_RIGHT;
		frame ++;
	}
	
	// if airman is running left
	else if ( velocity < 0){
		status = AIRMAN_LEFT;
		frame++;
	}

	// if airman is standing still
	else{
		frame = 0;
		yoff = DEFAULT_Y;
		offSet -= 5;
	}

	// loop animation
	if (frame >= 5){
		frame = 0;
		yoff = DEFAULT_Y;
	}

	// right surface applied
	if (status == AIRMAN_RIGHT){
		apply_surface( offSet, yoff, airman, screen, &clipsRight[ frame ] );
	}
	
	// left surface applied
	if (status == AIRMAN_LEFT){
		apply_surface( offSet, yoff, airman, screen, &clipsLeft[ frame ] );
	}

	// jump surface applied
	if (status == AIRMAN_JUMP_RIGHT){
		apply_surface( offSet, yoff, airman, screen, &clipsUpRight[ frame ] );
	}
	
}

Timer::Timer()
{
    // initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    // start the timer
    started = true;

    // unpause the timer
    paused = false;

    // get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    // stop the timer
    started = false;

    // unpause the timer
    paused = false;
}

void Timer::pause()
{
    // if the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        // pause the timer
        paused = true;

        // calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    // if the timer is paused
    if( paused == true )
    {
        // unpause the timer
        paused = false;

        // reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        // reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    // if the timer is running
    if( started == true )
    {
        // if the timer is paused
        if( paused == true )
        {
            // return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            // return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    // if the timer isn't running
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
