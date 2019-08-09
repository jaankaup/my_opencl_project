#ifndef CL_BUFFER_H
#define CL_BUFFER_H

#include "../Program/includes.h"
#include "GPU_Device.h"

/**
 * A class for representing a opencl buffer.
 */
class CL_Buffer
{
  public:

    /** 
     * Creates a new buffer on GPU_Devices context.
     * @param device A pointer to the GPU_Device
     * @param flags Must be one of cl_mem_flags
     * @param data The size of the data.
     * @param return Was the buffer created succesfully.
     */
    bool create(GPU_Device* device, const cl_mem_flags flags, uint32_t size);

    /** 
     * Add data to the buffer.
     * @param data Pointer to the data.
     * @param size The size of data.
     * @param return Was the data added succesfully.
     */
    bool addData(const void* data, uint32_t size);

  private:

    cl::Buffer pBuffer; /**< The actual buffer. */

    bool pInitialized = false; /**< Is the buffer initialized properly. */

};

#endif
