#include "ScreenSimulation.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
TTF_Font *font = NULL;
int width = 1024;
int height = 768;
SDL_Rect building;
SDL_Color gray = {0x95, 0x95, 0x95, 0xff};
SDL_Color black = {0x00, 0x00, 0x00, 0xff};
SDL_Color white = {0xff, 0xff, 0xff, 0xff};
SDL_Rect elevator;
int floors;
int delay;

int init_app(void)
{
	/* Init SDL. */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		fprintf(stderr, "SDL could not be inited! SDL Error: %s\n", SDL_GetError());
		return 0;
	}
	/* Create window. */
	window = SDL_CreateWindow("Lab7-elevator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
	if (window == NULL) {
		fprintf(stderr, "Window could not be created! SDL Error: %s\n", SDL_GetError());
		return 0;
	}
	/* Create renderer. */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (renderer == NULL) {
		fprintf(stderr, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return 0;
	}
	/* Main black screen buffer texture. */
	texture = SDL_CreateTexture(renderer, 0, SDL_TEXTUREACCESS_TARGET, width, height);
	if (texture == NULL) {
		fprintf(stderr, "Texture could not be created! SDL Error: %s\n", SDL_GetError());
		return 0;
	}
	/* Init SDL_ttf. */
	if (TTF_Init() < 0) {
		fprintf(stderr, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return 0;
	}
	/* Load custom/fallback font. */
	font = TTF_OpenFont("NotoSansCJKsc-Regular.otf", height / 30);
	if (font == NULL) {
		fprintf(stderr, "Font could not be opened! TTF Error: %s\n", TTF_GetError());
		return 0;
	}
	return 1;
}

void clear_background(const SDL_Color background_color)
{
	SDL_SetRenderTarget(renderer, texture);
	SDL_SetRenderDrawColor(renderer, \
			       background_color.r, background_color.g, \
			       background_color.b, background_color.a);
	SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer, NULL);
}

void draw_rounded_box(const SDL_Rect target_rect, int radius)
{
	if (radius <= 1) {
		SDL_RenderFillRect(renderer, &target_rect);
		return;
	}
	if (2 * radius > target_rect.w)
		radius = target_rect.w / 2;
	if (2 * radius > target_rect.h)
		radius = target_rect.h / 2;
	int x = 0;
	int y = radius;
	int d = 3 - 2 * radius;
        while (x <= y) {
		SDL_RenderDrawLine(renderer, \
				   target_rect.x + radius - x, \
				   target_rect.y + radius - y, \
				   target_rect.x + target_rect.w - \
				   radius + x - 1, \
				   target_rect.y + radius - y);
		SDL_RenderDrawLine(renderer, \
				   target_rect.x + radius - x, \
				   target_rect.y + target_rect.h - \
				   radius + y, \
				   target_rect.x + target_rect.w - \
				   radius + x - 1, \
				   target_rect.y + target_rect.h - \
				   radius + y);
		SDL_RenderDrawLine(renderer, \
				   target_rect.x + radius - y, \
				   target_rect.y + radius - x, \
				   target_rect.x + target_rect.w - \
				   radius + y - 1, \
				   target_rect.y + radius - x);
		SDL_RenderDrawLine(renderer, \
				   target_rect.x + radius - y, \
				   target_rect.y + target_rect.h - \
				   radius + x, \
				   target_rect.x + target_rect.w - \
				   radius + y - 1, \
				   target_rect.y + target_rect.h - \
				   radius + x);
        	if (d < 0) {
			d = d + 4 * x + 6;
        	} else {
			d = d + 4 * (x - y) + 10;
        		y--;
        	}
        	x++;
        }
	SDL_Rect temp_rect;
	temp_rect.x = target_rect.x;
	temp_rect.y = target_rect.y + radius;
	temp_rect.w = target_rect.w;
	temp_rect.h = target_rect.h - 2 * radius;
	SDL_RenderFillRect(renderer, &temp_rect);
}

void render_frame(void)
{
	clear_background(gray);
	SDL_SetRenderTarget(renderer, texture);
	int radius = height / 100;
	SDL_Rect text;
	SDL_Rect text_rect;
	char details[256];
	SDL_Surface *text_surface = TTF_RenderUTF8_Blended(font, u8"建筑物", black);
	SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = building.x + (building.w - text_surface->w) / 2;
	text.y = (building.y - text_surface->h) / 2;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	text_surface = TTF_RenderUTF8_Blended(font, u8"A电梯状态：", black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = building.x + building.w + (width - (building.x + building.w)) * 1 / 20;
	text.y = (building.y - text_surface->h) / 2;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
	text_rect.x = building.x + building.w + (width - (building.x + building.w)) * 1 / 20;
	text_rect.y = building.y;
	text_rect.w = (width - (building.x + building.w)) * 8 / 20;
	text_rect.h = height / 2 - text_rect.y;
	draw_rounded_box(text_rect, radius);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	sprintf(details, u8"电梯当前楼层：%d", elevator_a.current_floor);
	text_surface = TTF_RenderUTF8_Blended(font, details, black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y = text_rect.y;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	if (elevator_a.run_state == 'U')
		sprintf(details, u8"电梯当前状态：上行");
	else if (elevator_a.run_state == 'D')
		sprintf(details, u8"电梯当前状态：下行");
	else
		sprintf(details, u8"电梯当前状态：静止");
	text_surface = TTF_RenderUTF8_Blended(font, details, black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y += text.h;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	text_surface = TTF_RenderUTF8_Blended(font, u8"电梯服务指令：", black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y += text.h;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	if (elevator_a.current_usercall != NULL)
		sprintf(details, u8"  用户楼层：%d", elevator_a.current_usercall->user_floor);
	else
		sprintf(details, u8"  用户楼层：");
	text_surface = TTF_RenderUTF8_Blended(font, details, black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y += text.h;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	if (elevator_a.current_usercall != NULL)
		sprintf(details, u8"  目标楼层：%d", elevator_a.current_usercall->user_target);
	else
		sprintf(details, u8"  目标楼层：");
	text_surface = TTF_RenderUTF8_Blended(font, details, black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y += text.h;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	if (elevator_a.serve_state == 'E')
		sprintf(details, u8"  服务状态：服务中");
	else
		sprintf(details, u8"  服务状态：服务前");
	text_surface = TTF_RenderUTF8_Blended(font, details, black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y += text.h;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	if (elevator_a.current_usercall != NULL)
		if (elevator_a.current_usercall->call_type == 'U')
			sprintf(details, u8"  指令类型：上行");
		else if (elevator_a.current_usercall->call_type == 'D')
			sprintf(details, u8"  指令类型：下行");
		else
			sprintf(details, u8"  指令类型：静止");
	else
		sprintf(details, u8"  指令类型：");
	text_surface = TTF_RenderUTF8_Blended(font, details, black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y += text.h;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	text_surface = TTF_RenderUTF8_Blended(font, u8"B电梯状态：", black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = building.x + building.w + (width - (building.x + building.w)) * 11 / 20;
	text.y = (building.y - text_surface->h) / 2;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
	text_rect.x = building.x + building.w + (width - (building.x + building.w)) * 11 / 20;
	text_rect.y = building.y;
	text_rect.w = (width - (building.x + building.w)) * 8 / 20;
	text_rect.h = height / 2 - text_rect.y;
	draw_rounded_box(text_rect, radius);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	sprintf(details, u8"电梯当前楼层：%d", elevator_b.current_floor);
	text_surface = TTF_RenderUTF8_Blended(font, details, black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y = text_rect.y;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	if (elevator_b.run_state == 'U')
		sprintf(details, u8"电梯当前状态：上行");
	else if (elevator_b.run_state == 'D')
		sprintf(details, u8"电梯当前状态：下行");
	else
		sprintf(details, u8"电梯当前状态：静止");
	text_surface = TTF_RenderUTF8_Blended(font, details, black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y += text.h;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	text_surface = TTF_RenderUTF8_Blended(font, u8"电梯服务指令：", black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y += text.h;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	if (elevator_b.current_usercall != NULL)
		sprintf(details, u8"  用户楼层：%d", elevator_b.current_usercall->user_floor);
	else
		sprintf(details, u8"  用户楼层：");
	text_surface = TTF_RenderUTF8_Blended(font, details, black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y += text.h;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	if (elevator_b.current_usercall != NULL)
		sprintf(details, u8"  目标楼层：%d", elevator_b.current_usercall->user_target);
	else
		sprintf(details, u8"  目标楼层：");
	text_surface = TTF_RenderUTF8_Blended(font, details, black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y += text.h;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	if (elevator_b.serve_state == 'E')
		sprintf(details, u8"  服务状态：服务中");
	else
		sprintf(details, u8"  服务状态：服务前");
	text_surface = TTF_RenderUTF8_Blended(font, details, black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y += text.h;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	if (elevator_b.current_usercall != NULL)
		if (elevator_b.current_usercall->call_type == 'U')
			sprintf(details, u8"  指令类型：上行");
		else if (elevator_b.current_usercall->call_type == 'D')
			sprintf(details, u8"  指令类型：下行");
		else
			sprintf(details, u8"  指令类型：静止");
	else
		sprintf(details, u8"  指令类型：");
	text_surface = TTF_RenderUTF8_Blended(font, details, black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y += text.h;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	text_surface = TTF_RenderUTF8_Blended(font, u8"仿真进度信息：", black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = building.x + building.w + (width - (building.x + building.w)) * 1 / 20;
	text.y = height / 2;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
	text_rect.x = building.x + building.w + (width - (building.x + building.w)) * 1 / 20;
	text_rect.y = height / 2 + text_surface->h;
	text_rect.w = (width - (building.x + building.w)) * 18 / 20;
	text_rect.h = height / 2 - (text_rect.y - height / 2) - text_surface->h;
	draw_rounded_box(text_rect, radius);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	text_surface = TTF_RenderUTF8_Blended(font, u8"Powered by SDL2. Code by 请叫我喵 Alynx.", black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + (text_rect.w - text_surface->w) / 2;
	text.y = text_rect.y + text_rect.h;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	sprintf(details, u8"当前时刻：%d", time - 1);
	text_surface = TTF_RenderUTF8_Blended(font, details, black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y = text_rect.y;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	text_surface = TTF_RenderUTF8_Blended(font, u8"待响应用户指令：", black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y += text.h;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	if (finish_call_num < usercall_num)
		sprintf(details, u8"  指令序号：%d", finish_call_num);
	else
		sprintf(details, u8"  指令序号：");
	text_surface = TTF_RenderUTF8_Blended(font, details, black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y += text.h;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	if (finish_call_num < usercall_num)
		sprintf(details, u8"  用户楼层：%d", usercall_list[finish_call_num].user_floor);
	else
		sprintf(details, u8"  用户楼层：");
	text_surface = TTF_RenderUTF8_Blended(font, details, black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y += text.h;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	if (finish_call_num < usercall_num)
		sprintf(details, u8"  目标楼层：%d", usercall_list[finish_call_num].user_target);
	else
		sprintf(details, u8"  目标楼层：");
	text_surface = TTF_RenderUTF8_Blended(font, details, black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y += text.h;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	if (finish_call_num < usercall_num)
		if (usercall_list[finish_call_num].call_type == 'U')
			sprintf(details, u8"  指令类型：上行");
		else if (usercall_list[finish_call_num].call_type == 'D')
			sprintf(details, u8"  指令类型：下行");
		else
			sprintf(details, u8"  指令类型：静止");
	else
		sprintf(details, u8"  指令类型：");
	text_surface = TTF_RenderUTF8_Blended(font, details, black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y += text.h;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	if (finish_call_num < usercall_num)
		sprintf(details, u8"  呼叫时间：%d", usercall_list[finish_call_num].call_time);
	else
		sprintf(details, u8"  呼叫时间：");
	text_surface = TTF_RenderUTF8_Blended(font, details, black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = text_rect.x + radius;
	text.y += text.h;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	SDL_Rect floor;
	const int split = height / floors / 20;
	floor.w = building.w;
	floor.h = (building.h - (floors - 1) * split) / floors;
	floor.x = building.x;
	for (int i = 0; i < floors; i++) {
		floor.y = building.y + split + (floor.h + split) * i;
		SDL_RenderFillRect(renderer, &floor);
		char floor_num[3];
		sprintf(floor_num, u8"%d", floors - i);
		text_surface = TTF_RenderUTF8_Blended(font, floor_num, black);
		text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		text.x = (floor.x - text_surface->w) / 2;
		text.y = floor.y + (floor.h - text_surface->h) / 2;
		text.w = text_surface->w;
		text.h = text_surface->h;
		SDL_RenderCopy(renderer, text_texture, NULL, &text);
		SDL_FreeSurface(text_surface);
		SDL_DestroyTexture(text_texture);
	}
	elevator.w = floor.w * 3 / 10;
	elevator.h = floor.h * 8 / 10;
	elevator.y = building.y + building.h - (building.h / floors) * (elevator_a.current_floor - 1) - (floor.h - elevator.h) / 2 - elevator.h;
	elevator.x = floor.x + floor.w / 10;
	SDL_SetRenderDrawColor(renderer, gray.r, gray.g, gray.b, gray.a);
	SDL_RenderFillRect(renderer, &elevator);
	SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
	SDL_RenderDrawRect(renderer, &elevator);
	text_surface = TTF_RenderUTF8_Blended(font, u8"电梯A", black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = elevator.x + (elevator.w - text_surface->w) / 2;
	text.y = elevator.y + (elevator.h - text_surface->h) / 2;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	elevator.x = floor.x + floor.w / 10 + elevator.w + floor.w * 2 / 10;
	elevator.y = building.y + building.h - (building.h / floors) * (elevator_b.current_floor - 1) - (floor.h - elevator.h) / 2 - elevator.h;
	SDL_SetRenderDrawColor(renderer, gray.r, gray.g, gray.b, gray.a);
	SDL_RenderFillRect(renderer, &elevator);
	SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
	SDL_RenderDrawRect(renderer, &elevator);
	text_surface = TTF_RenderUTF8_Blended(font, u8"电梯B", black);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text.x = elevator.x + (elevator.w - text_surface->w) / 2;
	text.y = elevator.y + (elevator.h - text_surface->h) / 2;
	text.w = text_surface->w;
	text.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void quit_app(void)
{
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

Uint32 update_time(Uint32 interval, void *param)
{
	ELEVATORSTATE *elevators[2] = {&elevator_a, &elevator_b};
	for (int i = 0; i < 2; i++) {
		if (elevators[i]->current_usercall == NULL && usercall_list[finish_call_num].call_time <= time && finish_call_num < usercall_num) {
			elevators[i]->current_usercall = &usercall_list[finish_call_num];
			finish_call_num++;
		}
		if (elevators[i]->current_usercall != NULL) {
			switch (elevators[i]->run_state) {
			case 'U':
				elevators[i]->current_floor++;
				break;
			case 'D':
				elevators[i]->current_floor--;
				break;
			case 'S':
				// Check whether the user/target floor is the same as current_floor.
				if (elevators[i]->serve_state == 'P') {
					if (elevators[i]->current_floor == elevators[i]->current_usercall->user_floor) {
						elevators[i]->serve_state = 'E';
					}
				}
				if (elevators[i]->serve_state == 'E') {
					if (elevators[i]->current_floor == elevators[i]->current_usercall->user_target) {
						elevators[i]->serve_state = 'P';
						elevators[i]->current_usercall = NULL;
					}
				}
			}
		}
		if (elevators[i]->current_usercall != NULL) {
			char new_run_state = 'S';
			switch (elevators[i]->serve_state) {
			case 'P':
				if (elevators[i]->current_floor < elevators[i]->current_usercall->user_floor) {
					new_run_state = 'U';
				} else if (elevators[i]->current_floor > elevators[i]->current_usercall->user_floor) {
					new_run_state = 'D';
				} else {
					new_run_state = 'S';
					elevators[i]->serve_state = 'E';
				}
				break;
			case 'E':
				if (elevators[i]->current_floor < elevators[i]->current_usercall->user_target) {
					new_run_state = 'U';
				} else if (elevators[i]->current_floor > elevators[i]->current_usercall->user_target) {
					new_run_state = 'D';
				} else {
					new_run_state = 'S';
					elevators[i]->serve_state = 'P';
					elevators[i]->current_usercall = NULL;
				}
				break;
			}
			if (elevators[i]->run_state != new_run_state) {
				elevators[i]->run_state = new_run_state;
			}
		}
	}
	SDL_Event event;
	event.type = SDL_USEREVENT;
	event.user.code = 0;
	event.user.data1 = NULL;
	event.user.data2 = NULL;
	SDL_PushEvent(&event);
	time++;
	return interval;
}

void animate_route(void)
{
	for (int i = 0; i < sysparam_num; i++) {
		if (strcmp(sysparam_list[i].param_name,  "ElevatorHeight") == 0) {
			sscanf(sysparam_list[i].param_value, "%d", &floors);
		} else if (strcmp(sysparam_list[i].param_name,  "SimulateSpeed") == 0) {
			sscanf(sysparam_list[i].param_value, "%d", &delay);
		}
	}
	ELEVATORSTATE *elevators[2] = {&elevator_a, &elevator_b};
	for (int i = 0; i < 2; i++) {
		InitElevator(elevators[i]);
	}
	int quit = 0;
	int wait = 0;
	if (!init_app()) {
		quit_app();
		exit(EXIT_FAILURE);
	}
	building.x = width / 8;
	building.y = height / 16;
	building.w = width * 2 / 8;
	building.h = height * 15 / 16;
	SDL_Event event;
	SDL_TimerID timer = SDL_AddTimer(delay, update_time, NULL);
	render_frame();
	while (!quit && (finish_call_num < usercall_num || elevator_a.current_usercall != NULL || elevator_b.current_usercall != NULL) && SDL_WaitEvent(&event)) {
		switch (event.type) {
		case SDL_USEREVENT:
			if (!wait)
				render_frame();
			break;
		case SDL_WINDOWEVENT:
			switch (event.window.event) {
			case SDL_WINDOWEVENT_MINIMIZED:
				wait = 1;
				break;
			case SDL_WINDOWEVENT_RESTORED:
				wait = 0;
				render_frame();
				break;
			case SDL_WINDOWEVENT_CLOSE:
				quit = 1;
				break;
			default:
				break;
			}
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
			case SDLK_q:
				quit = 1;
				break;
			default:
				break;
			}
			break;
		case SDL_QUIT:
			quit = 1;
			break;
		default:
			break;
		}
	}
	SDL_RemoveTimer(timer);
	if (!quit)
		SDL_Delay(delay * 3);
	quit_app();
}

void AnimateSimulation(const char filename[])
{
	InitSimulation();
	for (int i = 0; i < sysparam_num; i++) {
		if (strcmp(sysparam_list[i].param_name,  "UserRequestFilePath") == 0) {
			if (strcmp(sysparam_list[i].param_value, "*")) {
				char fullpath[256];
				strncpy(fullpath, sysparam_list[i].param_value, sizeof(fullpath));
				strncat(fullpath, OS_SEP, sizeof(fullpath) - strlen(fullpath));
				strncat(fullpath, filename, sizeof(fullpath) - strlen(fullpath));
				usercall_num = parseUserCall(fullpath, &usercall_list);
			} else {
				usercall_num = parseUserCall(filename, &usercall_list);
			}
		}
	}
	if (usercall_num <= 0) {
		fprintf(stderr, "Parse error: %d.\n", usercall_num);
		return;
	}
	animate_route();
}
