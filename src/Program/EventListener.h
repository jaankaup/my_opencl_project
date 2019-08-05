#ifndef EVENTLISTENER
#define EVENTLISTENER

#include "InputCache.h"

class EventListener
{

  public: 

    uint32_t register_keydown(const std::function<void(const InputCache*)>& function) {};

    uint32_t register_keyup(const std::function<void(const InputCache*)>& function) {};

    uint32_t register_mouseMotion(const std::function<void(const InputCache*)>& function) {};

    uint32_t register_mouseDown(const std::function<void(const InputCache*)>& function) {};

    uint32_t register_mouseUp(const std::function<void(const InputCache*)>& function) {};

    uint32_t register_quit(const std::function<void(const InputCache*)>& function) {};

    uint32_t register_windowResized(const std::function<void(const InputCache*)>& function) {};

}


#endif
