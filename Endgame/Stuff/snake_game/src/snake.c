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

#include <stdlib.h>
#include <stdbool.h>

#include "snake.h"
#include "config.h"

static bool isHead(const struct snake* snake, const struct segment* segment) {
  const struct segment head = snake->body[snake->length - 1];
  return head.x == segment->x && head.y == segment->y;
}

static inline void circle_increment(size_t* n, size_t min, size_t max) {
  *n = *n >= max ? min : *n + 1;
}

static inline void circle_decrement(size_t* n, size_t min, size_t max) {
  *n = *n <= min ? max : *n - 1;
}

static void moveHead(struct snake* snake) {
  struct segment* head = &snake->body[snake->length - 1];

  switch(snake->dir) {
    case DIR_UP:
      circle_decrement(&head->y, 0, FIELD_HEIGHT - 1);
      break;
    case DIR_LEFT:
      circle_decrement(&head->x, 0, FIELD_WIDTH - 1);
      break;
    case DIR_DOWN:
      circle_increment(&head->y, 0, FIELD_HEIGHT - 1);
      break;
    case DIR_RIGHT:
      circle_increment(&head->x, 0, FIELD_WIDTH - 1);
      break;
  }
}

static void moveTail(struct snake* snake) {
  for(size_t i = 0; i < snake->length - 1; ++i) {
    snake->body[i] = snake->body[i + 1];
  }
}

static void moveApple(const struct snake* snake, struct segment* apple) {
  do {
    apple->x = rand() % FIELD_WIDTH;
    apple->y = rand() % FIELD_HEIGHT;
  } while(tailContains(snake, apple) || isHead(snake, apple));
}

void moveSnake(struct snake* snake) {
  moveTail(snake);
  moveHead(snake);
}

bool eatApple(struct snake* snake, struct segment* apple) {
  if(!isHead(snake, apple)) {
    return false;
  }

  snake->body = realloc(snake->body, ++snake->length * sizeof(struct segment));
  snake->body[snake->length - 1] = snake->body[snake->length - 2];
  moveHead(snake);

  moveApple(snake, apple);

  return true;
}

bool tailContains(const struct snake* snake, const struct segment* segment) {
  for(size_t i = 0; i < snake->length - 1; ++i) {
    if(segment->x == snake->body[i].x && segment->y == snake->body[i].y) {
      return true;
    }
  }
  return false;
}
