#include <pebble.h>
#include "time_container.h"
#include "animations.h"
	
Container base;
GBitmap *highlighter;
GRect r_frame;

void container_set_time(struct tm *t){
	static char buffer[] = "00:00";
	static char d_buffer[] = "September 26th";
	if(clock_is_24h_style()){
		strftime(buffer, sizeof(buffer), "%H:%M", t);
	}
	else{
		strftime(buffer,sizeof(buffer),"%I:%M", t);
	}
	strftime(d_buffer,sizeof(d_buffer),"%d. %b. '%y", t);
	
	text_layer_set_text(base.time, buffer);
	text_layer_set_text(base.date, d_buffer);
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "%s %s", buffer, d_buffer);
	
	int fix, hour;
	hour = t->tm_hour;
	if(hour > 12){
		hour = hour-12;
	}
	fix = 132-(11*hour);
	if(hour == 12 || hour == 0){
		fix = 0;
	}
	
	r_frame = GRect(14, fix, 144, 168);
	if(t->tm_sec == 0 && t->tm_min == 0){
		GRect temp = layer_get_frame(base.root);
		animate_layer(base.root, &temp, &r_frame, 1000, 0);
		return;
	}
	layer_set_frame(base.root, GRect(14, fix, 144, 168));
}

void container_init(Window *w){
	Layer *window_layer = window_get_root_layer(w);
	base.root = layer_create(GRect(14, 0, 144, 16));
	layer_add_child(window_layer, base.root);
	
	highlighter = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TIME_CONTAINTER);
	
	base.highlighter = bitmap_layer_create(GRect(0, 0, 144, 19));
	bitmap_layer_set_bitmap(base.highlighter, highlighter);
	layer_add_child(base.root, bitmap_layer_get_layer(base.highlighter));
		
	base.time = text_layer_create(GRect(24, -3, 50, 20));
	text_layer_set_font(base.time, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_text_color(base.time, GColorWhite);
	text_layer_set_background_color(base.time, GColorClear);
	layer_add_child(base.root, text_layer_get_layer(base.time));
	
	base.date = text_layer_create(GRect(58, -3, 74, 20));
	text_layer_set_font(base.date, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_text_color(base.date, GColorWhite);
	text_layer_set_background_color(base.date, GColorClear);
	layer_add_child(base.root, text_layer_get_layer(base.date));
}

void container_deinit(){
	text_layer_destroy(base.time);
	text_layer_destroy(base.date);
	bitmap_layer_destroy(base.highlighter);
	layer_destroy(base.root);
	gbitmap_destroy(highlighter);
}