#include "thysio.h"

extern struct EventController* headController;
extern struct EventController* tailController;

struct EventController* CreateEvent(enum EventCodes eventcode){
    struct EventController* eventControl = (struct EventController*)malloc(sizeof(struct EventController));
    (*eventControl).next = NULL; 
    (*eventControl).event_code = eventcode;
    return eventControl;
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

void ResolveSystemEvent(){
    SDL_Touch* touch;
    SDL_Event sdl_event;
    float x, y;

    while( SDL_PollEvent(&sdl_event) ) {
        //system switch
    //    SDL_Log("Entered system resolver");
    //    SDL_Log("SDL event resolved: %d undecoded %d=WindowEvent", sdl_event.type,SDL_WINDOWEVENT);
        switch(sdl_event.type){

            case SDL_QUIT:
                //this definitely needs some handling shit
                break;

            case SDL_WINDOWEVENT:
    //                SDL_Log("Window Event: %d", sdl_event.window.event);
                if(sdl_event.window.event == SDL_WINDOWEVENT_HIDDEN ||
                   sdl_event.window.event == SDL_WINDOWEVENT_FOCUS_LOST){
                    InputPushQueue(CreateEvent(TAND_PAUSE));
                    SDL_Log("window goes to background"); 
                }
                if(sdl_event.window.event == SDL_WINDOWEVENT_SHOWN ||
                   sdl_event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED){ 
                    InputPushQueue(CreateEvent(TAND_RESUME));
                    SDL_Log("window goes to foreground"); 
                }
                break;

            case SDL_FINGERMOTION:
            case SDL_FINGERDOWN:
            case SDL_FINGERUP:
                touch = SDL_GetTouch( sdl_event.tfinger.touchId );
                
                if( touch ) {
                    x = ( (float)sdl_event.tfinger.x ) / touch->xres;
                    y = ( (float)sdl_event.tfinger.y ) / touch->yres;

                    x *= blahder->w;
                    y *= blahder->h;

                    UpdatePosition( x, y );
                }

                break;
        }
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
