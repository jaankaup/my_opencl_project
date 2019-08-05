#ifndef GPU_DEVICE_H
#define GPU_DEVICE_H

#define CL_ENABLE_EXCEPTIONS
//#define CL_HPP_TARGET_OPENCL_VERSION 200

#include <vector>
//#include <CL/cl2.hpp>
#include <CL/cl.hpp>
#include "../Utils/log.h"

/* A singleton class for a single GPU opencl device. */
class GPU_Device
{ 

	public:

    static GPU_Device* getInstance() { static GPU_Device gd; return &gd; }
		~GPU_Device() {};

    bool init();

    /// Returns a pointer to the context. If context is not initialized, return
    /// nullptr.
    cl::Context* getContext();

    GPU_Device(const GPU_Device&) = delete;
    GPU_Device(GPU_Device&&) = delete;
    GPU_Device& operator=(const GPU_Device&) = delete;
    GPU_Device& operator=(GPU_Device&&) = delete;

	private:

		GPU_Device() {};

    bool pInitialized = false;
    cl::Device pDevice;
    cl::Context pContext;
};

#endif
