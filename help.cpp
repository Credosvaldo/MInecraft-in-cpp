#include <iostream>
#include <cstdlib>
#include <ctime>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

int gerarNumeroAleatorio(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

std::string intToString(int number)
{
    return std::to_string(number);
}

int main()
{
    if (!al_init())
    {
        std::cerr << "Falha ao inicializar o Allegro!" << std::endl;
        return -1;
    }

    if (!al_install_keyboard())
    {
        std::cerr << "Falha ao instalar o teclado!" << std::endl;
        return -1;
    }

    if (!al_install_mouse())
    {
        std::cerr << "Falha ao instalar o mouse!" << std::endl;
        return -1;
    }

    if (!al_init_font_addon())
    {
        std::cerr << "Falha ao inicializar o addon de fonte do Allegro!" << std::endl;
        return -1;
    }

    if (!al_init_ttf_addon())
    {
        std::cerr << "Falha ao inicializar o addon de TrueType Font do Allegro!" << std::endl;
        return -1;
    }

    if (!al_init_primitives_addon())
    {
        std::cerr << "Falha ao inicializar o addon de primitivas do Allegro!" << std::endl;
        return -1;
    }

    ALLEGRO_DISPLAY *display = al_create_display(800, 600);
    if (!display)
    {
        std::cerr << "Falha ao criar a janela do jogo!" << std::endl;
        return -1;
    }

    ALLEGRO_FONT *font = al_create_builtin_font();
    if (!font)
    {
        std::cerr << "Falha ao criar a fonte!" << std::endl;
        return -1;
    }

    ALLEGRO_MOUSE_STATE mouseState;

    srand(time(NULL));

    bool sair = false;
    bool iniciar = false;
    int acertos = 0;
    int erros = 0;
    int pontos = 0;

    std::string nomeJogador;

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    if (!event_queue)
    {
        std::cerr << "Falha ao criar a fila de eventos!" << std::endl;
        al_destroy_font(font);
        al_destroy_display(display);
        return -1;
    }

    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    ALLEGRO_KEYBOARD_STATE keyboardState;
    std::string respostaString;

    while (!sair)
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(event_queue, &evento);

        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            al_get_mouse_state(&mouseState);

            if (!iniciar && mouseState.x >= 200 && mouseState.x <= 600 && mouseState.y >= 300 && mouseState.y <= 350)
            {
                iniciar = true;
                pontos = gerarNumeroAleatorio(1, 10);
            }
            else if (iniciar && mouseState.x >= 10 && mouseState.x <= 60 && mouseState.y >= 10 && mouseState.y <= 50)
            {
                sair = true;
            }
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            al_get_keyboard_state(&keyboardState);

            if (al_key_down(&keyboardState, ALLEGRO_KEY_ENTER) && iniciar)
            {
                int resposta = std::stoi(respostaString);

                if (resposta == pontos)
                {
                    acertos++;
                }
                else
                {
                    erros++;
                }

                pontos = gerarNumeroAleatorio(1, 10);
                respostaString = "";
            }
            else if (al_key_down(&keyboardState, ALLEGRO_KEY_ESCAPE))
            {
                sair = true;
            }
            else if (al_key_down(&keyboardState, ALLEGRO_KEY_BACKSPACE) && respostaString.length() > 0)
            {
                respostaString.pop_back();
            }
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_CHAR && iniciar)
        {
            if (evento.keyboard.unichar >= 48 && evento.keyboard.unichar <= 57 && respostaString.length() < 3)
            {
                respostaString += evento.keyboard.unichar;
            }
        }

        al_draw_filled_rectangle(0, 0, 800, 600, al_map_rgb(255, 255, 255));

        if (!iniciar)
        {
            al_draw_text(font, al_map_rgb(0, 0, 0), 400, 250, ALLEGRO_ALIGN_CENTRE, "APERTE PARA INICIAR");
            al_draw_filled_rectangle(200, 300, 600, 350, al_map_rgb(0, 255, 0));
            al_draw_text(font, al_map_rgb(0, 0, 0), 400, 325, ALLEGRO_ALIGN_CENTRE, "JOGAR");
        }
        else
        {
            al_draw_text(font, al_map_rgb(0, 0, 0), 10, 10, 0, "SAIR");

            // Perguntas e opções de adição
            std::string pergunta;
            int opcao1, opcao2, opcao3;
            int respostaCorreta;
            switch (pontos)
            {
                case 1:
                    pergunta = "2 + 3 = ?";
                    opcao1 = 4;
                    opcao2 = 5;
                    opcao3 = 6;
                    respostaCorreta = opcao2;
                    break;
                case 2:
                    pergunta = "4 + 1 = ?";
                    opcao1 = 3;
                    opcao2 = 5;
                    opcao3 = 6;
                    respostaCorreta = opcao2;
                    break;
                case 3:
                    pergunta = "7 + 2 = ?";
                    opcao1 = 8;
                    opcao2 = 9;
                    opcao3 = 10;
                    respostaCorreta = opcao2;
                    break;
                case 4:
                    pergunta = "9 + 5 = ?";
                    opcao1 = 13;
                    opcao2 = 14;
                    opcao3 = 15;
                    respostaCorreta = opcao1;
                    break;
                case 5:
                    pergunta = "6 + 4 = ?";
                    opcao1 = 8;
                    opcao2 = 9;
                    opcao3 = 10;
                    respostaCorreta = opcao3;
                    break;
                case 6:
                    pergunta = "3 + 8 = ?";
                    opcao1 = 11;
                    opcao2 = 10;
                    opcao3 = 11;
                    respostaCorreta = opcao2;
                    break;
                case 7:
                    pergunta = "1 + 6 = ?";
                    opcao1 = 6;
                    opcao2 = 7;
                    opcao3 = 8;
                    respostaCorreta = opcao3;
                    break;
                case 8:
                    pergunta = "5 + 5 = ?";
                    opcao1 = 9;
                    opcao2 = 10;
                    opcao3 = 11;
                    respostaCorreta = opcao2;
                    break;
                default:
                    break;
            }

            al_draw_text(font, al_map_rgb(0, 0, 0), 400, 250, ALLEGRO_ALIGN_CENTRE, pergunta.c_str());
            al_draw_text(font, al_map_rgb(0, 0, 0), 400, 300, ALLEGRO_ALIGN_CENTRE, "Opções:");

            al_draw_text(font, al_map_rgb(0, 0, 0), 400, 350, ALLEGRO_ALIGN_CENTRE, ("A) " + intToString(opcao1)).c_str());
            al_draw_text(font, al_map_rgb(0, 0, 0), 400, 400, ALLEGRO_ALIGN_CENTRE, ("B) " + intToString(opcao2)).c_str());
            al_draw_text(font, al_map_rgb(0, 0, 0), 400, 450, ALLEGRO_ALIGN_CENTRE, ("C) " + intToString(opcao3)).c_str());
            al_draw_text(font, al_map_rgb(0, 0, 0), 400, 500, ALLEGRO_ALIGN_CENTRE, ("Resposta: " + respostaString).c_str());

            if (acertos + erros >= 8)
            {
                al_draw_text(font, al_map_rgb(0, 0, 0), 400, 550, ALLEGRO_ALIGN_CENTRE, ("Pontuação: " + intToString(acertos) + " acertos, " + intToString(erros) + " erros").c_str());
            }
        }

        al_flip_display();
    }

    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}