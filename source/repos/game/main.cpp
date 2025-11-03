#include <windows.h>
#include <stdio.h>
#include <iostream>
#include "renderer.h"
#include "command.h"
#include "music.h"
#include "game.cpp"


extern bool running = true;

// Close, max. mini window
LRESULT CALLBACK windowCallBack(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;

	switch (uMsg) {
		case WM_CLOSE: 
		case WM_DESTROY: {
			running = false;
		} break;

		case WM_SIZE: {
			RECT rect;
			GetClientRect(hwnd, &rect);
			render_state.width = rect.right - rect.left;
			render_state.height = rect.bottom - rect.top;

			int size = render_state.width * render_state.height * sizeof(unsigned int);

			if (render_state.mem) VirtualFree(render_state.mem, 0, MEM_RELEASE);
			render_state.mem = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
			render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
			render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
			render_state.bitmap_info.bmiHeader.biPlanes = 1;
			render_state.bitmap_info.bmiHeader.biBitCount = 32;
			render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;

		} break;

		default: {
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	// Create window class
	WNDCLASS wc = {};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = TEXT("Sample Window Class");
	wc.lpfnWndProc = windowCallBack;

	// Register class
	RegisterClass(&wc);

	// Create window
	HWND window = CreateWindow(wc.lpszClassName, TEXT("Ping Pong"), WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, NULL, NULL, hInstance, NULL);

	HDC hdc = GetDC(window);

	Input input = {};
	 
	// Calibration frame rate affects speed
	float delta_time = 0.016f;
	LARGE_INTEGER begin_time;
	QueryPerformanceCounter(&begin_time);

	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = perf.QuadPart;
	}

	play_music();

	// Open the window 
	while (running) {
		MSG msg;

		// Reset the frame
		for (int i = 0; i < BTN_COUNT; i++) {
			input.buttons[i].changed = false;
		}

		while (PeekMessage(&msg, window, 0, 0, PM_REMOVE)) {
			switch (msg.message) {
				case WM_KEYUP:
				case WM_KEYDOWN: {
					unsigned int vkCode = (unsigned int)msg.wParam;
					bool isDown = ((msg.lParam & (1 << 31)) == 0);

					switch (vkCode) {
						case VK_UP: {
							input.buttons[BTN_UP].isDown = isDown;
							input.buttons[BTN_UP].changed = true;
						} break;
						case VK_DOWN: {
							input.buttons[BTN_DOWN].isDown = isDown;
							input.buttons[BTN_DOWN].changed = true;
						} break;
						case 'W': {
							input.buttons[BTN_W].isDown = isDown;
							input.buttons[BTN_W].changed = true;
						} break;
						case 'S': {
							input.buttons[BTN_S].isDown = isDown;
							input.buttons[BTN_S].changed = true;
						} break;
						case VK_LEFT: {
							input.buttons[BTN_LEFT].isDown = isDown;
							input.buttons[BTN_LEFT].changed = true;
						} break;
						case VK_RIGHT: {
							input.buttons[BTN_RIGHT].isDown = isDown;
							input.buttons[BTN_RIGHT].changed = true;
						} break;
						case VK_RETURN: {
							input.buttons[BTN_ENTER].isDown = isDown;
							input.buttons[BTN_ENTER].changed = true;
						} break;
					}
				}
				default: {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				} break;
			}
		}
		// Simulate 
		simualte_game(&input, delta_time);

		// Render
		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.mem, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		LARGE_INTEGER end_time;
		QueryPerformanceCounter(&end_time);
		delta_time = (float)(end_time.QuadPart - begin_time.QuadPart) / performance_frequency;
		begin_time = end_time;
	}
}