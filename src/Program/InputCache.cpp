#include "InputCache.h"
#include "../Utils/Helper.h"
#include "../Utils/static_string.h"

//uint32_t RegisteredFunction::nextId = 0;

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

      //for (const auto& icf : pJoo)
      for (int i=0; i<pJoo.size() ; i++)
      {
        pJoo[i].callFunction(this);
        //std::invoke(f,this);
//        if (auto f (std::get_if<ICF>(&pJoo[i])); f)
//        {
//          Log::getDebug().log("InputCache::pollEvents: calling lambda %", f);
//        //  //(*f)(this);
//          (*f)(this);
//        }
//        else if (auto f (std::get_if<void (*)(const InputCache*)> (&pJoo[i])); f)
//        {
//          Log::getDebug().log("InputCache::pollEvents: calling pointer to function");
//          (*(*f))(this);
//        }
      }

      // Awake registered functions for mouse/keyboard.
      //for (auto ic : pKeyMouse)
      //{
      //  if (auto val = ic.f; val) { (*val)(this); } 
      //  else if (auto val = ic.f_ptr; val) { (*(*val))(this); } 
      //}
    }

    if (pQuitEvent.size() > 0)
    {
      // Awake registered functions for quit.
      for (const auto& f : pQuit) f.second(this);
    }
}

/////////////////////////////////////////////////////////////////////////////////////

//ICF* InputCache::register_function(const EventType et, const ICF function)
//{
//   Log::getDebug().log("InputCache::register_kyboardMouse by ICF function");
//   return nullptr;
//}

/////////////////////////////////////////////////////////////////////////////////////

uint32_t InputCache::register_lambda_function(const EventType et, ICF&& lambda_function) // -> decltype(&function)
{
   Log::getDebug().log("InputCache::register_kyboardMouse by ICF&& function");

   RegisteredFunction rf;
   rf.add_lambda_function(std::move(lambda_function));

   auto id = rf.getId();
   
   pJoo.push_back(std::move(rf));

   return id;
   
//  InputCache_Function icf;
//  icf = std::move(lambda_function);
//  switch (et) {
//    case EventType::KEYBOARD_MOUSE: pJoo.push_back(std::move(icf));
//    //case EventType::KEYBOARD_MOUSE: pKeyMouse.push_back(std::move(icf));
//  };
//
//        if (auto f (std::get_if<ICF>(&pJoo[pJoo.size()-1])); f)
//        {
//          Log::getDebug().log("InputCache::pollEvents: calling lambda now!!! %", f);
//        //  //(*f)(this);
//          (*f)(this);
//        }
//
//  if (auto f (std::get_if<ICF>(&pJoo[pJoo.size()-1])); f) { Log::getError().log("iiiiiiiiiiiiiiiiiiik"); return f; }
//  else{Log::getError().log("AAAAAAAAAAAAAAAAAAAAARRRRGGGHHHH");} 
//  //return &pJoo.back(); //return nullptr; // This should never happen! 
//  //return nullptr;
  
}

/////////////////////////////////////////////////////////////////////////////////////

uint32_t InputCache::register_function_pointer(const EventType et, void (*function_pointer)(const InputCache*) )
{
   Log::getDebug().log("InputCache::register_function_pointer");

   RegisteredFunction rf;
   rf.add_function_pointer(function_pointer);

   auto id = rf.getId();
   
   pJoo.push_back(std::move(rf));

   return id;
//  Log::getDebug().log("InputCache::register_function_pointer: before.");
//
//      for (int i=0; i<pJoo.size() ; i++)
//      {
//        if (auto f (std::get_if<ICF>(&pJoo[i])); f)
//        {
//          Log::getDebug().log("pJoo[%] == %",i, f);
//        }
//        else if (auto f (std::get_if<void (*)(const InputCache*)> (&pJoo[i])); f)
//        {
//          Log::getDebug().log("pJoo[%] == %",i, f);
//        }
//      }
//  Log::getDebug().log("InputCache::register_kyboardMouse by ICF function pointer");
//  InputCache_Function icf;
//  icf = function_pointer;
//  //std::cout << type_name<decltype(function)>() << std::endl; 
//  switch (et) {
//    case EventType::KEYBOARD_MOUSE: pJoo.push_back(std::move(icf));
//    //case EventType::KEYBOARD_MOUSE: pJoo.push_back(std::move(icf));
//    //case EventType::KEYBOARD_MOUSE: pKeyMouse.push_back(std::move(icf));
//  };
//
//  Log::getDebug().log("InputCache::register_function_pointer: after.");
//      for (int i=0; i<pJoo.size() ; i++)
//      {
//        if (auto f (std::get_if<ICF>(&pJoo[i])); f)
//        {
//          Log::getDebug().log("pJoo[%] == %",i, f);
//        }
//        else if (auto f (std::get_if<void (*)(const InputCache*)> (&pJoo[i])); f)
//        {
//          Log::getDebug().log("pJoo[%] == %",i, f);
//        }
//      }
//  if (auto f (std::get_if<void (*)(const InputCache*)> (&pJoo.back())); f) return *f;
//  else return nullptr; // This should never happen.
}

/////////////////////////////////////////////////////////////////////////////////////

//ICF* InputCache::register_function(const EventType et, ICF* function) // -> decltype(&function)
//{
//   Log::getDebug().log("InputCache::register_kyboardMouse by ICF* function: %", function);
//    InputCache_Function icf;
//    icf = function;
//    switch (et) {
//      case EventType::KEYBOARD_MOUSE: pKeyMouse.push_back(std::move(icf));
//    };
//   return function;
//}

/////////////////////////////////////////////////////////////////////////////////////

uint32_t InputCache::register_quit(const ICF& function)
{
  Log::getDebug().log("InputCache::register_quit: %", &function);
  pQuit.insert({pNext_Id, function});
  return pNext_Id++;
}

/////////////////////////////////////////////////////////////////////////////////////

uint32_t InputCache::register_resize(const ICF& function)
{
  Log::getDebug().log("InputCache::register_resize: %", &function);
  pResize.insert({pNext_Id, function});
  return pNext_Id++;
}

/////////////////////////////////////////////////////////////////////////////////////

bool InputCache::unregister(const uint32_t id)
{
  Log::getDebug().log("InputCache::unregister %",id);
  for (int i=0; i<pJoo.size() ; i++)
  {
     if (pJoo[i].getId() == id)
     {
       Log::getDebug().log("InputCache::unregister: removed function.");
       pJoo.erase(pJoo.begin() + i); 
       return true;
     }
    
      //Log::getDebug().log("InputCache::unregister: removed lambda.");
      //pJoo.erase(pJoo.begin() + i);
      //return true;
//      if (auto f (std::get_if<ICF>(&pJoo[i])); f)
//      {
//        Log::getError().log("öhömöhhöm.");
//        Log::getDebug().log("i == % : f (%) == ptr (%) .",i, f, ptr);
//        if ( f == ptr ) {
//          pJoo.erase(pJoo.begin() + i);
//          Log::getDebug().log("InputCache::unregister: removed lambda.");
//          return true;
//        }
//      }
//      else if (auto f (std::get_if<void (*)(const InputCache*)>(&pJoo[i])); f)
//      {
//        //Log::getDebug().log("InputCache::unregister: removed pointer to function.");
//        //if ( (*f) == ptr ) pKeyMouse.erase(pKeyMouse.begin() + i);
//        return true;
//      }
    
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

