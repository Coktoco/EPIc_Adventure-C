#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <memory>
#include <cmath>

#include "zewnklasy.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

using namespace std;

int losuj_dy() { //losuje wspolrzedna w pionie wrogow, zeby pojawiali sie na roznych wysokosciach
	int dy = rand() % (950 - 340);
	return dy;
}

bool zderzenie(Przeciwnik p, int x, int y) { //sprawdza czy bylo zderzenie z konkretnym przeciwnikiem
	if ((x + 140 > p.dx && x + 140 < p.dx + 170) && (y + 140 > p.dy && y + 140 < p.dy + 340))
		return true;
	else
		return false;

}

int main() {
	srand(time(NULL));
	al_init();
	al_install_keyboard();
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_ttf_addon();

	Gracz player(0, 0, 10, 10, 0, 3, 0);	//tworzymy gracza na punkty i zycie gracza

	//tworzymy trzech wrogow
	Przeciwnik enemy1(1800, losuj_dy(), 10);
	Przeciwnik enemy2(2200, losuj_dy(), 10);
	Przeciwnik enemy3(enemy2);

	enemy3.dx = 2600;


	Piwo beer;
	beer.dy = losuj_dy();

	Tarcza shield;
	shield.dy = losuj_dy();


	vector < int > tab;		// implementacja szablonu stl: vector
	int utraty_zyc = 1;		// ilosc zyc straconych zapisywana w vectorze

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	ALLEGRO_DISPLAY* disp = al_create_display(1800, 950);
	ALLEGRO_FONT* font = al_create_builtin_font();
	// ALLEGRO_FONT* font2 = al_load_ttf_font("pixelated.ttf", 150, 0);
	ALLEGRO_FONT* font2 = al_load_ttf_font("YARDSALE.ttf", 150, 0); // Alternatywny font 
	ALLEGRO_FONT* font_MAIN = al_load_ttf_font("YARDSALE.ttf", 80, 0); // Alternatywny font 
	ALLEGRO_BITMAP* background = al_load_bitmap("Background_v2.png");
	ALLEGRO_BITMAP* enemy = al_load_bitmap("Enemy_v2.png");
	ALLEGRO_BITMAP* jetpack = al_load_bitmap("jetpackman_v2.png");
	ALLEGRO_BITMAP* ekran_start1 = al_load_bitmap("Ekran_start1.png");
	ALLEGRO_BITMAP* ekran_start2 = al_load_bitmap("Ekran_start2.png");
	ALLEGRO_BITMAP* ekran_koniec = al_load_bitmap("Ekran_koniec.png");
	ALLEGRO_BITMAP* cien_jetpack = al_load_bitmap("Cien_jetpackman.png");
	ALLEGRO_BITMAP* cien_enemy = al_load_bitmap("Cien_enemy.png");
	ALLEGRO_BITMAP* piwo = al_load_bitmap("Piwo.png");
	ALLEGRO_BITMAP* serca1 = al_load_bitmap("1serca.png");
	ALLEGRO_BITMAP* serca2 = al_load_bitmap("2serca.png");
	ALLEGRO_BITMAP* serca3 = al_load_bitmap("3serca.png");
	ALLEGRO_BITMAP* shield1 = al_load_bitmap("Shield_ghost.png");

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	//zmienna od opadania
	int t = 0;

	bool game_over = false;
	bool trzeba_zapisac = true;;
	//wspolrzedne ekranow
	int xb1 = 0;
	int xb2 = 1800;

	// Zmienne cienia
	float yc = 728;
	float w_c = 150, h_c = 30;

	// Zmienna ekranu
	int ek = 0;

	// Zmienna wynik koncowy
	int wynik = 0;

	// Bool Epiwo Easter Egg
	int e = 0;
	int p = 0;
	int i = 0;
	int w = 0;
	int o = 0;
	int epiwo = 0;

	bool redraw = true;
	bool done = false;
	ALLEGRO_EVENT event;

	int counter = 0; //licznik klatek
	unsigned char key[ALLEGRO_KEY_MAX];
	memset(key, 0, sizeof(key));

	try {	// obsluga mechanizmu wyjatkow w celu obslugi bledow, ktore moga wystapic w trakcie ladowania np. addonow
		if (!al_init_image_addon()) {
			throw runtime_error("Nie udalo sie zaladowac dodatku/addona do obrazow!");
		}

		if (!al_load_bitmap("jetpackman_v2.png")) {
			throw std::runtime_error("Nie udalo sie zaladowac zdjecia!");
		}
	}
	catch (const std::exception& ex) {
		cerr << "Wystapil blad: " << ex.what() << std::endl;
		return 1;

		}

	al_start_timer(timer);
	while (true) {
		al_wait_for_event(queue, &event);

		switch (event.type) {
		case ALLEGRO_EVENT_TIMER:
			if (key[ALLEGRO_KEY_UP]) {
				t = 0;
				player.y -= player.vy;
				yc -= 2;
				w_c -= 2;
				h_c -= 1;
			}
			if (key[ALLEGRO_KEY_DOWN]) {
				player.y += player.vy;
				yc += 1;
				w_c += 1;
				h_c += 0.5;
			}

			if (yc < 728) {
				yc = 728;
			}

			if (w_c < 150)
				w_c = 150;

			if (h_c < 30)
				h_c = 30;

			if (yc > 860) {
				yc = 860;
			}

			if (w_c > 300)
				w_c = 300;

			if (h_c > 100)
				h_c = 100;
			// OPAD MECHANIKA
			if (!key[ALLEGRO_KEY_UP]) {
				t++;
				yc += 1;
				w_c += 1;
				h_c += 0.5;

				if (t > 0 && t <= 60) {
					player.y += 3;
				}
				if (t > 60 && t <= 120) {
					player.y += 4;
				}
				if (t > 120) {
					player.y += 6;
				}
			}

			if (key[ALLEGRO_KEY_RIGHT])
				player.x += player.vx;
			if (key[ALLEGRO_KEY_LEFT])
				player.x -= player.vx;
			if (key[ALLEGRO_KEY_ESCAPE])
				done = true;
			// Zmienne od zmiany ekranu
			if (key[ALLEGRO_KEY_ENTER])
				ek = 1;

			// Aktywacja Easter Egga
			if (key[ALLEGRO_KEY_E])
				e = 1;
			if (key[ALLEGRO_KEY_P]) {
				if (e == 1)
					p = 1;
			}
			if (key[ALLEGRO_KEY_I]) {
				if (e == 1 && p == 1)
					i = 1;
			}
			if (key[ALLEGRO_KEY_W]) {
				if (e == 1 && p == 1 && i == 1)
					w = 1;
			}
			if (key[ALLEGRO_KEY_O]) {
				if (e == 1 && p == 1 && i == 1 && w == 1)
					epiwo = 1;
			}

			//spawnowanie piwa
			//beer.klatki_spawn = (rand() % 600) + 600;
			beer.spawn = rand() % 1000; //losowanie liczby od 0 do 999
			if (beer.spawn == 0 && !beer.ruch) { //piwo ma wjechac na ekran kiedy zostanie wylosowane 0 i piwo sie jeszcze nie porusza
				beer.dx = 1800;
				beer.dy = losuj_dy();
				beer.ruch = true; //piwo ma zaczac wjezdzac na ekran
			}
			if (beer.dx < -120) { //piwo wyjezdza poza ekran
				beer.dx = 1800;
				beer.dy = losuj_dy();
				beer.ruch = false; //piwo ma sie pojawic za ekranem, ale ma sie nie ruszac dopoki nie zostanie wylosowane 
			}
			if (beer.zderzenie(player.x, player.y)) {
				if (player.zycia < 3 && player.zycia > 0) {
					beer.dx = 1800;
					beer.dy = losuj_dy();
					beer.ruch = false; //piwo ma sie pojawic za ekranem, ale ma sie nie ruszac dopoki nie zostanie wylosowane 
					player.zycia++; //gracz dostanie +1 zycie
				}
			}

			//tarcza
			shield.spawn = rand() % 1000;
			if (shield.spawn == 0 && !shield.ruch) {
				shield.dx = 1800;
				shield.dy = losuj_dy();
				shield.ruch = true;
			}
			if (shield.dx < -120) {
				shield.dx = 1800;
				shield.dy = losuj_dy();
				shield.ruch = false;
			}
			if (shield.zderzenie(player.x, player.y)) {
				shield.aktywna = true;
				shield.czas_start = counter;
				shield.dx = 1800;
				shield.dy = losuj_dy();
				shield.ruch = false;
			}

			if (counter - shield.czas_start >= 300) //jesli minelo 5 sekund to tarcza sie deaktywuje
				shield.aktywna = false;

			//jesli wrog wyszedl za lewa krawedz mapy to spawnuje sie z powrotem z prawej strony
			if (enemy1.dx < -340) {
				enemy1.dx = 1800; //dla kazdego z trzech wrogow jest przesuniecie o 200 w poziomie, zeby atakowali w odstepach
				enemy1.dy = losuj_dy();
			}
			if (enemy2.dx < -340) {
				enemy2.dx = 2200;
				enemy2.dy = losuj_dy();
			}
			if (enemy3.dx < -340) {
				enemy3.dx = 2600;
				enemy3.dy = losuj_dy();
			}

			//jesli nastapilo zderzenie to wrog sie respawnuje z prawej, a gracz traci jedno zycie
			if (zderzenie(enemy1, player.x, player.y) && !shield.aktywna) {
				enemy1.dx = 1800;
				enemy1.dy = losuj_dy();
				player.zycia--;
				tab.push_back(utraty_zyc);
				tab.push_back(1);
				utraty_zyc++;
			}
			if (zderzenie(enemy2, player.x, player.y) && !shield.aktywna) {
				enemy2.dx = 2000;
				enemy2.dy = losuj_dy();
				player.zycia--;
				tab.push_back(utraty_zyc);
				tab.push_back(2);
				utraty_zyc++;
			}
			if (zderzenie(enemy3, player.x, player.y) && !shield.aktywna) {
				enemy3.dx = 2200;
				enemy3.dy = losuj_dy();
				player.zycia--;
				tab.push_back(utraty_zyc);
				tab.push_back(3);
				utraty_zyc++;
			}

			//blokowanie gracza zeby nie przekroczyl granic mapy
			player(1800, 950, 280);		// implementacja przeciazonego operatora ()

			redraw = true;
			break;

			case ALLEGRO_EVENT_KEY_DOWN:
				key[event.keyboard.keycode] = 1;
				break;
			case ALLEGRO_EVENT_KEY_UP:
				key[event.keyboard.keycode] = 0;
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				done = true;
				break;
			}
			if (done)
				break;

			//wrogowie sie przesuwaja
			enemy1.dx -= enemy1.vdx;
			enemy2.dx -= enemy2.vdx;
			enemy3.dx -= enemy3.vdx;

			if (beer.ruch)
				beer.dx -= enemy1.vdx;
			if (shield.ruch)
				shield.dx -= enemy1.vdx;

			if (redraw && al_is_event_queue_empty(queue)) {

				if (ek == 0) {
					al_draw_scaled_bitmap(background, 0, 0, 1920, 1080, xb1, 0, 1800, 950, 0);
					al_draw_scaled_bitmap(background, 0, 0, 1920, 1080, xb2, 0, 1800, 950, 0);
					al_draw_scaled_bitmap(ekran_start2, 0, 0, 1920, 1080, 0, 0, 1800, 950, 0);
					if (counter % 60 > 0 && counter % 60 < 30)
						al_draw_scaled_bitmap(ekran_start1, 0, 0, 1920, 1080, 0, 0, 1800, 950, 0);
					enemy1.dx = -300;
					enemy2.dx = -300;
					enemy3.dx = -300;
					player.pkt = 0;
					al_flip_display();
					redraw = false;

				}
				else {
					if (player.zycia <= 0) {
						game_over = true;
						al_draw_scaled_bitmap(background, 0, 0, 1920, 1080, 0, 0, 1800, 950, 0);
						al_draw_scaled_bitmap(ekran_koniec, 0, 0, 1920, 1080, 0, 0, 1800, 950, 0);
						al_draw_textf(font2, al_map_rgb(255, 255, 255), 1200, 640, 0, "%d", wynik);

						//zapisywanie nowego rekordu do pliku
						bool trzeba_zapisac = true;
						if (trzeba_zapisac) {
							trzeba_zapisac = false;

							ifstream plik("highscore.txt");
							if (!plik.is_open()) {
								cout << "Nie udalo sie otworzyc pliku" << endl;
							}

							int bufor; // bufor do zmiennej z pliku
							plik >> bufor;
							plik.close();

							if (player.highscore > bufor) {
								ofstream plik("highscore.txt");
								plik << player.highscore;
								plik.close();
							}
						}
					}
					else {
						al_clear_to_color(al_map_rgb(0, 0, 0));
						al_draw_scaled_bitmap(background, 0, 0, 1920, 1080, xb1, 0, 1800, 950, 0);
						al_draw_scaled_bitmap(background, 0, 0, 1920, 1080, xb2, 0, 1800, 950, 0);
						// Dodanie cienia TEST
						al_draw_tinted_scaled_bitmap(cien_enemy, al_map_rgba_f(1, 1, 1, 0.5), 0, 0, 750, 350, enemy1.dx + 95, 840, 150, 70, 0);
						al_draw_tinted_scaled_bitmap(cien_enemy, al_map_rgba_f(1, 1, 1, 0.5), 0, 0, 750, 350, enemy2.dx + 95, 840, 150, 70, 0);
						al_draw_tinted_scaled_bitmap(cien_enemy, al_map_rgba_f(1, 1, 1, 0.5), 0, 0, 750, 350, enemy3.dx + 95, 840, 150, 70, 0);
						// JetPackMan
						if (shield.aktywna && epiwo == 0) {
							al_draw_tinted_scaled_bitmap(jetpack, al_map_rgba_f(1, 1, 1, 0.3), 0, 0, 866, 883, player.x, player.y, 280, 280, 0);
						}
						else if (epiwo == 0) {
							al_draw_tinted_scaled_bitmap(cien_jetpack, al_map_rgba_f(1, 1, 1, 0.5), 0, 0, 960, 320, player.x + 20, yc, w_c, h_c, 0);
							al_draw_scaled_bitmap(jetpack, 0, 0, 866, 883, player.x, player.y, 280, 280, 0);
						}
						// Easter Egg
						if (epiwo == 1) {
							al_draw_tinted_scaled_bitmap(piwo, al_map_rgba_f(1, 1, 1, 1), 0, 0, 254, 367, player.x, player.y, 260, 280, 0);
							player.zycia = 1;
						}
						// Enemies 
						al_draw_scaled_bitmap(enemy, 0, 0, 1000, 1000, enemy1.dx, enemy1.dy, 340, 340, 0);
						al_draw_scaled_bitmap(enemy, 0, 0, 1000, 1000, enemy2.dx, enemy2.dy, 340, 340, 0);
						al_draw_scaled_bitmap(enemy, 0, 0, 1000, 1000, enemy3.dx, enemy3.dy, 340, 340, 0);
						// Piwo
						al_draw_scaled_bitmap(piwo, 0, 0, 254, 367, beer.dx, beer.dy, 120, 180, 0);
						// Tarcza
						al_draw_scaled_bitmap(shield1, 0, 0, 508, 734, shield.dx, shield.dy, 140, 180, 0);
						// Serca
						if (player.zycia == 3) {
							al_draw_tinted_scaled_bitmap(serca3, al_map_rgba_f(1, 1, 1, 0.8), 0, 0, 1800, 950, 0, 170, 1360, 800, 0);
						}
						if (player.zycia == 2) {
							al_draw_tinted_scaled_bitmap(serca2, al_map_rgba_f(1, 1, 1, 0.8), 0, 0, 1800, 950, 0, 170, 1360, 800, 0);
						}
						if (player.zycia == 1) {
							al_draw_tinted_scaled_bitmap(serca1, al_map_rgba_f(1, 1, 1, 0.8), 0, 0, 1800, 950, 0, 170, 1360, 800, 0);
						}
						// Napisy na ekranie
						// al_draw_text(font_MAIN, al_map_rgb(255, 255, 255), 40, 25, 0, "EPIc Adventure");
						al_draw_textf(font_MAIN, al_map_rgb(255, 255, 255), 1310, 830, 0, "Score: %d", player.pkt);
						// al_draw_textf(font_MAIN, al_map_rgb(255, 255, 255), 40, 75, 0, "Lives: %d", player.zycia);
						// Ustawiamy aktualny Player.score na zmienna wynik
						wynik = player.pkt;
					}
					al_flip_display();
				}
				redraw = false;
			}
			//liczymy fpsy
			if (!game_over) {
				counter++;
				//liczymy sekundy
				if (counter % 60 == 0) {
					player.pkt++; //kazda sekunda to +1 pkt dla gracza
					if (player.pkt > player.highscore)
						player.highscore = player.pkt;
				}

			}

			// Nowy system zmiany poziomu trudnoœci
			if (counter % 3000 == 0) {
				enemy1.vdx = enemy1.vdx + 1;
				enemy2.vdx = enemy2.vdx + 1;
				enemy3.vdx = enemy3.vdx + 1;
			}
			// Przesuwanie tla
			xb1 -= 5;
			xb2 -= 5;
			if (xb1 == -1800) {
				xb1 = 0;
				xb2 = 1800;
			}

	}
	cout << ":::::::::: DATA LOGS ::::::::::::: \n:\n";
	cout << ": Numer Utraconego Zycia: + Numer Przeciwnika: \n: ";
	for (int i = 0; i < tab.size()-1; i=i+2)
	{
		cout << tab[i] << " BY " << tab[i + 1] << " | ";
	}

	auto it_start = tab.begin();
	auto it_end = tab.end();

	int count_1 = count(it_start,it_end, 1);
	int count_2 = count(it_start, it_end, 2);
	int count_3 = count(it_start, it_end, 3);

	cout << endl << ":\n: Ilosc Smierci Na Danego Wroga !" << endl ;

	cout << ": DEATH BY 1: " << count_1 - 1 << endl;
	cout << ": DEATH BY 2: " << count_2 - 1 << endl;
	cout << ": DEATH BY 3: " << count_3 - 1 << endl;

	cout << ":\n: LOGI: ";


	for (auto& v : tab) {
		cout << v << " ";
	}
	cout << "\n:\n";

	if (shield.egzystencja(shield.aktywna)) {
		shield.aktywna = false;
	}

	if (beer.egzystencja(beer.aktywna)) {
		beer.aktywna = false;
	}

	cout << ":\n::::::::::::::::::::::::::::::::::";

	al_destroy_bitmap(jetpack);
	al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);

	return 0;
}