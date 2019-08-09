#ifndef CL_PROGRAM_H
#define CL_PROGRAM_H

#include <vector>
#include "GPU_Device.h"

/**
 * A simple class that represents a single OpenCL program (kernel). 
 */
class CL_Program
{
  public:

    /** Creates a program.
     * @param device A pointer to the gpu_device.
     * @param src is the source code for the program. */
    bool create(const GPU_Device* device, const std::vector<std::string>& src);


  private:

    cl::Kernel pKernel; /**< The kernel. */

};

#endif
