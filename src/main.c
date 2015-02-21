#include <pebble.h>

//Defining some shit
static Window *window;
static Window *menu_window;

static SimpleMenuLayer *menu_settings;
//static SimpleMenuItem menu_items[1];
static SimpleMenuSection menu_sections[1];

//Array for menu items
char *item_names[1] = { "00:00"};

//Some methods over here and shit
void handle_init(void) {
	// Create a window and text layer
	window = window_create();
	//text_layer = text_layer_create(GRect(0, 0, 144, 154));
	
  // Set the text, font, and text alignment
	//text_layer_set_text(text_layer, "I am an alarm clock! What would you like to do?");
	//text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	//text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	  
  
	// Add the text layer to the window
  Layer *layer = window_get_root_layer(menu_window);
  
	layer_add_child(layer, simple_menu_layer_get_layer(menu_settings));

  menu_settings = simple_menu_layer_create(layer_get_bounds(layer),
			            menu_window, menu_sections, 1, NULL);
  
  simple_menu_layer_set_selected_index(menu_settings, 0, true);
  layer_add_child(window_get_root_layer(window), simple_menu_layer_get_layer(menu_settings));
  
	// Push the window
	window_stack_push(window, true);
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}

void handle_deinit(void) {
	// Destroy the text layer
	//text_layer_destroy(text_layer);
  
  layer_destroy(window_get_root_layer(menu_window));
	simple_menu_layer_destroy(menu_settings);
	window_destroy(menu_window);
	
	// Destroy the window
	window_destroy(window);
}
int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
 