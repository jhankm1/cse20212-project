
//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "test.h"
#include <string>
#include <iostream>



void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect * clip = NULL)
{
    //Holds offsets
	SDL_Rect offset;

    //Get offsets
	offset.x = x;
	offset.y = y;

    //Blit
	SDL_BlitSurface( source, clip, destination, &offset );
}



Foo::Foo(){
    //Initialize movement variables
	offSet = 0;
	velocity = 0;

    //Initialize animation variables
	frame = 0;
	status = FOO_RIGHT;
}


void Foo::handle_events(){

    //If a key was pressed
	if( event.type == SDL_KEYDOWN ){
        //Set the velocity
		switch( event.key.keysym.sym ){

			case SDLK_RIGHT: 
				velocity += FOO_WIDTH / 3; 
			break;
            
			case SDLK_LEFT: 
				velocity -= FOO_WIDTH / 4; 
			break;
	    //case (SDLK_UP && SDLK_RIGHT): velocity += FOO_WIDTH / 4; DEFAULT_Y += 30; break;
	    //case (SDLK_UP && SDLK_RIGHT): velocity -= FOO_WIDTH / 5; DEFAULT_Y += 30; break;
			case SDLK_UP: 
				jump = 1; 
			break;

			case SDLK_9: 
		 //If there is no music playing
				if( Mix_PlayingMusic() == 0 ){
                     //Play the music
                     			if( Mix_PlayMusic( music, -1 ) == -1 ){
                         			//return 1;
                     			}    
                 		}
		 		else{
                     //If the music is paused

					if( Mix_PausedMusic() == 1 ){
                        //Resume the music
                         			Mix_ResumeMusic();
                     			}
                     //If the music is playing
                     			else{
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
    //If a key was released
    else if( event.type == SDL_KEYUP )
    {
        //Set the velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_RIGHT: velocity -= FOO_WIDTH / 3; break;
            case SDLK_LEFT: velocity += FOO_WIDTH / 4; break;
	    //case (SDLK_UP && SDLK_RIGHT): velocity -= FOO_WIDTH / 4; DEFAULT_Y -= 30; break;
	    //case (SDLK_UP && SDLK_RIGHT): velocity += FOO_WIDTH / 5; DEFAULT_Y -= 30; break;
	    //case SDLK_UP: DEFAULT_Y += 30; break;
        }
    }
}


void Foo::move()
{
    //Move
    offSet += velocity;

    //Keep the stick figure in bounds
    if( ( offSet < 0 ) || ( offSet + FOO_WIDTH > SCREEN_WIDTH ) )
    {
        offSet -= velocity;
    }
    else{
	offSet -= 5;
    }
}

void Foo::show()
{
    //If Foo is moving left
    if( jump == 0 && velocity < 0 )
    {
        //Set the animation to left
        status = FOO_LEFT;
	DEFAULT_Y = (SCREEN_HEIGHT - FOO_HEIGHT - 18);
	std::cout << "STATUS IS: " << status << std::endl;

        //Move to the next frame in the animation
        frame++;
    }
    //If Foo is moving right
    else if( jump == 0 && velocity > 0 )
    {
	//Set the animation to right
	status = FOO_RIGHT;
	DEFAULT_Y = (SCREEN_HEIGHT - FOO_HEIGHT - 18);
	std::cout << "STATUS IS: " << status << std::endl;

	//Move to the next frame in the animation
	frame++;
    }

    //If Airman jumping right
    else if(jump == 1) //&& velocity >= 0)
    {
    	status = FOO_JUMP_RIGHT;
	std::cout << "STATUS IS: " << status << std::endl;
	//frame = 0;
	if (frame <= 2){
		DEFAULT_Y -= 30;
		frame ++;
	}
		
	else if (frame > 2 && frame < 4){
		DEFAULT_Y += 40;
		frame ++;
	}

	else if (frame >= 4){
		DEFAULT_Y += 50;
		frame = 0;
		jump = 0;
		DEFAULT_Y = (SCREEN_HEIGHT - FOO_HEIGHT - 18);
	}
    }

    //If Airman jumping left
    else if(jump == 1 && velocity < 0)
    {
    	status = FOO_JUMP_LEFT;
	std::cout << "STATUS IS: " << status << std::endl;
	DEFAULT_Y -= 30;
	//frame = 0;
	if (frame >= 4){
		DEFAULT_Y = (SCREEN_HEIGHT - FOO_HEIGHT - 18);
		frame = 0;
		jump = 0;
	}
	else{
		frame++;
	}
    }


    //If Foo standing
    else
    {
        //Restart the animation
        frame = 0;
    }

    //Loop the animation
    if( frame >= 5 )
    {
        frame = 0;
    }

    //Show the stick figure
    if( status == FOO_RIGHT )
    {
        apply_surface( offSet, /*SCREEN_HEIGHT - FOO_HEIGHT - 18*/ DEFAULT_Y, foo, screen, &clipsRight[ frame ] );
    }
    else if( status == FOO_LEFT )
    {
        apply_surface( offSet, /*SCREEN_HEIGHT - FOO_HEIGHT - 18*/DEFAULT_Y, foo, screen, &clipsLeft[ frame ] );
    }
    else if (status == FOO_JUMP_RIGHT )
    {
    	apply_surface (offSet, DEFAULT_Y, foo, screen, &clipsUpRight[ frame ] );
    }
    else if (status == FOO_JUMP_LEFT )
    {
    	apply_surface (offSet, DEFAULT_Y, foo, screen, &clipsUpLeft[ frame ] );
    }

}

Timer::Timer()
{
    //Initialize the variable
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


