#include "menu.h"

float temperature = 0.0;
float humidity = 0.0;
int people_counter = 0;

WINDOW* data_infowin;

void set_temperature(float value) {
    temperature = value;
    mvwprintw(data_infowin, 3, 1, "Temperatura: %.1f", temperature);
    wrefresh(data_infowin);
    //data_info();
}

void set_humidity(float value) {
    humidity = value;
    mvwprintw(data_infowin, 4, 1, "Umidade: %.1f", humidity);
    wrefresh(data_infowin);
    //data_info();
}

void person_enter() {
    people_counter++;
    mvwprintw(data_infowin, 5, 1, "Contador de Pessoas: %d", people_counter);
    wrefresh(data_infowin);
    //data_info();
}

void person_leaves() {
    people_counter--;
    mvwprintw(data_infowin, 5, 1, "Contador de Pessoas: %d", people_counter);
    wrefresh(data_infowin);
    //data_info();
}

void init_menu() {
    initscr();
    cbreak();
    echo();
    keypad(stdscr, 1);
    curs_set(0);
}

void close_menu() {
    endwin();
}

int get_option(WINDOW * menu, char * choices[], int size) {
    int choice;
    int highlight = 0;

    while(1) {
        for(int i = 0; i < size; i++) {
            if (i == highlight)
                wattron(menu, A_REVERSE);
            mvwprintw(menu, i+3, 1, choices[i]);
            wattroff(menu, A_REVERSE);
        }
        choice = wgetch(menu);

        switch(choice) {
            case KEY_UP:
                if (highlight > 0) 
                    highlight--;
                break;
            case KEY_DOWN:
            if (highlight < size - 1)
                    highlight++;
                break;
            default:
                break;
        }
        if(choice == 10)
            return highlight;
    }
}

void set_title(WINDOW* menu, char* title) {
    wattron(menu, A_BOLD);
    mvwprintw(menu, 1, 1, title);
    wattroff(menu, A_BOLD);
}

WINDOW* get_menu(int position, int ammount) {
    int y_max, x_max;
    getmaxyx(stdscr, y_max, x_max);

    WINDOW* menu = newwin(y_max, x_max/ammount, 0, position*(x_max/ammount));
    box(menu, 0, 0);
    //refresh();
    wrefresh(menu);

    return menu;
}

void set_box(WINDOW* menu, char* title) {
    box(menu, 0, 0);
    set_title(menu, title);
    //refresh();
    wrefresh(menu);
}

void data_info() {
    data_infowin = get_menu(1, 2);
    set_title(data_infowin, "Dados");
    mvwprintw(data_infowin, 3, 1, "Temperatura: %.1f", temperature);
    mvwprintw(data_infowin, 4, 1, "Umidade: %.1f", humidity);
    mvwprintw(data_infowin, 5, 1, "Contador de Pessoas: %d", people_counter);
    //refresh();
    wrefresh(data_infowin);
}

void menu() {
    WINDOW* menuwin = get_menu(0, 2);
    keypad(menuwin, true);

    while(1) {
        set_box(menuwin, "Menu");
        char* options[] = {"Acionar dispositivo", "Ligar alarme de presença", "Ligar alarme de fumaça"};

        if (get_presece_alarm()) {
            options[1] = "Desligar alarme de presença";
        }

        if (get_smoke_alarm()) {
            options[2] = "Desligar alarme de fumaça";
        }

        int input = get_option(menuwin, options, 3);

        // wclear(menu);

        if (input == 0) {
            // set_loading();

            struct connection* conn_list = get_conn_list();
            int conn_list_size = get_conn_list_size();

            int ammount_options = 0;

            for (int i = 0; i < conn_list_size; i++) {
                ammount_options += conn_list[i].config.outputs_length;
            }

            char** option_names = malloc(ammount_options * sizeof(*option_names));
            int* option_gpios = malloc(ammount_options * sizeof(int));
            int* option_states = malloc(ammount_options * sizeof(int));

            int counter = 0;

            char** final_options = malloc((ammount_options + 5) * sizeof(*final_options));

            for(int i = 0; i < conn_list_size; i++) {
                for (int j = 0; j < conn_list[i].config.outputs_length; j++) {
                    *(option_names + counter) = conn_list[i].config.outputs[j].tag;
                    option_gpios[counter] = conn_list[i].config.outputs[j].gpio;

                    if (conn_list[i].config.outputs[j].state >= 0) 
                        option_states[counter] = conn_list[i].config.outputs[j].state;
                    else {
                        option_states[counter] = request_state(conn_list[i].config.outputs[j].gpio, conn_list[i].socket);
                        conn_list[i].config.outputs[j].state = option_states[counter];
                    }

                    char* result = malloc(9 + strlen(conn_list[i].config.outputs[j].tag));

                    if (option_states[counter])
                        strcpy(result, "Desligar ");
                    else
                        strcpy(result, "Ligar    ");

                    strcat(result, conn_list[i].config.outputs[j].tag);

                    *(final_options + counter) = result;

                    counter++;
                }
            }
            int extra_options_size = 0;

            if (check_type("lampada")) {
                final_options[ammount_options + extra_options_size] = "Ligar todas as lampadas";
                extra_options_size++;
                final_options[ammount_options + extra_options_size] = "Desligar todas as lampadas";
                extra_options_size++;
            }
            if (check_type("ar-condicionado")) {
                final_options[ammount_options + extra_options_size] = "Ligar todos os ar-condicionados";
                extra_options_size++;
                final_options[ammount_options + extra_options_size] = "Desligar todos os ar-condicionados";
                extra_options_size++;
            }
            final_options[ammount_options + extra_options_size] = "Voltar";
            extra_options_size++;

            wclear(menuwin);

            set_box(menuwin, "Menu");

            int input = get_option(menuwin, final_options, ammount_options + extra_options_size);

            if (strcmp(final_options[input], "Ligar todas as lampadas") == 0) {
                turn_all_devices_by_type("lampada", 1);
            } else if (strcmp(final_options[input], "Desligar todas as lampadas") == 0) {
                turn_all_devices_by_type("lampada", 0);
            } else if (strcmp(final_options[input], "Ligar todos os ar-condicionados") == 0) {
                turn_all_devices_by_type("ar-condicionado", 1);
            } else if (strcmp(final_options[input], "Desligar todos os ar-condicionados") == 0) {
                turn_all_devices_by_type("ar-condicionado", 0);
            } else if (strcmp(final_options[input], "Voltar") == 0) {
                // nada acontece
            } else if (input >= 0 && input < ammount_options) {
                make_order(option_gpios[input]);
            }

            free(final_options);
            free(option_names);
            free(option_gpios);
            free(option_states);
        }
        else if (input == 1) {
            invert_presece_alarm();
        }
        else if (input == 2) {
            invert_smoke_alarm();
        }
    }
}