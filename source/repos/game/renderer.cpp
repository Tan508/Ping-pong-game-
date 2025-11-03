#include "renderer.h"

RenderState render_state;

// clear the screen with specified color
void clear_screen(unsigned int color) {
	unsigned int* pixel = (unsigned int*)render_state.mem;
	for (int y = 0; y < render_state.height; y++) {
		for (int x = 0; x < render_state.width; x++) {
			*pixel++ = color;
		}
	}
}

inline int
clamp(int value, int min, int max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

// draw a rectangle on a specific area 
// x0: rectangle startig x
// x1: rectangle ending x
// y0: rectangle startig y
// y1: rectangle ending y
void draw_rec_in_pixels(int x0, int y0, int x1, int y1, unsigned int color) {
	
	x0 = clamp(x0, 0, render_state.width);
	x1 = clamp(x1, 0, render_state.width);
	y0 = clamp(y0, 0, render_state.height);
	y1 = clamp(y1, 0, render_state.height);

	for (int y = y0; y < y1; y++) {
		unsigned int* pixel = (unsigned int*)render_state.mem + x0 + y * render_state.width;
		for (int x = x0; x < x1; x++) {
			*pixel++ = color;
		}
	}
}

void draw_rec(float x, float y, float half_x, float half_y, unsigned int color) {
	x *= render_state.height;
	y *= render_state.height;
	half_x *= render_state.height;
	half_y *= render_state.height;

	// Let (0,0) central
	x += render_state.width / 2;
	y += render_state.height / 2;

	// Change to pixels
	int x0 = (int)(x - half_x);
	int y0 = (int)(y - half_y);
	int x1 = (int)(x + half_x);
	int y1 = (int)(y + half_y);

	draw_rec_in_pixels(x0, y0, x1, y1, color);
}
const char* letters[][7] = {
	" 00",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"000",

	" 000",
	"0",
	"0",
	"0",
	"0",
	"0",
	" 000",

	"000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0",

	" 000",
	"0",
	"0",
	"0 00",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	"000",

	" 000",
	"   0",
	"   0",
	"   0",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0 0",
	"00",
	"0 0",
	"0  0",
	"0  0",

	"0",
	"0",
	"0",
	"0",
	"0",
	"0",
	"0000",

	"00 00",
	"0 0 0",
	"0 0 0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",

	"00  0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0  00",

	"0000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0000",

	" 000",
	"0  0",
	"0  0",
	"000",
	"0",
	"0",
	"0",

	" 000 ",
	"0   0",
	"0   0",
	"0   0",
	"0 0 0",
	"0  0 ",
	" 00 0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"0  0",

	" 000",
	"0",
	"0 ",
	" 00",
	"   0",
	"   0",
	"000 ",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",

	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	" 00",

	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	" 0 0",
	"  0",

	"0   0 ",
	"0   0",
	"0   0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	" 0 0 ",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	" 0 0",
	"0   0",
	"0   0",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	"  0",
	"  0",
	"  0",

	"0000",
	"   0",
	"  0",
	" 0",
	"0",
	"0",
	"0000",

	"",
	"",
	"",
	"",
	"",
	"",
	"0",

	"   0",
	"  0",
	"  0",
	" 0",
	" 0",
	"0",
	"0",
};

void draw_text(const char* text, float x, float y, float size, unsigned int color) {
	float half_size = size * .5f;
	float original_y = y;

	while (*text) {
		if (*text != 32) {
			const char** letter;
			if (*text == 47) letter = letters[27];
			else if (*text == 46) letter = letters[26];
			else letter = letters[*text - 'A'];
			float original_x = x;

			for (int i = 0; i < 7; i++) {
				const char* row = letter[i];
				while (*row) {
					if (*row == '0') {
						draw_rec(x, y, half_size, half_size, color);
					}
					x += size;
					row++;
				}
				y -= size;
				x = original_x;
			}
		}
		text++;
		x += size * 6.f;
		y = original_y;
	}
}

void draw_number(int score, float x, float y, float size, unsigned int color) {
	bool drew = false;
	while (score || !drew) {
		drew = true; 
		int digit = score % 10;
		score = score / 10;

		switch (digit) {
		case 0: {
			draw_rec(x - size, y, size * 0.5f, 2.5f * size, color);
			draw_rec(x + size, y, size * 0.5f, 2.5f * size, color);
			draw_rec(x, y - size * 2.f, size * 0.5f, 0.5f * size, color);
			draw_rec(x, y + size * 2.f, size * 0.5f, 0.5f * size, color);
			x -= size * 4.f;
		} break;
		case 1: {
			draw_rec(x + size, y, size * 0.5f, 2.5f * size, color);
			x -= size * 2.f;
		} break;
		case 2: {
			draw_rec(x, y + size * 2.f, size * 1.5f, 0.5f * size, color);
			draw_rec(x, y, size * 1.5f, 0.5f * size, color);
			draw_rec(x, y - size * 2.f, size * 1.5f, 0.5f * size, color);
			draw_rec(x + size, y + size, size * 0.5f, 0.5f * size, color);
			draw_rec(x - size, y - size, size * 0.5f, 0.5f * size, color);
			x -= size * 4.f;
		} break;

		case 3: {
			draw_rec(x, y + size * 2.f, size * 1.5f, 0.5f * size, color);
			draw_rec(x, y, size * 1.5f, 0.5f * size, color);
			draw_rec(x, y - size * 2.f, size * 1.5f, 0.5f * size, color);
			draw_rec(x + size, y + size, size * 0.5f, 0.5f * size, color);
			draw_rec(x + size, y - size, size * 0.5f, 0.5f * size, color);
			x -= size * 4.f;
		} break;

		case 4: {
			draw_rec(x + size, y, size * 0.5f, 2.5f * size, color);
			draw_rec(x - size, y + size, size * 0.5f, 1.5f * size, color);
			draw_rec(x, y, size * 0.5f, 0.5f * size, color);
			x -= size * 4.f;
		} break;

		case 5: {
			draw_rec(x, y + size * 2.f, size * 1.5f, 0.5f * size, color);
			draw_rec(x, y, size * 1.5f, 0.5f * size, color);
			draw_rec(x, y - size * 2.f, size * 1.5f, 0.5f * size, color);
			draw_rec(x - size, y + size, size * 0.5f, 0.5f * size, color);
			draw_rec(x + size, y - size, size * 0.5f, 0.5f * size, color);
			x -= size * 4.f;
		} break;

		case 6: {
			draw_rec(x, y + size * 2.f, size * 1.5f, 0.5f * size, color);
			draw_rec(x, y, size * 1.5f, 0.5f * size, color);
			draw_rec(x, y - size * 2.f, size * 1.5f, 0.5f * size, color);
			draw_rec(x - size, y + size, size * 0.5f, 0.5f * size, color);
			draw_rec(x - size, y - size, size * 0.5f, 0.5f * size, color);
			draw_rec(x + size, y - size, size * 0.5f, 0.5f * size, color);
			x -= size * 4.f;
		} break;

		case 7: {
			draw_rec(x, y + size * 2.f, size * 1.5f, 0.5f * size, color);
			draw_rec(x + size, y + size, size * 0.5f, 1.5f * size, color);
			draw_rec(x + size, y - size, size * 0.5f, 1.5f * size, color);
			x -= size * 4.f;
		} break;

		case 8: {
			draw_rec(x - size, y, size * 0.5f, 2.5f * size, color);
			draw_rec(x + size, y, size * 0.5f, 2.5f * size, color);
			draw_rec(x, y - size * 2.f, size * 0.5f, 0.5f * size, color);
			draw_rec(x, y + size * 2.f, size * 0.5f, 0.5f * size, color);
			draw_rec(x, y, size * 0.5f, 0.5f * size, color);
			x -= size * 4.f;
		} break;

		case 9: {
			draw_rec(x, y + size * 2.f, size * 1.5f, 0.5f * size, color);
			draw_rec(x, y, size * 1.5f, 0.5f * size, color);
			draw_rec(x, y - size * 2.f, size * 1.5f, 0.5f * size, color);
			draw_rec(x - size, y + size, size * 0.5f, 0.5f * size, color);
			draw_rec(x + size, y - size, size * 0.5f, 1.5f * size, color);
			draw_rec(x + size, y + size, size * 0.5f, 1.5f * size, color);
			x -= size * 4.f;
		} break;
		}
	}
}