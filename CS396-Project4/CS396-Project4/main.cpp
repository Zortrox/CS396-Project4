//Matthew Clark
//Allegro test

#include <stdio.h>
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "physics.h"
#include "phys_object.h"
#include "globals.h"

#define FPS 60
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CANNON_X 100
#define CANNON_Y 500

enum keyCodes { KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT };

void fireCannonball(ALLEGRO_BITMAP* sprite, Physics* sim, std::vector<PhysObject*> &vecObjects, int x, int y, b2Vec2 velocity) {
	vecObjects.push_back(new PhysObject(sprite, sim->addCircle(x, y, 10)));
	vecObjects.back()->fire(velocity);
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
	bool drawCannonPull = false;
	int cannonballX = 0;
	int cannonballY = 0;
	b2Vec2 cannonballVel = b2Vec2(0, 0);

	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	//al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
	//al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	//al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	display = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
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

	if (!al_init_image_addon()) {
		fprintf(stderr, "failed to initialize image addon!\n");
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

	ALLEGRO_BITMAP* bmpCursor = al_create_bitmap(5, 5);
	al_set_target_bitmap(bmpCursor);
	al_clear_to_color(al_map_rgb(255, 0, 255));

	ALLEGRO_BITMAP* bmpStone = al_load_bitmap("assets/stone.jpg");
	ALLEGRO_BITMAP* bmpBox = al_create_bitmap(30, 30);
	al_set_target_bitmap(bmpBox);
	al_draw_scaled_bitmap(bmpStone, 0, 0, 512, 512, 0, 0, 30, 30, false);
	
	ALLEGRO_BITMAP* bmpWall = al_create_bitmap(120, 30);
	al_set_target_bitmap(bmpWall);
	al_draw_scaled_bitmap(bmpStone, 0, 0, 512, 512, 0, 0, 30, 30, false);
	al_draw_scaled_bitmap(bmpStone, 0, 0, 512, 512, 30, 0, 60, 30, false);
	al_draw_scaled_bitmap(bmpStone, 0, 0, 512, 512, 60, 0, 90, 30, false);
	al_draw_scaled_bitmap(bmpStone, 0, 0, 512, 512, 90, 0, 120, 30, false);

	ALLEGRO_BITMAP* bmpCannonball = al_create_bitmap(26, 26);
	al_set_target_bitmap(bmpCannonball);
	al_draw_circle(13, 13, 10, al_map_rgb(255, 0, 0), 1);

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
				drawCannonPull = true;
				break;
			case 2:
				vecObjects.push_back(new PhysObject(bmpBox, sim->addBox(cursorX, cursorY, 30, 30)));
				break;
			case 3:
				vecObjects.push_back(new PhysObject(bmpWall, sim->addBox(cursorX, cursorY, 120, 30)));
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			switch (ev.mouse.button) {
			case 1:
				drawCannonPull = false;
				fireCannonball(bmpCannonball, sim, vecObjects, cannonballX, cannonballY, cannonballVel);
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
			al_clear_to_color(al_map_rgb(173, 196, 234));
			//al_draw_text(font, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTER, "Welcome to Allegro!");

			sim->step(1.0f/FPS);

			al_draw_filled_rectangle(0, WINDOW_HEIGHT - 15, WINDOW_WIDTH, WINDOW_HEIGHT, al_map_rgb(23, 68, 9));

			for each (PhysObject* obj in vecObjects) {
				obj->draw(display);
			}

			if (drawCannonPull) {
				b2Vec2 dir = (b2Vec2(cursorX, cursorY) - b2Vec2(100, 500));
				float32 dist = dir.Normalize();
				cannonballVel = dir;

				if (dist > 80) dist = 80;
				dir *= dist;
				cannonballX = CANNON_X + dir.x;
				cannonballY = CANNON_Y + dir.y;

				cannonballVel *= -dist / 3;

				b2Vec2 currPoint = b2Vec2(1.0f * cannonballX / PHYS_PIX, 1.0f * cannonballY / PHYS_PIX);
				for (int i = 0; i < 180; i++) {
					b2Vec2 nextPoint = sim->getTrajectoryPoint(b2Vec2(1.0f * cannonballX / PHYS_PIX, 1.0f * cannonballY / PHYS_PIX),
						cannonballVel, i);
					al_draw_line(currPoint.x * PHYS_PIX, currPoint.y * PHYS_PIX, nextPoint.x * PHYS_PIX, nextPoint.y * PHYS_PIX,
						al_map_rgb(255, 255, 255), 1);
					currPoint = nextPoint;
				}

				al_draw_line(CANNON_X, CANNON_Y, cannonballX, cannonballY, al_map_rgb(80, 80, 10), 2);
				al_draw_line(CANNON_X, CANNON_Y, CANNON_X, WINDOW_HEIGHT, al_map_rgb(63, 0, 0), 5);
				al_draw_bitmap(bmpCannonball, cannonballX - 13, cannonballY - 13, false);
			}
			else {
				al_draw_line(CANNON_X, CANNON_Y, CANNON_X, WINDOW_HEIGHT, al_map_rgb(63, 0, 0), 5);
			}
			
			al_draw_bitmap(bmpCursor, cursorX, cursorY, 0);

			al_flip_display();
		}
	}

	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}