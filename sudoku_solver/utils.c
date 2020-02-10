#include "utils.h"


float clamp(float x, float a, float b) // keep x between a and b
{
  return x < a ? a : ( x > b ? b : x);
}

void freeTexture(SDL_Texture* tex)
{
    //Free texture if it exists
    if( tex )
    {
        SDL_DestroyTexture( tex );
        tex= NULL;
    }
}

// convert and integer to a string.
char* int_to_ascii(int num)
{
    int count = (num == 0) ? 1 : 0;
    // get the length of the number first
    int numCpy = num;
    while (numCpy)
    {
        numCpy /= 10;
        ++count;
    }
    char* str = malloc(count + 1); // +1 for the null terminator.
    str[count] = '\0';
    --count;
    str[0] = '0';
    while(num)
    {
        str[count] = (num % 10) + '0';
        num /= 10;
        --count;
    }
    // caller of this function has to release the memory allocated.
    
    return str;
}

// get the bigger of the two.
int max(int x, int y)
{
    return ( x >= y ) ? x : y;
}

int min(int x, int y)
{
    return ( x <= y ) ? x : y;
}

// create a texture from a string.
void loadFromRenderedTexture(SDL_Texture** tex, const char* str, TTF_Font* gFont, SDL_Color color ,SDL_Renderer* rend)
{
    // free pre exitsting texture.
    freeTexture(*tex);
    
    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, str, color );
    if( textSurface == NULL )
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    else
    {
        //Create texture from surface pixels
        *tex = SDL_CreateTextureFromSurface( rend, textSurface );
        if( tex == NULL )
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
}

















