#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "SDL.h"
#include "config.h"
#include "printer.h"
#include "field.h"
#include "rules.h"

#define RGBA_MASK	0, 0, 0, 0

void cleanup(){
	SDL_Quit();	
}

void update_matrix(SDL_Surface* screen, field* game){

	// check if field has already been printed - use printed_mutex
	pthread_mutex_lock(&game->field_printed_mutex);
	while (game->printed){
		pthread_cond_wait(&game->field_printed_cond, &game->field_printed_mutex);
	}	
	pthread_mutex_unlock(&game->field_printed_mutex); 

	SDL_Surface* cell = SDL_CreateRGBSurface(SDL_HWSURFACE, CELL_WIDTH - BORDER_WIDTH, CELL_HEIGHT - BORDER_WIDTH, 32, RGBA_MASK);

	Uint32 red = SDL_MapRGB(cell->format, 0xFF, 0x00, 0x00); 
	Uint32 green = SDL_MapRGB(cell->format, 0x00, 0xA0, 0x00);
	Uint32 blue = SDL_MapRGB(cell->format, 0x00, 0x00, 0x66);

	// set caption
	char title[30];
	sprintf(title, "Wa-Tor, Generation %i", game -> generation);
	SDL_WM_SetCaption(title, NULL);

	for (int i = 0; i < game -> height; i++){
		for (int j = 0; j < game -> width; j++){
			SDL_Rect pos;
			int elem = game -> mtx[i][j].type;

			if(elem == FISH){
				SDL_FillRect(cell, 0, green);  
				pos.x = j*CELL_WIDTH;
				pos.y = i*CELL_HEIGHT;
				SDL_BlitSurface(cell, 0, screen, &pos);
			} else if (elem == SHARK){
				SDL_FillRect(cell, 0, red);  
				pos.x = j*CELL_WIDTH;
				pos.y = i*CELL_HEIGHT;
				SDL_BlitSurface(cell, 0, screen, &pos);
			} else {
				SDL_FillRect(cell, 0, blue);  
				pos.x = j*CELL_WIDTH;
				pos.y = i*CELL_HEIGHT;
				SDL_BlitSurface(cell, 0, screen, &pos);				
			}
		}
	}

	//update status, field has been printed, send signal to other threads
	pthread_mutex_lock(&game->field_printed_mutex);                                
		game->printed = true;                                                          
		pthread_cond_broadcast(&game->field_printed_cond);
	pthread_mutex_unlock(&game->field_printed_mutex); 

	usleep(ANIMATION_SPEED * 10000);
}


void* print_game(void* p_args){
	field* game = (field*) p_args;

	if (GUI) {
		atexit(cleanup);

		SDL_Init(SDL_INIT_VIDEO);
		SDL_Surface* screen = SDL_SetVideoMode(game -> width * CELL_WIDTH, game -> height * CELL_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

		// set background color black
		Uint32 bgcolor = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);

		SDL_FillRect(screen, 0, bgcolor);

		SDL_Event event;

		int running = true;

		while(running){
			if (SDL_PollEvent(&event)) {

				// quit if x is clicked or any key is pressed
				if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN) {
					running = false;
					break;
				}
			}
			update_matrix(screen, game);

			SDL_Flip(screen);			
		}
		//SDL_Quit();
		exit(0);
	} else {

		int running = true;

		while(running){
			// check if field has already been printed - use printed_mutex
			pthread_mutex_lock(&game->field_printed_mutex);
			while (game->printed){
				pthread_cond_wait(&game->field_printed_cond, &game->field_printed_mutex);
			}	
			pthread_mutex_unlock(&game->field_printed_mutex); 

			printf("Generation %i:\n", game -> generation);
			printf("----------------------------------------\n");
			for (int i = 0; i < game -> height; i++){
				for (int j = 0; j < game -> width; j++){
					int elem = game -> mtx[i][j].type;
					if (elem == SHARK){
						printf("\x1B[31m%d ", elem);
					} else if (elem == FISH){
						printf("\x1B[32m%d ", elem);
					} else {
						printf("\x1B[34m%d ", elem);
					}
				}
				printf("\n");
			}
			printf("----------------------------------------\n");

			//update status, field has been printed, send signal to other threads
			pthread_mutex_lock(&game->field_printed_mutex);                                
			game->printed = true;                                                          
			pthread_cond_broadcast(&game->field_printed_cond);
			pthread_mutex_unlock(&game->field_printed_mutex);

			usleep(ANIMATION_SPEED * 10000);
		}
	}

	return 0;
}

