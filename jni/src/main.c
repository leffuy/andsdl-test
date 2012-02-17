
#include <stdio.h>
#include <math.h>

#include "SDL.h"
#include "SDL_touch.h"
#include "SDL_gesture.h"

#define ERROR   1
#define SUCCESS 0

#define VERBOSE 0

#define WIDTH  640
#define HEIGHT 480
#define BPP    4
#define DEPTH  32

/* Make sure we have good macros for printing 32 and 64 bit values */
#ifndef PRIs32
#define PRIs32 "d"
#endif
#ifndef PRIu32
#define PRIu32 "u"
#endif
#ifndef PRIs64
#ifdef __WIN32__
#define PRIs64 "I64"
#else
#define PRIs64 "lld"
#endif
#endif
#ifndef PRIu64
#ifdef __WIN32__
#define PRIu64 "I64u"
#else
#define PRIu64 "llu"
#endif
#endif


typedef struct
{
  float x;
  float y;
} Point;

typedef struct
{
  float ang;
  float r;
  Point p;
} Knob;


Knob knob;

SDL_Window *window;

SDL_bool running = SDL_TRUE;


void t_parse_event( SDL_Event event )
{
    SDL_Log( "Window event received: %i.", event.type );

    switch (event.type) 
    {
        case SDL_QUIT:
            running = SDL_FALSE;
        break;

        case SDL_FINGERMOTION:
            #if VERBOSE
                SDL_Log(
                    "Finger: %i,x: %i, y: %i",
                    event.tfinger.fingerId,
                    event.tfinger.x,
                    event.tfinger.y
                );
            #endif
            {
                SDL_Touch  *inTouch  = SDL_GetTouch(  event.tfinger.touchId );
                SDL_Finger *inFinger = SDL_GetFinger( inTouch, event.tfinger.fingerId );
            }
        break;

        case SDL_MULTIGESTURE:
            #if VERBOSE     
                SDL_Log(
                    "Multi Gesture: x = %f, y = %f, dAng = %f, dR = %f",
                    event.mgesture.x,
                    event.mgesture.y,
                    event.mgesture.dTheta,
                    event.mgesture.dDist
                );

                SDL_Log(
                    "MG: numDownTouch = %i",
                    event.mgesture.numFingers
                );
            #endif

            knob.p.x  = event.mgesture.x;
            knob.p.y  = event.mgesture.y;
            knob.ang += event.mgesture.dTheta;
            knob.r   += event.mgesture.dDist;
        break;

        case SDL_DOLLARGESTURE:
            SDL_Log(
                "Gesture %"PRIs64" performed, error: %f",
                event.dgesture.gestureId,
                event.dgesture.error
            );
        break;
    }
}

SDL_Surface* initScreen(void )
{
    SDL_Log( "Initializing screen." );

    if( !window )
    {
        window = SDL_CreateWindow(
                "Gesture Test",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                WIDTH, 
                HEIGHT,
                SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL
        );
    }

    if( !window )
    {
        return NULL;
    }

    return SDL_GetWindowSurface( window );
}

void DrawScreen( SDL_Surface *screen )
{
    static Uint32 color = 0;

    SDL_FillRect( screen, NULL, color );

    SDL_UpdateWindowSurface( window );

    color++;
}


int main( int argc, char* argv[] )
{
    SDL_Surface *screen;
    SDL_Event    event;
    SDL_RWops   *src;


    // Initialize
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        return ERROR;
    }

    if( !( screen = initScreen( ) ) )
    {
        SDL_Quit();
        return ERROR;
    }


    // Main loop
    while( running )
    {
        // Handle recent events
        while( SDL_PollEvent( &event ) )
        {
            t_parse_event( event );
        }

        DrawScreen( screen );
    }

    SDL_Quit();  

    return SUCCESS;
}

