#ifndef __SCREENSIMULATION_H__
#	define __SCREENSIMJUULATION_H__

#	include <stdio.h>
#	include <stdlib.h>
#	include <string.h>

#	include "SDL2/SDL.h"
#	include "SDL2/SDL_ttf.h"
#	include "ElevatorSimulation.h"
#	include "DataModel.h"
#	include "DataImport.h"

int init_app(void);
void clear_background(const SDL_Color background_color);
void draw_rounded_box(const SDL_Rect target_rect, int radius);
void render_frame(void);
void quit_app(void);
void animate_route(void);
void AnimateSimulation(const char filename[]);

#endif
