#ifndef INPUTCACHE_H
#define INPUTCACHE_H

#include <vector>
#include <functional>
#include <optional>
#include <variant>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "../Utils/log.h"

enum class EventType { UNKNOW, KEYBOARD_MOUSE };

// Forward declaration.
class InputCache;

typedef std::function<void(const InputCache*)> ICF;
typedef std::variant<ICF, void (*)(const InputCache*) > InputCache_Function;

class RegisteredFunction
{
  public:

    // Constructor.
    RegisteredFunction() { pId = nextId++; }// getNextId(); }

    // This object can't be copied.
    RegisteredFunction(const RegisteredFunction&) = delete;
    RegisteredFunction& operator=(const RegisteredFunction&) = delete;

    // Only moving is allowed.
    RegisteredFunction(RegisteredFunction&&) = default;
    RegisteredFunction& operator=(RegisteredFunction&&) = default;

    // Add a lambda function. This moves the given function.
    // After calling this funktion it's not possible to add any other functin.
    void add_lambda_function(ICF&& function)
    {
      Log::getDebug().log("InputCache::add_lambda_function");
      if (initialized) return; // TODO: assert
   
      InputCache_Function icf;
      icf = function;
      pIcf = std::move(icf);

      initialized = true;
    }

    // Add a function-pointer. One can only register one funktion.
    void add_function_pointer(void (*function_pointer)(const InputCache*))
    {
      Log::getDebug().log("RegisteredFunction::add_function_pointer.");
      if (initialized) return; // TODO: assert

      InputCache_Function icf;
      icf = function_pointer;

      pIcf = std::move(icf);
      initialized = true;
    }

    // Get the id.
    uint32_t getId() { return pId; } 

    // Call the function.
    void callFunction(InputCache* ic)
    { 
      if (auto f (std::get_if<ICF>(&pIcf)); f)
      {
        Log::getDebug().log("InputCache::pollEvents: calling lambda %", f);
        (*f)(ic);
      }
      else if (auto f (std::get_if<void (*)(const InputCache*)> (&pIcf)); f)
      {
        Log::getDebug().log("InputCache::pollEvents: calling pointer to function");
        (*(*f))(ic);
      }
    }

  private:
    InputCache_Function pIcf;
    uint32_t pId = 0;

    bool initialized = false;
    inline static uint32_t nextId = 0; 
    //uint32_t getNextId() { return nextId++; }
};

class InputCache
{
  public:

    static InputCache* getInstance() { static InputCache eh; return &eh; };

    InputCache(const InputCache&) = delete;
    InputCache(InputCache&&) = delete;
    InputCache& operator=(const InputCache&) = delete;
    InputCache& operator=(InputCache&&) = delete;

    bool init();

    void pollEvents();

    // Test function.
    bool whatsUpMan() const;

//    ICF* register_function(const EventType et, ICF* function); // -> decltype(&function)
//    ICF* register_function(const EventType et, ICF* function); // -> decltype(&function)
    uint32_t register_lambda_function(const EventType et, ICF&& lambda_f); // -> decltype(function);
    uint32_t register_function_pointer(const EventType et, void (*function_pointer)(const InputCache*)); // -> decltype(function);
    //ICF* register_function(const EventType et, ICF&& function); // -> decltype(function);

    // Register function for quit event.
    uint32_t register_quit(const ICF& function);

    // Register function for window resize.
    uint32_t register_resize(const ICF& function);

    // Remove an event listener function.
    bool unregister(const uint32_t id);

    bool isKeyDown(const uint32_t key) const;
    bool isKeyUp(const uint32_t key) const;

    int get_screenWidth() const;
    int get_screenHeight() const;

    const uint8_t* get_keyboardState() const;

    uint32_t get_timeDelta() const;


    ~InputCache() {};

  private:

    InputCache() {pJoo.reserve(1000); };
    static uint32_t nextId;

    // Prepare a new state for input. not implemented.
    void cacheInput();

    void updateTick();

    // Registered functions for KeyboardMouse event.
///    std::vector<ICF&& > pKeyMouse_ptrs;
    //std::vector<ICF> pKeyMouse;
    std::vector<ICF*> pKeyMouse_ptr;
    //std::vector<InputCache_Function> pJoo;
    std::vector<RegisteredFunction> pJoo;
   
//    std::vector<InputCache_Function> pKeyMouse;

    //std::unordered_map<const ICF*,ICF> pKeyMouse;
    std::unordered_map<int,ICF> pQuit;
    std::unordered_map<int,ICF> pResize;

    // Next id.
    uint32_t pNext_Id = 0;

    int pScreenWidth = 1024;
    int pScreenHeight = 800;

    // TODO: something more clever. Now only for testing.
    std::vector<SDL_Event> pKeyDownEvents;
    std::vector<SDL_Event> pKeyUpEvents;
    std::vector<SDL_Event> pMouseMotionEvents;
    std::vector<SDL_Event> pMouseButtonDown;
    std::vector<SDL_Event> pMouseButtonUp;
    std::vector<SDL_Event> pWindowEvents;
    std::vector<SDL_Event> pQuitEvent;

    uint32_t pTicks_prev = 0;
    uint32_t pTicks_now = 0;

    glm::vec2 pMousePos_prev = glm::vec2(0.0f);
    glm::vec2 pMousePos_now = glm::vec2(0.0f);
};

    //uint32_t registerKeysum(std::function<void(const uint8_t*)> rFunction);
#endif
