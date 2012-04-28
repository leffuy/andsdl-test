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

#include "thysio.h"

struct ConfigSys system_configs; //maybe these will be pointers one day...
struct SysObjs* system_objects;
struct EventController* headController;
struct EventController* tailController;
struct Cosmos kosmos;

//Utility functions
Uint32 getpixel(SDL_Surface *surface, int x, int y);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

//ewww I hate this function not part of the platform just a test
void renderTest();
void UpdatePosition(int x, int y);
SDL_Surface* blahder;
SDL_Surface* blahsprite;

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
    if (TTF_Init()){
        SDL_Log("Error with TTF_Init(): %s", SDL_GetError());
        return NULL;
    }
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
    (*tmpsysobj).aframetime = 0;
    (*tmpsysobj).framecount = 0;

    return tmpsysobj;
}

//model stuff functions


//these guys handle event creation/resolution

//Note to Patrick:
//Please fucking change this if you feel that it needs to be, don't even 
//hesitate. I don't like over using if statements and a switch() is just that


//I have a feeling at some point this will put in an interface 
//with a create_scheme

//these three guys manage the event loop
void InitSystem(){
    system_objects = InitConfig(&system_configs); //this is simple for now
    //setup system event queue here...
}

void StartSystem(){
    (*system_objects).framecount = 0;
    if(!(*system_objects).renderer)
        return; //kind of like throwing an exception?

    
//Root loop things can be scheduled around this
    for(;;){
        //start of count
        Uint32 startFrame = SDL_GetTicks();
        ResolveSystemEvent();
        struct EventController* tmpEvent = InputPopQueue();
        //events are read in the main thread, and pushed from all threads

        //resolve event is setup to launch it's own threads
/*
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
        } */
        //This should be in the most root thread along with IO

//ALl objects will have their own render function
        if((*(*system_objects).renderer).RenderFunc)
            (*(*system_objects).renderer).RenderFunc();

        RenderScreen();
        Uint32 endFrame = SDL_GetTicks();

        (*system_objects).aframetime = 1000 / (endFrame - startFrame);

        (*system_objects).framecount += 1;
        //end here this time should give you the amount of time it takes to render to the screen 
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

    //    SDL_Surface* tmpText = LoadTextToSurface("courier.ttf", "X and Y");
	//SDL_Log("Is there a number here or a segfault: %d", tmpText->h);
	FlushToScreen(blahder); //think of blahder as the master layer
	//FlushTextToScreen(tmpText, 0, 0);

	return 0;
}

//this will statically update the screen position of the hardcoded
//rectangle

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
