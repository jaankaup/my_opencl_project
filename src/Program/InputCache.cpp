#include "InputCache.h"
#include "../Utils/Helper.h"
#include "../Utils/static_string.h"

void RegisteredFunction::add_lambda_function(ICF&& function)
{
  Log::getDebug().log("InputCache::add_lambda_function");
  if (initialized) return; // TODO: assert

  InputCache_Function icf;
  icf = function;
  pIcf = std::move(icf);

  initialized = true;
}

/////////////////////////////////////////////////////////////////////////////////////

void RegisteredFunction::add_function_pointer(void (*function_pointer)(const InputCache*))
{
  Log::getDebug().log("RegisteredFunction::add_function_pointer.");
  if (initialized) return; // TODO: assert

  InputCache_Function icf;
  icf = function_pointer;

  pIcf = std::move(icf);
  initialized = true;
}

/////////////////////////////////////////////////////////////////////////////////////

void RegisteredFunction::callFunction(InputCache* ic)
{ 
  if (auto f (std::get_if<ICF>(&pIcf)); f)
  {
    //Log::getDebug().log("InputCache::pollEvents: calling lambda %", f);
    (*f)(ic);
  }
  else if (auto f (std::get_if<void (*)(const InputCache*)> (&pIcf)); f)
  {
    //Log::getDebug().log("InputCache::pollEvents: calling pointer to function");
    (*(*f))(ic);
  }
}

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
        case SDL_KEYUP: pKeyUpEvents.push_back(e);break;
        case SDL_MOUSEMOTION: pMouseMotionEvents.push_back(e); break;
        case SDL_MOUSEBUTTONDOWN: pMouseButtonDown.push_back(e); break;
        case SDL_MOUSEBUTTONUP: pMouseButtonUp.push_back(e); break;
        case SDL_WINDOWEVENT: pWindowEvents.push_back(e); break;
        //default: break;
        }
    }

    handle_window_events();
    handle_keyboad_mouse();

    // The window events.
//    if (pWindowEvents.size() > 0) {
//      //Log::getDebug().log("Windows eventtejä == % ", pWindowEvents.size());
//
//      handle_window_events();
//    }

    //SDL_PumpEvents();

    //if (pKeyDownEvents.size() > 0 ) Log::getDebug().log("Keydown eventtejä(1) == % ", pKeyDownEvents.size());
    //if (pKeyUpEvents.size() > 0 ) Log::getDebug().log("Keyup eventtejä(1) == % ", pKeyUpEvents.size());
    //Log::getDebug().log("Mousemotion eventtejä == % ", pMouseMotionEvents.size());
    //Log::getDebug().log("MouseButtonDown eventtejä == % ", pMouseButtonDown.size());
    //Log::getDebug().log("MouseButtonUp eventtejä == % ", pMouseButtonUp.size());


    if (pQuit.size() > 0)
    {
      // Awake registered functions for quit.
      for (auto& f : pQuit) f.callFunction(this);
    }
}

/////////////////////////////////////////////////////////////////////////////////////

void InputCache::handle_keyboad_mouse()
{
    if (pMouseMotionEvents.size() > 0 || pMouseButtonDown.size() > 0 || pMouseButtonUp.size() > 0 || pKeyDownEvents.size() > 0 || pKeyUpEvents.size()  > 0)
    {
      for (int i=0; i<pKeyboardMouse.size() ; i++) pKeyboardMouse[i].callFunction(this);
    }
}

/////////////////////////////////////////////////////////////////////////////////////

void InputCache::handle_window_events()
{
   for (const auto& event : pWindowEvents)
   {
     if (event.type == SDL_WINDOWEVENT) {
          switch (event.window.event) {
          case SDL_WINDOWEVENT_SHOWN:
              //Log::getDebug().log("Window % shown", event.window.windowID);
              break;
          case SDL_WINDOWEVENT_HIDDEN:
              //Log::getDebug().log("Window % hidden", event.window.windowID);
              break;
          case SDL_WINDOWEVENT_EXPOSED:
              //Log::getDebug().log("Window % exposed", event.window.windowID);
              break;
          case SDL_WINDOWEVENT_MOVED:
              //Log::getDebug().log("Window % moved to %,%",
              //        event.window.windowID, event.window.data1,
              //        event.window.data2);
              break;
          case SDL_WINDOWEVENT_RESIZED:
              //Log::getDebug().log("Window % resized to %x%",
              //        event.window.windowID, event.window.data1,
              //        event.window.data2);
              break;
          case SDL_WINDOWEVENT_SIZE_CHANGED:
              //Log::getDebug().log("Window % size changed to %x%",
              //        event.window.windowID, event.window.data1,
              //        event.window.data2);

              pScreenWidth = event.window.data1;
              pScreenHeight = event.window.data2; 
              for (auto& f : pResize) f.callFunction(this);

              break;
          case SDL_WINDOWEVENT_MINIMIZED:
              //Log::getDebug().log("Window % minimized", event.window.windowID);
              break;
          case SDL_WINDOWEVENT_MAXIMIZED:
              //Log::getDebug().log("Window % maximized", event.window.windowID);
              break;
          case SDL_WINDOWEVENT_RESTORED:
              //Log::getDebug().log("Window % restored", event.window.windowID);
              break;
          case SDL_WINDOWEVENT_ENTER:
              //Log::getDebug().log("Mouse entered window %",
              //        event.window.windowID);
              break;
          case SDL_WINDOWEVENT_LEAVE:
              //Log::getDebug().log("Mouse left window %", event.window.windowID);
              //break;
          case SDL_WINDOWEVENT_FOCUS_GAINED:
              //Log::getDebug().log("Window % gained keyboard focus",
              //        event.window.windowID);
              break;
          case SDL_WINDOWEVENT_FOCUS_LOST:
              //Log::getDebug().log("Window % lost keyboard focus",
              //        event.window.windowID);
              break;
          case SDL_WINDOWEVENT_CLOSE:
              //Log::getDebug().log("Window % closed", event.window.windowID);
              break;
  #if SDL_VERSION_ATLEAST(2, 0, 5)
          case SDL_WINDOWEVENT_TAKE_FOCUS:
              //Log::getDebug().log("Window % is offered a focus", event.window.windowID);
              break;
          case SDL_WINDOWEVENT_HIT_TEST:
              //Log::getDebug().log("Window % has a special hit test", event.window.windowID);
              break;
  #endif
          default:
              SDL_Log("Window % got unknown event %",
                      event.window.windowID, event.window.event);
              break;
          }
      }
   }



}

/////////////////////////////////////////////////////////////////////////////////////

uint32_t InputCache::register_lambda_function(const EventType et, ICF&& lambda_function) // -> decltype(&function)
{
   Log::getDebug().log("InputCache::register_kyboardMouse by ICF&& function");

   RegisteredFunction rf;
   rf.add_lambda_function(std::move(lambda_function));

   auto id = rf.getId();
   
   switch (et) {
     case EventType::KEYBOARD_MOUSE: pKeyboardMouse.push_back(std::move(rf)); break;
     case EventType::RESIZE_EVENT: pResize.push_back(std::move(rf)); break;
     case EventType::QUIT: pQuit.push_back(std::move(rf)); break;
   }
//   pJoo.push_back(std::move(rf));

   return id;
   
}

/////////////////////////////////////////////////////////////////////////////////////

uint32_t InputCache::register_function_pointer(const EventType et, void (*function_pointer)(const InputCache*) )
{
   Log::getDebug().log("InputCache::register_function_pointer");

   RegisteredFunction rf;
   rf.add_function_pointer(function_pointer);

   auto id = rf.getId();
   
   switch (et) {
     case EventType::KEYBOARD_MOUSE: pKeyboardMouse.push_back(std::move(rf)); break;
     case EventType::RESIZE_EVENT: pResize.push_back(std::move(rf)); break;
     case EventType::QUIT: pQuit.push_back(std::move(rf)); break;
   }
   //pJoo.push_back(std::move(rf));

   return id;
}

/////////////////////////////////////////////////////////////////////////////////////

//uint32_t InputCache::register_quit(const ICF& function)
//{
//  Log::getDebug().log("InputCache::register_quit: %", &function);
//  pQuit.insert({pNext_Id, function});
//  return pNext_Id++;
//}

/////////////////////////////////////////////////////////////////////////////////////

//uint32_t InputCache::register_resize(const ICF& function)
//{
//  Log::getDebug().log("InputCache::register_resize: %", &function);
//  pResize.insert({pNext_Id, function});
//  return pNext_Id++;
//}

/////////////////////////////////////////////////////////////////////////////////////

bool InputCache::unregister(const uint32_t id)
{
  Log::getDebug().log("InputCache::unregister %",id);
  for (int i=0; i<pKeyboardMouse.size() ; i++)
  {
     if (pKeyboardMouse[i].getId() == id)
     {
       Log::getDebug().log("InputCache::unregister: removed function.");
       pKeyboardMouse.erase(pKeyboardMouse.begin() + i); 
       return true;
     }
  }
  Log::getDebug().log("InputCache::unregister: unregister failed.");
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
      return true;
    }
  }
  return false;
}

/////////////////////////////////////////////////////////////////////////////////////

bool InputCache::isKeyUp(const uint32_t key) const
{
  if (pKeyUpEvents.size() > 0) {
    if (pKeyUpEvents[0].key.keysym.sym == key) {
      return true;
    }
  }
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

