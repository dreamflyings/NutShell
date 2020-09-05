#include "common.h"

#include <SDL2/SDL.h>

//#define SHOW_SCREEN

#define SCREEN_PORT 0x100 // Note that this is not the standard
#define SCREEN_MMIO 0x4100
#define SCREEN_H 600
#define SCREEN_W 800

static uint32_t vmem[800 * 600];

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;

extern "C" void put_pixel(uint32_t pixel) {
  static int i = 0;
  vmem[i++] = pixel;
  if (i >= 800 * 600) i = 0;
}

extern "C" void vmem_sync(void) {
#ifndef SHOW_SCREEN
  return;
#endif
  SDL_UpdateTexture(texture, NULL, vmem, SCREEN_W * sizeof(uint32_t));
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

void init_sdl() {
#ifndef SHOW_SCREEN
  return;
#endif
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(SCREEN_W, SCREEN_H, 0, &window, &renderer);
  SDL_SetWindowTitle(window, "NutCore");
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_STATIC, SCREEN_W, SCREEN_H);
}
