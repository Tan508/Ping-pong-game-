#pragma once

struct Button_State{
	bool isDown;
	bool changed;
};

enum {
	BTN_UP,
	BTN_DOWN,
	BTN_W,
	BTN_S,
	BTN_LEFT,
	BTN_RIGHT,
	BTN_ENTER,
	BTN_COUNT
};

struct Input {
	Button_State buttons[BTN_COUNT];
};
