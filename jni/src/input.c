#include "thysio.h"
#include "SDL_thread.h"


extern struct EventController* headController;
extern struct EventController* tailController;

extern SDL_Surface* blahder;
extern SDL_Rect dstrectum;




int EventHandler() {
    SDL_Event sdl_event;

    while( 1 ) {
        SDL_WaitEvent( &sdl_event );

        ResolveSystemEvent( &sdl_event );
    }
}

int InitializeSystemEventHandler() {
    SDL_Thread *thread;

    thread = SDL_CreateThread( EventHandler, "EventThread", NULL );

    if ( thread == NULL ) {
        SDL_Log( "Could not create event handler thread." );
        return 1;
    }

    return 0;
}

struct EventController* CreateEvent(enum EventCodes eventcode){
    struct EventController* eventControl = (struct EventController*)malloc(sizeof(struct EventController));
    (*eventControl).next = NULL; 
    (*eventControl).event_code = eventcode;
    return eventControl;
}

void UpdatePosition(int x, int y) {
    dstrectum.x = x;
    dstrectum.y = y;
}

//Another note to patrick:
//This will set up the input 

int ResolveEvent(struct EventController* resolver){
    if(!resolver) return 0;
    if((*resolver).event_code == TAND_PAUSE){
        free(resolver);
        return 1;
    }
    if((*resolver).event_code == TAND_RESUME){
        free(resolver);
        return 2;
    }
    if(!(*resolver).Controller){
        free(resolver);
        return 0; //I'm a big fan of returning early during checks
    }
    (*resolver).Controller(); // Simple enough
    free(resolver);
    return 0;
}

void ResolveSystemEvent( SDL_Event *sdl_event ) {
    SDL_Touch* touch;
    float x, y;

    //system switch
    //SDL_Log("Entered system resolver");
    //SDL_Log("SDL event resolved: %d undecoded %d=WindowEvent", sdl_event.type,SDL_WINDOWEVENT);
    switch(sdl_event->type){

        case SDL_QUIT:
            //this definitely needs some handling shit
            break;

        case SDL_WINDOWEVENT:
//                SDL_Log("Window Event: %d", sdl_event.window.event);
            if(sdl_event->window.event == SDL_WINDOWEVENT_HIDDEN ||
               sdl_event->window.event == SDL_WINDOWEVENT_FOCUS_LOST){
                InputPushQueue(CreateEvent(TAND_PAUSE));
                SDL_Log("window goes to background"); 
            }
            if(sdl_event->window.event == SDL_WINDOWEVENT_SHOWN ||
               sdl_event->window.event == SDL_WINDOWEVENT_FOCUS_GAINED){ 
                InputPushQueue(CreateEvent(TAND_RESUME));
                SDL_Log("window goes to foreground"); 
            }
            break;

        case SDL_FINGERMOTION:
        case SDL_FINGERDOWN:
        case SDL_FINGERUP:
            touch = SDL_GetTouch( sdl_event->tfinger.touchId );
            
            if( touch ) {
                x = ( (float)sdl_event->tfinger.x ) / touch->xres;
                y = ( (float)sdl_event->tfinger.y ) / touch->yres;

                x *= blahder->w;
                y *= blahder->h;

                UpdatePosition( x, y );
            }

            break;
    }
}

//these guys manage the event queue
struct EventController* InputPopQueue(){
    if(!headController)
        return NULL;

    struct EventController* controlman =  headController;

    if((*headController).next){
       headController = (*headController).next;
    }
    else{
       headController = NULL;
    }
    return controlman;
}

void InputPushQueue(struct EventController* pushed){
    if(!headController){
        headController = pushed;
        tailController = pushed;
        return;
    }
    (*tailController).next = pushed;
    tailController = pushed;
}
