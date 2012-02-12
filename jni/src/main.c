//Basis for TANDUM engine

/*Small Description: I'm always told to keep the commenting simple. I guess
* that's true; all of this text in blue, won't be used. It's just for human
* eyes. Given this is completely written in C for simplicity in thinking
* I've tried to keep things organized by their components. Labeled with
* comments.
*
* Ultimately, if you're reading this comment, I've been killed. JUST KIDDING.
* But seriously in all likelyhood if you're reading this comment then this
* code is still in the prototype stages. 
* Change change change as much as you want.
*
*/

//includes
#include <malloc.h> //my favorite of all memory allocators, never lets me down
#include <stdio.h> //for NULL. I mean seriously...
#include "SDL.h"
#include "SDL_image.h"

//defines


//platform structures

//expandable configurator
struct ConfigSys{
    char* windowName; //Name the window mang
    int width,height; //I can do this right?
    
}; //This goes into a configuration function

struct SysObjs{
    struct Renderer* renderer;

}; //This comes out via a pointer with all the system objects

//Small struct to hold video objects
struct Renderer{
    SDL_Window* window;
    SDL_Surface* screen;
    int (*RenderFunc)();
};

//Event-Controller interface
struct EventController{
    unsigned int event_code;
    unsigned char resolved; //this needs to be a bit...
    int (*Controller)(); //it's like a reverse constructor, forced no params
    struct EventController* next;
};

enum EventCodes {
    TAND_INIT,
    TAND_QUIT
}; //Basic event codes for case in event creation

//Basic struct for event queue <- nixed cause I don't want to dereference that

//Controller list
//Event code 1, 2, 3...etc


//System-wide Variable Structs
//Contains Renderer and Input mechanisms, etc.
struct ConfigSys system_configs; //maybe these will be pointers one day...
struct SysObjs* system_objects;
struct EventController* headController;
struct EventController* tailController;

//platform functions declares
struct SysObjs* InitConfig(struct ConfigSys *conf);
void InitSystem();
void StartSystem();
void StopSystem();
void QuitSystem();


//Init's screen and window stuff
struct SysObjs* InitConfig(struct ConfigSys *conf){
    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return NULL;
    SDL_Window *window = SDL_CreateWindow((*conf).windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (*conf).width, (*conf).height, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
    //do something with these 
    struct SysObjs* tmpsysobj = (struct SysObjs*)malloc(sizeof(struct SysObjs));
    (*tmpsysobj).renderer = (struct Renderer*)malloc(sizeof(struct Renderer));

    (*(*tmpsysobj).renderer).window = window;
    (*(*tmpsysobj).renderer).screen = SDL_GetWindowSurface(window);

    return tmpsysobj;
}

//these guys handle event creation/resolution

//Note to Patrick:
//Please fucking change this if you feel that it needs to be, don't even 
//hesitate. I don't like over using if statements and a switch() is just that

struct EventController* CreateEvent(enum EventCodes eventcode){
    struct EventController* eventControl = (struct EventController*)malloc(sizeof(struct EventController));
    
    (eventControl*).event_code = eventcode;

    switch(eventcode)
    {
        case TAND_INIT:
          //I imagine a specific controller function ptr is set here
        break;

        case TAND_QUIT:
          //For each scenario and shit
        break;

        default:
            return NULL;
    }
    return eventControl;
}

void ResolveEvent(struct EventController* resolver){
    if(!(*resolver).Controller){
        free(resolver);
        return; //I'm a big fan of returning early during checks
    }
    (*resolver).Controller(); // Simple enough
}

//these guys manage the event queue
struct EventController* InputPopQueue(){
    if(!headController)
        return NULL;

    return headController;

    if((*headController).next){
       headController = (*headController).next;
    }

}

void InputPushQueue(struct EventController* pushed){
    if(!headController){
        headController = pushed;
        tailController = pushed;
        return;
    }
    (*tailController).next = pushed;
}

//I have a feeling at some point this will put in an interface 
//with a create_scheme

//these three guys manage the event loop
void InitSystem(){
    system_objects = InitConfig(&system_configs); //this is simple for now
    //setup system event queue here...
    
}


void StartSystem(){
    if(!(*system_objects).renderer)
        return; //kind of like throwing an exception?

    if((*(*system_objects).renderer).screen != NULL){
        (*(*system_objects).renderer).screen = SDL_GetWindowSurface((*(*system_objects).renderer).window); //get a new handle to the screen if lost somehow
}
//Root loop things can be scheduled around this
    for(;;){
        struct EventController* tmpEvent = InputPopQueue();
	
    }
}

//Throws an event to back up 
void StopSystem(){
//save state of the buffer to somewhere!

//need more of the system implemented before I can fill out this stub
}

void QuitSystem(){

}



//here some kind of rampant loader to load the fields 
//do some tests first

//SANDBOX AREA below

//test "user code" area
void userFunction1(){ //I really like this name
//I guess I'll use this to define my system_configs
//And as an example of a "loader"

//here in this huge block of code a loader would do something
//I see evolution this way, hardcode -> macros -> JSON C objs
system_configs.windowName = "Figurine";
system_configs.height = 640;
system_configs.width = 480;
}
//Well the above shit works for sure

//entry point for application start
int main(int argc, char* argv[])
{
    userFunction1();
    InitSystem();
//Here we should load game related stuff;
//This is what I imagine:
    //LoadCosmos("userDataFile"); //loads the world file
    StartSystem();

    //Wow with the way we set it up main is very simple
    return 0;
}


//One day I will remove all the comments. This shit is stupid.
//I feel like this will annoy a good programmer rather than help.
