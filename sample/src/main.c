/*
  AUTHOR: Siddarth Udayakumar
  DATE: 8/18/2016
  DESCRIPTION:
  A simple sample Pebble watchface I created
  using the directions on the Pebble Dev page.
  The code is directly followed from the 
  instructions on the page. 
  
  Version : 1.0
  Changelog: Initial Test
*/

#include <pebble.h> 

static Window *s_main_window; //static pointer to Window variable. 
static TextLayer *s_time_layer; //Used to show text on the watchface canvas

/*
  window_load and window_unload subsequently manage the 
  creation and manipulation of the elements in the 
  watchface. Also note that both the functions should
  be declared before the init function.
*/

//Below function contains logic to update time on text
static void update_time() {
  //Get the struct from the tickhandler
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  //Writing current hour and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
  
  //Display below time on the textlayer
  text_layer_set_text(s_time_layer, s_buffer);
}

static void main_window_load(Window *window) {
  //get information about the window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  //Create textlayer with specific boundaries
  s_time_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  
  //Improve the layout of the watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  //text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  //Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
  //Destroying the text layer here 
  text_layer_destroy(s_time_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  //Calling update_time function
  update_time();
}

//function to create Pebble Environment
static void init() {
  //Creating a window and assigning it to the pointer
  s_main_window = window_create();
  
  //Handlers to manage elements in the window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load, .unload = main_window_unload
  });
  
  //Show window on the watch with animated = true
  window_stack_push(s_main_window, true);
  
  //This call is to enure time is displayed from the start
  update_time();
  
  //Register with TickTimeServer
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

//function to destroy Pebble Environment
static void deinit() {
  //Destroy the window
  window_destroy(s_main_window);
  /*
    Good practice to assign a destroy 
    function to the create function.
  */
}

//Standard main function
int main(void) {
  init();
  app_event_loop();
  deinit();
}
