#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include <string>
#include <iostream>
#include "game.h"
#include <unistd.h>

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
    SDL_WM_SetCaption( "Air Force Super Awesome Adventure Game: The Quest for Total Air Supremacy", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{

    //Load the sprite sheet
    airman = load_image( "Airman.png" );

    // load the bullets
    bullet1 = load_image( "bullet.png");
    bullet2 = load_image( "bullet.png");
    bullet3 = load_image( "bullet.png");


     if(bullet1 == NULL)
	{
		return false;
	}
     if(bullet2 == NULL)
	{
		return false;
	}
     if(bullet3 == NULL)
	{
		return false;
	}
    //If there was a problem in loading the sprite
    if( airman == NULL )
    {
        return false;
    }

    //load font for timer


    //Load the sprite sheet
    enemy = load_image( "enemy.png" );

    // Load Gameover Screen
    gameover = load_image( "GameOver.bmp");
    if( gameover == NULL )
       {
           return false;
       }    

   // Load Start Screen
	start = load_image ( "airforce.png" );
	if( start == NULL )
       {
           return false;
       }  
 

    //If there was a problem in loading the sprite
    if( enemy == NULL )
    {
        return false;
    }    

     boss = load_image( "Boss.png");
     if( boss == NULL ){
    	return false;

    }

    //Load the background
    background = load_image( "city_background.bmp" );

    //If there was a problem in loading the background
    if( background == NULL )
    {
        return false;
    }



    // load obstacles

    obstacle = load_image( "obstacles.png");

    //problem loading them

    if( obstacle == NULL ){
    	return false;

    }

   // load win screen
   
   winner = load_image ( "winscreen.png" );
   if( winner == NULL){
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
    SDL_FreeSurface( airman );
    SDL_FreeSurface ( bullet1 );
    SDL_FreeSurface ( bullet2 );
    SDL_FreeSurface ( bullet3 );
    SDL_FreeSurface ( gameover );
    SDL_FreeSurface ( start );
    SDL_FreeSurface ( winner );

    //Free the surface
    SDL_FreeSurface( enemy );

    //Free the background
    SDL_FreeSurface( background );

    // Free obstacle background
    SDL_FreeSurface( obstacle );

    SDL_FreeSurface( boss );

    //Free the music
    Mix_FreeMusic( music );
    
    //Quit SDL_mixer
    Mix_CloseAudio();

    //Quit SDL
    SDL_Quit();
}

void set_clips()
{
    // clips for jumping
    clipsUpRight[ 0 ].x = 0;
    clipsUpRight[ 0 ].y = AIRMAN_HEIGHT * 2;
    clipsUpRight[ 0 ].w = JUMP_WIDTH;
    clipsUpRight[ 0 ].h = JUMP_HEIGHT;

    clipsUpRight[ 1 ].x = JUMP_WIDTH;
    clipsUpRight[ 1 ].y = AIRMAN_HEIGHT * 2;
    clipsUpRight[ 1 ].w = JUMP_WIDTH;
    clipsUpRight[ 1 ].h = JUMP_HEIGHT;

    clipsUpRight[ 2 ].x = JUMP_WIDTH * 2;
    clipsUpRight[ 2 ].y = AIRMAN_HEIGHT * 2;
    clipsUpRight[ 2 ].w = JUMP_WIDTH;
    clipsUpRight[ 2 ].h = JUMP_HEIGHT;

    clipsUpRight[ 3 ].x = JUMP_WIDTH * 3;
    clipsUpRight[ 3 ].y = AIRMAN_HEIGHT * 2;
    clipsUpRight[ 3 ].w = JUMP_WIDTH;
    clipsUpRight[ 3 ].h = JUMP_HEIGHT;

    clipsUpRight[ 4 ].x = JUMP_WIDTH * 4;
    clipsUpRight[ 4 ].y = AIRMAN_HEIGHT * 2;
    clipsUpRight[ 4 ].w = JUMP_WIDTH;
    clipsUpRight[ 4 ].h = JUMP_HEIGHT;

    clipsUpRight[ 5 ].x = JUMP_WIDTH * 5;
    clipsUpRight[ 5 ].y = AIRMAN_HEIGHT * 2;
    clipsUpRight[ 5 ].w = JUMP_WIDTH;
    clipsUpRight[ 5 ].h = JUMP_HEIGHT;

    // clip running sprites
   
    clipsRight[ 0 ].x = 0;
    clipsRight[ 0 ].y = 0;
    clipsRight[ 0 ].w = AIRMAN_WIDTH;
    clipsRight[ 0 ].h = AIRMAN_HEIGHT;

    clipsRight[ 1 ].x = AIRMAN_WIDTH;
    clipsRight[ 1 ].y = 0;
    clipsRight[ 1 ].w = AIRMAN_WIDTH;
    clipsRight[ 1 ].h = AIRMAN_HEIGHT;

    clipsRight[ 2 ].x = AIRMAN_WIDTH * 2;
    clipsRight[ 2 ].y = 0;
    clipsRight[ 2 ].w = AIRMAN_WIDTH;
    clipsRight[ 2 ].h = AIRMAN_HEIGHT;

    clipsRight[ 3 ].x = AIRMAN_WIDTH * 3;
    clipsRight[ 3 ].y = 0;
    clipsRight[ 3 ].w = AIRMAN_WIDTH;
    clipsRight[ 3 ].h = AIRMAN_HEIGHT;

    clipsRight[ 4 ].x = AIRMAN_WIDTH * 4;
    clipsRight[ 4 ].y = 0;
    clipsRight[ 4 ].w = AIRMAN_WIDTH;
    clipsRight[ 4 ].h = AIRMAN_HEIGHT;
 
    clipsRight[ 5 ].x = AIRMAN_WIDTH * 5;
    clipsRight[ 5 ].y = 0;
    clipsRight[ 5 ].w = AIRMAN_WIDTH;
    clipsRight[ 5 ].h = AIRMAN_HEIGHT;

    clipsLeft[ 0 ].x = 0;
    clipsLeft[ 0 ].y = AIRMAN_HEIGHT;
    clipsLeft[ 0 ].w = AIRMAN_WIDTH;
    clipsLeft[ 0 ].h = AIRMAN_HEIGHT;

    clipsLeft[ 1 ].x = AIRMAN_WIDTH;
    clipsLeft[ 1 ].y = AIRMAN_HEIGHT;
    clipsLeft[ 1 ].w = AIRMAN_WIDTH;
    clipsLeft[ 1 ].h = AIRMAN_HEIGHT;

    clipsLeft[ 2 ].x = AIRMAN_WIDTH * 2;
    clipsLeft[ 2 ].y = AIRMAN_HEIGHT;
    clipsLeft[ 2 ].w = AIRMAN_WIDTH;
    clipsLeft[ 2 ].h = AIRMAN_HEIGHT;

    clipsLeft[ 3 ].x = AIRMAN_WIDTH * 3;
    clipsLeft[ 3 ].y = AIRMAN_HEIGHT;
    clipsLeft[ 3 ].w = AIRMAN_WIDTH;
    clipsLeft[ 3 ].h = AIRMAN_HEIGHT;

    clipsLeft[ 4 ].x = AIRMAN_WIDTH * 4;
    clipsLeft[ 4 ].y = AIRMAN_HEIGHT;
    clipsLeft[ 4 ].w = AIRMAN_WIDTH;
    clipsLeft[ 4 ].h = AIRMAN_HEIGHT;

    clipsLeft[ 5 ].x = AIRMAN_WIDTH * 5;
    clipsLeft[ 5 ].y = AIRMAN_HEIGHT;
    clipsLeft[ 5 ].w = AIRMAN_WIDTH;
    clipsLeft[ 5 ].h = AIRMAN_HEIGHT;
}


int main( int argc, char* args[] )
{

    // background positioning variables
    int bgx = 0;
    int bgy = 0;
    int bgx2 = 0;
    int bgy2 = 0;
    int bulletx1 = 680;
    int bullety1 = 350;
    int bulletx2 = 650;
    int bullety2 = 280;
    int bulletx3 = 640;
    int bullety3 = 310;
    int bossx = 680;
    int bossy = 300;
    int bossup = 1;

    // quit flag
    bool quit = false;

    // initialize
    if( init() == false )
    {
        return 1;
    }

    // load the files
    if( load_files() == false )
    {
        return 1;
    }

    // clip the sprite sheet
    set_clips();

    //frame rate regulator
    Timer fps;
  

      
    // figure
    Airman player;
	
    apply_surface(0, 0, start, screen, NULL);
    if( SDL_Flip( screen ) == -1 )
    {
            return 1;
    }
    sleep(2);

    // while the user hasn't quit
    while( quit == false )
    {
        // start the frame timer
        fps.start();

        // while there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            // handle events for the airman
            player.handle_events();

            // if the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                // quit the program
                quit = true;
            }
        }
	

	// scroll the background and the obstacles
	bgx -= 5;
	bgx2 -=5;
	obstacleRect1.x -= 5;
	obstacleRect2.x -= 5;
	obstacleRect3.x -= 5;
	obstacleRect4.x -= 5;
	obstacleRect5.x -= 5;
	obstacleRect6.x -= 5;
	obstacleRect7.x -= 5;
	obstacleRect8.x -= 5;
	obstacleRect9.x -= 5;
	obstacleRect10.x -= 5;
	obstacleRect11.x -= 5;
	obstacleRect12.x -= 5;
	obstacleRect13.x -= 5;
	obstacleRect14.x -= 5;
	bulletx1 -= 20;
	bullety1 += 15;
	bulletx2 -= 20;
	bullety2 += 15;
	bulletx3 -= 20;
	bullety3 += 15;

	// boss movement
        if(bossup == 1){
		bossy -= 10;
	}
	else{
		bossy += 10;
	}
	if(bossy <= 100){
		bossup = 0;
	}
	if(bossy >= 400){
		bossup = 1;
	}

	// see if player is colliding with boss
	
	if ( player.getoffset() >= bossx && player.getoffset() <= bossx + 280 && player.getyoff() <= bossy + 100 && player.getyoff() >= bossy){
		apply_surface(0, 0, gameover, screen, NULL);
		if( SDL_Flip( screen ) == -1 )
        	{
            		return 1;
      		}
		sleep(5);
		return 0;
	}

	// check for player collision with bullets

	if(bulletx1  >= player.getoffset() && bulletx1 + 80 <= player.getoffset() + AIRMAN_WIDTH && bullety1 - 50 >= player.getyoff() && bullety1+20 <= player.getyoff() + AIRMAN_HEIGHT){
		apply_surface(0, 0, gameover, screen, NULL);
		if( SDL_Flip( screen ) == -1 )
        	{
            		return 1;
      		}
		sleep(5);
		return 0;
		
	}
	if(bulletx2  >= player.getoffset() && bulletx2 + 80 <= player.getoffset() + AIRMAN_WIDTH && bullety2 - 50 >= player.getyoff() && bullety2+20 <= player.getyoff() + AIRMAN_HEIGHT && level >= 2){
		apply_surface(0, 0, gameover, screen, NULL);
		if( SDL_Flip( screen ) == -1 )
        	{
            		return 1;
      		}
		sleep(5);
		return 0;
	}
	
	if(bulletx3 >= player.getoffset() && bulletx3 + 80 <= player.getoffset() + AIRMAN_WIDTH && bullety3 - 50 >= player.getyoff() && bullety3+20 <= player.getyoff() + AIRMAN_HEIGHT && level >= 3){
		apply_surface(0, 0, gameover, screen, NULL);
		if( SDL_Flip( screen ) == -1 )
        	{
            		return 1;
      		}
		sleep(5);
		return 0;
	}
	

	// reset the background (loop)
	if (bgx <= -background->w){
		bgx = 0;
	}
	if(bulletx1 <= 0 || bullety1 >= SCREEN_HEIGHT){
		    bulletx1 = bossx;
   		    bullety1 = bossy + 50;
	}
        if(bulletx2 <= 0 || bullety2 >= SCREEN_HEIGHT){
		    bulletx2 = bossx - 30;
   		    bullety2 = bossy - 30;
	}
        if(bulletx3 <= 0 || bullety3 >= SCREEN_HEIGHT){
		    bulletx3 = bossx - 40;
   		    bullety3 = bossy + 10;
	}

	// reset obstacle rect locations
		
	if (bgx2 <= -obstacle->w){
		bgx2 = 0;
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
	}

	// apply all surfaces for background
	apply_surface(bgx, bgy, background, screen, NULL);
	apply_surface(bgx+background->w, bgy, background, screen, NULL);
	apply_surface(bgx2, bgy2, obstacle, screen, NULL);
	apply_surface(bgx2+obstacle->w, bgy, obstacle, screen, NULL);
	// apply_surface ( 450, 525, enemy, screen, NULL);	
        apply_surface ( bossx, bossy, boss, screen, NULL);
	if(level >= 1){	
       		apply_surface( bulletx1, bullety1, bullet1, screen, NULL);
		if(level >= 2){	
			apply_surface( bulletx2, bullety2, bullet2, screen, NULL);
			if(level >= 3){	
				apply_surface( bulletx3, bullety3, bullet3, screen, NULL);
			}
		}
	
	}	

        // move the airman
        player.move();


        // show the airman on the screen
        player.show();

        // update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }

        // cap the frame rate
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
    }

    // clean up the screen
    clean_up();

    return 0;
}
