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
#include <stdbool.h>

#include "snake.h"
#include "config.h"

static void changeWindowTitle(SDL_Window* window, size_t score, bool is_pause) {
  char new_window_title[32];

  sprintf(
    new_window_title,
    "Snake score: %zu%s",
    score,
    is_pause ? ". Paused" : ""
  );

  SDL_SetWindowTitle(window, new_window_title);
}

static void drawSegment(SDL_Renderer* renderer, struct segment* segment) {
  SDL_Rect rect = {
    segment->x * TILE_SIZE,
    segment->y * TILE_SIZE,
    TILE_SIZE,
    TILE_SIZE
  };

  SDL_RenderFillRect(renderer, &rect);
}

static void drawFrame(
  SDL_Renderer* renderer,
  struct snake* snake,
  struct segment* apple
) {
  SDL_SetRenderDrawColor(renderer, 27, 27, 27, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 196, 30, 58, 255);
  drawSegment(renderer, apple);

  SDL_SetRenderDrawColor(renderer, 32, 124, 36, 255);
  for(size_t N, i = 0; i < snake->length; ++i) {
    N = snake->length - i;
    if(N <= 30) {
      SDL_SetRenderDrawColor(renderer, 2 + N, 94 + N, 6 + N, 255);
    }
    drawSegment(renderer, &snake->body[i]);
  }

  SDL_RenderPresent(renderer);
}

void mainloop(
  SDL_Window* window,
  SDL_Renderer* renderer,
  struct snake* snake,
  struct segment* apple
) {
  int previous_time;
  bool is_pause = false;

  for(;;) {
    previous_time = SDL_GetTicks();
    // Check window's events
    for(SDL_Event event; SDL_PollEvent(&event); ) {
      switch(event.type) {
      case SDL_QUIT:
        return;
      // Keyboard keys
      case SDL_KEYDOWN:
        switch(event.key.keysym.sym) {
        // Change snake direction
        case SDLK_UP:
          snake->dir = DIR_UP;
          break;
        case SDLK_DOWN:
          snake->dir = DIR_DOWN;
          break;
        case SDLK_LEFT:
          snake->dir = DIR_LEFT;
          break;
        case SDLK_RIGHT:
          snake->dir = DIR_RIGHT;
          break;
        // Pause game
        case SDLK_ESCAPE:
          is_pause = !is_pause;
          changeWindowTitle(window, snake->length - 1, is_pause);
          break;
        default:
          break;
        }
        break;
      default:
        break;
      }
    }

    if(!is_pause) {
      if(tailContains(snake, &snake->body[snake->length - 1])) {
        return;
      }

      if(eatApple(snake, apple)) {
        changeWindowTitle(window, snake->length - 1, false);
      } else {
        moveSnake(snake);
      }
    }

    drawFrame(renderer, snake, apple);
    SDL_Delay(1000 / FPS - SDL_GetTicks() + previous_time);
  }
}
