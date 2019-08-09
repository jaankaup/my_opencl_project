#ifndef CL_PROGRAM_H
#define CL_PROGRAM_H

#include <vector>
#include "GPU_Device.h"
#include "../Utils/static_string.h"

/**
 * A simple class that represents a single OpenCL program (kernel). 
 */
class CL_Program
{
  public:

    /** 
     * Creates and compiles the program. Also creates the kernel.
     * @param device A pointer to the gpu_device.
     * @param src is the source code for the program.
     * @param return Was the program created succesfully.
     * */
    bool create(GPU_Device* device, const cl::Program::Sources& src);

    /**
     * Adds one value to the kernel.
     * @param index is the index of the arg.
     * @param value is the value.
     * @param return Was the add operation succeeded.
     */
    template<typename T>
    bool setArg(const int index, T value)
    {
      cl_int error = pKernel.setArg(index, value);

      if (error != CL_SUCCESS)
      {
        Log::getError().log("CL_Program::setArg(%,%): failed to add arg with error code (%)", error);
        return false;
      }
      return true;
    }

  private:

    cl::Program pProgram; /**< The cl::Program instance. */

//    template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24, class T25, class T26, class T27, class T28, class T29, class T30, class T31>
//    cl::make::kernel pKernel;
    cl::Kernel pKernel; /**< The cl::Kernel instance. */

};

#endif
