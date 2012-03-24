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

#include "uthash.h"
//#include "hashmap.h"
#include "SDL.h"
#include "SDL_image.h"

//defines


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
    struct Renderer* renderer;

}; //This comes out via a pointer with all the system objects

//Small struct to hold video objects
struct Renderer{
    int width, height;
    SDL_Window* window;
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
struct ConfigSys system_configs; //maybe these will be pointers one day...
struct SysObjs* system_objects;
struct EventController* headController;
struct EventController* tailController;
struct Cosmos kosmos;

//platform function's declares
  //system functions 
struct SysObjs* InitConfig(struct ConfigSys *conf);
void InitSystem();
void StartSystem();
void StopSystem();
void ResumeSystem();
void QuitSystem();
  //events and input section
struct EventController* CreateEvent(enum EventCodes eventcode);
int ResolveEvent(struct EventController* resolver);
void ResolveSystemEvent();
struct EventController* InputPopQueue();
void InputPushQueue(struct EventController* pushed);
  //render functions
void FlushToScreen(SDL_Surface* layer);
void RenderScreen();
void SetRenderFunc(int (*RenderFunc)());
  //render utility
SDL_Surface* LoadImageToSurface(char* imgname);

SDL_Surface* blahder;
SDL_Surface* blahsprite;

//Model system (plugs into renderer above);
  //Cosmos model function's declares
void InitCosmos();
void AddCosmos(struct Model *m);
struct Model* GetCosmos(int key);
void DelCosmos(int key);


//Utility functions
Uint32 getpixel(SDL_Surface *surface, int x, int y);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

//ewww I hate this function not part of the platform just a test
void renderTest();
void UpdatePosition(int x, int y);

//utility functions
Uint32 getpixel(SDL_Surface *surface, int x, int y){
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel){
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

//Cosmos Stuff?
void InitCosmos(){
    kosmos.model_hash = NULL;
}

void AddCosmos(struct Model *m){
    HASH_ADD_INT(kosmos.model_hash, id, m);
}

struct Model* GetCosmos(int key){
    struct Model* m = NULL;
    
    HASH_FIND_INT(kosmos.model_hash, &key, m);

    return m;
}

void DelCosmos(int key){
    struct Model* m = NULL;
    m = GetCosmos(key);
    HASH_DEL(kosmos.model_hash, m);
}

//Init's screen and window stuff
struct SysObjs* InitConfig(struct ConfigSys *conf){
    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return NULL;
    SDL_Window *window = SDL_CreateWindow((*conf).windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (*conf).width, (*conf).height, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
    //do something with these 
    struct SysObjs* tmpsysobj = (struct SysObjs*)malloc(sizeof(struct SysObjs));
    (*tmpsysobj).renderer = (struct Renderer*)malloc(sizeof(struct Renderer));

    (*(*tmpsysobj).renderer).width = (*conf).width;
    (*(*tmpsysobj).renderer).height = (*conf).height;

    (*(*tmpsysobj).renderer).window = window;
    (*(*tmpsysobj).renderer).screen = SDL_GetWindowSurface(window);
    SDL_Surface* sptr = (*(*tmpsysobj).renderer).screen;
    (*(*tmpsysobj).renderer).back_buff = SDL_CreateRGBSurface(0, (*sptr).w, (*sptr).h, (*(*sptr).format).BitsPerPixel,(*(*sptr).format).Rmask,(*(*sptr).format).Gmask,(*(*sptr).format).Bmask,(*(*sptr).format).Amask);
    //At some point this thing below will be configurable
    //For now, just use the fucking value: 255,162,249
    //will send out value from jpg created using gimp

    //Tom theorizes this will work. (by induction)
 //   SDL_Log("The address of format: %d", (*(*(*tmpsysobj).renderer).screen).format);
    //Should print SOMETHING before it segfaults, which I'm sure it will
//    SDL_Log("What kind is this? %d", SDL_MapRGB((*(*(*tmpsysobj).renderer).screen).format, 250,162,255));
    Uint32 alpha = SDL_MapRGB((*(*(*tmpsysobj).renderer).screen).format, 255,162,249);
    (*(*tmpsysobj).renderer).alpha_color = alpha;
//    SDL_Log("Alpha color raw: %d", (*(*tmpsysobj).renderer).alpha_color);

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
//    SDL_SetSurfaceAlphaMod(layer, 0);
    SDL_SetColorKey(layer, SDL_TRUE, (*(*system_objects).renderer).alpha_color);
//    SDL_Log("Alpha color raw: %d", (*(*system_objects).renderer).alpha_color);

    SDL_BlitSurface(layer, NULL, (*(*system_objects).renderer).screen, NULL);
    //I like this line here; instead of screen back up buffer
    SDL_BlitSurface(layer, NULL, (*(*system_objects).renderer).back_buff, NULL);
    //is a mirror of the screen; this may be unnecessary in future
}

void RenderScreen(){
    SDL_UpdateWindowSurface((*(*system_objects).renderer).window);
//clear screen buffer after successful update
    SDL_Log("Render error %s", SDL_GetError());
    SDL_FillRect((*(*system_objects).renderer).screen, NULL, 0);
    SDL_FillRect((*(*system_objects).renderer).back_buff, NULL, 0);
    //ready for new frame!
}

//these guys handle event creation/resolution

//Note to Patrick:
//Please fucking change this if you feel that it needs to be, don't even 
//hesitate. I don't like over using if statements and a switch() is just that

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

    SDL_PollEvent(&sdl_event);
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

//I have a feeling at some point this will put in an interface 
//with a create_scheme

//these three guys manage the event loop
void InitSystem(){
    system_objects = InitConfig(&system_configs); //this is simple for now
    //setup system event queue here...
}

void SetRenderFunc(int (*RenderFunc)()){
    (*(*system_objects).renderer).RenderFunc = RenderFunc;
}

void StartSystem(){
    if(!(*system_objects).renderer)
        return; //kind of like throwing an exception?

    
//Root loop things can be scheduled around this
    for(;;){
        ResolveSystemEvent();
        struct EventController* tmpEvent = InputPopQueue();
        //events are read in the main thread, and pushed from all threads

        //resolve event is setup to launch it's own threads
	if(ResolveEvent(tmpEvent) == 1){ //this will plug into the pause menu
        //omg another one                   one day
            SDL_MinimizeWindow((*(*system_objects).renderer).window);
            for(;;){
//                SDL_Log("We have entered the idle event looper");
                ResolveSystemEvent();
                struct EventController* tmpEventIdle = InputPopQueue();
                if(ResolveEvent(tmpEventIdle) == 2){
                    ResumeSystem();
                    SDL_Log("Resuming last screen state");
                    break;
                }
            }
        }
        //This should be in the most root thread along with IO

//ALl objects will have their own render function
        if((*(*system_objects).renderer).RenderFunc)
            (*(*system_objects).renderer).RenderFunc();

        RenderScreen();
    }
}

//Throws an event from any thread to cause start_system to break earlier when
//event is found 
void StopSystem(){
//need more of the system implemented before I can fill out this stub
}

void QuitSystem(){

}

void ResumeSystem(){
    SDL_Log("Handle to screen: %d", (*(*system_objects).renderer).screen);
    SDL_Log("Handle to backup: %d", (*(*system_objects).renderer).back_buff);
    SDL_Log("Handle to window: %d", (*(*system_objects).renderer).window);
//    SDL_Delay(5000);
    //Try a window swap to get thie 
    SDL_GL_SwapWindow((*(*system_objects).renderer).window);
//recover the screen data hopefully
    SDL_Log("Window recreation");
 //   SDL_RecreateWindow((*(*system_objects).renderer).window, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
    SDL_Log("Window recreation done, Getting new surface");
    (*(*system_objects).renderer).screen = SDL_GetWindowSurface((*(*system_objects).renderer).window); //get a new handle to the screen if lost somehow
    SDL_Log("GetWindow error? %s \n", SDL_GetError());
    SDL_Log("GetWindow error? %s \n", SDL_GetError());
    SDL_BlitSurface((*(*system_objects).renderer).back_buff, NULL, (*(*system_objects).renderer).screen, NULL);
 
//    renderTest();
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


//flush to screen here per frame 
int myRenderFunc(){
    /*
	static int tx = 20;
	static int ty = 20;
	UpdatePosition(tx,ty);
	tx += 1;
	ty += 1;

	if(tx >= 50) tx = 20;

	if(ty >= 50) ty = 20;
    */

	FlushToScreen(blahder); //think of blahder as the master layer

	return 0;
}

//this will statically update the screen position of the hardcoded
//rectangle
void UpdatePosition(int x, int y){
    SDL_FillRect(blahder, NULL, 0);
    SDL_Rect dstrectum;
    dstrectum.x = x;
    dstrectum.y = y;
    dstrectum.w = 40;
    dstrectum.h = 40;


    SDL_BlitSurface(blahsprite, NULL, blahder, &dstrectum);
}

void renderTest(){
SDL_Rect dstrectum;
dstrectum.x = 20;
dstrectum.y = 20;
dstrectum.w = 40;
dstrectum.h = 40;

SDL_Surface* sptr = (*(*system_objects).renderer).screen;


SDL_Surface* rectangleTest = LoadImageToSurface("rectangle.jpg");

SDL_Surface* testSurface = SDL_CreateRGBSurface(0, (*sptr).w, (*sptr).h, (*(*rectangleTest).format).BitsPerPixel,(*(*rectangleTest).format).Rmask,(*(*rectangleTest).format).Gmask,(*(*rectangleTest).format).Bmask,0);

//going to lock the surface and make the entire rectangle the alpha color
int i = 0;
int j = 0;
SDL_LockSurface( rectangleTest );
Uint32 testcol = getpixel(rectangleTest, 0, 0);

/*for(i = 0; i < 40; i++){
for(j = 0; j < 40; j++){
putpixel(rectangleTest, i, j, (system_objects->renderer)->alpha_color);
}
}*/

//i=0;
//j=0;
SDL_UnlockSurface( rectangleTest );

SDL_Log("Check alpha color.\n SystemSettting: %i\nFromDrawing: %i", (system_objects->renderer)->alpha_color, testcol);
//SDL_Log(" ");


//this seems to work for the original blit
//SDL_SetColorKey(rectangleTest, SDL_TRUE, (*(*system_objects).renderer).alpha_color);
SDL_BlitSurface(rectangleTest, NULL, testSurface, &dstrectum);

//SDL_FreeSurface(rectangleTest);

blahder = testSurface;
blahsprite = rectangleTest;
//SDL_SetColorKey(blahder, SDL_TRUE, (*(*system_objects).renderer).alpha_color);
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
    renderTest(); //setups the render system sort of
    SetRenderFunc(&myRenderFunc);
    StartSystem();

    //Wow with the way we set it up main is very simple
    return 0;
}


//One day I will remove all the comments. This shit is stupid.
//I feel like this will annoy a good programmer rather than help.
//but for some reason it helps me (as of now)
