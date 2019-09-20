#ifndef GPU_DEVICE_H
#define GPU_DEVICE_H

//#define __CL_ENABLE_EXCEPTIONS
//#define CL_HPP_TARGET_OPENCL_VERSION 200

#include <vector>
#include <unordered_map>
#include <memory>
#include <GL/glew.h>
//#include <CL/cl2.hpp>
#include <CL/cl.hpp>
#include <CL/cl_gl.h>
#include "../Utils/log.h"
#include "CL_Helper.h"

class CL_Program;

/** A singleton class for represention of a single GPU opencl device. */
class GPU_Device
{ 

	public:

    /**
     * A method for getting the pointer to the this singleton object.
     * @param return The pointer to the GPU_Device instance.
     */
    static GPU_Device* getInstance() { static GPU_Device gd; return &gd; }

    /**
     * The destructor.
     */
		~GPU_Device() {};

    /**
     * Initializes the opencl device and context.
     * @param return True if initialization succeed, false otherwise.
     */
    bool init();

    /**
     * A method for getting a pointer for the created context.
     * @param return a pointer to the gpu device context of nullptr if these is
     * no context.
     */
    cl::Context* getContext();

    /**
     * Copy and move operations are deleted because this is an singleton class.
     * */
    GPU_Device(const GPU_Device&) = delete;

    /**
     * Copy and move operations are deleted because this is an singleton class.
     * */
    GPU_Device(GPU_Device&&) = delete;

    /**
     * Copy and move operations are deleted because this is an singleton class.
     * */
    GPU_Device& operator=(const GPU_Device&) = delete;

    /** Copy and move operations are deleted because this is an singleton class. */
    GPU_Device& operator=(GPU_Device&&) = delete;

    /**
     * Get pointer to the actual device.
     * @param return Pointer to the device.
     */
    cl::Device* getDevice();

//    /**
//     * Get pointer to the cl::CommandQueue. TODO: Remove.
//     * @param return Pointer to the cl::CommandQueue.
//     */
//    cl::CommandQueue* getCommandQueue();

    /**
     * Get the maximum group size.
     * @param return Maximum group size.
     */
    size_t getMaxGroupSize() const;

    /**
     * Get the global dimension.
     * @param total_count The number of elements.
     * @param return The global dimension.
     */
    cl::NDRange getGlobalDim(const int total_count);

    /**
     * Get the local dimension.
     * @param total_count The number of elements.
     * @param return The local dimension.
     */
    cl::NDRange getLocalDim();

//    /**
//     * Makes the GPU_Devices CommandQueue to run the kernel. TODO: Remove.
//     * @param globalDim The global dimension.
//     * @param localDim The local dimension.
//     * @param return Did the kernel execution succeed.
//     */
//    bool runKernel(CL_Program* kernel, cl::NDRange globalDim, cl::NDRange localDim);

    cl::Buffer* createBuffer(const std::string& name, size_t size, cl_mem_flags); 

    cl::ImageGL* createImage(const std::string& name, cl_mem_flags flags, GLenum target, GLuint textureID)
    {
      cl_int error = 0;
      std::unique_ptr<cl::ImageGL> b(new cl::ImageGL(pContext, flags, target, 0, textureID, &error));
      if (error != CL_SUCCESS)
      {
        Log::getDebug().log("GPU_Device::createImage(%,%,%,%)",name,flags,target, textureID);
        print_cl_error(error);
        return nullptr;
      }
      pImages[name] = std::move(b);
      return pImages[name].get();
    }

    cl::Program* createProgram(const std::string& name, cl::Program::Sources& sources); 

    /**
     * Create a new resource (gl::CommandQueue).
     * @param key A key for getting and deleting the resource.
     * @param return A pointer to the created resource or nullptr on
     * failure.
     */
    template<typename T>
    T* create(const std::string& key)
    {
      assert(pInitialized);
      if constexpr (std::is_same<T,cl::CommandQueue>::value) {
        cl_int error;
        std::unique_ptr<cl::CommandQueue> c(new cl::CommandQueue(pContext, pDevice, 0, &error));
        if (error != CL_SUCCESS)
        {
          Log::getError().log("GPU:Device::create(): Failed to create CommmandQueue.");     
          Log::getError().log("%",errorcode_toString(error));
          return nullptr;
        }
        pCommandQueues[key] = std::move(c);
        return pCommandQueues[key].get();
      }
    }

    /**
     * Delete a resource.
     * @param key for deleting the resource.
     */
    template<typename T>
    void del(const std::string& key)
    {
      assert(pInitialized);
      if constexpr (std::is_same<T,cl::CommandQueue>::value) { pCommandQueues.erase(key); }
      if constexpr (std::is_same<T,cl::Buffer>::value) { pBuffers.erase(key); }
      if constexpr (std::is_same<T,cl::ImageGL>::value) { pImages.erase(key); }
      if constexpr (std::is_same<T,cl::Program>::value) { pPrograms.erase(key); }
    }

    /**
     * Get a resource for given key.
     * @param key for getting the resource.
     * @return A pointer to the resource or nullptr is resource is not found.
     */
    template<typename T>
    T* get(const std::string& key)
    {
      assert(pInitialized);
      if constexpr (std::is_same<T,cl::CommandQueue>::value) { auto f =  pCommandQueues.find(key); if (f != pCommandQueues.end()) return f->second.get(); else return nullptr; }
      if constexpr (std::is_same<T,cl::Buffer>::value) { auto f =  pBuffers.find(key); if (f != pBuffers.end()) return f->second.get(); else return nullptr; }
      if constexpr (std::is_same<T,cl::Program>::value) { auto f =  pPrograms.find(key); if (f != pPrograms.end()) return f->second.get(); else return nullptr; }
      if constexpr (std::is_same<T,cl::ImageGL>::value) { auto f =  pImages.find(key); if (f != pImages.end()) return f->second.get(); else return nullptr; }
    }

	private:

    /** The constructor. */
		GPU_Device() {};

    std::unordered_map<std::string, std::unique_ptr<cl::Buffer>> pBuffers; /**< cl::Buffers. */
    std::unordered_map<std::string, std::unique_ptr<cl::ImageGL>> pImages; /**< cl::ImageGLs. */
    std::unordered_map<std::string, std::unique_ptr<cl::CommandQueue>> pCommandQueues; /**< cl::CommandQueues. */
    std::unordered_map<std::string, std::unique_ptr<cl::Program>> pPrograms; /**< cl::Programs. */

    bool pInitialized = false; /**< Indicates the initialization state. */ 
    cl::Device pDevice; /**< The cl::Device. */ 
    cl::Context pContext; /**< The cl::Context for this device. */ 
    cl::CommandQueue pQueue; /**< The cl::CommandQueue for this device. TODO: remove. */ 
};

#endif
