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
     * @param data A pointer to the data.
     * @param data The size of the data.
     */
    bool create(const GPU_Device* device, const void* data, uint32_t size);

  private:
    cl::Buffer pBuffer; /**< The actual buffer. */

};

#endif
