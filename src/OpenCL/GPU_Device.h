#ifndef GPU_DEVICE_H
#define GPU_DEVICE_H

//#define __CL_ENABLE_EXCEPTIONS
//#define CL_HPP_TARGET_OPENCL_VERSION 200

#include <vector>
#include <unordered_map>
//#include <CL/cl2.hpp>
#include <CL/cl.hpp>
#include "../Utils/log.h"
#include "CL_Program.h"
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

    /**
     * Get pointer to the cl::CommandQueue. TODO: Remove.
     * @param return Pointer to the cl::CommandQueue.
     */
    cl::CommandQueue* getCommandQueue();

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

    /**
     * Makes the GPU_Devices CommandQueue to run the kernel. TODO: Remove.
     * @param globalDim The global dimension.
     * @param localDim The local dimension.
     * @param return Did the kernel execution succeed.
     */
    bool runKernel(CL_Program* kernel, cl::NDRange globalDim, cl::NDRange localDim);

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
        cl::CommandQueue cq = cl::CommandQueue(pContext, pDevice, 0, &error);
        if (error != CL_SUCCESS)
        {
          Log::getError().log("GPU:Device::create(): Failed to create CommmandQueue.");     
          Log::getError().log("%",errorcode_toString(error));
          return nullptr;
        }
        pCommandQueues[key] = cq;
        return &pCommandQueues[key];
      }
    }

    /**
     * Create a new resource (gl::Buffer).
     * @param key A key for getting and deleting the resource.
     * @param value The resource.
     * @param return A pointer to the created resource. TODO: how to react if
     * key already existst?
     */
    template<typename T>
    T* create(const std::string& key, T value)
    {
      assert(pInitialized);
      if constexpr (std::is_same<T,cl::Buffer>::value) {
        pBuffers[key] = value;
        return &pBuffers[key];
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
      if constexpr (std::is_same<T,cl::CommandQueue>::value) { auto f =  pCommandQueues.find(key); if (f != pCommandQueues.end()) return &f->second; else return nullptr; }
    }

	private:

    /** The constructor. */
		GPU_Device() {};

    std::unordered_map<std::string, cl::Buffer> pBuffers; /**< cl::Buffers. */
    std::unordered_map<std::string, cl::CommandQueue> pCommandQueues; /**< cl::CommandQueues. */

    bool pInitialized = false; /**< Indicates the initialization state. */ 
    cl::Device pDevice; /**< The cl::Device. */ 
    cl::Context pContext; /**< The cl::Context for this device. */ 
    cl::CommandQueue pQueue; /**< The cl::CommandQueue for this device. TODO: remove. */ 
};

#endif
