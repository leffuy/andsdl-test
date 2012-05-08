
#include "thysio.h"

extern struct SysObjs* system_objects;
//render stuff functions

void SetRenderFunc(int (*RenderFunc)()){
    (*(*system_objects).renderer).RenderFunc = RenderFunc;
}

SDL_Surface* LoadImageToSurface(char* imgname){
    SDL_RWops *file = SDL_RWFromFile(imgname, "rb");
    SDL_Surface *image = IMG_Load_RW(file, 1);
    return image;
}

SDL_Surface* LoadTextToSurface(char* fontname, char* text){
    SDL_Color font_clr = {255 , 255 , 255 , 0 };
    SDL_RWops* font_raw = SDL_RWFromFile(fontname, "rb");
    TTF_Font* font = TTF_OpenFontRW(font_raw, 1, 12);
    SDL_Surface* surf_text = TTF_RenderText_Solid(font, (const char*)text, font_clr);
    return surf_text;
}

void FlushTextToScreen(SDL_Surface* textLayer, int x, int y){
    SDL_Rect dstrectum;
    if(textLayer != NULL){
        dstrectum.x = x;
        dstrectum.y = y;
        dstrectum.w = textLayer->w;
        dstrectum.h = textLayer->h;
    }
    else{
        return;
    }
    SDL_BlitSurface(textLayer, NULL, (*(*system_objects).renderer).screen, &dstrectum);
}

//TODO Should probably rename this to FlushLayerToScreen to make it more descriptive 
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
    //SDL_BlitSurface(layer, NULL, (*(*system_objects).renderer).back_buff, NULL);
    //is a mirror of the screen; this may be unnecessary in future
}

void RenderScreen(){
    SDL_UpdateWindowSurface((*(*system_objects).renderer).window);
//clear screen buffer after successful update
    //SDL_Log("Render error %s", SDL_GetError());
    SDL_FillRect((*(*system_objects).renderer).screen, NULL, 0);
    //SDL_FillRect((*(*system_objects).renderer).back_buff, NULL, 0);
    //ready for new frame!
}
