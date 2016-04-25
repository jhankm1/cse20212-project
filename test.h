//The headers
#ifndef TEST_H
#define TEST_H

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

//The dimenstions of the Airman
const int FOO_WIDTH = 84;
const int FOO_HEIGHT = 80;
const int JUMP_WIDTH = 100;
const int JUMP_HEIGHT = 108;
int DEFAULT_Y = (SCREEN_HEIGHT - FOO_HEIGHT - 18);
int jump = 0;

//The direction status of the Airman
const int FOO_RIGHT = 0;
const int FOO_LEFT = 1;
const int FOO_JUMP_RIGHT = 3;
const int FOO_JUMP_LEFT = 4;

//The surfaces
SDL_Surface *foo = NULL;
SDL_Surface *enemy = NULL;
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

void apply_surface( int, int , SDL_Surface*, SDL_Surface*, SDL_Rect*);

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

#endif





