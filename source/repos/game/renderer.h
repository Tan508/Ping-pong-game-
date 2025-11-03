#pragma once
#include <windows.h>

struct RenderState {
	int width;
	int height;
	void* mem;
	BITMAPINFO bitmap_info;
};

extern RenderState render_state;

void clear_screen(unsigned int color);
void draw_rec_in_pixels(int x0, int y0, int x1, int y1, unsigned int color);
void draw_rec(float x0, float y0, float x1, float y1, unsigned int color);
void draw_number(int score, float x, float y, float size, unsigned int color);
void draw_text(const char* text, float x, float y, float size, unsigned int color);