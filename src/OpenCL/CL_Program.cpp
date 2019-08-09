#include "CL_Program.h"
#include "../Utils/Helper.h"

/////////////////////////////////////////////////////////////////////////////////////

bool CL_Program::create(GPU_Device* device, const cl::Program::Sources& src)
{
  assert(device != nullptr);
  
  auto context_ptr = device->getContext();

  assert(context_ptr != nullptr);

  cl_int error;

  pProgram = cl::Program(*context_ptr, src, &error);  

  if (error != CL_SUCCESS)
  {
    Log::getError().log("CL_Program::create: failed to create program with error code (%)", error);
    return false;
  }

  error = pProgram.build({*(device->getDevice())},0);

  if (error != CL_SUCCESS)
  {
    Log::getError().log("CL_Program::create: Program build failed.");
    Log::getError().log("%", pProgram.getBuildInfo<CL_PROGRAM_BUILD_LOG>(*(device->getDevice())));
    return false;
  }

  pKernel = cl::Kernel(pProgram, "juu", &error);
  
  if (error != CL_SUCCESS)
  {
    Log::getError().log("CL_Program::create: Kernel creation failed.");
    return false;
  }
//  std::cout << type_name<decltype(cl::make_kernel<cl::Buffer&, int>(pProgram, "Ohojelma"))>() << std::endl;

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////
