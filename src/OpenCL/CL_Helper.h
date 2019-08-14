#ifndef CL_HELPER_H
#define CL_HELPER_H

#include <string>
#include <functional>
#include <CL/cl.hpp>

//__kernel void mc(__global float3* base_points, __global float* base_values, int n)

typedef cl::make_kernel<cl::Buffer&, cl::Buffer&, int> create_EvalDensity_kernel;  
typedef std::function<create_EvalDensity_kernel::type_> EvalDensity_kernelType;  
//typedef cl::make_kernel<cl::Buffer&, cl::Buffer&, int> create_EvalDensity_kernel;  

extern std::string errorcode_toString(const cl_int errorCode);

#endif
