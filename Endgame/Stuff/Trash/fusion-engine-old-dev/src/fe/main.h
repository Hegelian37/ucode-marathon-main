// Created by Dima Marhitych
#include "files/include.h"
#include "files/window.h"
#include "files/event.h"
#include "files/draw.h"
#include "files/renderer.h"
#include "files/image.h"
#include "files/input.h"

#define TEST_PNG "fe/test_files/test.png"


void init(void) {
    SDL_Init(SDL_INIT_EVERYTHING);
}

int windowShouldClose(void) {
    int running = 1;

    Event event;
    while (checkEvents(&event)) {

        if (event.type == SDL_QUIT) {
            running = 0;
        }
    }

    return running;
}

void Quit(SDL_Window *window) {
    SDL_DestroyWindow(window);
    SDL_Quit(void);
}

void refresh(SDL_Renderer *renderer) {
    SDL_RenderPresent(renderer);
}
