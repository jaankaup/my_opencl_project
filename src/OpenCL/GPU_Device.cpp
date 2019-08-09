#include "GPU_Device.h"

bool GPU_Device::init()
{
  /* cl::Platform Provides functionality for working with OpenCL platforms. */
  std::vector<cl::Platform> all_platforms;

  /* Static method for lists all available opencl platforms. */
  /* cl_int == CL_SUCCESS. CL_INVALID_VALUE if platforms is NULL. */
  cl_int success = cl::Platform::get(&all_platforms);

  if (success == CL_INVALID_VALUE) {
    Log::getError().log("GPU:Device::init(): Platforms is NULL.");     return false;
  }

  if(all_platforms.size()==0){
    Log::getError().log("GPU:Device::init(): No platforms found. Check OpenCL installation!.");     
    return false; 
  }

  cl::Platform default_platform=all_platforms[0];

  Log::getInfo().log("GPU:Device::init(): Using platform: %.",default_platform.getInfo<CL_PLATFORM_NAME>());     

  //get default device of the default platform
  std::vector<cl::Device> all_devices;

  success = default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);

  if (success != CL_SUCCESS) {
    std::string error = "Failed to create default platform. ";
    switch (success) {
        case CL_INVALID_DEVICE_TYPE: error += "CL_INVALID_DEVICE_TYPE"; break;
        case CL_INVALID_ARG_VALUE : error += "CL_INVALID_ARG_VALUE"; break;
        case CL_DEVICE_NOT_FOUND: error += "CL_DEVICE_NOT_FOUND"; break;
    }
    Log::getError().log("GPU:Device::init(): %",error);     
  }

  if (all_devices.size() == 0) {
    Log::getError().log("GPU:Device::init(): No devices found. Check OpenCL installation!.");     
    return false;
  }

  pDevice = all_devices[0];

  Log::getInfo().log("CL_DEVICE_NAME: %", pDevice.getInfo<CL_DEVICE_NAME>());
  Log::getInfo().log("CL_DEVICE_BUILT_IN_KERNELS: %", pDevice.getInfo<CL_DEVICE_BUILT_IN_KERNELS>());
  Log::getInfo().log("CL_DEVICE_EXTENSIONS: %", pDevice.getInfo<CL_DEVICE_EXTENSIONS>());
  Log::getInfo().log("CL_DEVICE_BUILT_IN_KERNELS: %", pDevice.getInfo<CL_DEVICE_BUILT_IN_KERNELS>());
  Log::getInfo().log("CL_DEVICE_OPENCL_C_VERSION: %", pDevice.getInfo<CL_DEVICE_OPENCL_C_VERSION>());
  Log::getInfo().log("CL_DEVICE_PROFILE: %", pDevice.getInfo<CL_DEVICE_PROFILE>());
  Log::getInfo().log("CL_DEVICE_VENDOR: %", pDevice.getInfo<CL_DEVICE_VENDOR>());
  Log::getInfo().log("CL_DEVICE_VERSION: %", pDevice.getInfo<CL_DEVICE_VERSION>());
  Log::getInfo().log("CL_DRIVER_VERSION: %", pDevice.getInfo<CL_DRIVER_VERSION>());
  Log::getInfo().log("CL_DEVICE_MAX_WORK_ITEM_SIZES: ");
  for (const auto& x : pDevice.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>()) {
    Log::getInfo().log("   %", x);
  }

  // TODO: error handling for context creation.
  pContext = cl::Context({pDevice});

  pInitialized = true;

  return true;
}

cl::Context* GPU_Device::getContext()
{
  if (pInitialized) return &pContext;
  return nullptr;
}
