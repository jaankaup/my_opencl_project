#include "CommandQueue.h"

bool CommandQueue::create(GPU_Device* device)
{
  assert(device != nullptr);
  
  auto context_ptr = device->getContext();

  assert(context_ptr != nullptr);

  cl_int error;

  pQueue = cl::CommandQueue(*(device->getContext()), *device->getDevice(), 0, &error);

  if (error != CL_SUCCESS)
  {
    Log::getError().log("CL_CommandQueue::create: failed to create cl::CommandQueue with error code (%)", error);
    return false;
  }

}
