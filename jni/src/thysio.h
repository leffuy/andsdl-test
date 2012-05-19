//includes
#include <malloc.h> //my favorite of all memory allocators, never lets me down
#include <stdio.h> //for NULL. I mean seriously...

#include "uthash.h"
//#include "hashmap.h"
#include "SDL.h"
#include "SDL_render.h"
#include "SDL_image.h"
#include "SDL_ttf.h"


#define MAX_TOUCH_INPUTS 2

#define SUCCESS 0
#define FAIL    1


typedef enum {
    false,
    true
} __attribute__ ((packed)) boolean;
//Note For Packie: what does the above tag do? Comment via github

//platform structures declarations
struct Model;
struct ModelIndex;
struct Cosmos;
struct Sprite;

//expandable configurator
struct ConfigSys{
    char* windowName; //Name the window mang
    int width,height; //I can do this right?
    
}; //This goes into a configuration function

struct SysObjs{
    Uint32 framecount, aframetime;
    struct Renderer* renderer;

}; //This comes out via a pointer with all the system objects

//Small struct to hold video objects
struct Renderer{
    int width, height;
    SDL_Window* window;
    SDL_Renderer* sdl_renderer;
    SDL_Surface* back_buff;
    SDL_Surface* screen;
    Uint32 alpha_color;
    int (*RenderFunc)(); //need to deprecate this
    
};

//A render helper to allow for sprites
struct Sprite{
    int frames;
    int currentframe;
    int width, height;
    int screenX, screenY;
    SDL_Surface* frame_strip;
    SDL_Surface* currentFrame;
};

//Event-Controller interface
struct EventController{
    unsigned int event_code;
    unsigned char resolved; //this needs to be a bit...
    int (*Controller)(); //it's like a reverse constructor, forced no params
    struct EventController* next;
};

// This might be floats later. Might as well typedef it.
typedef int POSITION;

struct TouchInput{
    POSITION x;
    POSITION y;
    float dx;
    float dy;
};

enum EventCodes {
    TAND_INIT,
    TAND_PAUSE,
    TAND_RESUME,
    TAND_QUIT
}; //Basic event codes for case in event creation


//Here goes the model stuff
//Models based on a 2-dimensional Cosmos. For the start
//the screenX and screenY determine the camera placement
//in the huge "open" world. The models correlate to everything in the world
//including basic terrain (this will likely be the first thing to be expanded)
struct Cosmos{
    int screenX, screenY;
    //models go here
    struct Model* model_hash;
    struct Renderer* r_ptr; //Cosmos gets direct access to the renderer assets
}; //Serves as a model database
//A first optimization will revolve around partitioning the very large space in
//which the cosmos encompasses and organizing it easily for rendering and 
//computation

//Model Index unnecessary; wasted lines like this comment
struct Model{
    int id; //serves as pk
    char* name;
    struct Sprite* appearance; //appearance map
    int worldX, worldY; //from the world
    //here is where the chipmunk actor goes
    UT_hash_handle hh;         /* makes this structure hashable */
};
//Essentially an actor with a sprite attached

//here goes render stuff

//System-wide Variable Structs; aka Singletons
//Contains Renderer and Input mechanisms, etc.

//platform function's declares
  //system functions 
struct SysObjs* InitConfig(struct ConfigSys *conf);
void InitSystem();
void StartSystem();
void StopSystem();
void ResumeSystem();
void QuitSystem();
  //events and input section
int InitializeSystemEventHandler();
struct EventController* CreateEvent(enum EventCodes eventcode);
int ResolveEvent(struct EventController* resolver);
void ResolveSystemEvent();
struct EventController* InputPopQueue();
void InputPushQueue(struct EventController* pushed);
  //render functions
void FlushTextToScreen(SDL_Surface* textLayer, int x, int y);
void FlushToScreen(SDL_Surface* layer);
void FlushToHWScreen(SDL_Texture* layer, const SDL_Rect* dstrect);
void RenderHWScreen();
void RenderScreen();
void SetRenderFunc(int (*RenderFunc)());
  //render utility
SDL_Surface* LoadImageToSurface(char* imgname);
SDL_Surface* LoadTextToSurface(char* fontname, char* text);

//Model system (plugs into renderer above);
  //Cosmos model function's declares
void InitCosmos();
void AddCosmos(struct Model *m);
struct Model* GetCosmos(int key);
void DelCosmos(int key);

