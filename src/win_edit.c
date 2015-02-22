#include <pebble.h>
#include "win_edit.h"

static void am_pm_window_load(Window* window);
static void am_pm_window_unload(Window* window);

static void window_load(Window* window);
static void window_unload(Window* window);

static Window *s_window;
static Window *s_am_pm_window;
static ActionBarLayer *s_am_pm_actionbar;
static TextLayer *s_am_pm_textlayer;

static NumberWindow *hour_window,*minute_window;

static bool s_is_am;

void win_edit_init(void)
{
  /*s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });*/
  
  s_am_pm_window = window_create();
  window_set_window_handlers(s_am_pm_window, (WindowHandlers) {
    .load = am_pm_window_load,
    .unload = am_pm_window_unload
  });
  
  window_stack_push(s_am_pm_window, true);
}

static void up_down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (s_is_am) {
    text_layer_set_text(s_am_pm_textlayer, "PM");
    s_is_am = false;
  } else {
    text_layer_set_text(s_am_pm_textlayer, "AM");
    s_is_am = true;
  }
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  //Launch hour window yo
  //window_stack_push(number_window_get_window(hour_window),true);
}

void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, up_down_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_down_click_handler);
}

void window_load(Window *window)
{
  
}

void window_unload(Window *window)
{
  window_destroy(s_window);
}

void am_pm_window_load(Window* window)
{
  GRect bounds = layer_get_bounds(window_get_root_layer(window));

  s_am_pm_actionbar = action_bar_layer_create();
  action_bar_layer_set_click_config_provider(s_am_pm_actionbar, click_config_provider);
  action_bar_layer_add_to_window(s_am_pm_actionbar,window);
  
  s_am_pm_textlayer = text_layer_create(GRect(0, bounds.size.h/2-21, bounds.size.w-ACTION_BAR_WIDTH, bounds.size.h));
  text_layer_set_text_alignment(s_am_pm_textlayer, GTextAlignmentCenter);
  text_layer_set_font(s_am_pm_textlayer,fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text(s_am_pm_textlayer, "AM");
  layer_add_child(window_get_root_layer(s_am_pm_window), text_layer_get_layer(s_am_pm_textlayer));
  s_is_am = true;
}

void am_pm_window_unload(Window *window)
{
  action_bar_layer_destroy(s_am_pm_actionbar);
  text_layer_destroy(s_am_pm_textlayer);
  
}
