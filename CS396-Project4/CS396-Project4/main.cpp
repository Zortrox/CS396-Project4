//Matthew Clark
//Allegro test

#include <stdio.h>
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include "physics.h"
#include "phys_object.h"
#include "globals.h"

#define FPS 60

enum keyCodes { KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT };

void fireCannonball(ALLEGRO_BITMAP* sprite, Physics* sim, std::vector<PhysObject*> &vecObjects, int x, int y) {
	vecObjects.push_back(new PhysObject(sprite, sim->addBody(1.0f * 100 / PHYS_PIX, 1.0f * 500 / PHYS_PIX)));
	b2Vec2 dir = (b2Vec2(x, y) - b2Vec2(100, 500));
	dir *= 1.0f / 5;
	vecObjects.back()->fire(dir);
}

int main(int argc, char **argv) {
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	bool redraw = true;
	bool quit = false;

	bool keys[5];
	int cursorX = 0;
	int cursorY = 0;

	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	display = al_create_display(800, 600);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}
	al_hide_mouse_cursor(display);

	if (!al_init_primitives_addon()) {
		fprintf(stderr, "failed to initialize primitives!\n");
		return -1;
	}

	if (!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize the keyboard!\n");
		return -1;
	}

	if (!al_install_mouse()) {
		fprintf(stderr, "failed to initialize the mouse!\n");
		return -1;
	}

	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	ALLEGRO_FONT* font = al_create_builtin_font();

	ALLEGRO_BITMAP* bouncer = al_create_bitmap(5, 5);
	al_set_target_bitmap(bouncer);
	al_clear_to_color(al_map_rgb(255, 0, 255));

	ALLEGRO_BITMAP* box = al_create_bitmap(30, 30);
	al_set_target_bitmap(box);
	al_draw_rectangle(0, 0, 30, 30, al_map_rgb(0, 255, 0), 2);

	al_set_target_bitmap(al_get_backbuffer(display));
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();

	Physics* sim = new Physics();
	std::vector<PhysObject*> vecObjects;

	al_start_timer(timer);
	while (!quit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			quit = true;
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES ||
			ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {

			cursorX = ev.mouse.x;
			cursorY = ev.mouse.y;
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			switch (ev.mouse.button) {
			case 1:
				fireCannonball(box, sim, vecObjects, cursorX, cursorY);
				break;
			case 2:
				vecObjects.push_back(new PhysObject(box, sim->addBody(1.0f * cursorX / PHYS_PIX, 1.0f * cursorY / PHYS_PIX)));
				break;
			}
			
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				keys[KEY_UP] = true;
				break;

			case ALLEGRO_KEY_DOWN:
				keys[KEY_DOWN] = true;
				break;

			case ALLEGRO_KEY_LEFT:
				keys[KEY_LEFT] = true;
				break;

			case ALLEGRO_KEY_RIGHT:
				keys[KEY_RIGHT] = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				keys[KEY_UP] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				keys[KEY_DOWN] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				keys[KEY_LEFT] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				keys[KEY_RIGHT] = false;
				break;

			case ALLEGRO_KEY_ESCAPE:
				quit = true;
			}
		}

		if (redraw && al_event_queue_is_empty(event_queue)) {
			redraw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_text(font, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTER, "Welcome to Allegro!");

			sim->step(1.0f/FPS);
			
			for each (PhysObject* obj in vecObjects) {
				obj->draw(display);
			}

			al_draw_bitmap(bouncer, cursorX, cursorY, 0);

			al_flip_display();
		}
	}

	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}