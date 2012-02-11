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
};

//Event-Controller interface
struct EventController{
    unsigned int event_code;
    int (*FunctionPtr)(); //it's like a reverse constructor, forced no params
};

//Basic struct for event queue
struct EventQueue{
    struct EventController* event;
};
//Controller list

//Event code 1

//platform functions

//Small set of functions that manipulates the system (start, stop, etc.)

//Init's screen and window stuff
struct SysObjs* InitConfig(struct ConfigSys *conf){
    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return NULL;
    SDL_Window *window = SDL_CreateWindow((*conf).windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (*conf).width, (*conf).height, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
//TODO this will need a returnable object
   // 
    //do something with these 
    struct SysObjs* tmpsysobj = (struct SysObjs*)malloc(sizeof(struct SysObjs));
    (*tmpsysobj).renderer = (struct Renderer*)malloc(sizeof(struct Renderer));

    (*(*tmpsysobj).renderer).window = window;
    (*(*tmpsysobj).renderer).screen = SDL_GetWindowSurface(window);

    return tmpsysobj;
}

//System Variable Structs
//Contains Renderer and Input mechanisms
struct ConfigSys system_configs;
struct SysObjs system_objects;

//I have a feeling at some point this will put in an interface 
//with a create_scheme

//these three guys manage the event loop
void InitSystem(){
    system_objects = InitConfig(&system_configs); //this is simple for now
}


void StartSystem(){
    if(!(system_objects.renderer))
        return; //kind of like throwing an exception?

    if(!(*(system_objects.renderer)).screen)
        (*(system_objects.renderer)).screen = SDL_GetWindowSurface(system_objects.()); //get a new handle to the screen if lost somehow

    for(;;){
        
    }
}

void StopSystem(){
//save state of the buffer to somewhere!
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

//entry point for application start
int main(int argc, char* argv[])
{
    

    //Wow with the way we set it up main is very simple
    return 0;
}


