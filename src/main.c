#include <pebble.h>
#include "time_container.h"
#include "climber.h"

Window *window;
BitmapLayer *background_layer, *bt_icon_layer;
GBitmap *background, *bt_icon, *bt_icon_invert;
Layer *battery_layer;

int battery_percent = 10;

void tick_handler(struct tm *t, TimeUnits units_changed){
	container_set_time(t);
	climber_refresh_position(true, t);
}

void bt_handler(bool connected){
	if(connected){
		bitmap_layer_set_bitmap(bt_icon_layer, bt_icon);
	}
	else{
		bitmap_layer_set_bitmap(bt_icon_layer, bt_icon_invert);
	}
}

void battery_handler(BatteryChargeState charge){
	battery_percent = charge.charge_percent;
	layer_mark_dirty(battery_layer);
}

void battery_proc(Layer *layer, GContext *ctx){
	graphics_context_set_stroke_color(ctx, GColorWhite);
	graphics_context_set_fill_color(ctx, GColorWhite);
	int height = 155;
	int circle_radius = 4;
	int k, l;
	for(k = 10; k > 0; k--){
		l = 15+(12*k);
		graphics_draw_circle(ctx, GPoint(l, height), circle_radius);
	}
		
	int i, j;
	for(i = battery_percent/10; i > 0; i--){
		j = 15+(i*12);
		graphics_fill_circle(ctx, GPoint(j, height), circle_radius);
	}
}

void window_load(Window *w){
	background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);

	background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
	bitmap_layer_set_bitmap(background_layer, background);
	layer_add_child(window_get_root_layer(w), bitmap_layer_get_layer(background_layer));
	
	battery_layer = layer_create(GRect(0, 0, 144, 168));
	layer_set_update_proc(battery_layer, battery_proc);
	layer_add_child(window_get_root_layer(w), battery_layer);
	
	bt_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT_ICON);
	bt_icon_invert = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT_BLACK);
	
	bt_icon_layer = bitmap_layer_create(GRect(2, 146, 18, 18));
	bitmap_layer_set_bitmap(bt_icon_layer, bt_icon);
	layer_add_child(window_get_root_layer(w), bitmap_layer_get_layer(bt_icon_layer));
	
	container_init(w);
	climber_init(w);
	
	struct tm *t;
  	time_t temp;        
  	temp = time(NULL);        
  	t = localtime(&temp);
	container_set_time(t);
	climber_refresh_position(false, t);
	
	BatteryChargeState state = battery_state_service_peek();
	battery_handler(state);
	
	bool con = bluetooth_connection_service_peek();
	bt_handler(con);
}

void window_unload(Window *w){
	container_deinit();
	climber_deinit();
	gbitmap_destroy(background);
}

void init() {
	window = window_create();
	window_set_window_handlers(window, (WindowHandlers){
		.load = window_load,
		.unload = window_unload,
	});
	battery_state_service_subscribe(battery_handler);
	bluetooth_connection_service_subscribe(bt_handler);
	tick_timer_service_subscribe(MINUTE_UNIT, &tick_handler);
	window_stack_push(window, true);
}

void deinit() {
	battery_state_service_unsubscribe();
	bluetooth_connection_service_unsubscribe();
	tick_timer_service_unsubscribe();
	window_destroy(window);
}

int main() {
	init();
	app_event_loop();
	deinit();
}
