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
     * Add data to the buffer (GPU_Devices CommandQueue).
     * @param device Pointer to the device.
     * @param data Pointer to the data.
     * @param size The size of data.
     * @param return Was the data added succesfully.
     */
    bool addData(GPU_Device* device, const void* data, uint32_t size);

    /** 
     * Get data from the GPU_Devices CommandQueue.
     * @param device Pointer to the device.
     * @param blocking Should this operation block.
     * @param dest_buffer is the pointer to the destination buffer.
     * @param size The size of data.
     */
    bool getData(GPU_Device* device, const bool blocking, void* dest_buffer, size_t size);

    /** 
     * Get the pointer to the cl:Buffer object.
     * @param return The pointer to the cl::Buffer. Is CL_Buffer is not
     * initialized properly, return nullptr;
     */
    cl::Buffer* getBuffer();

  private:

    cl::Buffer pBuffer; /**< The actual buffer. */

    bool pInitialized = false; /**< Is the buffer initialized properly. */

};

#endif
