#ifndef INPUTCACHE_H
#define INPUTCACHE_H

#include <chrono>
#include <vector>
#include <functional>
#include <variant>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "../Utils/log.h"

/**
 * An enum for different event types.
 * For example: an event handler function which has type of KEYBOARD_MOUSE will be executed always when an 
 * keyboar or mouse event is reqognized.
 */
enum class EventType { UNKNOW, /**< An unknow event type. */
                       KEYBOARD_MOUSE, /**< An event type for both keyboard and mouse events. */
                       RESIZE_EVENT, /**< An event type for window resize event. */
                       QUIT} /**< An event for application quit event. **/ ;

/** A typedef for time. */
typedef std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::duration<long int, std::ratio<1, 1000000000>>> HTime;

class InputCache;

/** A typedef for the registered function prototype. */
typedef std::function<void(const InputCache*)> ICF;

/** A typedef for the a variant which can hold an labmda function of a function
 * pointer for the registered function. */
typedef std::variant<ICF, void (*)(const InputCache*) > InputCache_Function;

/**
 * A struct for a single mouse button.
 */
struct MouseButton {
  HTime buttonDown_time_start;
  HTime buttonDown_time_end;
  uint8_t buttonType = 0;
  bool buttonPressed = false;
  bool buttonReleased = false;
  bool buttonDown = false;
};

/**
 * A struct for the mouse.
 * This used as an internal structure for the state of the mouse.
 */
struct Mouse {
  std::vector<MouseButton> buttons; /**< A vector for all the mouse buttons. */
  glm::ivec2 old_position = glm::ivec2(0); /** An ivec2 for holding the value for previous mouse position. */
  glm::ivec2 current_position = glm::ivec2(0); /** An ivec2 for holdin the value for the current mouse position. */
  bool isMoving = false; /** A bool value which tells the state of mouse movement. */
  bool cursor_initialized = false; /** A value for which is set to true the first time mouse is moved. */
};

/**
 * A class that stores the registered function.
 */
class RegisteredFunction
{
  public:

    /** The constructor. */
    RegisteredFunction() { pId = nextId++; }

    /** This prevents the object copying. */ 
    RegisteredFunction(const RegisteredFunction&) = delete;

    /** This prevents the object copying. */ 
    RegisteredFunction& operator=(const RegisteredFunction&) = delete;

    /** A default behavios for moving. */ 
    RegisteredFunction(RegisteredFunction&&) = default;

    /** A default behavios for moving. */ 
    RegisteredFunction& operator=(RegisteredFunction&&) = default;

    /** Move a lambda-function to this object.
     * A only one lambda function or function pointer can be added. 
     * This operation is permanent.
     * @param ICF&& is the function to be moved.
     */ 
    void add_lambda_function(ICF&& function);

    /** Copy a function pointer to this object.
     * A only one lambda function or function pointer can be added. 
     * This operation is permanent.
     * @param function_pointer is the function to be copied.
     */ 
    void add_function_pointer(void (*function_pointer)(const InputCache*));

    /** Get the id of this object. This id can be used to remove this
     * Registered function object from the InputCache class. */ 
    uint32_t getId() { return pId; } 

    /** This method invokes the function.
     * If there is no function to call, this function will log an error. TODO:
     * add an exception or assert.
     * @param ic it a pointer to the InputCache object.
     */
    void callFunction(InputCache* ic);

  private:
    InputCache_Function pIcf;
    uint32_t pId = 0;

    bool initialized = false;
    inline static uint32_t nextId = 0; 
};

/**
 * A singleton class for handling SDL events.
 */
class InputCache
{
  public:

    /**
     * A method for getting the pointer to the InputCache object.
     * @param return The pointer to the InputCache instance.
     */
    static InputCache* getInstance() { static InputCache eh; return &eh; };

    /** Copy and move operations are deleted because this is an singleton class. */
    InputCache(const InputCache&) = delete;

    /** Copy and move operations are deleted because this is an singleton class. */
    InputCache(InputCache&&) = delete;

    /** Copy and move operations are deleted because this is an singleton class. */
    InputCache& operator=(const InputCache&) = delete;

    /** Copy and move operations are deleted because this is an singleton class. */
    InputCache& operator=(InputCache&&) = delete;

    /** Initializes the object.
     * @param return true is initializtion succeeded, false otherwise.
     */
    bool init();

    /** Get the all events from SDL and execute registered functions.
     */
    void pollEvents();

    /** A test function. TODO: remove.
     */
    bool whatsUpMan() const;

    /** A method for registerin a labmda function.
     * @et is the event type.
     * @lambda_f if the lambda function that will be moved to the registered
     * functions.
     * @return an id for unregisterin the function
     */
    uint32_t register_lambda_function(const EventType et, ICF&& lambda_f);

    /** A method for registerin a function pointer.
     * @et is the event type.
     * @function_pointer if the function pointer that will be registered.
     * @return an id for unregisterin the function
     */
    uint32_t register_function_pointer(const EventType et, void (*function_pointer)(const InputCache*));

    /** A method for preprosessing the SDL_Events.  */
    void preprocess_inputs();

    /** A method for invoking all the KEYBOARD_MOUSE event functions. */
    void handle_keyboad_mouse();

    /** A method for invoking all the window related event functions. */
    void handle_window_events();

    /** A method for unregistering an function.
     * @param id is the key for the unregistered funtion. The key is the result 
     * of the register methods.
     * @param return True if function was removed succesfully, false otherwise. */
    bool unregister(const uint32_t id);

    /** A method for checking if a key is being pressed.
     * @param key is a SDL key to be checked. 
     * @param return The result. */
    bool isKeyPressed(const uint32_t key) const;

    /** A method for checking if a key is being released.
     * @param key is a SDL key to be checked. 
     * @param return The result. */
    bool isKeyReleased(const uint32_t key) const;

    /** A method for checking if a key is down.
     * @param key is a SDL key to be checked. 
     * @param return The result. */
    bool isKeyDown(const uint32_t key) const;

    /** A method for checking if a mouse button is pressed.
     * @param button_type is a SDL mouse button to be checked. 
     * @param return The result. */
    bool isMousePressed(const uint8_t button_type) const;

    /** A method for checking if a mouse button is released.
     * @param button_type is a SDL mouse button to be checked. 
     * @param return The result. */
    bool isMouseReleased(const uint8_t button_type) const;

    /** A method for checking if a mouse button is down.
     * @param button_type is a SDL mouse button to be checked. 
     * @param return The result. */
    bool isMouseDown(const uint8_t button_type) const;

    /** A method for checking how long the button has been pressed.
     * @param button_type is a SDL mouse button to be checked. 
     * @param return The result in microseconds. */
    uint64_t getButtonDown_time(const uint8_t button_type) const;

    /** A method for checking if mouse is moving.
     * @param return The result. */
    bool isMouseMoving() const;

    /** A method for checking if mouse is moving.
     * @param return The result. */
    glm::ivec2 getMouseDelta() const;

    /** A method for getting the current position of the mouse in the screen
     * coordinates.
     * @param return The result. */
    glm::ivec2 getCurrent_mousePosition() const;

    /** A method for getting the current screen width.
     * @param return The result. */
    int get_screenWidth() const;

    /** A method for getting the current screen height.
     * @param return The result. */
    int get_screenHeight() const;

    /** TODO: remove.
     * @param return The result. */
    const uint8_t* get_keyboardState() const;

    /** A method for getting the time difference between the last time check in
     * microseconds.
     * @param return The result. */
    uint64_t get_timeDelta() const;

    ~InputCache() {};

  private:

    /** The constructor.
     */
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

      MouseButton left; left.buttonType = SDL_BUTTON_LEFT;
      MouseButton middle; middle.buttonType = SDL_BUTTON_MIDDLE;
      MouseButton right; right.buttonType = SDL_BUTTON_RIGHT;

      pMouse.buttons.push_back(left);
      pMouse.buttons.push_back(middle);
      pMouse.buttons.push_back(right);
      auto time = std::chrono::system_clock::now();
      pTicks_prev = time;
      pTicks_now = time;
    };

    /** TODO: check this.
     */
    static uint32_t nextId;

    /** Prepares the state for before invoking the registered functions.  */
    void cacheInput();

    /** Update time.  */
    void updateTick();

    /** The registered functions for KEYBOAR_MOUSE.  */
    std::vector<RegisteredFunction> pKeyboardMouse;

    /** The registered functions for RESIZE_EVENT.  */
    std::vector<RegisteredFunction> pResize;

    /** The registered functions for QUIT.  */
    std::vector<RegisteredFunction> pQuit;
   
    /** TODO: check this.
     */
    uint32_t pNext_Id = 0;

    /** The current screen width.  */
    int pScreenWidth = 1024;

    /** The current screen heihts.  */
    int pScreenHeight = 800;

    // TODO: something more clever. Now only for testing.
    //
    /** The SDL key down events.  */
    std::vector<SDL_Event> pKeyDownEvents;

    /** The SDL key up events.  */
    std::vector<SDL_Event> pKeyUpEvents;

    /** The SDL mouse motion events.  */
    std::vector<SDL_Event> pMouseMotionEvents;

    /** The SDL mouse button down events.  */
    std::vector<SDL_Event> pMouseButtonDown;

    /** The SDL mouse button up events.  */
    std::vector<SDL_Event> pMouseButtonUp;

    /** The SDL window events.  */
    std::vector<SDL_Event> pWindowEvents;
    
    /** The SDL quit events.  */
    std::vector<SDL_Event> pQuitEvent;

    /** The last timestamp.  */
    HTime pTicks_prev;

    /** The current timestamp.  */
    HTime pTicks_now;

    /** The previous mouse location on the screen.  */
    glm::ivec2 pMousePos_prev = glm::vec2(0);

    /** The current mouse location on the screen.  */
    glm::ivec2 pMousePos_now = glm::vec2(0);

    /** The internal mouse stuct.  */
    Mouse pMouse; 
      
};

#endif
