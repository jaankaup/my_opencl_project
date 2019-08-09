#ifndef GPU_DEVICE_H
#define GPU_DEVICE_H

#define CL_ENABLE_EXCEPTIONS
//#define CL_HPP_TARGET_OPENCL_VERSION 200

#include <vector>
//#include <CL/cl2.hpp>
#include <CL/cl.hpp>
#include "../Utils/log.h"

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

    /** Copy and move operations are deleted because this is an singleton class. */
    GPU_Device(const GPU_Device&) = delete;

    /** Copy and move operations are deleted because this is an singleton class. */
    GPU_Device(GPU_Device&&) = delete;

    /** Copy and move operations are deleted because this is an singleton class. */
    GPU_Device& operator=(const GPU_Device&) = delete;

    /** Copy and move operations are deleted because this is an singleton class. */
    GPU_Device& operator=(GPU_Device&&) = delete;

	private:

    /** The constructor. */
		GPU_Device() {};

    bool pInitialized = false; /**< Indicates the initialization state. */ 
    cl::Device pDevice; /**< The cl::Device. */ 
    cl::Context pContext; /**< The cl::Context fot this device. */ 
};

#endif
