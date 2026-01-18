// обязательное
#define _WIN32_WINNT 0x0601
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <windows.h>

// собственное
#include "settings.hpp"
#include "draws.hpp"
#include "player.hpp"
#include "map.hpp"
#include "ray_casting.hpp"

// сторонние
#include "json.hpp"

using json = nlohmann::json;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Инициализация SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return -1;
    }
	if (TTF_Init() == -1) {
		SDL_Quit();
		return -1;
	}
	
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		SDL_Quit();
		TTF_Quit();
		return -1;
	}
	
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		SDL_Quit();
		TTF_Quit();
		IMG_Quit();
		return -1;
		
	}
	
	// Создание окна
    SDL_Window* window = SDL_CreateWindow(
        "Window :)",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        0
    );

    if (!window) {
        SDL_Quit();
		TTF_Quit();
		IMG_Quit();
		Mix_Quit();
        return -1;
    }

    // Создание рендерера
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
		TTF_Quit();
		IMG_Quit();
		Mix_Quit();
        return -1;
    }
	
    bool running = true;
    SDL_Event event;
	
	// переменные
	
	Player player;
	
	Uint32 start_time;
	Uint32 end_time;
	

    while (running) {
		start_time = SDL_GetTicks();
        // Обработка событий (цикл)
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Логика игры здесь

        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Очистка экрана (черный цвет)
        SDL_RenderClear(renderer);
        // Отрисовка здесь
		
		
		player.update(renderer);
		ray_casting(renderer, player.getPos(), player.getAngle());
        
        SDL_RenderPresent(renderer); // Обновление экрана
		
		end_time = SDL_GetTicks();
        if (end_time - start_time < 1000/FPS) SDL_Delay(1000/FPS - (end_time - start_time)); // Ограничение FPS
    }

    // Очистка ресурсов
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
    SDL_Quit();

    return 0;
}