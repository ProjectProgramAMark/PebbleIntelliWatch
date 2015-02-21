#include <pebble.h>

//Defining some shit
static Window *window;
static Window *menu_window;
static TextLayer *text_layer;
//static SimpleMenuLayer *menu_items[1];
static SimpleMenuSection *menu_sections[1];
static SimpleMenuLayer *menu_settings;

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
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));

  menu_settings = simple_menu_layer_create(window_get_root_layer(window), menu_window, item_names, 1, NULL);
  simple_menu_layer_set_selected_index(menu_settings, 0, true);
  layer_add_child(window_get_root_layer(window), simple_menu_layer_get_layer(menu_settings));
  
	// Push the window
	window_stack_push(window, true);
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}

void handle_deinit(void) {
	// Destroy the text layer
	text_layer_destroy(text_layer);
	
	// Destroy the window
	window_destroy(window);
}
int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
