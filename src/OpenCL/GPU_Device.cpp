#include "GPU_Device.h"
#include <GL/glx.h>

bool GPU_Device::init()
{
  /* cl::Platform Provides functionality for working with OpenCL platforms. */
  std::vector<cl::Platform> all_platforms;

  /* Static method for lists all available opencl platforms. */
  cl_int success = cl::Platform::get(&all_platforms);

  if (success == CL_INVALID_VALUE) {
    Log::getError().log("GPU:Device::init(): Platforms is NULL.");
    return false;
  }

  cl_uint platforms_available = all_platforms.size();

  if(platforms_available==0){
    Log::getError().log("GPU:Device::init(): No platforms found. Check OpenCL installation!.");     
    return false; 
  }


  cl::Platform default_platform=all_platforms[0];

//  cl_uint n = 0;
//  success = clGetPlatformIDs(0, NULL, &n);
//  if (success != CL_SUCCESS) {
//    Log::getError().log("GPU:Device::init(): clGetPlatformIDs.");
//    print_cl_error(success);
//    return false;
//  }

//§  /* Get the platform ids. */
//§  std::vector<cl_platform_id> ids(platforms_available);
//§  success = clGetPlatformIDs(platforms_available, ids.data(), NULL);
//§  if (success != CL_SUCCESS) {
//§    Log::getError().log("GPU:Device::init(): clGetPlatformIDs.");
//§    print_cl_error(success);
//§    return false;
//§  }

  Log::getInfo().log("Using platform: %.",default_platform.getInfo<CL_PLATFORM_NAME>());     

  //get default device of the default platform
  std::vector<cl::Device> all_devices;

  success = default_platform.getDevices(CL_DEVICE_TYPE_GPU, &all_devices);

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

  size_t bytes = 0;
// Notice that extension functions are accessed via pointers
// initialized with clGetExtensionFunctionAddressForPlatform.

//  cl_context_properties props[] = {
//    CL_CONTEXT_PLATFORM, (cl_context_properties)platform,
//    CL_GL_CONTEXT_KHR, (cl_context_properties)hRC,
//    CL_WGL_HDC_KHR, (cl_context_properties)hDC,
//    0
//  };
//// queuring how much bytes we need to read
//clGetGLContextInfoKHR(props, CL_DEVICES_FOR_GL_CONTEXT_KHR, 0, NULL, &bytes);
//// allocating the mem
//size_t devNum = bytes/sizeof(cl_device_id);
//std::vector<cl_device_id> devs (devNum);
////reading the info
//clGetGLContextInfoKHR(props, CL_DEVICES_FOR_GL_CONTEXT_KHR, bytes, devs, NULL));
//looping over all devices
//for(size_t i=0;i<devNum; i++)
//{
//      //enumerating the devices for the type, names, CL_DEVICE_EXTENSIONS, etc
//      clGetDeviceInfo(devs[i],CL_DEVICE_TYPE, …);
//      …
//      clGetDeviceInfo(devs[i],CL_DEVICE_EXTENSIONS,…);
//      …
//}

  
  /* OS X
  CGLContextObj     CGLGetCurrentContext(void);
CGLShareGroupObj  CGLGetShareGroup(CGLContextObj);

CGLContextObj     kCGLContext     = CGLGetCurrentContext();
CGLShareGroupObj  kCGLShareGroup  = CGLGetShareGroup(kCGLContext);

cl_context_properties properties[] =
{
  CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
  (cl_context_properties) kCGLShareGroup,
  0
};
*/

// Linux (with GLX)

//cl_int error = clGetDeviceIDdefault_platform;

cl_context_properties properties[] =
{
  CL_GL_CONTEXT_KHR,   (cl_context_properties)glXGetCurrentContext(),
  CL_GLX_DISPLAY_KHR,  (cl_context_properties)glXGetCurrentDisplay(),
  CL_CONTEXT_PLATFORM, (cl_context_properties)default_platform() /*ids[0] */, // OpenCL platform object
  0
};

for (int i=0 ; i<7 ; i++) {
  Log::getDebug().log("::::::::::::::::: %", properties[i]);
}

/*
Windows (WGL)

cl_context_properties properties[] =
{
  CL_GL_CONTEXT_KHR,   (cl_context_properties)wglGetCurrentContext(),
  CL_WGL_HDC_KHR,      (cl_context_properties)wglGetCurrentDC(),
  CL_CONTEXT_PLATFORM, (cl_context_properties)platform, // OpenCL platform object
  0
};
*/

/*
Android (with EGL)

cl_context_properties properties[] =
{
  CL_GL_CONTEXT_KHR,   (cl_context_properties)eglGetCurrentContext(),
  CL_EGL_DISPLAY_KHR,  (cl_context_properties)eglGetCurrentDisplay(),
  CL_CONTEXT_PLATFORM, (cl_context_properties)platform, // OpenCL platform object
  0
};
*/
  //auto a = clGetGLContextInfoKHR();

  //CGLContextObj cgl_ctx = CGLGetCurrentContext();
//  cl_context_properties props[] = {
//    CL_CONTEXT_PLATFORM, (cl_context_properties)platform,
//    CL_GL_CONTEXT_KHR, (cl_context_properties)hRC,
//    CL_WGL_HDC_KHR, (cl_context_properties)hDC,
//    0
//  };

  // TODO: error handling for context creation.
  pContext = cl::Context({pDevice},properties);

  //cl_int error;

  //pQueue = cl::CommandQueue(pContext, pDevice, 0, &success);

  //if (success != CL_SUCCESS)
  //{
  //  Log::getError().log("GPU:Device::init(): Failed to create CommmandQueue with error code %.",success);     
  //  return false;
  //}
  pInitialized = true;

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////

cl::Context* GPU_Device::getContext()
{
  if (pInitialized) return &pContext;
  return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////

cl::Device* GPU_Device::getDevice()
{
  if (!pInitialized) return nullptr;
  return &pDevice;
}

/////////////////////////////////////////////////////////////////////////////////////

//cl::CommandQueue* GPU_Device::getCommandQueue()
//{
//  if (!pInitialized) return nullptr;
//  return &pQueue;
//}

/////////////////////////////////////////////////////////////////////////////////////

size_t GPU_Device::getMaxGroupSize() const
{
  size_t max_size;
  cl_int error;

  pDevice.getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &max_size);
  //pDevice.getInfo(CL_KERNEL_PREFERRED_GROUP_SIZE_MULTIPLE, &max_size);

  if (error != CL_SUCCESS)
  {
    Log::getError().log("GPU_Device::getMaxGroupSize(): Failed to get the maximum group size with error code %.",error);     
    return 0;
  }

  return max_size;
}

/////////////////////////////////////////////////////////////////////////////////////

cl::NDRange GPU_Device::getGlobalDim(const int total_count)
{
  Log::getDebug().log("GPU_Device::getGlobalDim(%)",total_count);     

  size_t max_group_size = getMaxGroupSize();
  Log::getDebug().log("max_group_size %.",max_group_size);     

  size_t workGroupCount = total_count/max_group_size + 1;
  Log::getDebug().log("workGroupCount =  %/% + 1 == %", total_count,max_group_size,workGroupCount);     

  cl::NDRange globalDim(workGroupCount*max_group_size, 1, 1);
  Log::getDebug().log("cl::NDRange globalDim(%*% == %, 1, 1);", workGroupCount,max_group_size,workGroupCount*max_group_size);     
  return globalDim;
}

/////////////////////////////////////////////////////////////////////////////////////

cl::NDRange GPU_Device::getLocalDim()
{
  cl::NDRange localDim(getMaxGroupSize(), 1, 1);
  Log::getDebug().log("cl::NDRange localDim(%, 1, 1);", getMaxGroupSize());     
  return localDim;
}

/////////////////////////////////////////////////////////////////////////////////////

//bool GPU_Device::runKernel(CL_Program* program, cl::NDRange globalDim, cl::NDRange localDim)
//{
//  assert(program != nullptr);
//  cl_int error;
//
//  pQueue.enqueueNDRangeKernel(*(program->getKernel()), cl::NullRange, globalDim, localDim, 0, 0);
//
//  if (error != CL_SUCCESS)
//  {
//    Log::getError().log("GPU_Device::runKernel(): Failed to run the kernel with error code %.",error);     
//    return false;
//  }
//
//  return true;
//}

/////////////////////////////////////////////////////////////////////////////////////

cl::Buffer* GPU_Device::createBuffer(const std::string& key, size_t size, cl_mem_flags flags)
{
  cl_int error = 0;
  std::unique_ptr<cl::Buffer> b(new cl::Buffer(pContext, flags , size, &error));
  if (error != CL_SUCCESS)
  {
    Log::getDebug().log("GPU_Device::create(%,%,%)",key,size,flags);
    print_cl_error(error);
    return nullptr;
  }
  pBuffers[key] = std::move(b);
  return pBuffers[key].get();
}

/////////////////////////////////////////////////////////////////////////////////////

cl::Program* GPU_Device::createProgram(const std::string& key, cl::Program::Sources& sources) 
{

  cl_int error = CL_SUCCESS;

  std::unique_ptr<cl::Program> program(new cl::Program(pContext, sources, &error));  
  //cl::Program program  = cl::Program({*(d->getDevice())}, sources, &error);  

  if (error != CL_SUCCESS)
  {
    Log::getError().log("GPU_Device::createProgram: failed to create program with error code (%)", error);
    print_cl_error(error);
    return nullptr;
  }

  error = program.get()->build({pDevice});
  //error = program.get()->build({pDevice},"-cl-opt-disable");
  //cl::Program program  = pProgram.build({*(device->getDevice())},"-cl-opt-disable");
  //error = program.build({*(d->getDevice())},"-cl-opt-disable");

  if (error != CL_SUCCESS)
  {
    Log::getError().log("GPU_Device::createProgram: Program build failed.");
    print_cl_error(error);
    Log::getError().log("%", program.get()->getBuildInfo<CL_PROGRAM_BUILD_LOG>(pDevice));
    return nullptr;
  }
  
  pPrograms[key] = std::move(program);
  return pPrograms[key].get();
}
