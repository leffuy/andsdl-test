#include "SDL.h"

#ifdef __ANDROID__
    #define DIR_CUR "/sdcard/"
    #define DIR_SEP	"/"
#else //UNIX
    #define DIR_CUR ""
    #define DIR_SEP "/"
#endif

#define DATAFILE(X)	DIR_CUR "data" DIR_SEP X

int main(int argc, char* argv[])
{
	// Window and Renderer.
	SDL_Window* window;
	SDL_Renderer* renderer;

	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		   return 1;

	// Create the application window. The size chosen is logical, not the exact
	// number of pixels of the window (mobiles will override it).
	window = SDL_CreateWindow("My SDL 1.3 Test",
		           SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		           640, 480, SDL_WINDOW_SHOWN);

	// Get the first available Hardware-accelerated renderer for this window.
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Select the base colour for drawing. It is set to red here.
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Load the image we will draw and convert it to a renderer-specific texture
    // for greater efficiency.
    SDL_Texture *texture = NULL;
    SDL_Surface *surface = NULL;
    // The App shouldn't crash even if this file is not actually present ;)
	surface = SDL_LoadBMP(DATAFILE("test.bmp"));
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	// Execute the main loop.
	int running = 1;
	while(running)
	{
		// Clear the entire screen to the Renderer's base colour.
		SDL_RenderClear(renderer);

		// Render the image on top of this. The two final parameters are the
		// source and destination sub-rectangle: NULL for both means we use
		// the whole image and stretch it across the whole screen.s
		SDL_RenderCopy(renderer, texture, NULL, NULL);

		// Flip the shown and hidden buffers to refresh the screen.
		SDL_RenderPresent(renderer);

		// Give other applications some time to execute.
		SDL_Delay(50);

		// Check for input events and exit if the window is closed (ex: pressing
		// the cross, closed by the OS)
		static SDL_Event event;
		while (SDL_PollEvent(&event))
        	if(event.type == SDL_QUIT)
        		running = 0;
	}

	// Remember to free up all the memory we've used, this ain't no Java!
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	// all clear, return EXIT_SUCCESS
	return 0;
}
