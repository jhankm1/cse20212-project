/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include <string>
#include <iostream>

//Screen attributes
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;

//The frames per second
const int FRAMES_PER_SECOND = 10;

//The dimensions of the stick figure
const int FOO_WIDTH = 84;
const int FOO_HEIGHT = 80;
const int JUMP_WIDTH = 100;
const int JUMP_HEIGHT = 108;
int DEFAULT_Y = (SCREEN_HEIGHT - FOO_HEIGHT - 18);
int jump = 0;



//The direction status of the stick figure
const int FOO_RIGHT = 0;
const int FOO_LEFT = 1;
const int FOO_JUMP_RIGHT = 3;
const int FOO_JUMP_LEFT = 4;

//The surfaces
SDL_Surface *foo = NULL;
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;

//The music
Mix_Music *music = NULL;

//The event structure
SDL_Event event;

//The areas of the sprite sheet
SDL_Rect clipsRight[ 6 ];
SDL_Rect clipsLeft[ 6 ];
SDL_Rect clipsUpRight[ 5 ];
SDL_Rect clipsUpLeft[ 5 ];

//The stick figure
class Foo
{
    private:
    //The offset
    int offSet;

    //Its rate of movement
    int velocity;

    //Its current frame
    int frame;

    //Its animation status
    int status;


    // box boundaries
    int box_x1 = 0;
    int box_x2 = 84;
    int box_y1 = 582;
    int box_y2 = 502;


    public:
    //Initializes the variables
    Foo();

    //Handles input
    void handle_events();

    //Moves the stick figure
    void move();

    //Shows the stick figure
    void show();
};

//The timer
class Timer
{
    private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

    public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {

            //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, 0, 254, 254));
        }
    }

    //Return the optimized surface
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

void set_clips()
{
    clipsUpRight[ 0 ].x = 0;
    clipsUpRight[ 0 ].y = FOO_HEIGHT * 2;
    clipsUpRight[ 0 ].w = JUMP_WIDTH;
    clipsUpRight[ 0 ].h = JUMP_HEIGHT;

    clipsUpRight[ 1 ].x = JUMP_WIDTH;
    clipsUpRight[ 1 ].y = FOO_HEIGHT * 2;
    clipsUpRight[ 1 ].w = JUMP_WIDTH;
    clipsUpRight[ 1 ].h = JUMP_HEIGHT;

    clipsUpRight[ 2 ].x = JUMP_WIDTH * 2;
    clipsUpRight[ 2 ].y = FOO_HEIGHT * 2;
    clipsUpRight[ 2 ].w = JUMP_WIDTH;
    clipsUpRight[ 2 ].h = JUMP_HEIGHT;

    clipsUpRight[ 3 ].x = JUMP_WIDTH * 3;
    clipsUpRight[ 3 ].y = FOO_HEIGHT * 2;
    clipsUpRight[ 3 ].w = JUMP_WIDTH;
    clipsUpRight[ 3 ].h = JUMP_HEIGHT;

    clipsUpRight[ 4 ].x = JUMP_WIDTH * 4;
    clipsUpRight[ 4 ].y = FOO_HEIGHT * 2;
    clipsUpRight[ 4 ].w = JUMP_WIDTH;
    clipsUpRight[ 4 ].h = JUMP_HEIGHT;

    clipsUpLeft[ 0 ].x = 0;
    clipsUpLeft[ 0 ].y = (FOO_HEIGHT * 2) + JUMP_HEIGHT;
    clipsUpLeft[ 0 ].w = JUMP_WIDTH;
    clipsUpLeft[ 0 ].h = JUMP_HEIGHT;

    clipsUpLeft[ 1 ].x = JUMP_WIDTH;
    clipsUpLeft[ 1 ].y = (FOO_HEIGHT * 2) + JUMP_HEIGHT;
    clipsUpLeft[ 1 ].w = JUMP_WIDTH;
    clipsUpLeft[ 1 ].h = JUMP_HEIGHT;

    clipsUpLeft[ 2 ].x = JUMP_WIDTH * 2;
    clipsUpLeft[ 2 ].y = (FOO_HEIGHT * 2) + JUMP_HEIGHT;
    clipsUpLeft[ 2 ].w = JUMP_WIDTH;
    clipsUpLeft[ 2 ].h = JUMP_HEIGHT;

    clipsUpLeft[ 3 ].x = JUMP_WIDTH * 3;
    clipsUpLeft[ 3 ].y = (FOO_HEIGHT * 2) + JUMP_HEIGHT;
    clipsUpLeft[ 3 ].w = JUMP_WIDTH;
    clipsUpLeft[ 3 ].h = JUMP_HEIGHT;

    clipsUpLeft[ 4 ].x = JUMP_WIDTH * 4;
    clipsUpLeft[ 4 ].y = (FOO_HEIGHT * 2) + JUMP_HEIGHT;
    clipsUpLeft[ 4 ].w = JUMP_WIDTH;
    clipsUpLeft[ 4 ].h = JUMP_HEIGHT;

    //Clip the sprites
   
    clipsRight[ 0 ].x = 0;
    clipsRight[ 0 ].y = 0;
    clipsRight[ 0 ].w = FOO_WIDTH;
    clipsRight[ 0 ].h = FOO_HEIGHT;

    clipsRight[ 1 ].x = FOO_WIDTH;
    clipsRight[ 1 ].y = 0;
    clipsRight[ 1 ].w = FOO_WIDTH;
    clipsRight[ 1 ].h = FOO_HEIGHT;

    clipsRight[ 2 ].x = FOO_WIDTH * 2;
    clipsRight[ 2 ].y = 0;
    clipsRight[ 2 ].w = FOO_WIDTH;
    clipsRight[ 2 ].h = FOO_HEIGHT;

    clipsRight[ 3 ].x = FOO_WIDTH * 3;
    clipsRight[ 3 ].y = 0;
    clipsRight[ 3 ].w = FOO_WIDTH;
    clipsRight[ 3 ].h = FOO_HEIGHT;

    clipsRight[ 4 ].x = FOO_WIDTH * 4;
    clipsRight[ 4 ].y = 0;
    clipsRight[ 4 ].w = FOO_WIDTH;
    clipsRight[ 4 ].h = FOO_HEIGHT;
 
    clipsRight[ 5 ].x = FOO_WIDTH * 5;
    clipsRight[ 5 ].y = 0;
    clipsRight[ 5 ].w = FOO_WIDTH;
    clipsRight[ 5 ].h = FOO_HEIGHT;

    clipsLeft[ 0 ].x = 0;
    clipsLeft[ 0 ].y = FOO_HEIGHT;
    clipsLeft[ 0 ].w = FOO_WIDTH;
    clipsLeft[ 0 ].h = FOO_HEIGHT;

    clipsLeft[ 1 ].x = FOO_WIDTH;
    clipsLeft[ 1 ].y = FOO_HEIGHT;
    clipsLeft[ 1 ].w = FOO_WIDTH;
    clipsLeft[ 1 ].h = FOO_HEIGHT;

    clipsLeft[ 2 ].x = FOO_WIDTH * 2;
    clipsLeft[ 2 ].y = FOO_HEIGHT;
    clipsLeft[ 2 ].w = FOO_WIDTH;
    clipsLeft[ 2 ].h = FOO_HEIGHT;

    clipsLeft[ 3 ].x = FOO_WIDTH * 3;
    clipsLeft[ 3 ].y = FOO_HEIGHT;
    clipsLeft[ 3 ].w = FOO_WIDTH;
    clipsLeft[ 3 ].h = FOO_HEIGHT;

    clipsLeft[ 4 ].x = FOO_WIDTH * 4;
    clipsLeft[ 4 ].y = FOO_HEIGHT;
    clipsLeft[ 4 ].w = FOO_WIDTH;
    clipsLeft[ 4 ].h = FOO_HEIGHT;

    clipsLeft[ 5 ].x = FOO_WIDTH * 5;
    clipsLeft[ 5 ].y = FOO_HEIGHT;
    clipsLeft[ 5 ].w = FOO_WIDTH;
    clipsLeft[ 5 ].h = FOO_HEIGHT;
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return false;    
    }

    //Set the window caption
    SDL_WM_SetCaption( "Animation Test", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{

    //Load the sprite sheet
    foo = load_image( "Airman.png" );

    //If there was a problem in loading the sprite
    if( foo == NULL )
    {
        return false;
    }

    //Load the background
    background = load_image( "city_background.bmp" );

    //If there was a problem in loading the background
    if( background == NULL )
    {
        return false;
    }

    //Load the music
    music = Mix_LoadMUS( "new_af_song.wav" );
    
    //If there was a problem loading the music
    if( music == NULL )
    {
        return false;    
    }

    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surface
    SDL_FreeSurface( foo );

    //Free the background
    SDL_FreeSurface( background );

    //Free the music
    Mix_FreeMusic( music );
    
    //Quit SDL_mixer
    Mix_CloseAudio();

    //Quit SDL
    SDL_Quit();
}

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
            case SDLK_RIGHT: velocity += FOO_WIDTH / 3; break;
            case SDLK_LEFT: velocity -= FOO_WIDTH / 4; break;
	    //case (SDLK_UP && SDLK_RIGHT): velocity += FOO_WIDTH / 4; DEFAULT_Y += 30; break;
	    //case (SDLK_UP && SDLK_RIGHT): velocity -= FOO_WIDTH / 5; DEFAULT_Y += 30; break;
	    case SDLK_UP: jump = 1; break;
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
    box_x1 += velocity;
    box_x2 += velocity;

    //Keep the stick figure in bounds
    if( ( offSet < 0 ) || ( offSet + FOO_WIDTH > SCREEN_WIDTH ) )
    {
        offSet -= velocity;
	box_x1 -= velocity;
	box_x2 -= velocity;
    }
    else{
	offSet -= 5;
	box_x1 -= 5;
	box_x2 -= 5;
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
	box_y1 = (SCREEN_HEIGHT - FOO_HEIGHT - 18)+80;
	box_y2 = (SCREEN_HEIGHT - FOO_HEIGHT - 18);
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
	box_y1 = (SCREEN_HEIGHT - FOO_HEIGHT - 18)+80;
	box_y2 = (SCREEN_HEIGHT - FOO_HEIGHT - 18);
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
		box_y1 -= 30;
		box_y2 -= 30;
		frame ++;
	}
		
	else if (frame > 2 && frame < 4){
		DEFAULT_Y += 40;
		box_y1 += 40;
		box_y2 += 40;
		frame ++;
	}

	else if (frame >= 4){
		DEFAULT_Y += 50;
		box_y1 += 50;
		box_y2 += 50;
		frame = 0;
		jump = 0;
		DEFAULT_Y = (SCREEN_HEIGHT - FOO_HEIGHT - 18);
		box_y1 = (SCREEN_HEIGHT - FOO_HEIGHT - 18)+80;
		box_y2 = (SCREEN_HEIGHT - FOO_HEIGHT - 18);
	}
    }

    //If Airman jumping left
    else if(jump == 1 && velocity < 0)
    {
    	status = FOO_JUMP_LEFT;
	std::cout << "STATUS IS: " << status << std::endl;
	DEFAULT_Y -= 30;
	box_y1 -= 30;
	box_y2 -= 30;
	//frame = 0;
	if (frame >= 4){
		DEFAULT_Y = (SCREEN_HEIGHT - FOO_HEIGHT - 18);
		box_y1 = (SCREEN_HEIGHT - FOO_HEIGHT - 18)+80;
		box_y2 = (SCREEN_HEIGHT - FOO_HEIGHT - 18);
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



int main( int argc, char* args[] )
{

	int bgx = 0;
	int bgy = 0;

    //Quit flag
    bool quit = false;

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }

    //Clip the sprite sheet
    set_clips();

    //The frame rate regulator
    Timer fps;

    //The stick figure
    Foo walk;

    //While the user hasn't quit
    while( quit == false )
    {
        //Start the frame timer
        fps.start();

        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            //Handle events for the stick figure
            walk.handle_events();

            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }

	bgx -= 5;
	
	if (bgx <= -background -> w){
		bgx = 0;
	}

	apply_surface(bgx, bgy, background, screen);
	apply_surface(bgx+background->w, bgy, background, screen);

	

        //Move the stick figure
        walk.move();


        //Show the stick figure on the screen
        walk.show();

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }

        //Cap the frame rate
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
    }

    //Clean up
    clean_up();

    return 0;
}
