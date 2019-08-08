#ifndef GLOBALPROPERTYMANAGER_H
#define GLOBALPROPERTYMANAGER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <optional>
#include <memory>
#include <glm/glm.hpp>
#include "../Utils/log.h"

namespace Program {

/* A global property. An ugly class for storing and modifying program global
 * variables. */
template<typename T>
class GlobalProperty {
  public:
  
    // Get value.
    auto get() { return pValue; }
    
    // Set value.
    void set(T value) {
      for (const auto& f : tests)
      {
        // Run all tests.
        if (!f(value)) return;
      }
      
      // All test passed. Add the value.
      pValue = value;
    }
    
    // Register tests which must satisfy when applying a new value.
    void registerTest(std::function<bool(const T)> test) {tests.push_back(test);}
    
  private:
    T pValue;
    std::vector<std::function<bool(T)>> tests;

};

using IntProperty    = GlobalProperty<int>; 
using FloatProperty  = GlobalProperty<float>; 
using BoolProperty   = GlobalProperty<bool>; 
using StringProperty = GlobalProperty<std::string>; 
using Vec2Property   = GlobalProperty<glm::vec2>; 
using Vec3Property   = GlobalProperty<glm::vec3>; 
using Vec4Property   = GlobalProperty<glm::vec4>; 
using Mat3Property   = GlobalProperty<glm::mat3>; 
using Mat4Property   = GlobalProperty<glm::mat4>; 

//using MaybeInt = std::optional<IntProperty>; 
//using MaybeString = std::optional<StringProperty>; 

/* Class for global properties. */
class GlobalPropertyManager
{
    public:
    
      // Delete or copying/moving operations Because this is a singleton class.
      GlobalPropertyManager(const GlobalPropertyManager&) = delete;
      GlobalPropertyManager(GlobalPropertyManager&&) = delete;
      GlobalPropertyManager& operator=(const GlobalPropertyManager&) = delete;
      GlobalPropertyManager& operator=(GlobalPropertyManager&&) = delete;
    
      // Get a pointer to the manager.
      static GlobalPropertyManager* getInstance() { static GlobalPropertyManager gp; return &gp; }
      
      /* Add functions for global variables. */
      void add(std::string key, const IntProperty& value)    { pInts[key] = std::move(value); }
      void add(std::string key, const FloatProperty& value)  { pFloats[key] = std::move(value); }
      void add(std::string key, const BoolProperty& value)   { pBools[key] = std::move(value); }
      void add(std::string key, const StringProperty& value) { pStrings[key] = std::move(value); }
      void add(std::string key, const Vec2Property& value)   { pVec2[key] = std::move(value); }
      void add(std::string key, const Vec3Property& value)   { pVec3[key] = std::move(value); }
      void add(std::string key, const Vec4Property& value)   { pVec4[key] = std::move(value); }
      void add(std::string key, const Mat3Property& value)   { pMat3[key] = std::move(value); }
      void add(std::string key, const Mat4Property& value)   { pMat4[key] = std::move(value); }
      
      /* Get functions. */
      template<typename C>
      C* get(std::string key)
      {    
         if constexpr (std::is_same<C, IntProperty>::value) {
           if (pInts.count(key) == 0) { Log::getError().log("Trying to access IntProperty '%' but the property doesn't exist!", key); return nullptr;}
           return &pInts[key];   
         }

         if constexpr (std::is_same<C, FloatProperty>::value) {
           if (pFloats.count(key) == 0) { Log::getError().log("Trying to access FloatProperty '%' but the property doesn't exist!", key); return nullptr;}
           return &pFloats[key];   
         }

         if constexpr (std::is_same<C, StringProperty>::value) {
           if (pStrings.count(key) == 0) { Log::getError().log("Trying to access StringProperty '%' but the property doesn't exist!", key); return nullptr;}
           return &pStrings[key];   
         }

         if constexpr (std::is_same<C, BoolProperty>::value) {
           if (pBools.count(key) == 0) { Log::getError().log("Trying to access BoolProperty '%' but the property doesn't exist!", key); return nullptr;}
           return &pBools[key];   
         }

         if constexpr (std::is_same<C, Vec2Property>::value) {
           if (pVec2.count(key) == 0) { Log::getError().log("Trying to access Vec2Property '%' but the property doesn't exist!", key); return nullptr;}
           return &pVec2[key];   
         }

         if constexpr (std::is_same<C, Vec3Property>::value) {
           if (pVec3.count(key) == 0) { Log::getError().log("Trying to access Vec3Property '%' but the property doesn't exist!", key); return nullptr;}
           return &pVec3[key];   
         }

         if constexpr (std::is_same<C, Vec4Property>::value) {
           if (pVec4.count(key) == 0) { Log::getError().log("Trying to access Vec4Property '%' but the property doesn't exist!", key); return nullptr;}
           return &pVec4[key];   
         }

         if constexpr (std::is_same<C, Mat3Property>::value) {
           if (pMat3.count(key) == 0) { Log::getError().log("Trying to access Mat3Property '%' but the property doesn't exist!", key); return nullptr;}
           return &pMat3[key];   
         }

         if constexpr (std::is_same<C, Mat4Property>::value) {
           if (pMat4.count(key) == 0) { Log::getError().log("Trying to access Mat4Property '%' but the property doesn't exist!", key); return nullptr;}
           return &pMat4[key];   
         }
      }
    
    private:
    
      // A private contructor for manager.
      GlobalPropertyManager() {};
    
      std::unordered_map<std::string,IntProperty> pInts;
      std::unordered_map<std::string,FloatProperty> pFloats;
      std::unordered_map<std::string,StringProperty> pStrings;
      std::unordered_map<std::string,BoolProperty> pBools;
      std::unordered_map<std::string,Vec2Property> pVec2;
      std::unordered_map<std::string,Vec3Property> pVec3;
      std::unordered_map<std::string,Vec4Property> pVec4;
      std::unordered_map<std::string,Mat3Property> pMat3;
      std::unordered_map<std::string,Mat4Property> pMat4;
};

/* Examples */
// Creating property.
//  IntProperty i;
//  i.set(5);
//  i.registerTest([](int x) {if (x < 15) { Log::getDebug().log("hellureijaa. alle 15."); return true;} else { Log::getDebug().log("Ei onnaa."); return false; }});
//
// Accessing property.
//  auto hevonkukkuu = ptrManager->get<IntProperty>("kisuli");
//  if (auto val = hevonkukkuu ; val) val->value().set(12);

} // namespace Program
#endif
