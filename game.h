#ifndef TEST_H
#define TEST_H

//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include <string>
#include <iostream>

//Screen attributes
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SCREEN_BPP;

//The frames per second
extern const int FRAMES_PER_SECOND;

//The dimenstions of the stick figure
extern const int FOO_WIDTH;
extern const int FOO_HEIGHT;
extern const int JUMP_WIDTH;
extern const int JUMP_HEIGHT;
extern int DEFAULT_Y;
extern int jump;

//The direction status of the stick figure
extern const int FOO_RIGHT;
extern const int FOO_LEFT;
extern const int FOO_JUMP_RIGHT;
extern const int FOO_JUMP_LEFT;

//The surfaces
extern SDL_Surface *foo;
extern SDL_Surface *background;
extern SDL_Surface *screen;

//The music
extern Mix_Music *music;

//The event structure
extern SDL_Event event;

//The areas of the sprite sheet
extern SDL_Rect clipsRight[];
extern SDL_Rect clipsLeft[];
extern SDL_Rect clipsUpRight[];
extern SDL_Rect clipsUpLeft[];


//void apply_surface( int, int, SDL_Surface*, SDL_Surface*, SDL_Rect*);

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


inline void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

#endif




