#ifndef GLOBALPROPERTYMANAGER_H
#define GLOBALPROPERTYMANAGER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <optional>
#include <memory>

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
      void add(std::string key, const IntProperty& value)    { pInts[key] = std::make_optional(value); }
      void add(std::string key, const FloatProperty& value)  { pFloats[key] = std::make_optional(value); }
      void add(std::string key, const BoolProperty& value)   { pBools[key] = std::make_optional(value); }
      void add(std::string key, const StringProperty& value) { pStrings[key] = std::make_optional(value); }
      void add(std::string key, const Vec2Property& value)   { pVec2[key] = std::make_optional(value); }
      void add(std::string key, const Vec3Property& value)   { pVec3[key] = std::make_optional(value); }
      void add(std::string key, const Vec4Property& value)   { pVec4[key] = std::make_optional(value); }
      void add(std::string key, const Mat3Property& value)   { pMat3[key] = std::make_optional(value); }
      void add(std::string key, const Mat4Property& value)   { pMat4[key] = std::make_optional(value); }
      
      /* Get functions. */
      template<typename C>
      std::optional<C>* get(std::string key)
      {    
         if constexpr (std::is_same<C, IntProperty>::value) {
           static auto empty = std::optional<C>();
           if (pInts.count(key) == 0) return &empty;
           return &pInts[key];   
         }

         if constexpr (std::is_same<C, FloatProperty>::value) {
           static auto empty = std::optional<C>();
           if (pFloats.count(key) == 0) return &empty;
           return &pFloats[key];   
         }

         if constexpr (std::is_same<C, StringProperty>::value) {
           static auto empty = std::optional<C>();
           if (pStrings.count(key) == 0) return &empty;
           return &pStrings[key];   
         }

         if constexpr (std::is_same<C, BoolProperty>::value) {
           static auto empty = std::optional<C>();
           if (pBools.count(key) == 0) return &empty;
           return &pBools[key];   
         }

         if constexpr (std::is_same<C, Vec2Property>::value) {
           static auto empty = std::optional<C>();
           if (pVec2.count(key) == 0) return &empty;
           return &pVec2[key];   
         }

         if constexpr (std::is_same<C, Vec3Property>::value) {
           static auto empty = std::optional<C>();
           if (pVec3.count(key) == 0) return &empty;
           return &pVec3[key];   
         }

         if constexpr (std::is_same<C, Vec4Property>::value) {
           static auto empty = std::optional<C>();
           if (pVec4.count(key) == 0) return &empty;
           return &pVec4[key];   
         }

         if constexpr (std::is_same<C, Mat3Property>::value) {
           static auto empty = std::optional<C>();
           if (pMat3.count(key) == 0) return &empty;
           return &pMat3[key];   
         }

         if constexpr (std::is_same<C, Mat4Property>::value) {
           static auto empty = std::optional<C>();
           if (pMat4.count(key) == 0) return &empty;
           return &pMat4[key];   
         }
      }
    
    private:
    
      // A private contructor for manager.
      GlobalPropertyManager() {};
    
      std::unordered_map<std::string,std::optional<GlobalProperty<int>>> pInts;
      std::unordered_map<std::string,std::optional<GlobalProperty<float>>> pFloats;
      std::unordered_map<std::string,std::optional<GlobalProperty<std::string>>> pStrings;
      std::unordered_map<std::string,std::optional<GlobalProperty<bool>>> pBools;
      std::unordered_map<std::string,std::optional<GlobalProperty<glm::vec2>>> pVec2;
      std::unordered_map<std::string,std::optional<GlobalProperty<glm::vec3>>> pVec3;
      std::unordered_map<std::string,std::optional<GlobalProperty<glm::vec4>>> pVec4;
      std::unordered_map<std::string,std::optional<GlobalProperty<glm::mat3>>> pMat3;
      std::unordered_map<std::string,std::optional<GlobalProperty<glm::mat4>>> pMat4;
};

/* Examples */
//int main()
//{
//  IntProperty i;
//  i.set(5);
//  std::cout << i.get() << std::endl;
//
//
//  StringProperty s;
//  s.set("hauki");
//  std::cout << s.get() << std::endl;
//
//  auto ptrManager = GlobalPropertyManager::getInstance();
//  ptrManager->add("kisuli",i);
//  ptrManager->add("kala",s);
//
//  auto hevonkukkuu = ptrManager->get<StringProperty>(std::string(std::string("kala")));
//  if (hevonkukkuu->has_value())
//  {
//     auto val = &(hevonkukkuu->value());
//     val->set(std::string("joopajoo"));
//     std::cout << val->get() << std::endl;
//  }
//
//  auto hevonkukkuu2 = ptrManager->get<StringProperty>(std::string(std::string("kala")));
//  if (hevonkukkuu2->has_value())
//  {
//     auto val = &(hevonkukkuu2->value());
//     std::cout << val->get() << std::endl;
//  }
//}

} // namespace Program
#endif
