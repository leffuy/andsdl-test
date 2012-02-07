//Basis for TANDUM engine

/*Small Description: I'm always told to keep the commenting simple. I guess
* that's true; all of this text in blue, won't be used. It's just for human
* eyes. Given this is completely written in C for simplicity in thinking
* I've tried to keep things organized by their components. Labeled with
* comments. 
*/

//includes
#include <stdio.h>
#include "SDL.h"

//defines

//platform structures


//platform functions


//entry point for application start
int main(int argc, char* argv[])
{
//I suppose everything else is implicitly initialized  
if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1;

//The Loop
while(1){


}

return 0;
}
