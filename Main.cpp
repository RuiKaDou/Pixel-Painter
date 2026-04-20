// pixel_painter.cpp
// A simple pixel art editor using SDL2 library.
// Compile: g++ -o pixel_painter pixel_painter.cpp -lSDL2

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cmath>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int CANVAS_SIZE = 400;
const int PIXEL_SIZE = 8; // 8x8 pixel blocks
const int GRID_SIZE = CANVAS_SIZE / PIXEL_SIZE; // 50x50 grid

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
std::vector<SDL_Color> palette;
int currentColor = 0;
bool drawing = false;
bool gridVisible = true;

// Canvas: each cell stores color index (0 = transparent/white)
std::vector<std::vector<int>> canvas(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));

// Predefined palette (16 colors)
void initPalette() {
    palette = {
        {255, 255, 255, 255}, // 0 White
        {0, 0, 0, 255},       // 1 Black
        {255, 0, 0, 255},     // 2 Red
        {0, 255, 0, 255},     // 3 Green
        {0, 0, 255, 255},     // 4 Blue
        {255, 255, 0, 255},   // 5 Yellow
        {255, 0, 255, 255},   // 6 Magenta
        {0, 255, 255, 255},   // 7 Cyan
        {128, 128, 128, 255}, // 8 Gray
        {255, 128, 0, 255},   // 9 Orange
        {128, 0, 128, 255},   // 10 Purple
        {0, 128, 0, 255},     // 11 Dark Green
        {128, 64, 0, 255},    // 12 Brown
        {255, 192, 203, 255}, // 13 Pink
        {173, 216, 230, 255}, // 14 Light Blue
        {240, 230, 140, 255}  // 15 Khaki
    };
}

void drawPixel(int x, int y, int colorIdx) {
    if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
        canvas[y][x] = colorIdx;
    }
}

void renderCanvas() {
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            SDL_Rect rect = {x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE};
            int idx = canvas[y][x];
            SDL_Color col = palette[idx];
            SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void drawGrid() {
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    for (int i = 0; i <= GRID_SIZE; ++i) {
        SDL_RenderDrawLine(renderer, i * PIXEL_SIZE, 0, i * PIXEL_SIZE, CANVAS_SIZE);
        SDL_RenderDrawLine(renderer, 0, i * PIXEL_SIZE, CANVAS_SIZE, i * PIXEL_SIZE);
    }
}

void drawUI() {
    // Color palette bar at the bottom
    int swatchSize = 40;
    int startX = (WINDOW_WIDTH - (palette.size() * swatchSize)) / 2;
    int startY = CANVAS_SIZE + 20;
    for (size_t i = 0; i < palette.size(); ++i) {
        SDL_Rect swatch = {startX + (int)i * swatchSize, startY, swatchSize, swatchSize};
        SDL_SetRenderDrawColor(renderer, palette[i].r, palette[i].g, palette[i].b, palette[i].a);
        SDL_RenderFillRect(renderer, &swatch);
        if ((int)i == currentColor) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &swatch);
        }
    }
    // Instructions
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // (Simplified: no text rendering for brevity, but we can use SDL_ttf for better. Here just visual)
}

void saveImage() {
    // Save canvas as BMP (simple, just for demo)
    SDL_Surface* surface = SDL_CreateRGBSurface(0, CANVAS_SIZE, CANVAS_SIZE, 32, 0, 0, 0, 0);
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            int idx = canvas[y][x];
            SDL_Color col = palette[idx];
            Uint32 pixel = SDL_MapRGB(surface->format, col.r, col.g, col.b);
            for (int py = 0; py < PIXEL_SIZE; ++py) {
                for (int px = 0; px < PIXEL_SIZE; ++px) {
                    ((Uint32*)surface->pixels)[(y*PIXEL_SIZE+py)*CANVAS_SIZE + (x*PIXEL_SIZE+px)] = pixel;
                }
            }
        }
    }
    SDL_SaveBMP(surface, "pixel_art.bmp");
    std::cout << "Saved as pixel_art.bmp" << std::endl;
    SDL_FreeSurface(surface);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    window = SDL_CreateWindow("Pixel Painter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    initPalette();

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mx, my;
                SDL_GetMouseState(&mx, &my);
                if (mx < CANVAS_SIZE && my < CANVAS_SIZE) {
                    drawing = true;
                    int gx = mx / PIXEL_SIZE;
                    int gy = my / PIXEL_SIZE;
                    drawPixel(gx, gy, currentColor);
                }
                // Check color swatch click
                int swatchSize = 40;
                int startX = (WINDOW_WIDTH - (palette.size() * swatchSize)) / 2;
                int startY = CANVAS_SIZE + 20;
                if (my > startY && my < startY + swatchSize) {
                    int idx = (mx - startX) / swatchSize;
                    if (idx >= 0 && idx < (int)palette.size()) currentColor = idx;
                }
            }
            else if (e.type == SDL_MOUSEMOTION && drawing) {
                int mx, my;
                SDL_GetMouseState(&mx, &my);
                if (mx < CANVAS_SIZE && my < CANVAS_SIZE && mx >= 0 && my >= 0) {
                    int gx = mx / PIXEL_SIZE;
                    int gy = my / PIXEL_SIZE;
                    drawPixel(gx, gy, currentColor);
                }
            }
            else if (e.type == SDL_MOUSEBUTTONUP) {
                drawing = false;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_g: gridVisible = !gridVisible; break;
                    case SDLK_s: saveImage(); break;
                    case SDLK_c: // clear canvas
                        for (auto& row : canvas) std::fill(row.begin(), row.end(), 0);
                        break;
                    case SDLK_ESCAPE: quit = true; break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);
        renderCanvas();
        if (gridVisible) drawGrid();
        drawUI();
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
