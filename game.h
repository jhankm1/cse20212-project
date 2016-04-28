#ifndef GAME_H
#define GAME_H

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

// difficulty level
extern int level;

//The frames per second
extern const int FRAMES_PER_SECOND;

//The dimenstions of the stick figure
extern const int AIRMAN_WIDTH;
extern const int AIRMAN_HEIGHT;
extern const int JUMP_WIDTH;
extern const int JUMP_HEIGHT;
extern int DEFAULT_Y;
extern int jump;

//The direction status of the stick figure
extern const int AIRMAN_RIGHT;
extern const int AIRMAN_LEFT;
extern const int AIRMAN_JUMP_RIGHT;
extern const int AIRMAN_JUMP_LEFT;

// dimensions of our obstacle
extern int OBST_WIDTH;
extern int OBST_HEIGHT;
extern int OBST_XCOORD;
extern int OBST_YCOORD;

//The surfaces
extern SDL_Surface *airman;
extern SDL_Surface *background;
extern SDL_Surface *obstacle;
extern SDL_Surface *screen;
extern SDL_Surface *enemy;
extern SDL_Surface *boss;
extern SDL_Surface *bullet1;
extern SDL_Surface *bullet2;
extern SDL_Surface *bullet3;
extern SDL_Surface *gameover;
extern SDL_Surface *start;
extern SDL_Surface *winner;

//The music
extern Mix_Music *music;

//The event structure
extern SDL_Event event;

//The areas of the sprite sheet
extern SDL_Rect clipsRight[];
extern SDL_Rect clipsLeft[];
extern SDL_Rect clipsUpRight[];
extern SDL_Rect clipsUpLeft[];
extern SDL_Rect airmanRect;
extern SDL_Rect enemyRect;
extern SDL_Rect obstacleRect;
extern SDL_Rect obstacleRect1;
extern SDL_Rect obstacleRect2;
extern SDL_Rect obstacleRect3;
extern SDL_Rect obstacleRect4;
extern SDL_Rect obstacleRect5;
extern SDL_Rect obstacleRect6;
extern SDL_Rect obstacleRect7;
extern SDL_Rect obstacleRect8;
extern SDL_Rect obstacleRect9;
extern SDL_Rect obstacleRect10;
extern SDL_Rect obstacleRect11;
extern SDL_Rect obstacleRect12;
extern SDL_Rect obstacleRect13;
extern SDL_Rect obstacleRect14;



//The stick figure
class Airman
{
    private:
    //The offset
    int offSet;
    int yoff;

    //Its rate of movement
    int velocity;
    int yvel;

    //Its current frame
    int frame;

    //Its animation status
    int status;

    public:
    //Initializes the variables
    Airman();

    //Handles input
    void handle_events();

    //Moves the stick figure
    void move();

    //Shows the stick figure
    void show();

    int getoffset(){ return offSet;};
    int getyoff(){ return yoff;};
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

inline bool collision( SDL_Rect A, SDL_Rect B ){
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = A.x;
    rightA = A.x + A.w;
    topA = A.y;
    bottomA = A.y + A.h;
        
    //Calculate the sides of rect B
    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    bottomB = B.y + B.h;


    if ( (rightA >= leftB) && (rightB >= leftA)){
	return true;
    }

    else if (( leftA <= rightB) && (leftB <= rightA)){
        return true;
    }

    else if (( topA >= bottomB) && (topB >= bottomA)){
	return true;
    }

    else if (( bottomA <= topB) && ( bottomB <= topA)){
	return true;
    }

    
}
#endif
