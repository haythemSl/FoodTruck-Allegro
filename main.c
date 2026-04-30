#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

const int LARGEUR = 800;
const int HAUTEUR = 600;

/
enum EtatJeu {
    MENU,
    JEU
};

int main() {

    if (!al_init()) {
        printf("Erreur d'initialisation d'Allegro.\n");
        return -1;
    }

    if (!al_init_image_addon()) {
        printf("Erreur d'initialisation du module d'image.\n");
        return -1;
    }

    al_install_mouse();
    al_install_keyboard();

    ALLEGRO_DISPLAY* display = al_create_display(LARGEUR, HAUTEUR);
    al_set_window_title(display, "Food Truck Tycoon");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));


    ALLEGRO_BITMAP* bg_menu = al_load_bitmap("assets/bg_menu.png");
    ALLEGRO_BITMAP* btn_play = al_load_bitmap("assets/btn_play.png");
    ALLEGRO_BITMAP* bg_cuisine = al_load_bitmap("assets/bg_cuisine.png");

    if (!bg_menu || !btn_play || !bg_cuisine) {
        printf("Attention : Au moins une image manque dans le dossier assets/.\n");
    }


    int btn_w = 200;
    int btn_h = 80;
    int btn_x = (LARGEUR - btn_w) / 2;
    int btn_y = 400;

    enum EtatJeu etat_actuel = MENU;
    bool en_cours = true;
    bool redessiner = true;

    al_start_timer(timer);


    while (en_cours) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            en_cours = false;
        }
        else if (event.type == ALLEGRO_EVENT_TIMER) {
            redessiner = true;
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (event.mouse.button == 1) { // Clic gauche
                if (etat_actuel == MENU) {

                    if (event.mouse.x >= btn_x && event.mouse.x <= btn_x + btn_w &&
                        event.mouse.y >= btn_y && event.mouse.y <= btn_y + btn_h) {
                        etat_actuel = JEU; // Transition vers la cuisine
                    }
                }
            }
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                if (etat_actuel == JEU) {
                    etat_actuel = MENU;
                } else {
                    en_cours = false;
                }
            }
        }


        if (redessiner && al_is_event_queue_empty(queue)) {
            redessiner = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));

            if (etat_actuel == MENU) {
                if (bg_menu) al_draw_bitmap(bg_menu, 0, 0, 0);
                if (btn_play) al_draw_bitmap(btn_play, btn_x, btn_y, 0);
            }
            else if (etat_actuel == JEU) {
                if (bg_cuisine) al_draw_bitmap(bg_cuisine, 0, 0, 0);
            }

            al_flip_display();
        }
    }


    if (bg_menu) al_destroy_bitmap(bg_menu);
    if (btn_play) al_destroy_bitmap(btn_play);
    if (bg_cuisine) al_destroy_bitmap(bg_cuisine);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);

    return 0;
}