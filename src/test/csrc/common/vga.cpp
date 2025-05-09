/***************************************************************************************
* Copyright (c) 2020-2023 Institute of Computing Technology, Chinese Academy of Sciences
* Copyright (c) 2020-2021 Peng Cheng Laboratory
*
* DiffTest is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include "vga.h"
#include "common.h"
#ifdef CONFIG_DIFFTEST_PERFCNT
#include "perf.h"
#endif // CONFIG_DIFFTEST_PERFCNT
#ifdef SHOW_SCREEN
#include <SDL2/SDL.h>

#define SCREEN_PORT 0x100 // Note that this is not the standard
#define SCREEN_MMIO 0x4100
#define SCREEN_H    600
#define SCREEN_W    800

static uint32_t vmem[800 * 600];

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;

void put_pixel(uint32_t pixel) {
#ifdef CONFIG_DIFFTEST_PERFCNT
  difftest_calls[perf_put_pixel]++;
  difftest_bytes[perf_put_pixel] += 4;
#endif // CONFIG_DIFFTEST_PERFCNT
  static int i = 0;
  vmem[i++] = pixel;
  if (i >= 800 * 600)
    i = 0;
}

void vmem_sync(void) {
#ifdef CONFIG_DIFFTEST_PERFCNT
  difftest_calls[perf_vmem_sync]++;
#endif // CONFIG_DIFFTEST_PERFCNT
  SDL_UpdateTexture(texture, NULL, vmem, SCREEN_W * sizeof(uint32_t));
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

void init_sdl() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(SCREEN_W, SCREEN_H, 0, &window, &renderer);
  SDL_SetWindowTitle(window, "NOOP");
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_W, SCREEN_H);
}

void finish_sdl() {
  memset(vmem, 0, sizeof(vmem));
}
#else
void put_pixel(uint32_t pixel) {}
void vmem_sync(void) {}
#endif
