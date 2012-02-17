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
    SDL_Surface* back_buff;
    SDL_Surface* screen;
    Uint32 alpha_color;
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


//Here goes the model stuff


//here goes render stuff


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
struct EventController* CreateSystemEvent(enum EventCodes eventcode);
void ResolveEvent(struct EventController* resolver);
struct EventController* InputPopQueue();
void InputPushQueue(struct EventController* pushed);
SDL_Surface* LoadImageToSurface(char* imgname);
void RenderScreen();
void renderTest();

//Init's screen and window stuff
struct SysObjs* InitConfig(struct ConfigSys *conf){
    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return NULL;
    SDL_Window *window = SDL_CreateWindow((*conf).windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (*conf).width, (*conf).height, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
    //do something with these 
    struct SysObjs* tmpsysobj = (struct SysObjs*)malloc(sizeof(struct SysObjs));
    (*tmpsysobj).renderer = (struct Renderer*)malloc(sizeof(struct Renderer));

    (*(*tmpsysobj).renderer).window = window;
    (*(*tmpsysobj).renderer).screen = SDL_GetWindowSurface(window);
    SDL_Surface* sptr = (*(*tmpsysobj).renderer).screen;
    (*(*tmpsysobj).renderer).back_buff = SDL_CreateRGBSurface(0, (*sptr).w, (*sptr).h, (*(*sptr).format).BitsPerPixel,(*(*sptr).format).Rmask,(*(*sptr).format).Gmask,(*(*sptr).format).Bmask,(*(*sptr).format).Amask);
    //At some point this thing below will be configurable
    //For now, just use the fucking value

    (*(*tmpsysobj).renderer).alpha_color = SDL_MapRGB((*(*(*system_objects).renderer).screen).format, 250,162,255);
 
    return tmpsysobj;
}

//model stuff functions

//render stuff functions
SDL_Surface* LoadImageToSurface(char* imgname){
    SDL_RWops *file = SDL_RWFromFile(imgname, "rb");
    SDL_Surface *image = IMG_Load_RW(file, 1);
    return image;
}

void FlushToScreen(SDL_Surface* layer){
    if(!layer) return; //check layer
    //Check for size sameness
    if((*(*(*system_objects).renderer).screen).w != (*layer).w || (*(*(*system_objects).renderer).screen).h != (*layer).h) return; //I'm a real big fan of returning early if error encountered
    //assume alpha channel for 250,162,255
    //Should have alpha channel now for transparency in layers
    SDL_SetSurfaceAlphaMod(layer, 0);
    SDL_SetColorKey(layer, SDL_TRUE, (*(*system_objects).renderer).alpha_color);
    
    SDL_BlitSurface(layer, NULL, (*(*system_objects).renderer).screen, NULL);
    //I like this line here; instead of screen back up buffer
    SDL_BlitSurface(layer, NULL, (*(*system_objects).renderer).back_buff, NULL);
    //is a mirror of the screen
}

void RenderScreen(){
    SDL_UpdateWindowSurface((*(*system_objects).renderer).window);
//clear screen buffer after successful update
    SDL_FillRect((*(*system_objects).renderer).screen, NULL, 0);
    SDL_FillRect((*(*system_objects).renderer).back_buff, NULL, 0);
    //ready for new frame!
}

//these guys handle event creation/resolution

//Note to Patrick:
//Please fucking change this if you feel that it needs to be, don't even 
//hesitate. I don't like over using if statements and a switch() is just that

struct EventController* CreateSystemEvent(enum EventCodes eventcode){
    struct EventController* eventControl = (struct EventController*)malloc(sizeof(struct EventController));
    
    (*eventControl).event_code = eventcode;

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
    free(resolver);
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
//recover the screen data hopefully
    SDL_BlitSurface((*(*system_objects).renderer).back_buff, NULL, (*(*system_objects).renderer).screen, NULL);
    }
//Root loop things can be scheduled around this
    for(;;){
        struct EventController* tmpEvent = InputPopQueue();
	ResolveEvent(tmpEvent);
	renderTest();
        RenderScreen();
    }
}

//Throws an event to back up 
void StopSystem(){
//hurry
//save state of the buffer to somewhere!


//need more of the system implemented before I can fill out this stub
}

void QuitSystem(){

}



//here some kind of rampant loader to load the fields 
//do some tests first

//SANDBOX AREA below

//test "user code" area
void setupTest(){ //I really like this name
//I guess I'll use this to define my system_configs
//And as an example of a "loader"

//here in this huge block of code a loader would do something
//I see evolution this way, hardcode -> macros -> JSON C objs
system_configs.windowName = "Figurine";
system_configs.width = 640;
system_configs.height = 480;
}

void renderTest(){
SDL_Rect dstrectum;
dstrectum.x = 20;
dstrectum.y = 20;
dstrectum.w = 40;
dstrectum.h = 40;

SDL_Surface* sptr = (*(*system_objects).renderer).screen;

SDL_Surface* testSurface = SDL_CreateRGBSurface(0, (*sptr).w, (*sptr).h, (*(*sptr).format).BitsPerPixel,(*(*sptr).format).Rmask,(*(*sptr).format).Gmask,(*(*sptr).format).Bmask,(*(*sptr).format).Amask);

SDL_Surface* rectangleTest = LoadImageToSurface("rectangle.jpg");

SDL_BlitSurface(rectangleTest, NULL, testSurface, &dstrectum);

FlushToScreen(testSurface);

SDL_FreeSurface(rectangleTest);
SDL_FreeSurface(testSurface);
} // blah I refuse to bloat the code with this mess before it's too early; RENDER FUCKING TEST bleeh
//Well the above shit works for sure

//entry point for application start
int main(int argc, char* argv[])
{
    setupTest();
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
