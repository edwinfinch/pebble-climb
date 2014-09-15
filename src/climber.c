#include <pebble.h>
#include "climber.h"
#include "animations.h"
	
BitmapLayer *guy;
GBitmap *default_guy, *flipped_guy, *standing_guy;
int climber_runs = 0;
bool climber_invert = 0;
AppTimer *guy_timer;
bool is12 = false;

void climber_animation(){
	climber_runs++;
	climber_invert = !climber_invert;
	if(!is12){
		if(climber_runs == 4){
			climber_runs = 0;
			return;
		}
	}
	else{
		if(climber_runs == 12){
			climber_runs = 0;
			GRect temp1 = layer_get_frame(bitmap_layer_get_layer(guy));
			GRect temp2 = GRect(30, 110, 14, 35);
			animate_layer(bitmap_layer_get_layer(guy), &temp1, &temp2, 1000, 0);
			bitmap_layer_set_bitmap(guy, standing_guy);
			return;
		}
	}
	
	if(climber_invert){
		bitmap_layer_set_bitmap(guy, flipped_guy);
	}
	else{
		bitmap_layer_set_bitmap(guy, default_guy);
	}
	if(!is12){
		guy_timer = app_timer_register(625, climber_animation, NULL);
	}
	else{
		guy_timer = app_timer_register(400, climber_animation, NULL);
	}
}
	
void climber_refresh_position(bool boot, struct tm *t){
	int hour = t->tm_hour;
	if(hour > 12){
		hour = hour-12;
	}
	int fix = 110-(11*hour);
	int fix2 = 4;
	if(hour%2 == 0){
		fix2 = 3;
	}
	else{
		fix2 = 7;
	}
	if(boot){
		layer_set_frame(bitmap_layer_get_layer(guy), GRect(fix2, fix, 14, 27));
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "GRect(0, %d, 16, 29) (hour: %d)", fix, hour);
		
		if(hour%2 == 0){
			bitmap_layer_set_bitmap(guy, default_guy);
			climber_invert = 0;
		}
		else{
			bitmap_layer_set_bitmap(guy, flipped_guy);
			climber_invert = 1;
		}
		
		if(hour == 0 || hour == 12){
			bitmap_layer_set_bitmap(guy, standing_guy);
			layer_set_frame(bitmap_layer_get_layer(guy), GRect(30, 110, 14, 35));
			fix2 = 30;
			fix = 110;
		}
	}
	else{
		if(t->tm_sec == 0 && t->tm_min == 0){
			GRect temp = layer_get_frame(bitmap_layer_get_layer(guy));
			int width = 0;
			if(hour == 12 || hour == 0){
				width = 35;
				is12 = true;
			}
			else{
				width = 27;
				is12 = false;
			}
			int length = 2500;
			int timerlength = 625;
			if(is12){
				length = 4800;
				timerlength = 400;
				fix -= 4;
			}
		    animate_layer(bitmap_layer_get_layer(guy), &temp, &GRect(fix2, fix, 14, width), length, 0);
			guy_timer = app_timer_register(timerlength, climber_animation, NULL);
		}
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