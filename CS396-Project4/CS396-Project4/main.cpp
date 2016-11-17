//Matthew Clark
//Allegro test

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "physics.h"
#include "phys_object.h"
#include "platform.h"
#include "person.h"
#include "globals.h"

#define FPS 60
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define GAME_HEIGHT 525
#define CANNON_X 100
#define CANNON_Y 400

enum keyCodes { KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT };
enum spriteCodes { SPR_PERSON, SPR_BOX, SPR_WALL, SPR_MAX_SIZE };

//trim string
std::string trim(std::string &str)
{
	if (str != "") {
		size_t first = str.find_first_not_of(" \t");
		size_t last = str.find_last_not_of(" \t");
		return str.substr(first, (last - first + 1));
	}
	else {
		return "";
	}
}

std::vector<std::string> split(std::string &str) {
	if (str == "") {
		return std::vector<std::string>();
	}

	std::stringstream ss(str);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vecArguments(begin, end);

	return vecArguments;
}

bool loadLevel(std::string filename, Physics* sim, std::vector<PhysObject*> &vecObjects, std::vector<Person*> &vecPeople, std::vector<ALLEGRO_BITMAP*> &vecSprites) {
	bool tutorialLevel = false;
	std::ifstream file(filename);

	if (!file) {
		std::cout << "Failed opening file:" << filename << std::endl;
	}
	else {
		std::string line;
		int lineNumber = 1;
		while (std::getline(file, line)) {
			line = trim(line);

			if (lineNumber == 1 && line == "true") {
				tutorialLevel = true;
			}
			else {
				std::vector<std::string> vecArguments = split(line);

				if (vecArguments.size() > 0) {
					int x = std::stoi(vecArguments.at(1));
					int y = 0;
					size_t indexY = vecArguments.at(2).find('-');
					if (indexY == -1) {
						y = std::stoi(vecArguments.at(2));
					}
					else {
						y = GAME_HEIGHT - std::stoi(vecArguments.at(2).substr(indexY + 1));
					}

					if (vecArguments.at(0) == "wall") {
						vecObjects.push_back(new PhysObject(vecSprites.at(SPR_WALL), sim->addBox(x, y, 120, 30), OBJ_STRUCTURE));
					}
					else if (vecArguments.at(0) == "wallr") {

					}
					else if (vecArguments.at(0) == "box") {
						vecObjects.push_back(new PhysObject(vecSprites.at(SPR_BOX), sim->addBox(x, y, 30, 30), OBJ_STRUCTURE));
					}
					else if (vecArguments.at(0) == "person") {
						vecPeople.push_back(new Person(vecSprites.at(SPR_PERSON), sim->addBox(x, y, 33, 22)));
					}
				}
			}
		}
	}

	return tutorialLevel;
}

void fireCannonball(ALLEGRO_BITMAP* sprite, Physics* sim, std::vector<PhysObject*> &vecObjects, int x, int y, b2Vec2 velocity) {
	vecObjects.push_back(new PhysObject(sprite, sim->addCircle(x, y, 10), OBJ_BALL));
	vecObjects.back()->fire(velocity);
}

int main(int argc, char **argv) {
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	bool redraw = true;
	bool quit = false;
	bool bWin = false;

	bool keys[5];
	bool bDisplayMenu = false;
	int cursorX = 0;
	int cursorY = 0;
	int cursorClickX = 0;
	int cursorClickY = 0;
	bool drawCannonPull = false;
	int cannonballX = 0;
	int cannonballY = 0;
	b2Vec2 slingDir = b2Vec2(0, 0);
	b2Vec2 cannonballVel = b2Vec2(0, 0);

	int groundHeight = 15;
	int shotsLeft = 10;
	int points = 0;

	int menuWidth = 200;
	std::vector<std::string> vecMenu;
	vecMenu.push_back("Resume");
	vecMenu.push_back("Restart");
	vecMenu.push_back("Levels");
	vecMenu.push_back("Quit");
	int itemHover = -1;

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

	al_init_font_addon();
	al_init_ttf_addon();

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
	ALLEGRO_FONT* fontUI = al_load_font("assets/arial.ttf", 30, false);
	ALLEGRO_FONT* fontMenuHeading = al_load_font("assets/arial.ttf", 20, false);
	ALLEGRO_FONT* fontMenuItem = al_load_font("assets/arial.ttf", 16, false);

	ALLEGRO_BITMAP* bmpCursor = al_create_bitmap(5, 5);
	al_set_target_bitmap(bmpCursor);
	al_clear_to_color(al_map_rgb(255, 0, 255));

	ALLEGRO_BITMAP* bmpStone = al_load_bitmap("assets/stone.jpg");
	ALLEGRO_BITMAP* bmpBox = al_create_bitmap(30, 30);
	al_set_target_bitmap(bmpBox);
	al_draw_scaled_bitmap(bmpStone, 0, 0, 512, 512, 0, 0, 30, 30, false);
	
	ALLEGRO_BITMAP* bmpWood = al_load_bitmap("assets/wood.jpg");
	ALLEGRO_BITMAP* bmpWall = al_create_bitmap(120, 30);
	al_set_target_bitmap(bmpWall);
	al_draw_scaled_bitmap(bmpWood, 0, 0, 256, 64, 0, 0, 120, 30, false);

	ALLEGRO_BITMAP* bmpPlatform = al_create_bitmap(150, 20);
	al_set_target_bitmap(bmpPlatform);
	al_draw_scaled_bitmap(bmpWood, 0, 0, 256, 64, 0, 0, 150, 20, false);

	ALLEGRO_BITMAP* bmpFurball = al_load_bitmap("assets/furball-64.png");
	ALLEGRO_BITMAP* bmpCannonball = al_create_bitmap(26, 26);
	al_set_target_bitmap(bmpCannonball);
	al_draw_scaled_bitmap(bmpFurball, 0, 0, 64, 64, 0, 0, 26, 26, false);

	ALLEGRO_BITMAP* bmpMammoth = al_load_bitmap("assets/mammoth-64.png");
	ALLEGRO_BITMAP* bmpPerson = al_create_bitmap(33, 22);
	al_set_target_bitmap(bmpPerson);
	al_draw_scaled_bitmap(bmpMammoth, 0, 0, 99, 64, 0, 0, 33, 22, false);

	ALLEGRO_BITMAP* bmpSlingFront = al_load_bitmap("assets/sling-front.png");
	ALLEGRO_BITMAP* bmpSlingBack = al_load_bitmap("assets/sling-back.png");

	al_set_target_bitmap(al_get_backbuffer(display));
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();

	Physics* sim = new Physics(WINDOW_WIDTH, GAME_HEIGHT - groundHeight);
	PhysObject* physGround = new PhysObject(NULL, sim->addGround(), OBJ_STRUCTURE);

	//level object storage
	std::vector<PhysObject*> vecObjects;
	std::vector<Person*> vecPeople;
	std::vector<ALLEGRO_BITMAP*> vecSprites;

	//add bitmaps to vector
	vecSprites.resize(SPR_MAX_SIZE);
	vecSprites.at(SPR_BOX) = bmpBox;
	vecSprites.at(SPR_PERSON) = bmpPerson;
	vecSprites.at(SPR_WALL) = bmpWall;

	//tutorial variables
	Platform* platform;
	Person* platformPerson;
	b2Joint* joint;

	//load level from file
	bool tutorial = loadLevel("levels/level1.lvl", sim, vecObjects, vecPeople, vecSprites);
	if (tutorial) {
		platform = new Platform(bmpPlatform, sim->addPlatform(200, 50, 150, 20));
		platform->getBody()->SetLinearVelocity(b2Vec2(-4, 0));
		platformPerson = new Person(bmpPerson, sim->addBox(200, 0, 33, 22));
		joint = sim->addJoint(platform->getBody(), platformPerson->getBody());
		vecPeople.push_back(platformPerson);
	}

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
				if (shotsLeft > 0 && !bDisplayMenu) {
					drawCannonPull = true;
					cursorClickX = ev.mouse.x;
					cursorClickY = ev.mouse.y;
				}
				break;
			case 2:
				vecObjects.push_back(new PhysObject(bmpBox, sim->addBox(cursorX, cursorY, 30, 30), OBJ_STRUCTURE));
				break;
			case 3:
				vecObjects.push_back(new PhysObject(bmpWall, sim->addBox(cursorX, cursorY, 120, 30), OBJ_STRUCTURE));
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			switch (ev.mouse.button) {
			case 1:
				if (drawCannonPull && !bDisplayMenu) {
					drawCannonPull = false;
					fireCannonball(bmpCannonball, sim, vecObjects, cannonballX, cannonballY, cannonballVel);
					shotsLeft--;
				}
				else if (bDisplayMenu && itemHover != -1) {
					std::string text = vecMenu.at(itemHover);
					if (text == "Resume") {
						bDisplayMenu = false;
					}
					else if (text == "Restart") {

					}
					else if (text == "Levels") {

					}
					else if (text == "Quit") {
						quit = true;
					}
				}
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
				bDisplayMenu = !bDisplayMenu;
				break;
			}
		}

		if (redraw && al_event_queue_is_empty(event_queue)) {
			redraw = false;
			al_clear_to_color(al_map_rgb(173, 196, 234));
			//al_draw_text(font, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTER, "Welcome to Allegro!");

			if (!bDisplayMenu) {
				sim->step(1.0f / FPS);
			}

			//draw grass
			al_draw_filled_rectangle(0, GAME_HEIGHT - groundHeight, WINDOW_WIDTH, GAME_HEIGHT, al_map_rgb(23, 68, 9));
			
			b2Vec2 posPlatform = platform->getBody()->GetPosition();
			b2Vec2 velPlatform = platform->getBody()->GetLinearVelocity();
			if (posPlatform.x > 1.0f * (WINDOW_WIDTH - 100) / PHYS_PIX && velPlatform.x > 0 && platform->getBody()->GetType() == b2_kinematicBody) {
				platform->getBody()->SetLinearVelocity(b2Vec2(-4, 0));
			}
			else if (posPlatform.x < 1.0f * 100 / PHYS_PIX  && velPlatform.x < 0 && platform->getBody()->GetType() == b2_kinematicBody) {
				platform->getBody()->SetLinearVelocity(b2Vec2(4, 0));
			}

			platform->draw(display);
			for each (PhysObject* obj in vecObjects) {
				obj->draw(display);
			}
			int numDead = 0;
			for each (Person* obj in vecPeople) {
				obj->draw(display);
				points += obj->calcPoints();
				if (obj->getDestroyed()) numDead++;
			}
			if (numDead == vecPeople.size()) {
				bWin = true;
			}

			if (platform->setType()) {
				sim->destroyJoint(joint);
			}

			if (drawCannonPull) {
				if (!bDisplayMenu) {
					slingDir = (b2Vec2(cursorX, cursorY) - b2Vec2(CANNON_X, CANNON_Y));
				}
				float32 dist = slingDir.Normalize();
				cannonballVel = slingDir;

				if (dist > 80) dist = 80;
				slingDir *= dist;
				if (!bDisplayMenu) {
					cannonballX = CANNON_X + slingDir.x;
					cannonballY = CANNON_Y + slingDir.y;
				}

				cannonballVel *= -dist / 3;

				b2Vec2 currPoint = b2Vec2(1.0f * cannonballX / PHYS_PIX, 1.0f * cannonballY / PHYS_PIX);
				for (int i = 0; i < 180; i++) {
					b2Vec2 nextPoint = sim->getTrajectoryPoint(b2Vec2(1.0f * cannonballX / PHYS_PIX, 1.0f * cannonballY / PHYS_PIX),
						cannonballVel, i);
					al_draw_line(currPoint.x * PHYS_PIX, currPoint.y * PHYS_PIX, nextPoint.x * PHYS_PIX, nextPoint.y * PHYS_PIX,
						al_map_rgb(255, 255, 255), 1);
					currPoint = nextPoint;
				}

				b2Vec2 bandLength = slingDir;
				bandLength.Normalize();
				bandLength *= 13;

				//slingshot
				al_draw_scaled_bitmap(bmpSlingBack, 0, 0, 43, 275, CANNON_X - 3, CANNON_Y - 18, 22, 138, false);
				al_draw_line(CANNON_X + 10, CANNON_Y, cannonballX + (bandLength.x), cannonballY + (bandLength.y), al_map_rgb(51, 25, 8), 4);
				

				//ball
				al_draw_bitmap(bmpCannonball, cannonballX - 13, cannonballY - 13, false);

				al_draw_line(CANNON_X - 10, CANNON_Y, cannonballX + (bandLength.x), cannonballY + (bandLength.y), al_map_rgb(51, 25, 8), 4);
				al_draw_scaled_bitmap(bmpSlingFront, 0, 0, 50, 138, CANNON_X - 20, CANNON_Y - 21, 25, 69, false);

				
			}
			else {
				al_draw_scaled_bitmap(bmpSlingBack, 0, 0, 43, 275, CANNON_X - 3, CANNON_Y - 18, 22, 138, false);
				al_draw_scaled_bitmap(bmpSlingFront, 0, 0, 50, 138, CANNON_X - 20, CANNON_Y - 21, 25, 69, false);
			}
			
			//draw UI
			al_draw_filled_rectangle(0, WINDOW_HEIGHT - 78, WINDOW_WIDTH, WINDOW_HEIGHT, al_map_rgb(211, 255, 242));
			al_draw_filled_rectangle(0, WINDOW_HEIGHT - 80, WINDOW_WIDTH, WINDOW_HEIGHT - 75, al_map_rgb(12, 175, 129));

			std::string txtShots = std::to_string(shotsLeft) + " shots left";
			std::string txtPoints = "Score: " + std::to_string(points);
			al_draw_text(fontUI, al_map_rgb(0, 0, 0), 10, WINDOW_HEIGHT - 53, false, txtShots.c_str());
			al_draw_text(fontUI, al_map_rgb(0, 0, 0), WINDOW_WIDTH - 250, WINDOW_HEIGHT - 53, false, txtPoints.c_str());

			//draw menu
			if (bDisplayMenu) {
				int menuHeight = vecMenu.size() * 40 + 80;

				al_draw_filled_rectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, al_map_rgba(0, 0, 0, 150));
				al_draw_filled_rectangle(WINDOW_WIDTH / 2 - menuWidth / 2, WINDOW_HEIGHT / 2 - menuHeight / 2, WINDOW_WIDTH / 2 + menuWidth / 2, WINDOW_HEIGHT / 2 + menuHeight / 2, al_map_rgb(100, 100, 100));
				al_draw_text(fontMenuHeading, al_map_rgb(255, 255, 255), WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - menuHeight / 2 + 10, ALLEGRO_ALIGN_CENTER, "Menu");

				itemHover = -1;
				int yPos = WINDOW_HEIGHT / 2 - menuHeight / 2 + 60;
				int index = 0;
				for each (std::string item in vecMenu) {
					ALLEGRO_COLOR boxColor;
					ALLEGRO_COLOR textColor;

					int x1 = WINDOW_WIDTH / 2 - menuWidth / 2 + 20;
					int x2 = WINDOW_WIDTH / 2 + menuWidth / 2 - 20;
					int y1 = yPos;
					int y2 = yPos + 30;

					if (cursorX >= x1 && cursorX <= x2 && cursorY >= y1 && cursorY <= y2) {
						boxColor = al_map_rgb(255, 255, 255);
						textColor = al_map_rgb(0, 0, 0);
						itemHover = index;
					}
					else {
						boxColor = al_map_rgb(0, 0, 0);
						textColor = al_map_rgb(255, 255, 255);
					}

					al_draw_filled_rounded_rectangle(x1, y1, x2, y2, 5, 5, boxColor);
					al_draw_text(fontMenuItem, textColor, WINDOW_WIDTH / 2, y1 + 8, ALLEGRO_ALIGN_CENTER, item.c_str());
					yPos += 40;
					index++;
				}
			}

			//draw cursor
			al_draw_bitmap(bmpCursor, cursorX, cursorY, 0);

			al_flip_display();
		}
	}

	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}