#include "controller.h"
#include <iostream>
#include "SDL.h"

void Controller::HandleInput(bool &running) const {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {

        switch (e.type) {

            case SDL_QUIT:
                running = false;
                break;

            case SDL_KEYDOWN :
                switch (e.key.keysym.sym) {
                    case SDLK_PLUS:
                        //ChangeDirection(snake, Snake::Direction::kUp,
                        //Snake::Direction::kDown);
                        break;
                    case SDLK_MINUS:
                        //ChangeDirection(snake, Snake::Direction::kUp,
                        //Snake::Direction::kDown);
                        break;
                    case SDLK_UP:
                        //ChangeDirection(snake, Snake::Direction::kUp,
                        //Snake::Direction::kDown);
                        break;

                    case SDLK_DOWN:
                        //ChangeDirection(snake, Snake::Direction::kDown,
                        //Snake::Direction::kUp);
                        break;

                    case SDLK_LEFT:
                        //ChangeDirection(snake, Snake::Direction::kLeft,
                        //Snake::Direction::kRight);
                        break;

                    case SDLK_RIGHT:
                        //ChangeDirection(snake, Snake::Direction::kRight,
                        //Snake::Direction::kLeft);
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
                /*
                    std::stringstream ss;
                    ss << "X: " << mouseX << " Y: " << mouseY;
                    SDL_SetWindowTitle(window, ss.str().c_str());
                */
                //SDL_Log("Mouse at position %d %d \n", mouseX, mouseY);
                break;
        }

    }


}