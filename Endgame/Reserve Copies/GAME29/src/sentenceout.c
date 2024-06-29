#include "header.h"

void sentenceout(char* buf)
{
	const char *start = buf;
	const char *end = buf;
	char sentence[1000]; // Максимальная длина предложения
	int index = 0;

	while (*end)
	{
		if (*end == '.' || *end == '!' || *end == '?') 
		{
			while (start <= end) {
				sentence[index++] = *start++;
			}
			sentence[index] = '\0';
			TextSurface = TTF_RenderText_Blended_Wrapped(gFont, sentence, (SDL_Color){255,255,0,255}, 500);
			if (TextSurface == NULL) {
				printf("Surface creation failed: %s\n", TTF_GetError());
				exit(0);
			}
			TextTexture = SDL_CreateTextureFromSurface(renderer, TextSurface);
			if (TextSurface == NULL) {
				printf("Texture creation failed: %s\n", TTF_GetError());
				exit(0);
			}

			// Отображаем текстуру
			int textWidth = TextSurface->w;
			int textHeight = TextSurface->h;

			// Отображаем текстуру
			SDL_RenderCopy(renderer, TextTexture, NULL, &(SDL_Rect){0, 0, textWidth, textHeight});

			SDL_FreeSurface(TextSurface);

			// Уничтожаем текстуру
			SDL_DestroyTexture(TextTexture);

			SDL_Texture* cuda = loadTexture("resources/cuda.png");

			SDL_Rect cudarect = {700, 0, 100, 100};

			int x,y;

			SDL_Event event;

			SDL_RenderCopy(renderer, cuda, NULL, &cudarect);
			SDL_RenderPresent(renderer);

			int quit = 1;
			while (quit)
			{
				while (SDL_PollEvent(&event))
				{
					switch (event.type)
					{
						case SDL_QUIT:
						SDL_RenderClear(renderer);
						exit(1);
						case SDL_MOUSEBUTTONDOWN:
						x = event.button.x;
						y = event.button.y;
						if (x >= cudarect.x && x <= cudarect.x + cudarect.w && y >= cudarect.y && y <= cudarect.y + cudarect.h)
						{  
							quit = 0;
						}
					}
				}
			}
			SDL_RenderClear(renderer);
			// Задержка перед отображением следующей реплики (в миллисекундах)
			//SDL_Delay(2000);

			index = 0;
			while (*end && isspace(*++end));
			start = end;
		} else {
			++end;
		}
	}
}

