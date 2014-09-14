#include <pebble.h>
#include "climber.h"
	
BitmapLayer *guy;
GBitmap *default_guy, *flipped_guy, *standing_guy;
	
void climber_refresh_position(bool boot, struct tm *t){
	if(boot){
		int hour = t->tm_hour;
		if(hour > 12){
			hour = hour-12;
		}
		int fix = 110-(11*hour);
		int fix2 = 4;
		if(hour%2 == 0){
			fix2 = 3;
			bitmap_layer_set_bitmap(guy, default_guy);
		}
		else{
			fix2 = 7;
			bitmap_layer_set_bitmap(guy, flipped_guy);
		}
		layer_set_frame(bitmap_layer_get_layer(guy), GRect(fix2, fix, 14, 27));
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "GRect(0, %d, 16, 29) (hour: %d)", fix, hour);
		if(hour == 0 || hour == 12){
			bitmap_layer_set_bitmap(guy, standing_guy);
			layer_set_frame(bitmap_layer_get_layer(guy), GRect(30, 110, 14, 35));
		}
	}
	else{
		
	}
}

void climber_init(Window *w){
	default_guy = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DEFAULT_CLIMBER);
	flipped_guy = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_INVERTED_CLIMBER);
	standing_guy = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CLIMBER_STANDING);
	
	guy = bitmap_layer_create(GRect(5, 0, 14, 27));
	bitmap_layer_set_bitmap(guy, default_guy);
	layer_add_child(window_get_root_layer(w), bitmap_layer_get_layer(guy));
	
	struct tm *t;
  	time_t temp;        
  	temp = time(NULL);        
  	t = localtime(&temp);
	
	climber_refresh_position(true, t);
}

void climber_deinit(){
	gbitmap_destroy(default_guy);
	gbitmap_destroy(flipped_guy);
	gbitmap_destroy(standing_guy);
	bitmap_layer_destroy(guy);
}