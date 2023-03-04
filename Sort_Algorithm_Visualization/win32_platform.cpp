#include <windows.h>
#include "utilities.cpp"


// winmain mdsn


struct Render_State {
	int height, width;
	void* memory;
	
	BITMAPINFO bitmap_info;
};

global_variable Render_State render_state;
global_variable bool running = true;

#include "platform_common.cpp"
#include "renderer.cpp"
#include "game.cpp"
// Exlcude file from compilation since we included it here

// Windows sending stuff back to us
LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
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

			int size = render_state.width * render_state.height * sizeof(u32);

			// So if addres is already occupied, free it before assigning
			if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
			render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);


			render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
			render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
			render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
			render_state.bitmap_info.bmiHeader.biPlanes = 1; // That's just what this one is
			render_state.bitmap_info.bmiHeader.biBitCount = 32; // from 32 u32eger for buffer size
			render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;

		}
		default: {
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	
	
	return result;
}


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	
	//Hide cursor
	//ShowCursor(FALSE);
	
	// Create Window Class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = L"Game Window Class"; // Tutorial did not include L. Something to do with the underlying type
	window_class.lpfnWndProc = window_callback;
	// Register Class so it can be used
	RegisterClass(&window_class);

	// Create Window
	//Documentation: void CreateWindowA(lpClassName,lpWindowName,dwStyle, x, y,nWidth, nHeight,hWndParent, hMenu, hInstance, lpParam);
	HWND window = CreateWindow(window_class.lpszClassName, L"Pong - Tutorial", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CWMO_DEFAULT, CWMO_DEFAULT, 1280, 720, 0, 0, hInstance, 0);
	
	/*
	{
		// Fullscreen, if desired
		SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &mi);
		SetWindowPos(window, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
	*/
	
	
	HDC hdc = GetDC(window);

	Input input = {};

	float delta_time = 0.01666f; //Initial Guess
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);

	// Cycles per second
	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}

	// Game Loop
	while (running) {
		// Input
		MSG message;

		for (int i = 0; i < BUTTON_COUNT; i++) {
			input.button[i].changed = false;
		}

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
		
			switch (message.message) {
				case WM_KEYUP:
				case WM_KEYDOWN: {
					u32 vk_code = (u32)message.wParam;
					bool is_down = ((message.lParam & (1 << 31)) == 0);

#define process_button(b, vk)\
case vk: {\
input.button[b].changed = is_down != input.button[b].is_down;\
input.button[b].is_down = is_down;\
} break;

					switch (vk_code) {
						process_button(BUTTON_UP, VK_UP);
						process_button(BUTTON_DOWN, VK_DOWN);
						process_button(BUTTON_W, 'W');
						process_button(BUTTON_S, 'S');
						process_button(BUTTON_LEFT, VK_LEFT);
						process_button(BUTTON_RIGHT, VK_RIGHT);
						process_button(BUTTON_ENTER, VK_RETURN);
					}
				} break;
				default: {
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}
		}



		// Simulate
		simulate_game(&input,delta_time);

		// Render
		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);


		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);
		// Cycles per frame*seconds per cycle
		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
		frame_begin_time = frame_end_time;

	}

}


