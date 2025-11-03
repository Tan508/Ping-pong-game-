#include "command.h"
#include "renderer.h"

#define is_down(b) input->buttons[b].isDown
#define pressed(b) input->buttons[b].isDown && input->buttons[b].changed
#define released(b) !input->buttons[b].isDown && input->buttons[b].changed

float player_pos_1, player_v_1, player_pos_2, player_v_2 = 0.f;
float areana_half_x = 0.8, areana_half_y = 0.485;
float player_half_size_x = 0.02, player_half_size_y = 0.13;
float ball_pos_x = 0, ball_pos_y = 0, ball_v_x = 1.35f, ball_v_y;
float ball_half_size = 0.005;
int player_1_score, player_2_score;

enum Gamemode {
	MENU,
	GAME
};

Gamemode current_mode;
int select_button;
bool is_AI;
 
// Player's bar movement 
void simulate_player(float* pos, float* v, float accelaration, float dt) {
	accelaration -= *v * 15.f;  // Friction

	*pos = *pos + *v * dt + 0.5f * accelaration * dt * dt;  // s = s0 + v0 * t + 1/2 * a * t^2
	*v += accelaration * dt;  // v = v0 + a * t
	if (*pos > areana_half_y - player_half_size_y) {  // Avoid out of bound
		*pos = areana_half_y - player_half_size_y;
		*v = 0.f;
	}
	else if (*pos < -areana_half_y + player_half_size_y) {
		*pos = -areana_half_y + player_half_size_y;
		*v = 0.f;
	}
}

// Ball collision with bar
bool collision_bar(float bposx, float bposy, float bhs, float player_pos_y, float player_pos_x) {
	return (bposx + bhs > player_pos_x - player_half_size_x &&  // Ball collision on plyer's bar
		bposx - bhs < player_pos_x + player_half_size_x &&
		bposy + bhs > player_pos_y - player_half_size_y &&
		bposy + bhs < player_pos_y + player_half_size_y);
}

void simualte_game(Input* input, float dt) {
	clear_screen(0xffffff);
	draw_rec(0, 0, areana_half_x, areana_half_y, 0x00008b);

	if (current_mode == GAME) {
		float accelaration_1 = 0.f;
		if (is_down(BTN_W)) accelaration_1 += 18.f;
		if (is_down(BTN_S)) accelaration_1 -= 18.f;

		float accelaration_2 = 0.f;
		if (!is_AI) {
			if (is_down(BTN_UP)) accelaration_2 += 18.f;
			if (is_down(BTN_DOWN)) accelaration_2 -= 18.f;
		}
		else {
			accelaration_2 = (ball_pos_y - player_pos_2) * 100;
			if (accelaration_2 > 17) accelaration_2 = 17;
			if (accelaration_2 < -17) accelaration_2 = -17;
		}

		simulate_player(&player_pos_1, &player_v_1, accelaration_1, dt);
		simulate_player(&player_pos_2, &player_v_2, accelaration_2, dt);

		ball_pos_x += ball_v_x * dt;
		ball_pos_y += ball_v_y * dt;

		// Ball controls
		{
			if (collision_bar(ball_pos_x, ball_pos_y, ball_half_size, player_pos_2, 0.76)) {
				ball_pos_x = 0.76 - player_half_size_x - ball_half_size;
				ball_v_x *= -1;
				ball_v_y = (ball_pos_y - player_pos_2) * 8 + player_v_1 * 0.75f;
			}
			else if (collision_bar(ball_pos_x, ball_pos_y, ball_half_size, player_pos_1, -0.76)) {
				ball_pos_x = -0.76 + player_half_size_x + ball_half_size;
				ball_v_x *= -1;
				ball_v_y = (ball_pos_y - player_pos_1) * 8 + player_v_2 * 0.75f;
			}

			if (ball_pos_y + ball_half_size > areana_half_y) {  // Ball colision on areana up down side
				ball_pos_y = areana_half_y - ball_half_size;
				ball_v_y *= -1;
			}
			else if (ball_pos_y - ball_half_size < -areana_half_y) {
				ball_pos_y = -areana_half_y + ball_half_size;
				ball_v_y *= -1;
			}

			if (ball_pos_x + ball_half_size > areana_half_x) {  // Players scored
				ball_v_x *= -1;
				ball_v_y = 0;
				ball_pos_x = 0;
				ball_pos_y = 0;
				player_1_score++;
			}
			else  if (ball_pos_x - ball_half_size < -areana_half_x) {
				ball_v_x *= -1;
				ball_v_y = 0;
				ball_pos_x = 0;
				ball_pos_y = 0;
				player_2_score++;
			}
		}

		draw_number(player_1_score, -0.2, 0.42, 0.01f, 0xfffdd0);
		draw_number(player_2_score, 0.2, 0.42, 0.01f, 0xfffdd0);


		draw_rec(ball_pos_x, ball_pos_y, 0.01, 0.01, 0xffa500);  // Ball

		draw_rec(-0.76, player_pos_1, player_half_size_x, player_half_size_y, 0xff0000);  // Player 1 bar
		draw_rec(0.76, player_pos_2, player_half_size_x, player_half_size_y, 0xff0000);  // Player 2 bar
	}
	else {
		draw_text("PING PONG", -0.5, 0.38, 0.02, 0xffa500);
		if (pressed(BTN_LEFT) || pressed(BTN_RIGHT)) {
			select_button = ~select_button;
		}

		if (pressed(BTN_ENTER)) {
			current_mode = GAME;
			is_AI = select_button ? 0 : 1;
		}

		if (select_button == 0) {
			draw_text("SINGLE", -0.5, 0, 0.01, 0x54534d);
			draw_text("DOUBLE", 0.2, 0, 0.01, 0xffffff);
		}
		else {
			draw_text("SINGLE", -0.5, 0, 0.01, 0xffffff);
			draw_text("DOUBLE", 0.2, 0, 0.01, 0x54534d);
		}
	}
}