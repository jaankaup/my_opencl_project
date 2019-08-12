#ifndef GPU_DEVICE_H
#define GPU_DEVICE_H

//#define __CL_ENABLE_EXCEPTIONS
//#define CL_HPP_TARGET_OPENCL_VERSION 200

#include <vector>
//#include <CL/cl2.hpp>
#include <CL/cl.hpp>
#include "../Utils/log.h"
#include "CL_Program.h"

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
     * Get pointer to the cl::CommandQueue.
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
     * Makes the GPU_Devices CommandQueue to run the kernel.
     * @param globalDim The global dimension.
     * @param localDim The local dimension.
     * @param return Did the kernel execution succeed.
     */
    bool runKernel(CL_Program* kernel, cl::NDRange globalDim, cl::NDRange localDim);

	private:

    /** The constructor. */
		GPU_Device() {};

    bool pInitialized = false; /**< Indicates the initialization state. */ 
    cl::Device pDevice; /**< The cl::Device. */ 
    cl::Context pContext; /**< The cl::Context for this device. */ 
    cl::CommandQueue pQueue; /**< The cl::CommandQueue for this device. */ 
};

#endif
