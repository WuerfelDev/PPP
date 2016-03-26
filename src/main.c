#include <pebble.h>

#define KEY_BUTTON    0
#define HBTNLBL       40

#define BUTTON_UP     0
#define BUTTON_SELECT 1
#define BUTTON_DOWN   2

static Window *s_main_window;
static TextLayer *s_text_layer_top;
static TextLayer *s_text_layer_middle;
static TextLayer *s_text_layer_bottom;


/******************************* AppMessage ***********************************/

static void send(int message) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  dict_write_int(iter, KEY_BUTTON, &message, sizeof(int), true);

  app_message_outbox_send();
}



static void outbox_failed_handler(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_handler(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

/********************************* Buttons ************************************/

/*static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  send(BUTTON_SELECT);
}*/

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  send(BUTTON_UP);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  send(BUTTON_DOWN);
}

static void click_config_provider(void *context) {
  // Assign button handlers
  //window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}


static void handle_tick(struct tm *currentTime, TimeUnits units_changed) {
  static char timeText[] = "00:00:00"; //00:00:00
  strftime(timeText, sizeof(timeText), clock_is_24h_style() ? "%X" : "%I:%M:%S", currentTime); //%H:%M:%S == %X
  text_layer_set_text(s_text_layer_middle, timeText);
}

/******************************* main_window **********************************/

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect boundst = GRect(0, 0, layer_get_bounds(window_layer).size.w, HBTNLBL);
  GRect boundsm = GRect(0, layer_get_bounds(window_layer).size.h/2-HBTNLBL/2, layer_get_bounds(window_layer).size.w, HBTNLBL);
  GRect boundsb = GRect(0, layer_get_bounds(window_layer).size.h-HBTNLBL, layer_get_bounds(window_layer).size.w, HBTNLBL);


  // Create main TextLayer_top
  s_text_layer_top = text_layer_create(boundst);
  text_layer_set_font(s_text_layer_top, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text(s_text_layer_top, "NEXT ->");
  text_layer_set_text_alignment(s_text_layer_top, PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentRight));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_text_layer_top));
  // Create main TextLayer_middle
  s_text_layer_middle = text_layer_create(boundsm);
  text_layer_set_font(s_text_layer_middle, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
  text_layer_set_text(s_text_layer_middle, "00:00:00");
  text_layer_set_text_alignment(s_text_layer_middle, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_text_layer_middle));
  // Create main TextLayer_bottom
  s_text_layer_bottom = text_layer_create(boundsb);
  text_layer_set_font(s_text_layer_bottom, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text(s_text_layer_bottom, "BACK ->");
  text_layer_set_text_alignment(s_text_layer_bottom, PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentRight));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_text_layer_bottom));
  
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);
  handle_tick(tick_time, SECOND_UNIT);
  tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_text_layer_top);
  text_layer_destroy(s_text_layer_bottom);
}

static void init(void) {
  app_message_register_outbox_failed(outbox_failed_handler);
  app_message_register_outbox_sent(outbox_sent_handler);

  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  // Create main Window
  s_main_window = window_create();
  window_set_click_config_provider(s_main_window, click_config_provider);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void deinit(void) {
  // Destroy main Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
