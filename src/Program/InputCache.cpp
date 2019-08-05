#include "InputCache.h"
#include "../Utils/Helper.h"
#include "../Utils/static_string.h"

/////////////////////////////////////////////////////////////////////////////////////

bool InputCache::init()
{
  return Helper::initSDL();
}

/////////////////////////////////////////////////////////////////////////////////////

void InputCache::pollEvents()
{
    pQuitEvent.clear();
    pKeyDownEvents.clear();
    pKeyUpEvents.clear();
    pMouseMotionEvents.clear();
    pMouseButtonDown.clear();
    pMouseButtonUp.clear();
    pWindowEvents.clear();

    updateTick();

    SDL_Event e;

    while (SDL_PollEvent(&e)) {

        switch (e.type) {

        case SDL_QUIT: pQuitEvent.push_back(e); break;
        case SDL_KEYDOWN: pKeyDownEvents.push_back(e); break;
        case SDL_KEYUP: pKeyUpEvents.push_back(e); break;
        case SDL_MOUSEMOTION: pMouseMotionEvents.push_back(e); break;
        case SDL_MOUSEBUTTONDOWN: pMouseButtonDown.push_back(e); break;
        case SDL_MOUSEBUTTONUP: pMouseButtonUp.push_back(e); break;
        case SDL_WINDOWEVENT: pWindowEvents.push_back(e); break;
        default: break;
        }
    }

    // The window events.
    if (pWindowEvents.size() > 0) {

      auto event = pWindowEvents[0];
      if (event.type == SDL_WINDOWEVENT_RESIZED) {
        pScreenWidth = event.window.data1; pScreenHeight = event.window.data2; 
        for (const auto& f : pResize) f.second(this);
      }
    }

    SDL_PumpEvents();

    if (pMouseMotionEvents.size() > 0 || pMouseButtonDown.size() > 0 || pMouseButtonUp.size() > 0 || pKeyDownEvents.size() > 0 || pKeyUpEvents.size()  > 0)
    {
      // Awake registered functions for mouse/keyboard.
      for (auto ic : pKeyMouse)
      {
        if (auto val = ic.f; val) { (*val)(this); } 
        else if (auto val = ic.f_ptr; val) { (*(*val))(this); } 
      }
    }

    if (pQuitEvent.size() > 0)
    {
      // Awake registered functions for quit.
      for (const auto& f : pQuit) f.second(this);
    }
}

/////////////////////////////////////////////////////////////////////////////////////

std::function<void(const InputCache*)>* InputCache::register_function(const EventType et, std::function<void(const InputCache*)>&& function) // -> decltype(&function)
{
   
  IC_Function ic;
  ic.f = std::make_optional(std::move(function));
  //ic.e_type = EventType::KEYBOARD_MOUSE;
  //pRegisteredFunctions.push_back(std::move(ic));
  switch (et) {
    case EventType::KEYBOARD_MOUSE: pKeyMouse.push_back(std::move(ic));
  };
  
  if (auto val = ic.f; val) { return &(*val); } 
  else return nullptr; // This should never happen! 

  ////pRegisteredFunctions.push_back(std::move(ic));
//  pKeyMouse.push_back(std::move(function));
//  auto ptr = &pKeyMouse.back();
//  pKeyMouse_ptr.push_back(ptr);
//  return ptr;
}

/////////////////////////////////////////////////////////////////////////////////////

std::function<void(const InputCache*)>* InputCache::register_function(const EventType et, std::function<void(const InputCache*)>* function) // -> decltype(&function)
{
   Log::getDebug().log("InputCache::register_kyboardMouse: %", function);
   //assert(et == EventType::UNKNOW);
   //std::cout << type_name<decltype(function)>() << std::endl;
   //pKeyMouse.push_back(std::move(function));
   IC_Function ic;
   ic.f_ptr = std::make_optional(function);
   switch (et) {
     case EventType::KEYBOARD_MOUSE: pKeyMouse.push_back(std::move(ic));
   };
   return function;
}

/////////////////////////////////////////////////////////////////////////////////////

uint32_t InputCache::register_quit(const std::function<void(const InputCache*)>& function)
{
  Log::getDebug().log("InputCache::register_quit: %", &function);
  pQuit.insert({pNext_Id, function});
  return pNext_Id++;
}

/////////////////////////////////////////////////////////////////////////////////////

uint32_t InputCache::register_resize(const std::function<void(const InputCache*)>& function)
{
  Log::getDebug().log("InputCache::register_resize: %", &function);
  pResize.insert({pNext_Id, function});
  return pNext_Id++;
}

bool InputCache::unregister(std::function<void(const InputCache*)>* ptr)
{
  for (int i=0; i<pKeyMouse.size() ; i++)
  {
    if (pKeyMouse_ptr[i] == ptr)
    {
      pKeyMouse_ptr.erase(pKeyMouse_ptr.begin() + i);
      for (auto x : pKeyMouse)
      {
        
      }
      return true;
    }
  }
  return false;

}
/////////////////////////////////////////////////////////////////////////////////////

bool InputCache::whatsUpMan() const
{
    // Print the number of events for debugging.
    Log::getDebug().log("QUIT: %", pQuitEvent.size());
    Log::getDebug().log("SDL_KEYDOWN: %", pKeyDownEvents.size());
    Log::getDebug().log("SDL_KEYUP: %", pKeyUpEvents.size());
    Log::getDebug().log("SDL_MOUSEMOTION: %", pMouseMotionEvents.size());
    Log::getDebug().log("SDL_MOUSEBUTTONDOWN: %", pMouseButtonDown.size());
    Log::getDebug().log("SDL_MOUSEBUTTONUP: %", pMouseButtonUp.size());
    Log::getDebug().log("SDL_WINDOWEVENT: %", pWindowEvents.size());
    
    // If there are quit events, return false.
    if (pQuitEvent.size() > 0) return false;

    // Else return true.
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////

int InputCache::get_screenWidth() const
{
    return pScreenWidth;
}

/////////////////////////////////////////////////////////////////////////////////////

int InputCache::get_screenHeight() const
{
    return pScreenHeight;
}

/////////////////////////////////////////////////////////////////////////////////////

bool InputCache::isKeyDown(const uint32_t key) const
{
  if (pKeyDownEvents.size() > 0) {
    if (pKeyDownEvents[0].key.keysym.sym == key) {
      Log::getDebug().log("Yeah");
      return true;
    }
  }
  Log::getDebug().log("Not Yeah");
  return false;
}

/////////////////////////////////////////////////////////////////////////////////////

bool InputCache::isKeyUp(const uint32_t key) const
{
  if (pKeyUpEvents.size() > 0) {
    if (pKeyUpEvents[0].key.keysym.sym == key) {
      Log::getDebug().log("Yeah");
      return true;
    }
  }
  Log::getDebug().log("Not Yeah");
  return false;
}

/////////////////////////////////////////////////////////////////////////////////////

const uint8_t* InputCache::get_keyboardState() const
{
    return SDL_GetKeyboardState(NULL);
}

/////////////////////////////////////////////////////////////////////////////////////

void InputCache::updateTick()
{
  pTicks_prev = pTicks_now;
  pTicks_now = SDL_GetTicks();
}

/////////////////////////////////////////////////////////////////////////////////////

uint32_t InputCache::get_timeDelta() const
{
  return pTicks_now - pTicks_prev;
  //auto del = pTicks_now - pTicks_prev;
  //if (del != 0)
  //{
  //  return float(newTick)/float(pTick);
  //}
  //return 0.0f;
}

/////////////////////////////////////////////////////////////////////////////////////

