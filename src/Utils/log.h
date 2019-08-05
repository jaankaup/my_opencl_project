#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <stdexcept>
#include <string>

#ifdef USE_GLM
  #include <glm/glm.hpp>
  #include <glm/gtc/type_ptr.hpp>
#endif

class Log
{
  public:

    // Singletons.
    static Log& getInfo();
    static Log& getDebug();
    static Log& getWarning();
    static Log& getError();

    // void log(const std::string& str) { log(str.c_str(); }

    void log(const char* str) {
      if (pDisabled) return;
      printPrefix();

      while (*str)
      {
        if (*str == '%')
        {
          if (*(str+1) == '%')
          {
            ++str;
          }
          else
          {
            throw std::runtime_error("Log() : invalid format string: missing arguments.");
          }
        }
        os_ << *str++;
      }
      printPrefix_ = true;
      printEOF_ = false;
      os_ << std::endl;
    }

    template <typename T, typename... Args>
      void log(const char* str, T val, Args... args)
      {
        if (pDisabled) return;
        printPrefix();

        while (*str)
        {
          if (*str == '%')
          {
            if (*(str+1) == '%')
            {
              ++str;
            }
            else
            {
              if constexpr (std::is_same<T, bool>::value) os_ << (val == false ? "False" : "True");
              else if constexpr (std::is_arithmetic<T>::value) os_ << std::to_string(val);
#ifdef USE_GLM
              else if constexpr (std::is_same<T, glm::vec2>::value) os_ << vec_toString(val);
              else if constexpr (std::is_same<T, glm::vec3>::value) os_ << vec_toString(val);
              else if constexpr (std::is_same<T, glm::vec4>::value) os_ << vec_toString(val);
              else if constexpr (std::is_same<T, glm::mat3>::value) os_ << vec_toString(val);
              else if constexpr (std::is_same<T, glm::mat4>::value) os_ << vec_toString(val);
#endif
              else  {os_ << val;}
              ++str;
              log(str, args...);
              return;
            }
          }
          os_ << *str++;
        }
        printPrefix_ = true;
        if (printEOF_)
        {
          os_ << std::endl;
        }
      }

    std::string addPadding(const std::string& str, const uint32_t count, const char paddingChar)
    {
      std::string result = str;
      if (count > str.size())
      {
        result.insert(0,count - str.size(), paddingChar);
      }
      return result;
    }

#ifdef USE_GLM
    std::string vec_toString(const glm::vec2& object)
    {
      return "glm::vec2(" + std::to_string(object.x) + "," + std::to_string(object.y) + ")";
    }

    std::string vec_toString(const glm::vec3& object)
    {
      return "glm::vec3(" + std::to_string(object.x) + "," + std::to_string(object.y) + "," + std::to_string(object.z) + ")";
    }

    std::string vec_toString(const glm::vec4& object)
    {
      return "glm::vec4(" + std::to_string(object.x) + "," + std::to_string(object.y) + "," + std::to_string(object.z) + "," + std::to_string(object.w) + ")";
    }

    std::string vec_toString(const glm::mat3& object)
    {
      std::string padded = addPadding("",std::string("glm::mat4").size() + 2 ,' ');
      std::string result = "\n  ";
      result.append("glm::mat3("+std::to_string(object[0][0])+","+std::to_string(object[0][1])+","+std::to_string(object[0][2])+")c\n");
      result.append(padded  +"("+std::to_string(object[1][0])+","+std::to_string(object[1][1])+","+std::to_string(object[1][2])+")c\n");
      result.append(padded  +"("+std::to_string(object[2][0])+","+std::to_string(object[2][1])+","+std::to_string(object[2][2])+")c\n");
      return result;
    }

    std::string vec_toString(const glm::mat4& object)
    {
      std::string padded = addPadding("",std::string("glm::mat4").size() + 2 ,' ');
      std::string result = "\n  ";
      result.append("glm::mat4("+std::to_string(object[0][0])+","+std::to_string(object[0][1])+","+std::to_string(object[0][2])+","+std::to_string(object[0][3])+")\n");
      result.append(padded  +"("+std::to_string(object[1][0])+","+std::to_string(object[1][1])+","+std::to_string(object[1][2])+","+std::to_string(object[1][3])+")\n");
      result.append(padded  +"("+std::to_string(object[2][0])+","+std::to_string(object[2][1])+","+std::to_string(object[2][2])+","+std::to_string(object[2][3])+")\n");
      result.append(padded  +"("+std::to_string(object[3][0])+","+std::to_string(object[3][1])+","+std::to_string(object[3][2])+","+std::to_string(object[3][3])+")\n");
      return result;
    }

    void logGLM(const std::string& name, const glm::vec2& object)
    {
      log("% = (%,%)",name,std::to_string(object.x),std::to_string(object.y));
    }
       
    void logGLM(const std::string& name, const glm::vec3& object)
    {
      log("% = (%,%,%)",name,std::to_string(object.x),std::to_string(object.y),std::to_string(object.z));
    }
    
    void logGLM(const std::string& name, const glm::vec4& object)
    {
      log("% = (%,%,%,%)",name,std::to_string(object.x),std::to_string(object.y),std::to_string(object.z),std::to_string(object.w));
    }
    
    void logGLM(const std::string& name, const glm::mat3& object)
    {
      std::string padded = addPadding("",name.size(),' ');
      log("% = (%,%,%)",name   ,std::to_string(object[0][0]),std::to_string(object[0][1]),std::to_string(object[0][2]));
      log("%   (%,%,%)",padded ,std::to_string(object[1][0]),std::to_string(object[1][1]),std::to_string(object[1][2]));
      log("%   (%,%,%)",padded ,std::to_string(object[2][0]),std::to_string(object[2][1]),std::to_string(object[2][2]));
    }
    
    void logGLM(const std::string& name, const glm::mat4& object)
    {
      std::string padded = addPadding("",name.size(),' ');
      log("% = (%,%,%,%)",name   ,std::to_string(object[0][0]),std::to_string(object[0][1]),std::to_string(object[0][2]),std::to_string(object[0][3]));
      log("%   (%,%,%,%)",padded ,std::to_string(object[1][0]),std::to_string(object[1][1]),std::to_string(object[1][2]),std::to_string(object[1][3]));
      log("%   (%,%,%,%)",padded ,std::to_string(object[2][0]),std::to_string(object[2][1]),std::to_string(object[2][2]),std::to_string(object[2][3]));
      log("%   (%,%,%,%)",padded ,std::to_string(object[3][0]),std::to_string(object[3][1]),std::to_string(object[3][2]),std::to_string(object[3][3]));
    }

#endif

  private:
    Log(const std::string& prefix, std::ostream& output) : prefix_(prefix), os_(output) {};
    ~Log() {};

    std::string prefix_;
    std::ostream& os_;
    bool printPrefix_ = true;
    bool printEOF_ = false;
    bool pDisabled = false;

    void printPrefix()
    {
      if (printPrefix_)
      {
        os_ << prefix_;
        printPrefix_ = false;
      }
    }

};

/* Examples. */
//  Log::getInfo().log("Eka = %", false);
//  Log::getInfo().log("joo = %", glm::vec3(1.0f,5.5f,23.0f));
//  Log::getInfo().log("ei = %", glm::mat4(2.0f));
//  Log::getInfo().log("ei = %", true);
//  Log::getInfo().log("ei = %", false);
//  Log::getInfo().log("ei = %", "kisuli");
//  Log::getInfo().log("ei = %", double(13));

#endif //LOG_H
