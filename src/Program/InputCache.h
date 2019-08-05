#ifndef INPUTCACHE_H
#define INPUTCACHE_H

#include <vector>
#include <functional>
#include <optional>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "../Utils/log.h"

enum class EventType { UNKNOW, KEYBOARD_MOUSE };

// Forward declaration.
class InputCache;

// A struct for registered functions.
struct IC_Function {

  std::optional<std::function<void(const InputCache*)>> f = std::nullopt; 
  std::optional<std::function<void(const InputCache*)>*> f_ptr = std::nullopt; 
  //EventType e_type = EventType::UNKNOW;

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

    std::function<void(const InputCache*)>* register_function(const EventType et, std::function<void(const InputCache*)>* function); // -> decltype(&function)

    std::function<void(const InputCache*)>* register_function(const EventType et, std::function<void(const InputCache*)>&& function); // -> decltype(function);

    // Register function for quit event.
    uint32_t register_quit(const std::function<void(const InputCache*)>& function);

    // Register function for window resize.
    uint32_t register_resize(const std::function<void(const InputCache*)>& function);

    // Remove an event listener function.
    bool unregister(std::function<void(const InputCache*)>* ptr);

    bool isKeyDown(const uint32_t key) const;
    bool isKeyUp(const uint32_t key) const;

    int get_screenWidth() const;
    int get_screenHeight() const;

    const uint8_t* get_keyboardState() const;

    uint32_t get_timeDelta() const;


    ~InputCache() {};

  private:

    InputCache() {};

    // Prepare a new state for input. not implemented.
    void cacheInput();

    void updateTick();

    // Registered functions for KeyboardMouse event.
///    std::vector<std::function<void(const InputCache*)>&& > pKeyMouse_ptrs;
    //std::vector<std::function<void(const InputCache*)>> pKeyMouse;
    std::vector<std::function<void(const InputCache*)>*> pKeyMouse_ptr;

    std::vector<IC_Function> pKeyMouse;

    //std::unordered_map<const std::function<void(const InputCache*)>*,std::function<void(const InputCache*)>> pKeyMouse;
    std::unordered_map<int,std::function<void(const InputCache*)>> pQuit;
    std::unordered_map<int,std::function<void(const InputCache*)>> pResize;

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
