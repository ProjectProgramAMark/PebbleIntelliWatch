#include <pebble.h>  
  
Window* detailW;
TextLayer* detailW_text;
MenuLayer* detail_menu;


static void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{
  switch(cell_index->row)
  {
    case 0:
      menu_cell_title_draw(ctx, (Layer*)cell_layer, "Disable");
      break;
    case 1:
      menu_cell_title_draw(ctx, (Layer*)cell_layer, "Edit");
      break;
    case 2:
      menu_cell_title_draw(ctx, (Layer*)cell_layer, "Delete");
      break;
  }
}
 
static uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
  return 3;
}
 
static void select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
   // Get which row was selected
   int rowIndex = cell_index->row;
}

void detailUnload(Window* win)
{
	APP_LOG(APP_LOG_LEVEL_INFO, "Destroying Detail Window.");
	text_layer_destroy(detailW_text);
	window_destroy(detailW);
}

void showDetail(MenuIndex* index)
{
	    detailW = window_create();
      // FIX-ME: Add window handlers
      /*WindowHandlers wh = { .unload = &detailUnload };
	    window_set_window_handlers(detailW, wh);*/

    
      //Create it - 12 is approx height of the top bar
      detail_menu = menu_layer_create(GRect(0, 0, 144, 168 - 16));
   
      //Let it receive clicks
      menu_layer_set_click_config_onto_window(detail_menu, detailW);
   
      //Give it its callbacks
      MenuLayerCallbacks callbacks = {
          .draw_row = (MenuLayerDrawRowCallback) draw_row_callback,
          .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) num_rows_callback,
          .select_click = (MenuLayerSelectCallback) select_click_callback
      };
      menu_layer_set_callbacks(detail_menu, NULL, callbacks);
   
      //Add to Window
      layer_add_child(window_get_root_layer(detailW), menu_layer_get_layer(detail_menu));
    
    	/*detailW_text = text_layer_create(GRect(0,52,144,40));
    	text_layer_set_text_alignment(detailW_text, GTextAlignmentCenter); // Center the text.
	    text_layer_set_font(detailW_text, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));

    	text_layer_set_text(detailW_text, "Testing");
    	layer_add_child(window_get_root_layer(detailW), text_layer_get_layer(detailW_text));*/

	    window_stack_push(detailW, true); // The back button will dismiss the current window, not close the app.  So just press back to go back to the master view.
}