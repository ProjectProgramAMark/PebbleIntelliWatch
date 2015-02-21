#include <pebble.h>
 
Window* window;
MenuLayer *menu_layer;

// Detail window
Window* detailW;
TextLayer* detailW_text;


void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{
   switch(cell_index->row)
    {
    case 0:
        menu_cell_basic_draw(ctx, cell_layer, "00:00", "Off", NULL);
        break;
    default:
        // Type some shit in here
        menu_cell_title_draw(ctx, cell_layer, "New Alarm");
        break;
    }
}
 
uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
   return 2;
}
 
void select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
   // Get which row was selected
   int rowIndex = cell_index->row;
  
  switch(cell_index->row)
  {
    case 0:
      detailW = window_create();
      // FIX-ME: Add window handlers
    
    	detailW_text = text_layer_create(GRect(0,52,144,40));
    	text_layer_set_text_alignment(detailW_text, GTextAlignmentCenter); // Center the text.
	    text_layer_set_font(detailW_text, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));

    	text_layer_set_text(detailW_text, "Testing");
    	layer_add_child(window_get_root_layer(detailW), text_layer_get_layer(detailW_text));

	    window_stack_push(detailW, true); // The back button will dismiss the current window, not close the app.  So just press back to go back to the master view.
      break;
  }
}
 
void window_load(Window *window)
{
    //Create it - 12 is approx height of the top bar
    menu_layer = menu_layer_create(GRect(0, 0, 144, 168 - 16));
 
    //Let it receive clicks
    menu_layer_set_click_config_onto_window(menu_layer, window);
 
    //Give it its callbacks
    MenuLayerCallbacks callbacks = {
        .draw_row = (MenuLayerDrawRowCallback) draw_row_callback,
        .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) num_rows_callback,
        .select_click = (MenuLayerSelectCallback) select_click_callback
    };
    menu_layer_set_callbacks(menu_layer, NULL, callbacks);
 
    //Add to Window
    layer_add_child(window_get_root_layer(window), menu_layer_get_layer(menu_layer));
}
 
void window_unload(Window *window)
{
    menu_layer_destroy(menu_layer);
}
 
void init()
{
    window = window_create();
    WindowHandlers handlers = {
        .load = window_load,
        .unload = window_unload
    };
    window_set_window_handlers(window, (WindowHandlers) handlers);
    window_stack_push(window, true);
}
 
void deinit()
{
    window_destroy(window);
}
 
int main(void)
{
    init();
    app_event_loop();
    deinit();
}