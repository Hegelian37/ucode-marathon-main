/*
Copyright (c) 2022 Petr Koshelkov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "snake.h"
#include "game.h"
#include "config.h"

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  srand(time(NULL));

  SDL_Window* window = SDL_CreateWindow(
    "Snake score: 0",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    FIELD_WIDTH * TILE_SIZE,
    FIELD_HEIGHT * TILE_SIZE,
    0
  );

  SDL_Renderer* renderer = SDL_CreateRenderer(
    window,
    -1,
    SDL_RENDERER_ACCELERATED
  );

  // Start game
  struct snake snake = {
    .body = malloc(sizeof(struct segment)),
    .length = 1,
    .dir = DIR_RIGHT
  };
  snake.body[0] = (struct segment){.y = FIELD_HEIGHT / 2};

  struct segment apple = {.x = FIELD_WIDTH / 2, .y = FIELD_HEIGHT / 2};

  mainloop(window, renderer, &snake, &apple);

  printf("Score: %zu\n", snake.length - 1);

  // Close program
  free(snake.body);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
