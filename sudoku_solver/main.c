#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "solver.h"
#include "utils.h"

#define PLAY_AREA_WIDTH 500
#define PLAY_AREA_HEIGHT 550
#define TEXT_STRIP_HEIGHT 50

struct Cursor
{
    SDL_Rect cursor; // the cursor size and world position (rect)
    SDL_Point pos; // the cursor grid position
};
typedef struct Cursor cursorStrct;

typedef struct textures
{
    SDL_Texture* texts[10]; 
    TTF_Font* myFont;
}Textures;

Textures myTextures;
float cursor_time; // accumulate time for the cursor flasing action.
bool isRunning = true;
bool unsolvable = false; // bad puzzle
cursorStrct cursor;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
char* message = "";

float getDeltaTime(); // get the ellapsed time since last upate.
void processEvents();
void update(float dt);
void render();
void drawCursor(); // a flashing cursor
void drawGrid(); 
bool init();//Starts up SDL and creates window
void prepare_textures(); // prepare the digit textures
void closeAll();//Frees media and shuts down SDL
SDL_Point get_world_for_coords(SDL_Point); // get the screen coordinates for grid coordinates 
void displayDigits(); // print the digits from the array onto the screen.
void clearAll(); // clear the puzzle


int grid[9][9] = {
		     { 1, 0, 0, 0, 5, 0, 0, 0, 0 },  
                     { 0, 2, 0, 0, 0, 0, 0, 0, 0 },
                     { 0, 0, 3, 0, 0, 0, 0, 0, 0 },  
                     { 0, 0, 0, 4, 0, 0, 0, 0, 0 },
                     { 0, 0, 0, 0, 5, 0, 0, 0, 0 },
                     { 0, 0, 0, 0, 0, 6, 0, 0, 0 },
                     { 0, 0, 0, 0, 0, 0, 7, 0, 0 },
                     { 0, 0, 0, 0, 0, 0, 0, 8, 0 },
                     { 0, 0, 0, 0, 0, 0, 0, 0, 9 } };


int main()
{
  if (!init())
   {
       printf("initialization failed\n");
       closeAll();
       return 0;
   }

  // load the font
    myTextures.myFont = TTF_OpenFont( "pixelFont.ttf", 18 );
   
   // prepare the digits
   prepare_textures();

    cursor.cursor.w = cursor.cursor.h = 50;
    cursor.pos.x = cursor.pos.y = 0;
    SDL_Point point = get_world_for_coords(cursor.pos); 
    cursor.cursor.x = point.x;
    cursor.cursor.y = point.y;
    //cursor.pos.x = cursor.pos.y = 0;
    cursor_time = 0;
    
    while(isRunning)
    {
        processEvents();
        update(getDeltaTime());
        render();
    }
    
    closeAll();
    return 0;
}

float getDeltaTime()
{
  static long long  last = 0;
  long long now = SDL_GetTicks();
  float deltaTime = (float)(now -last);
  last = now;
  return deltaTime;
 }

// initialize window and renderer. and keyboard state
bool init()
{
  isRunning = true;
  bool success = true;
  SDL_Init(SDL_INIT_VIDEO);
 window=SDL_CreateWindow("Sudoku Solver",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,PLAY_AREA_WIDTH,PLAY_AREA_HEIGHT, SDL_WINDOW_RESIZABLE);
  renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );

  // init ttf
    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not be initialized! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
  
  return success;
}

 // prepare the digit textures
void prepare_textures()
{
    myTextures.myFont = TTF_OpenFont( "pixelFont.ttf", 35 );
    SDL_Color color = { 0, 250, 150, 255 };
    
    loadFromRenderedTexture(&(myTextures.texts[1]), "1",myTextures.myFont, color ,renderer);
    loadFromRenderedTexture(&(myTextures.texts[2]), "2",myTextures.myFont, color ,renderer);
    loadFromRenderedTexture(&(myTextures.texts[3]), "3",myTextures.myFont, color ,renderer);
    loadFromRenderedTexture(&(myTextures.texts[4]), "4",myTextures.myFont, color ,renderer);
    loadFromRenderedTexture(&(myTextures.texts[5]), "5",myTextures.myFont, color ,renderer);
    loadFromRenderedTexture(&(myTextures.texts[6]), "6",myTextures.myFont, color ,renderer);
    loadFromRenderedTexture(&(myTextures.texts[7]), "7",myTextures.myFont, color ,renderer);
    loadFromRenderedTexture(&(myTextures.texts[8]), "8",myTextures.myFont, color ,renderer);
    loadFromRenderedTexture(&(myTextures.texts[9]), "9",myTextures.myFont, color ,renderer);
    loadFromRenderedTexture(&(myTextures.texts[0]), "Bad puzzle",myTextures.myFont, color ,renderer);
}

void processEvents()
{
 SDL_Event event;
 while (SDL_PollEvent(&event))
  {
    switch (event.type)
     {
          case SDL_QUIT:
           isRunning = false;
             break;
          case SDL_KEYUP:
            {
               if ( (event.key.keysym.sym) == SDLK_RETURN)
		{
                   if(!findSolution(grid))
                      unsolvable = true;
		}
                else if ((event.key.keysym.sym) == SDLK_c)
                    clearAll();
		else if ((event.key.keysym.sym) == SDLK_LEFT)
                    cursor.pos.x = max(cursor.pos.x - 1, 0);
		else if ((event.key.keysym.sym) == SDLK_RIGHT)
                    cursor.pos.x = min(cursor.pos.x + 1, 8);
		else if ((event.key.keysym.sym) == SDLK_UP)
                    cursor.pos.y = max(cursor.pos.y - 1, 0);
		else if ((event.key.keysym.sym) == SDLK_DOWN)
                    cursor.pos.y = min(cursor.pos.y + 1, 8);
		else if ( (event.key.keysym.sym) == SDLK_0)
                    grid[cursor.pos.y][cursor.pos.x] = 0;
		else if ( (event.key.keysym.sym) == SDLK_1)
                    grid[cursor.pos.y][cursor.pos.x] = 1;
		else if ( (event.key.keysym.sym) == SDLK_2)
                    grid[cursor.pos.y][cursor.pos.x] = 2;
		else if ( (event.key.keysym.sym) == SDLK_3)
                    grid[cursor.pos.y][cursor.pos.x] = 3;
		else if ( (event.key.keysym.sym) == SDLK_4)
                    grid[cursor.pos.y][cursor.pos.x] = 4;
		else if ( (event.key.keysym.sym) == SDLK_5)
                    grid[cursor.pos.y][cursor.pos.x] = 5;
		else if ( (event.key.keysym.sym) == SDLK_6)
                    grid[cursor.pos.y][cursor.pos.x] = 6;
		else if ( (event.key.keysym.sym) == SDLK_7)
                    grid[cursor.pos.y][cursor.pos.x] = 7;
		else if ( (event.key.keysym.sym) == SDLK_8)
                    grid[cursor.pos.y][cursor.pos.x] = 8;
		else if ( (event.key.keysym.sym) == SDLK_9)
                    grid[cursor.pos.y][cursor.pos.x] = 9;

                break;
             }
        }
    }
}

void update(float dt)
{
   cursor_time = fmod(cursor_time + dt, 1000.0);
  //update the cursor world positon.
  SDL_Point point;
  point.x = cursor.pos.x;
  point.y = cursor.pos.y;
  point = get_world_for_coords(point);
  cursor.cursor.x = point.x;
  cursor.cursor.y = point.y;
}

void render()
{

   //Clear screen
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);

    drawGrid();
    drawCursor();
    displayDigits();
    
   if (unsolvable) // bad puzze, display text indication that
   {
	int width, height;
        SDL_QueryTexture(myTextures.texts[0], NULL, NULL, &width, &height);
    	SDL_Rect rect = { width / 2, 25, width, height };
        SDL_RenderCopy(renderer,myTextures.texts[0], NULL, &rect);
   }
    //Update screen
    SDL_RenderPresent(renderer);
  
}

//draws a flashing cursor.
void drawCursor()
{
    if (cursor_time > 500)
    {
        SDL_SetRenderDrawColor(renderer, 0x00, 0xc0, 0x00, 0x00);
        SDL_RenderFillRect(renderer, &cursor.cursor);
    }
}

void drawGrid()
{
   // vertical lines
    for (int i = 2,counter = 0; i <= PLAY_AREA_WIDTH - 2; i += 55, ++counter)
    {
        SDL_SetRenderDrawColor(renderer, 0x00, 0xc0, 0x00, 0x00);
        if (!(counter%3))
            SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 0x00); // every 3rd line make a brighter one, ease of viewing.
        SDL_RenderDrawLine(renderer, i, TEXT_STRIP_HEIGHT, i, PLAY_AREA_HEIGHT - 2);
    }
    
    // horizontal lines
    for (int i = TEXT_STRIP_HEIGHT , counter = 0; i <= PLAY_AREA_HEIGHT - 2; i += 55,++counter)
    {
        SDL_SetRenderDrawColor(renderer, 0x00, 0xc0, 0x00, 0x00);
        if (!(counter%3))
            SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 0x00); // every 3rd line make a brighter one, ease of viewing.
        
        SDL_RenderDrawLine(renderer, 2, i, PLAY_AREA_WIDTH - 2, i);
    }
  
}

 // print the digits from the array onto the screen.
void displayDigits()
{
  int width,height;
  SDL_Point point;
  for (int row = 0; row < 9; ++row)
  {
    for (int col = 0; col < 9; ++col)
    {
      int num = grid[row][col];
      if (num)
      {
        point.x = col;
	point.y = row;
        point = get_world_for_coords(point);
        SDL_QueryTexture(myTextures.texts[num], NULL, NULL, &width, &height);
    	SDL_Rect rect = { point.x + width / 2, point.y + 7, width, height };
        SDL_RenderCopy(renderer,myTextures.texts[num], NULL, &rect);
      }
    }
  }
}

 // get the screen coordinates for grid coordinates 
SDL_Point get_world_for_coords(SDL_Point coords)
{
  coords.x = coords.x * (PLAY_AREA_WIDTH / 9) + 5;
  coords.y = coords.y * (PLAY_AREA_WIDTH / 9) + TEXT_STRIP_HEIGHT + 3; // I used Play area width on purpose, height is the same as width
  return coords;
}

// clear the puzzle
void clearAll()
{
  for (int i = 0; i < 9; ++i)
  {
    for (int j =0; j < 9; ++j)
    grid[i][j] = 0;
  }
  unsolvable = false;
}


// free everyting
void closeAll()
{
    free(myTextures.myFont);
    for (int i = 0; i <= 9; ++i)
     freeTexture(myTextures.texts[i]);

    myTextures.myFont = NULL;

    //Destroy window
    SDL_DestroyWindow( window);
    window = NULL;
    SDL_DestroyRenderer( renderer );
    renderer = NULL;

 //Quit SDL subsystems
   TTF_Quit();
   SDL_Quit();
}




















