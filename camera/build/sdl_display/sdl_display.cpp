/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <fstream>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

using namespace std;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
	
SDL_Surface* gPNGSurface = NULL;


//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

//Loads individual image
SDL_Surface* loadSurface( string path );

SDL_Surface* graySurface = NULL;

SDL_Rect srcrect;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN);		

		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
	        {
	            //Initialize PNG loading
	            int imgFlags = IMG_INIT_JPG;
	            if( !( IMG_Init( imgFlags ) & imgFlags ) )
	            {
	                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
	                success = false;
	            }
	            else
	            {
	                //Get window surface
	                gScreenSurface = SDL_GetWindowSurface( gWindow );
	            }
        	}
	}

	return success;
}

bool loadMedia(int fileCount)
{
	
	srcrect.x = 80;
	srcrect.y = 0;
	srcrect.w = 640;
	srcrect.h = 480;

	//Loading success flag
	bool success = true;
	
	string fileName;
	string prefix = "image";
	string postfix = ".bmp";
	char temp[20];
	string dir = "";

	memset(temp, '\0', sizeof(char) * 20);
	sprintf(temp, "%d", fileCount);
	fileName = dir + prefix + temp + postfix;


	gPNGSurface = loadSurface(fileName.c_str());


	if( gPNGSurface == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError() );
		success = false;
	}

	SDL_BlitSurface( gPNGSurface, NULL, gScreenSurface, &srcrect);

	SDL_UpdateWindowSurface (gWindow);

	return success;
}

void close()
{
	//Deallocate surface
	SDL_FreeSurface( gHelloWorld );
	gPNGSurface = NULL;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Surface* loadSurface( string path )
{
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, NULL );
        if( optimizedSurface == NULL )
        {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return optimizedSurface;
}

int main( int argc, char* args[] )
{
	string tok;
	char * tok_char;

	int fileCount = 0;
	string fileName;
	string prefix = "image";
	string postfix = ".bmp";
	char temp[20];
	string command;
	string command2;

	int currentPic = 0;

	string take;
	char * take_char;
	string prev;
	char * prev_char;
	string next;
	char * next_char;

	string command_prefix = "./ppm2bmp image.ppm ";
	string command2_prefix = "convert -resize 800x480 ";
	string space = " ";

	string tok_output = "0 0 0";

	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		graySurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
	        SDL_FillRect(graySurface, NULL, SDL_MapRGB(graySurface->format, 0, 0, 0));
	        SDL_BlitSurface( graySurface, NULL, gScreenSurface, &srcrect );
        	SDL_UpdateWindowSurface( gWindow );
		
		while (1) {
			// ./ppm2bmp fileName modifiedFileName
			ifstream read("tok");
			getline(read, tok);
			read.close();
			
			tok_char = (char *) tok.c_str();

			take = strtok(tok_char, " ");
			prev = strtok(NULL, " ");
			next = strtok(NULL, " ");
			
			if (strcmp(take.c_str(), "1") == 0) {
				fileCount++;
				ofstream write("tok");
	    			write << "0";
	    			write.close();

				memset(temp, '\0', sizeof(char) * 20);
	    			sprintf(temp, "%d", fileCount);
	    			fileName = prefix + temp + postfix;
				command = command_prefix + fileName;
				
				command2 = command2_prefix + fileName + space + fileName;
				
				system(command.c_str());
		
				system(command2.c_str());
		
				loadMedia(fileCount);	
				
				currentPic = fileCount;
			} else if (strcmp(prev.c_str(), "1") == 0) {
				if (currentPic > 1) {
					currentPic--;
					loadMedia(currentPic);
				} else {
					continue;
				}
			} else if (strcmp(next.c_str(), "1") == 0) {
				if (currentPic < fileCount) {
					currentPic++;
					loadMedia(currentPic);
				} else {
					continue;
				}
				
			}
			ofstream write2("tok");
	    		write2 << tok_output.c_str();
	    		write2.close();

			usleep(1000000);
			
		}

		//Load media
		
	}
	//Free resources and close SDL
	close();
	return 0;
}
