#include "Marching_Cubes_data.h"
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
                     dimensionX*LOCAL_GROUP_SIZE.y,
                     dimensionX*LOCAL_GROUP_SIZE.z);

  Log::getDebug().log("global[%],global[%],global[%]",global[0],global[1],global[2]);
  Log::getDebug().log("local[%],local[%],local[%]",local[0],local[1],local[2]);

  // The maximum size of elements that marching cubes array can hold. 
  int theSIZE = global[0] * global[1] * global[2] * 4;

  Log::getDebug().log("YEAH1.");
  // The initial value for counter. This is the value for stored vec4 values
  // after marching cubes.
  int c[] = {0};

  // The integer constant data.  // (localX,localY,localZ,globalX,globalY_glocalZ,0,0)
  int iConstants_data[] = {int(global[0]),int(global[1]),int(global[2]),LOCAL_GROUP_SIZE.x,LOCAL_GROUP_SIZE.y,LOCAL_GROUP_SIZE.z,0,0};

  // The integer constant data.  // (base_pointX,base_pointY,base_pointZ, block_size, isovalue, 0.0f 0.0f,0.0f).
  float fConstants_data[8] = {base_position.x,
                              base_position.y,
                              base_position.z,
                              block_size,
                              isovalue,
                              0.0f,0.0f,0.0f};

  // The buffer for constant data (dimensions).
  cl::Buffer* constants = d->createBuffer("constants",sizeof(int)*8, CL_MEM_READ_ONLY);

  // The buffer for float constant data.
  cl::Buffer* fConstants = d->createBuffer("fConstants",sizeof(float)*8, CL_MEM_READ_ONLY);

  // The buffer for the counter.
  cl::Buffer* counter = d->createBuffer("counter",sizeof(int), CL_MEM_READ_WRITE);

  // The buffer for the density values {f0 f1 f2 , .... }
  cl::Buffer* density_output = d->createBuffer("density_values",sizeof(float)*theSIZE,CL_MEM_READ_WRITE);

  // The buffer for the marching cubes output {vvvvnnnn vvvvnnn ..... }
  cl::Buffer* mc_output = d->createBuffer("mc_output",sizeof(glm::vec4)*theSIZE,CL_MEM_READ_WRITE);

  // Create the command queue for marching cubes.
  auto command = d->get<cl::CommandQueue>("command");
  if (command == nullptr) command = d->create<cl::CommandQueue>("command");

  // Write 0 to the atomic counter.
  writeToBuffer(*command, *counter, true, sizeof(int), &c);

  // Write the integer contant data.
  writeToBuffer(*command, *constants, true, sizeof(int)*8,&iConstants_data);

  // Write the float contant data.
  writeToBuffer(*command, *fConstants, true, sizeof(float)*8,&fConstants_data);

  Log::getDebug().log("Creating kernels and setting arguments.");

  cl_int error = CL_SUCCESS;


  // THE KERNEL CREATION.

  // The evalDentity kernel.
  cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer> evalDensity_kernel(*program,"evalDensity",&error);
  if (error != CL_SUCCESS) print_cl_error(error);

  // The marching cubes kernel.
  cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer> mc_kernel(*program,"mc",&error);
  if (error != CL_SUCCESS) print_cl_error(error);

  // THE INDICES for both evalDensity and mc.
  cl::EnqueueArgs eargs(*command, global, local);

  Log::getDebug().log("theSIZE == %",theSIZE);

  // THE EXECUTION.

  // Execute evalDensity
  evalDensity_kernel(eargs, *constants, *fConstants, *density_output);

  // Execute marching cubes.
  mc_kernel(eargs, *constants, *fConstants, *density_output, *mc_output, *counter).wait();

//  float eval_result[theSIZE];
//  error = command->enqueueReadBuffer(*density_output,CL_TRUE,0,sizeof(float)*theSIZE,eval_result);
//
//  for (int i=0; i<theSIZE; i++)
//  {
//     Log::getDebug().log("i == % : % ", i, eval_result[i]);
//  }

  // MC
//  Log::getDebug().log("YEAH3.");
  int lkm[1] = {6};
  error = command->enqueueReadBuffer(*counter,CL_TRUE,0,sizeof(int),lkm);
  if (error != CL_SUCCESS) { print_cl_error(error); }
//
  Log::getDebug().log("lkm[0] == %", lkm[0]);
//
//  auto result = std::make_unique<glm::vec4[]>(1);
    auto result = std::make_unique<glm::vec4[]>(lkm[0]);
//  glm::vec4 bee[lkm];

//  glm::vec4 joo[lkm[0]+1];
//  joo[0] = glm::vec4(1.1f,2.2f,3.3f,4.4f);
  error = command->enqueueReadBuffer(*mc_output,CL_TRUE,0,sizeof(glm::vec4)*lkm[0], result.get());
//////  error = command->enqueueReadBuffer(mc_output,CL_TRUE,0,sizeof(glm::vec4)*lkm[0], result.get());
////  if (error != CL_SUCCESS) { print_cl_error(error); }
////
//////
//  Log::getDebug().log("YEAH4.");
//  for (int i=0; i< lkm[0]; i++)
//  {
//     Log::getDebug().log("i == % : % ", i, result.get()[i]); // result.get()[i]);
//  }
//
////  auto vb = ResourceManager::getInstance()->create<Vertexbuffer>("hah");
////  vb->init(GL_ARRAY_BUFFER,GL_STATIC_DRAW);
////  std::vector<std::string> types = {"4f","4f"};
////  vb->addData(bee,sizeof(glm::vec4)*(lkm[0]+1),types);
////  vb->setCount((lkm[0]+1));
////  Log::getError().log("lkm == %",lkm[0]+1);
////  delete[] bee;


  *total_count = lkm[0];
  Log::getDebug().log("Before return");
  return std::move(result);
}
