#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <main.h>

int main(int argc, char *argv[])
{
    /* Inicializa a biblioteca SDL2 */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Não foi possível inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    /* Cria uma janela com o título "Video Buffer", com largura de 800 e altura de 600 pixels */
    SDL_Window *window = SDL_CreateWindow(
        "Video Buffer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_SHOWN
    );

    /* Verifica se a criação da janela foi bem-sucedida */
    if (!window)
    {
        fprintf(stderr, "Não foi possível criar a janela: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    /* Cria um renderer associado à janela criada */
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    /* Verifica se a criação do renderer foi bem-sucedida */
    if (!renderer)
    {
        fprintf(stderr, "Não foi possível criar o renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    /* Cria uma textura para o buffer de vídeo */
    SDL_Texture *texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH,
        HEIGHT
    );

    if (!texture)
    {
        fprintf(stderr, "Não foi possível criar a textura: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    /* Array para armazenar os dados do buffer de vídeo */
    Uint32 *pixels = (Uint32 *)malloc(WIDTH * HEIGHT * sizeof(Uint32));
    if (!pixels)
    {
        fprintf(stderr, "Não foi possível alocar memória para o buffer de vídeo.\n");
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    srand((unsigned int)time(NULL)); // Inicializa o gerador de números aleatórios

    /* Loop principal do programa */
    SDL_Event event;
    int running = 1;  /* Flag que controla o loop de execução */
    while (running)
    {
        /* Verifica eventos SDL */
        while (SDL_PollEvent(&event))
        {
            /* Se o evento for de fechamento da janela, termina o loop */
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
        }

        /* Atualiza o buffer de vídeo com cores aleatórias */
        for (int y = 0; y < HEIGHT; ++y)
        {
            for (int x = 0; x < WIDTH; ++x)
            {
                Uint8 r = rand() % 256;
                Uint8 g = rand() % 256;
                Uint8 b = rand() % 256;
                Uint8 a = 255; // Opacidade 100%
                pixels[y * WIDTH + x] = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), r, g, b, a);
            }
        }

        /* Atualiza a textura com o buffer de vídeo */
        SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));

        /* Limpa o buffer de desenho */
        SDL_RenderClear(renderer);
        /* Desenha a textura na tela */
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        /* Atualiza a tela com o que foi desenhado */
        SDL_RenderPresent(renderer);

        /* Delay para reduzir o uso da CPU */
        SDL_Delay(16);  /* Aprox 60 FPS */
    }

    /* Libera memoria e fecha a SDL */
    free(pixels);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

