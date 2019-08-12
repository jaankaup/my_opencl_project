#include "CL_Buffer.h"

/////////////////////////////////////////////////////////////////////////////////////

bool CL_Buffer::create(GPU_Device* device, const cl_mem_flags flags, uint32_t size)
{
  assert(device != nullptr); 
  assert(data != nullptr); 

  assert(flags == CL_MEM_READ_WRITE ||
         flags == CL_MEM_WRITE_ONLY ||
         flags == CL_MEM_READ_ONLY ||
         flags == CL_MEM_USE_HOST_PTR ||
         flags == CL_MEM_ALLOC_HOST_PTR ||
         flags == CL_MEM_COPY_HOST_PTR);


  auto context_ptr = device->getContext();

  assert(context_ptr != nullptr); 

  cl_int error;

  pBuffer = cl::Buffer(*context_ptr, flags, size, 0, &error);  
  
  if (error != CL_SUCCESS)
  {
    Log::getError().log("CL_Buffer::create: failed to create buffer with error code (%)", error);
    return false;
  }

  pInitialized = true;

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////

bool CL_Buffer::addData(GPU_Device* device, const void* data, uint32_t size)
{
  assert(pInitialized);

  cl_int error;

  error = device->getCommandQueue()->enqueueWriteBuffer(pBuffer, CL_FALSE, 0, size, data, 0, 0);  

  if (error != CL_SUCCESS)
  {
    Log::getError().log("CL_Buffer::addData: failed to add data to the  buffer with error code (%)", error);
    Log::getError().log("%",errorcode_toString(error));
    return false;
  }

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////

bool CL_Buffer::getData(GPU_Device* device, const bool blocking, void* dest_buffer, size_t size)
{
  assert(pInitialized);

  cl_int error;
  error = device->getCommandQueue()->enqueueReadBuffer(pBuffer, blocking, 0, size, dest_buffer, 0, 0);

  if (error != CL_SUCCESS)
  {
    Log::getError().log("CL_Buffer::addData: failed to add data to the buffer with error code (%)", error);
    Log::getError().log("%",errorcode_toString(error));
    return false;
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////

cl::Buffer* CL_Buffer::getBuffer()
{
  if (!pInitialized) return nullptr;
  return &pBuffer;
}
