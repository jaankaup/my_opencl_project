#ifndef INPUTCACHE_H
#define INPUTCACHE_H

#include <vector>
#include <functional>
//#include <optional>
#include <variant>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "../Utils/log.h"

enum class EventType { UNKNOW, KEYBOARD_MOUSE, RESIZE_EVENT, QUIT};

// Forward declaration.
class InputCache;

typedef std::function<void(const InputCache*)> ICF;
typedef std::variant<ICF, void (*)(const InputCache*) > InputCache_Function;

class RegisteredFunction
{
  public:

    // Constructor.
    RegisteredFunction() { pId = nextId++; }

    // This object can't be copied.
    RegisteredFunction(const RegisteredFunction&) = delete;
    RegisteredFunction& operator=(const RegisteredFunction&) = delete;

    // Only moving is allowed.
    RegisteredFunction(RegisteredFunction&&) = default;
    RegisteredFunction& operator=(RegisteredFunction&&) = default;


    // Add a lambda function. This moves the given function.
    // After calling this funktion it's not possible to add any other functin.
    void add_lambda_function(ICF&& function);

    // Add a function-pointer. One can only register one funktion.
    void add_function_pointer(void (*function_pointer)(const InputCache*));

    // Get the id.
    uint32_t getId() { return pId; } 

    // Call the function.
    void callFunction(InputCache* ic);

  private:
    InputCache_Function pIcf;
    uint32_t pId = 0;

    bool initialized = false;
    inline static uint32_t nextId = 0; 
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

    uint32_t register_lambda_function(const EventType et, ICF&& lambda_f);
    uint32_t register_function_pointer(const EventType et, void (*function_pointer)(const InputCache*));

    void handle_keyboad_mouse();
    void handle_window_events();

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

    InputCache() {
      pKeyboardMouse.reserve(100);
      pResize.reserve(100);
      pQuit.reserve(100);
      pKeyDownEvents.reserve(5);
      pKeyUpEvents.reserve(5);
      pMouseMotionEvents.reserve(5);
      pMouseButtonDown.reserve(5);
      pMouseButtonUp.reserve(5);
      pWindowEvents.reserve(5);
      pQuitEvent.reserve(5);
    };
    static uint32_t nextId;

    // Prepare a new state for input. not implemented.
    void cacheInput();

    void updateTick();

    //std::vector<ICF*> pKeyMouse_ptr;
    std::vector<RegisteredFunction> pKeyboardMouse;
    std::vector<RegisteredFunction> pResize;
    std::vector<RegisteredFunction> pQuit;
   
//    std::vector<InputCache_Function> pKeyMouse;

    //std::unordered_map<const ICF*,ICF> pKeyMouse;
//    std::unordered_map<int,ICF> pQuit;
//    std::unordered_map<int,ICF> pResize;

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
