//
// Created by Trebing, Peter on 2019-08-27.
//
#include "controller.h"
#include "SDL.h"

void Controller::HandleInput(bool &running, KeyState &keys) {

    SDL_Event e;
    while (SDL_PollEvent(&e)) {

        switch (e.type) {

            case SDL_QUIT:
                running = false;
                break;

            case SDL_KEYDOWN :
                switch (e.key.keysym.sym) {
                    case SDLK_PLUS:
                        keys.plus = true;
                        //SDL_Log("+ button was pressed!");
                        break;
                    case SDLK_MINUS:
                        keys.minus = true;
                        //SDL_Log("- button was pressed!");
                        break;
                    case SDLK_h:
                        keys.heat = true;
                        //SDL_Log("SPACE button was pressed!");
                        break;
                    case SDLK_c:
                        keys.cool = true;
                        //SDL_Log("SPACE button was pressed!");
                        break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                switch (e.button.button) {
                    case SDL_BUTTON_LEFT:
                        //SDL_Log("Left button was pressed!");
                        break;
                    case SDL_BUTTON_RIGHT:
                        //SDL_Log("Right button was pressed!");
                        break;
                    default:
                        //SDL_Log("Some other button was pressed!");
                        break;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                switch (e.button.button) {
                    case SDL_BUTTON_LEFT:
                        // SDL_Log("Left button was up!");
                        break;
                    case SDL_BUTTON_RIGHT:
                        //SDL_Log("Right button was up!");
                        break;
                    default:
                        //SDL_Log("Some other button was up689#äöljasdf klö'*Äüp0o76541!");
                        break;
                }
                break;

            case SDL_MOUSEMOTION:
                int mouseX = e.motion.x;
                int mouseY = e.motion.y;
                //SDL_Log("Mouse at position %d %d \n", mouseX, mouseY);
                break;
        }

    }

}