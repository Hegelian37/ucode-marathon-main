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

#ifndef SNAKE_H
#define SNAKE_H

#include <stdbool.h>

struct segment {
  size_t x, y;
};

enum direction {
  DIR_UP,
  DIR_LEFT,
  DIR_DOWN,
  DIR_RIGHT
};

struct snake {
  struct segment* body;
  size_t length;
  enum direction dir;
};

void moveSnake(struct snake* snake);
bool eatApple(struct snake* snake, struct segment* apple);

bool tailContains(const struct snake* snake, const struct segment* segment);

#endif /* SNAKE_H */
