#include "CL_Program.h"
#include "../Utils/Helper.h"

/////////////////////////////////////////////////////////////////////////////////////

bool CL_Program::create(GPU_Device* device, const cl::Program::Sources& src, const std::string kernel_name)
{
  assert(device != nullptr);
  
  auto context_ptr = device->getContext();

  assert(context_ptr != nullptr);

  cl_int error;

  pProgram = cl::Program(*context_ptr, src, &error);  

  if (error != CL_SUCCESS)
  {
    Log::getError().log("CL_Program::create: failed to create program with error code (%)", error);
    Log::getError().log("%",errorcode_toString(error));
    return false;
  }

  error = pProgram.build({*(device->getDevice())},0);

  if (error != CL_SUCCESS)
  {
    Log::getError().log("CL_Program::create: Program build failed.");
    Log::getError().log("%",errorcode_toString(error));
    Log::getError().log("%", pProgram.getBuildInfo<CL_PROGRAM_BUILD_LOG>(*(device->getDevice())));
    return false;
  }

  pInitialized = true;
  return true;

////  pKernel = cl::Kernel(pProgram, kernel_name.c_str(), &error);
////  
////  if (error != CL_SUCCESS)
////  {
////    Log::getError().log("CL_Program::create: Kernel creation failed.");
////    Log::getError().log("%",errorcode_toString(error));
////    return false;
////  }
//////  std::cout << type_name<decltype(cl::make_kernel<cl::Buffer&, int>(pProgram, "Ohojelma"))>() << std::endl;
////  pInitialized = true;
////  return true;
}

/////////////////////////////////////////////////////////////////////////////////////

cl::Kernel* CL_Program::getKernel()
{
  if (pInitialized) return &pKernel;
  return nullptr;
}

cl::Program* CL_Program::getProgram()
{
  if (pInitialized) return &pProgram;
  return nullptr;
}
