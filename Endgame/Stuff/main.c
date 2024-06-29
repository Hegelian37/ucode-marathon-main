#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

// Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GAME_WIDTH = 800;
const int GAME_HEIGHT = 600;
const int PLAYER_SIZE = 50;
const int OBJECT_SIZE = 30; // Define the size for objects
const int TARGET_SIZE = 30;  // Define the size for the target

// Player structure
typedef struct {
    int worldX, worldY;  // Store player position in the game world
} Player;

typedef struct {
    int x, y;
} Camera;

// Target structure
typedef struct {
    int x, y;
} Target;

// Object structure with different types
typedef enum {
    RECTANGLE,
    LEVER,
    BOOK,
    ENEMY
} ObjectType;

typedef struct {
    int x, y;
    ObjectType type;
    int interacted;  // New flag to track whether the object has been interacted with
} Object;

// Text structure with a timer
typedef struct {
    SDL_Texture* texture;
    SDL_Rect rect;
    time_t startTime;  // Added variable to store the start time
} Text;

// Function to render text with a timer
Text renderTimedText(SDL_Renderer* renderer, TTF_Font* font, const char* message, SDL_Color color, time_t* startTime, int x, int y) {
    Text text;
    SDL_Surface* surface = TTF_RenderText_Solid(font, message, color);
    text.texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_QueryTexture(text.texture, NULL, NULL, &text.rect.w, &text.rect.h);

    // Set the position based on the target object with an offset
    text.rect.x = x - (text.rect.w / 2);
    text.rect.y = y - (text.rect.h / 2) - 30; // Adjust the offset as needed

    // Save the start time
    *startTime = time(NULL);

    return text;
}

void handleInteraction(Object* object, Text* interactionText, time_t* interactionStartTime, TTF_Font* font, SDL_Renderer* renderer) {
    if (object->interacted) {
        // Object has already been interacted with, skip further interactions
        return;
    }

    switch (object->type) {
        case RECTANGLE:
            // No special interaction for the rectangle
            break;
        case LEVER:
            // Interaction with the lever (example: increase a variable)
            printf("Interacted with a lever! Increase the variable.\n");
            break;
        case BOOK:
            // Interaction with the book (example: pause game, open an in-game window)
            printf("Interacted with a book! Pause the game, open in-game window.\n");
            break;
        case ENEMY:
            // Interaction with the enemy (example: show a message and cause game over)
            printf("Interacted with an enemy! Show message and cause game over.\n");
            break;
    }

    // Display interaction message
    SDL_Color textColor = {255, 0, 0, 255};
    *interactionText = renderTimedText(renderer, font, "Interaction!", textColor, interactionStartTime, object->x, object->y);

    // Set the interacted flag to true
    object->interacted = 1;
}

// Initialize SDL and create a window
SDL_Window* initSDL() {
    SDL_Window* window = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    window = SDL_CreateWindow("Simple 2D Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
    }

    return window;
}

    // Clean up SDL and exit the program
void closeSDL(SDL_Window* window) {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

    // Render text on the screen
    Text renderText(SDL_Renderer* renderer, TTF_Font* font, const char* message, SDL_Color color) {
    Text text;
    SDL_Surface* surface = TTF_RenderText_Solid(font, message, color);
    text.texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_QueryTexture(text.texture, NULL, NULL, &text.rect.w, &text.rect.h);
    text.rect.x = (SCREEN_WIDTH - text.rect.w) / 2;
    text.rect.y = (SCREEN_HEIGHT - text.rect.h) / 2;

    return text;
}

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Endgame Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

     // Declare and initialize renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        fprintf(stderr, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load font
    TTF_Font* font = TTF_OpenFont("resources/fonts/Branda.ttf", 20);  // Replace with your font file path
    if (font == NULL) {
        fprintf(stderr, "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    Camera camera = {0, 0};  // Initialize the camera at the top-left corner initially

    // Initialize player
    Player player = {SCREEN_WIDTH / 2 - PLAYER_SIZE / 2, SCREEN_HEIGHT / 2 - PLAYER_SIZE / 2};

    // Initialize text and timer
    Text interactionText;
    time_t interactionStartTime = 0;
    
    // Define the target
    Target target = {400, 0};

    // Define other objects
    Object objects[] = {
        {200, 200, RECTANGLE, 0},  // Example rectangle
        {600, 200, LEVER, 0},      // Example lever
        {200, 400, BOOK, 0},       // Example book
        {600, 400, ENEMY, 0},      // Example enemy
        // Add more objects as needed
    };

    // Main loop
    int quit = 0;
    SDL_Event e;
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        player.worldY -= 10;
                        break;
                    case SDLK_DOWN:
                        player.worldY += 10;
                        break;
                    case SDLK_LEFT:
                        player.worldX -= 10;
                        break;
                    case SDLK_RIGHT:
                        player.worldX += 10;
                        break;
                }
            }
        }

        // Keep the player within the screen borders
        if (player.worldX < 0) player.worldX = 0;
        if (player.worldX > GAME_WIDTH - PLAYER_SIZE) player.worldX = GAME_WIDTH - PLAYER_SIZE;
        if (player.worldY < 0) player.worldY = 0;
        if (player.worldY > GAME_HEIGHT - PLAYER_SIZE) player.worldY = GAME_HEIGHT - PLAYER_SIZE;

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        // Render objects
	for (size_t i = 0; i < sizeof(objects) / sizeof(objects[0]); ++i) {
	    SDL_Rect objectRect = {
		objects[i].x - camera.x + SCREEN_WIDTH / 2,
		objects[i].y - camera.y + SCREEN_HEIGHT / 2,
		OBJECT_SIZE, OBJECT_SIZE
	    };
	    SDL_RenderFillRect(renderer, &objectRect);
	}

	// Render target
	SDL_Rect targetRect = {target.x - camera.x + SCREEN_WIDTH / 2, target.y - camera.y + SCREEN_HEIGHT / 2, TARGET_SIZE, TARGET_SIZE};
	SDL_RenderFillRect(renderer, &targetRect);

	// Render player (centered on the screen)
	SDL_Rect playerRect = {SCREEN_WIDTH / 2 - PLAYER_SIZE / 2, SCREEN_HEIGHT / 2 - PLAYER_SIZE / 2, PLAYER_SIZE, PLAYER_SIZE};
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x0F, 0xFF);
	SDL_RenderFillRect(renderer, &playerRect);
        
        // Update camera position based on player movement
        camera.x = player.worldX - SCREEN_WIDTH / 2;
        camera.y = player.worldY - SCREEN_HEIGHT / 2;

        // Limit camera position to stay within the game world
        camera.x = (camera.x < 0) ? 0 : camera.x;
        camera.y = (camera.y < 0) ? 0 : camera.y;
        camera.x = (camera.x > GAME_WIDTH - SCREEN_WIDTH) ? GAME_WIDTH - SCREEN_WIDTH : camera.x;
        camera.y = (camera.y > GAME_HEIGHT - SCREEN_HEIGHT) ? GAME_HEIGHT - SCREEN_HEIGHT : camera.y;


        // Check for interaction with objects, including the target
        for (size_t i = 0; i < sizeof(objects) / sizeof(objects[0]); ++i) {
            if (player.worldX < objects[i].x + OBJECT_SIZE && player.worldX + PLAYER_SIZE > objects[i].x &&
                player.worldY < objects[i].y + OBJECT_SIZE && player.worldY + PLAYER_SIZE > objects[i].y) {
                // Handle interaction with the current object
                handleInteraction(&objects[i], &interactionText, &interactionStartTime, font, renderer);  // Pass renderer to handleInteraction
            }
        }

        // Check for interaction with the target
        Object targetObject = {target.x, target.y, RECTANGLE, 0};  // Assuming RECTANGLE is an appropriate type for the target
        if (player.worldX < targetObject.x + TARGET_SIZE && player.worldX + PLAYER_SIZE > targetObject.x &&
            player.worldY < targetObject.y + TARGET_SIZE && player.worldY + PLAYER_SIZE > targetObject.y) {
            // Handle interaction with the target
            handleInteraction(&targetObject, &interactionText, &interactionStartTime, font, renderer);  // Pass renderer to handleInteraction
        }

        // Draw the interaction text if within the timer limit
        if (time(NULL) - interactionStartTime < 6) {
            SDL_RenderCopy(renderer, interactionText.texture, NULL, &interactionText.rect);
        }

        // Update the screen
        SDL_RenderPresent(renderer);

        // Add a short delay to control the frame rate
        SDL_Delay(16);
    }
    
    // Clean up font
    TTF_CloseFont(font);
    TTF_Quit();
    
    // Clean up and exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

