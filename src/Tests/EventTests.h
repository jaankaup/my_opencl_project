#ifndef EVENTTESTS
#define EVENTTESTS

#include "InputCache.h"

void addTestEvents() {
  Window window = Window::getInstance();
  Window::getInstance().setTitle("Juupajooo");

  auto eh = InputCache::getInstance();
  eh->init();

    ic->register_lambda_function(EventType::KEYBOARD_MOUSE,
              [](const InputCache* c) {  

              if (c->isMousePressed(SDL_BUTTON_LEFT)) Log::getDebug().log("Left mouse pressed.");
              if (c->isMousePressed(SDL_BUTTON_MIDDLE)) Log::getDebug().log("Middle mouse pressed.");
              if (c->isMousePressed(SDL_BUTTON_RIGHT)) Log::getDebug().log("Right mouse pressed.");

              if (c->isMouseReleased(SDL_BUTTON_LEFT)) Log::getDebug().log("Left mouse released.");
              if (c->isMouseReleased(SDL_BUTTON_MIDDLE)) Log::getDebug().log("Middle mouse released.");
              if (c->isMouseReleased(SDL_BUTTON_RIGHT)) Log::getDebug().log("Right mouse released.");

              if (c->isMouseDown(SDL_BUTTON_LEFT)) Log::getDebug().log("Left mouse down.");
              if (c->isMouseDown(SDL_BUTTON_MIDDLE)) Log::getDebug().log("Middle mouse down.");
              if (c->isMouseDown(SDL_BUTTON_RIGHT)) Log::getDebug().log("Right mouse down.");

              if (c->isMouseMoving()) Log::getDebug().log("Mouse is moving.");

              if (c->isMouseReleased(SDL_BUTTON_LEFT)) Log::getDebug().log("Left mouse delta == %.",c->getButtonDown_time(SDL_BUTTON_LEFT));
              if (c->isMouseReleased(SDL_BUTTON_MIDDLE)) Log::getDebug().log("Middle mouse delta == %.",c->getButtonDown_time(SDL_BUTTON_MIDDLE));
              if (c->isMouseReleased(SDL_BUTTON_RIGHT)) Log::getDebug().log("Right mouse delta == %.",c->getButtonDown_time(SDL_BUTTON_RIGHT));

              });
}

#endif
