#define is_down(b) input -> button[b].is_down
#define pressed(b) (input -> button[b].is_down && input->button[b].changed)
#define released(b) (!input -> button[b].is_down && input->button[b].changed)


float arena_half_size_x = 85, arena_half_size_y = 85;
float x_buffer = 1;
float y_min_height = 2;
float y_max_height = 80; // should not exceed arena half size
float y_scale = y_max_height - y_min_height;


float nums[10] = {10, 25, 100, 45, 50, 30, 15, 75, 90, 2};


enum Gamemode {
	GM_MENU,
	GM_GAMEPLAY,
};

Gamemode current_gamemode;
int hot_button;
bool enemy_is_ai;

internal void
simulate_game(Input* input, float dt) {

	// Clear screen is "overdraw" since we just draw the arena over it
	// clear_screen(0xff5500);
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xffaa33); // Arena
	draw_arena_border(arena_half_size_x, arena_half_size_y, 0xff5500);

	float n = sizeof(nums) / sizeof(int);
	float min = nums[0];
	float max = nums[0];;

	// Find Min/Max
	// Yes, there is probably an easier way to do this
	// Yes, doing this basically defeats the purpose of sorting again later
	// No, I don't care. This is an exercise
	for (int i = 1; i < n; i++) {
		if (nums[i] > max) {
			max = nums[i];
		} else {
			min = nums[i];
		}
	}
	
	
	float rect_width = ((arena_half_size_x - x_buffer) / n)/2.f;
	float edge_x = (-arena_half_size_x) + x_buffer;
	float edge_y = (-arena_half_size_y);
	float cur_x = 0;
	float cur_y = 0;
	
	
	for (int i = 0; i < n; i++) {
		cur_x = edge_x + (2.f+i*4.f)*rect_width;
		cur_y = y_min_height + ((nums[i] - min) / (max - min)) * y_scale;
		draw_rect(cur_x, edge_y+cur_y, rect_width*2.f, cur_y,0x000000);
	}

	
} 