#include "Marching_Cubes_data.h"
#include "Program.h"
#include "../OpenCL/GPU_Device.h"
#include "../OpenCL/CL_Helper.h"

std::unique_ptr<glm::vec4[]> Marching_Cubes_Data::create(const std::string& program_name,
                                                   int dimensionX,
                                                   int dimensionY,
                                                   int dimensionZ,
                                                   float block_size,
                                                   float isovalue,
                                                   glm::vec4 base_position,
                                                   int* total_count)
{

  bool succeed = true;
  std::string errorMsg;

//  Log::getDebug().log("Marching_Cubes_Data::create(%,%,%,%,%,%,%)",program_name,dimensionX,dimensionY,dimensionZ,block_size,isovalue,base_position); 
  Log::getDebug().log("Validating marching cubes data.");

  if (dimensionX <= 0 || dimensionY <= 0 || dimensionZ <= 0) {
    errorMsg += "The dimensions must be positive numbers.\n"; 
    succeed = false;
  }

  if (block_size <= 0) {
    errorMsg += "The block size must be positive float number.\n"; 
    succeed = false;
  }

  GPU_Device* d = GPU_Device::getInstance();
  cl::Program* program = d->get<cl::Program>(program_name);
  if (program == nullptr)
  {
    errorMsg += "Could't find the program.\n"; 
    succeed = false;
  }

  if (!succeed) {
    Log::getError().log("Marching_Cubes_Data::create(%,%,%,%,%,%,%)",program_name,dimensionX,dimensionY,dimensionZ,block_size,isovalue,base_position); 
    Log::getError().log("%",errorMsg); 
  }

  const static glm::ivec3 LOCAL_GROUP_SIZE(4,4,4);
  //glm::ivec3 global_group_size(dimensionX, dimensionY, dimensionZ);

  // The local group range.
  cl::NDRange local(LOCAL_GROUP_SIZE.x,LOCAL_GROUP_SIZE.y,LOCAL_GROUP_SIZE.z); 

  // The global range.
  cl::NDRange global(dimensionX*LOCAL_GROUP_SIZE.x,
                     dimensionY*LOCAL_GROUP_SIZE.y,
                     dimensionZ*LOCAL_GROUP_SIZE.z);

//  Log::getDebug().log("global[%],global[%],global[%]",global[0],global[1],global[2]);
//  Log::getDebug().log("local[%],local[%],local[%]",local[0],local[1],local[2]);

  // The maximum size of elements that marching cubes array can hold. 
  int theSIZE = global[0] * global[1] * global[2] * 4;

  // The initial value for counter. This is the value for stored vec4 values
  // after marching cubes.
  int c[] = {0};

//  // The integer constant data.  // (localX,localY,localZ,globalX,globalY_glocalZ,0,0)
//  int iConstants_data[] = {int(global[0]),int(global[1]),int(global[2]),LOCAL_GROUP_SIZE.x,LOCAL_GROUP_SIZE.y,LOCAL_GROUP_SIZE.z,0,0};
//
//  // The integer constant data.  // (base_pointX,base_pointY,base_pointZ, block_size, isovalue, 0.0f 0.0f,0.0f).
//  float fConstants_data[] = {base_position.x,
//                             base_position.y,
//                             base_position.z,
//                             block_size,
//                             isovalue,
//                             0.0f,
//                             0.0f,
//                             0.0f};

//  // The buffer for constant data (dimensions).
//  cl::Buffer* constants = d->createBuffer("constants",sizeof(int)*8, CL_MEM_READ_ONLY);
//
//  // The buffer for float constant data.
//  cl::Buffer* fConstants = d->createBuffer("fConstants",sizeof(float)*8, CL_MEM_READ_ONLY);

  // The buffer for the counter.
  cl::Buffer* counter = d->createBuffer("counter",sizeof(int), CL_MEM_READ_WRITE);

  // The buffer for the density values {f0 f1 f2 , .... }
  cl::Buffer* density_output = d->createBuffer("density_values",sizeof(float)*theSIZE,CL_MEM_READ_WRITE);

  // The buffer for the density values {f0 f1 f2 , .... }
  cl::Buffer* cubecase_output = d->createBuffer("cube_case_output",sizeof(glm::vec4)*theSIZE,CL_MEM_READ_WRITE);

  // The buffer for the marching cubes output {vvvvnnnn vvvvnnn ..... }
  cl::Buffer* mc_output = d->createBuffer("mc_output",sizeof(glm::vec4)*theSIZE,CL_MEM_READ_WRITE);

  // Create the command queue for marching cubes.
  auto command = d->get<cl::CommandQueue>("command");
  if (command == nullptr) command = d->create<cl::CommandQueue>("command");

  // Write initial value 0 to the atomic counter.
  writeToBuffer(*command, *counter, true, sizeof(int), &c);

  Log::getDebug().log("Creating kernels and setting arguments.");

  cl_int error = CL_SUCCESS;


  // THE KERNEL CREATION.

  cl_float4 b_pos;
  b_pos.x = base_position.x;
  b_pos.y = base_position.y;
  b_pos.z = base_position.z;
  b_pos.w = base_position.w;

  // The evalDentity kernel.
  cl::make_kernel<cl::Buffer, int, int, float, cl_float4> evalDensity_kernel(*program,"evalDensity",&error);
  if (error != CL_SUCCESS) print_cl_error(error);

  // The marching cubes kernel.
  cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, int, int, int, float, float, cl_float4> mc_kernel(*program,"mc",&error);
  if (error != CL_SUCCESS) print_cl_error(error);

  // THE INDICES for both evalDensity and mc.
  cl::EnqueueArgs eargs(*command, global, local);

  Log::getDebug().log("theSIZE == %",theSIZE);

  // THE EXECUTION.

  // Execute evalDensity
  evalDensity_kernel(eargs, *density_output, int(global[0]), int(global[1]), block_size, b_pos);

  // Execute marching cubes.
  mc_kernel(eargs, *density_output, *mc_output, *cubecase_output, *counter, int(global[0]), int(global[1]), int(global[2]), block_size, isovalue, b_pos).wait();

//  float eval_result[theSIZE];
//  error = command->enqueueReadBuffer(*density_output,CL_TRUE,0,sizeof(float)*theSIZE,eval_result);
//
//  for (int i=0; i<theSIZE; i++)
//  {
//     Log::getDebug().log("i == % : % ", i, eval_result[i]);
//  }

  // The marching cubes output count. The total count of the float4 values
  // (vvvv nnnn vvvv nnnn vvvv nnn.....) vvvv :: float4, nnnn :: float4
  int lkm[1] = {0};
  error = command->enqueueReadBuffer(*counter,CL_TRUE,0,sizeof(int),lkm);
  if (error != CL_SUCCESS) { print_cl_error(error); }
  
  Log::getDebug().log("lkm[0] == %", lkm[0]);
  
  // Create the array for maching cubes output.
  auto result = std::make_unique<glm::vec4[]>(lkm[0]);

  // Create the array for maching cubes output.
  auto d_values = std::make_unique<float[]>(theSIZE);
  error = command->enqueueReadBuffer(*density_output,CL_TRUE,0,sizeof(float)*theSIZE, d_values.get());
  if (error != CL_SUCCESS) { print_cl_error(error); }
  Program::density_values = std::move(d_values);

  auto c_values = std::make_unique<glm::vec4[]>(theSIZE);
  error = command->enqueueReadBuffer(*cubecase_output,CL_TRUE,0,sizeof(glm::vec4)*theSIZE, c_values.get());
  if (error != CL_SUCCESS) { print_cl_error(error); }
  Program::case_values = std::move(c_values);

  // Copy the result of marching cubes.
  error = command->enqueueReadBuffer(*mc_output,CL_TRUE,0,sizeof(glm::vec4)*lkm[0], result.get());
  if (error != CL_SUCCESS) { print_cl_error(error); }

  //for (int i=0; i< lkm[0]; i++)
  //{
  //   Log::getDebug().log("i == % : % ", i, c_values.get()[i]); // result.get()[i]);
  //}


  // Store the output count of marching cubes. This is the count of all float4
  // created in marching cubes shader. We need so we can store and draw 
  // the mc output with opengl.
  *total_count = lkm[0];

  return std::move(result);
}
