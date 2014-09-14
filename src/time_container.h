#pragma once
typedef struct Container {
	TextLayer *time;
	TextLayer *date;
	BitmapLayer *highlighter;
	Layer *root;
}Container;
	
void container_set_time(struct tm *t);
void container_init(Window *w);
void container_deinit();