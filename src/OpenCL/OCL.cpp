#include <fstream>
#include "OCL.h"
#include "../Utils/log.h"

bool OCL::init()
{
  /* cl::Platform Provides functionality for working with OpenCL platforms. */
  std::vector<cl::Platform> all_platforms;

  /* Static method for lists all available opencl platforms. */
  /* cl_int == CL_SUCCESS. CL_INVALID_VALUE if platforms is NULL. */
  cl_int success = cl::Platform::get(&all_platforms);

  if(all_platforms.size()==0){
    std::cout<<" No platforms found. Check OpenCL installation!\n";
    return false; 
  }

  cl::Platform default_platform=all_platforms[0];

  /* cl_int cl::Platform::getInfo(cl_platform_info name, STRING_CLASS * param)
   *
   * gets specific information about the OpenCL platform.
   * Table 4.1 of the OpenCL SpecificationVersion 1.2 specifies
   * the information that can be queried.  The table below lists
   * cl_platform_info values that differ in return type between 
   * the OpenCL C API and the OpenCL C++ AP
   * 4gets specific information about the OpenCL platform.  Table 4.1 of the
   * OpenCL SpecificationVersion 1.2 specifies the information that can be
   * queried.  The table below lists cl_platform_info values that differ in
   * return type between the OpenCL C API and the OpenCL C++
   * API.cl_platform_info   |  C++ return Type
   * CL_PLATFORM_EXTENSIONS | STRING_CLASS
   * CL_PLATFORM_NAME       | STRING_CLASS 
   * L_PLATFORM_PROFILE     | STRING_CLASS 
   * L_PLATFORM_VENDOR      | STRING_CLASS 
   * L_PLATFORM_VERSION     | STRING_CLASS 
   *
   * returns GL_SUCCESS OF GL_INVALID_VALUE
   */
  std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

  //get default device of the default platform
  std::vector<cl::Device> all_devices;

  /*
    cl_int cl::Platform::getDevices(cl_device_type type,VECTOR_CLASS<Device> * devices) 

    gets the list of devices available on a platform.
    
    cl::Platform::getDevicesreturns CL_SUCCESS if the method is executed
    successfully. Otherwise, it returns one of the following
    errors: CL_INVALID_DEVICE_TYPE if typeis not a valid
    value. CL_INVALID_ARG_VALUE if devicesis NULL. CL_DEVICE_NOT_FOUND if no
    OpenCL devices matching typewere found
    */

  default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
  if(all_devices.size()==0){
    std::cout<<" No devices found. Check OpenCL installation!\n";
    return false;
  }

  /*
   * Class cl::Deviceprovides functionality for working with OpenCL devices.
   *
   * The constructor cl::Device::Device(cl_device_id device) creates an OpenCL device
   * wrapper for a device. device is an OpenCL device id. 
   */
  //cl::Device default_device=all_devices[0];
  pDevice = all_devices[0];

  /*
  template <typename T>
  cl_int cl::Device::getInfo(cl_device_info name,T* param)
  
    gets specific information about the OpenCL device. Table 4.3 of the
    OpenCL Specification Version 1.2 specifies the information that can be
    queried.  The table below lists cl_device_info values that differ in return
    type between the OpenCL C API and the OpenCL C++ API.
    cl_device_info                | C++ return Type
    CL_DEVICE_BUILT_IN_KERNELS    | STRING_CLASS
    CL_DEVICE_EXTENSIONS          | STRING_CLASS
    CL_DEVICE_MAX_WORK_ITEM_SIZES | VECTOR_CLASS<::size_t>
    CL_DEVICE_NAME                | STRING_CLASS
    CL_DEVICE_OPENCL_C_VERSION    | STRING_CLASS
    CL_DEVICE_PARTITION_PROPERTIES| VECTOR_CLASS<cl_device_partition_property>
    CL_DEVICE_PARTITION_TYPE      | VECTOR_CLASS<cl_device_partition_property>
    CL_DEVICE_PROFILE             | STRING_CLASS
    CL_DEVICE_VENDOR              | STRING_CLASS
    CL_DEVICE_VERSION             | STRING_CLASS
    CL_DRIVER_VERSION             | STRING_CLASS

    return CL_SUCCESS , CL_INVALID_VALUE
    */
  Log::getInfo().log("CL_DEVICE_NAME: %", pDevice.getInfo<CL_DEVICE_NAME>());
  Log::getInfo().log("CL_DEVICE_BUILT_IN_KERNELS: %", pDevice.getInfo<CL_DEVICE_BUILT_IN_KERNELS>());
  Log::getInfo().log("CL_DEVICE_EXTENSIONS: %", pDevice.getInfo<CL_DEVICE_EXTENSIONS>());
  Log::getInfo().log("CL_DEVICE_BUILT_IN_KERNELS: %", pDevice.getInfo<CL_DEVICE_BUILT_IN_KERNELS>());
  Log::getInfo().log("CL_DEVICE_OPENCL_C_VERSION: %", pDevice.getInfo<CL_DEVICE_OPENCL_C_VERSION>());
  Log::getInfo().log("CL_DEVICE_PROFILE: %", pDevice.getInfo<CL_DEVICE_PROFILE>());
  Log::getInfo().log("CL_DEVICE_VENDOR: %", pDevice.getInfo<CL_DEVICE_VENDOR>());
  Log::getInfo().log("CL_DEVICE_VERSION: %", pDevice.getInfo<CL_DEVICE_VERSION>());
  Log::getInfo().log("CL_DRIVER_VERSION: %", pDevice.getInfo<CL_DRIVER_VERSION>());
  Log::getInfo().log("CL_DEVICE_MAX_WORK_ITEM_SIZES: ");
  for (const auto& x : pDevice.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>()) {
    Log::getInfo().log("   %", x);
  }

  //cl::Context context({default_device});
  //
  //
  /*
  Class cl::Context 
    
  provides functionality for working with OpenCL contexts. The
    constructor
    
    cl::Context::Context(VECTOR_CLASS<Device>& devices,
                         cl_context_properties * properties= NULL,
                         void (CL_CALLBACK * pfn_notify)(const char * errorinfo,const void * private_info,
                         ::size_t cb,
                         void * user_data) =NULL,
                         void * user_data= NULL,
                         cl_int * err = NULL)
    
    creates an OpenCL context.devicesis a pointer to a vector of
    unique devices returned by cl::Platform::getDevices. If more than one device
    is specified, a selection criteria may be applied to determine if the list
    of devices specified can be used together to create a
    context. properties specifies a list of context property names and their
    corresponding values. Each property name is immediately followed by the
    corresponding desired value. The list is terminated with 0. The list of
    supported properties is described in table 4.5 of the OpenCL Specification.
    propertiescan be NULL, in which case the platform that is selected is
    implementation-defined.pfn_notifyis a callback function registered by the
    application. This callback function is used by the OpenCL implementation to
    report information on errors that occur in this context. This callback
    function may be called asynchronously by the OpenCL implementation.  It is
    the application’s responsibility to ensure that the callback function is
    thread-safe. The parameters to the callback function are:
    
    * errinfois a pointer to an error string.
    * private_info and cb represent a pointer to binary data returned by the OpenCL implementation that can be used to log
      additional information helpful in debugging the error.
    * user_data is a pointer to user supplied data. If pfn_notify is NULL,
      no callback function is registered.
    * user_datais passed as the user_dataargument when pfn_notifyis
    called. user_datacan be NULL.errreturns an appropriate error code. If erris
    NULL, no error code is returned.cl::Context::Contextreturns a valid object
    of type cl::Contextand sets errto CL_SUCCESS if it creates the context
    successfully. Otherwise, it returns one of the following error values in
    err:CL_INVALID_PROPERTY if context property name in properties is not a
    supported property name, if the value specified for a supported property
    name is not valid, or if the same property name is specified more than
    once.CL_INVALID_VALUE if devicesis of length zero.CL_INVALID_VALUE if
    pfn_notifyis NULL but user_datais not NULL.CL_INVALID_DEVICE if
    devicescontains an invalid device.CL_DEVICE_NOT_AVAILABLE if a device in
    devicesis currently not available even though the device wasreturned by
    cl::Platform::getDevices.CL_OUT_OF_HOST_MEMORY if there is a failure to
    allocate resources required by the OpenCL implementation on the host.The
    constructorcl::Context::Context(cl_device_type type,cl_context_

    */
  pContext = cl::Context({pDevice});
  //if pContext 

//    cl::Context context({default_device});

  return true;
}

std::string OCL::loadSource(const std::string& fileLoc)
{
    std::ifstream file(fileLoc);

    if (!file.is_open())
    {
      Log::getError().log("Shader::loadSource(%): Failed to load source.","fileLoc");
      throw std::runtime_error("Shader::loadSource: counld'n open file '" + fileLoc + "'.");
    }

    std::string str;
    file.seekg(0, std::ios::end);
    str.reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    str.assign((std::istreambuf_iterator<char>(file)),
               std::istreambuf_iterator<char>());

    return str;
}

void OCL::createProgram()
{

    cl::Program::Sources sources;

    // kernel calculates for each element C=A+B
    std::string kernel_code = loadSource("shaders/mc.cl"); 
    sources.push_back({kernel_code.c_str(),kernel_code.length()});

    cl::Program program(pContext,sources);
    if (program.build({pDevice})!=CL_SUCCESS){
        std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(pDevice)<<"\n";
        exit(1);
    }

    // create buffers on the device
    cl::Buffer buffer_A(pContext,CL_MEM_READ_WRITE,sizeof(int)*10);
    cl::Buffer buffer_B(pContext,CL_MEM_READ_WRITE,sizeof(int)*10);
    cl::Buffer buffer_C(pContext,CL_MEM_READ_WRITE,sizeof(int)*10);

    int A[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int B[] = {0, 1, 2, 0, 1, 2, 0, 1, 2, 0};

    //create queue to which we will push commands for the device.
    cl::CommandQueue queue(pContext,pDevice);

    //write arrays A and B to the device
    queue.enqueueWriteBuffer(buffer_A,CL_TRUE,0,sizeof(int)*10,A);
    queue.enqueueWriteBuffer(buffer_B,CL_TRUE,0,sizeof(int)*10,B);


    //run the kernel
    //cl::KernelFunctor simple_add(cl::Kernel(program,"simple_add"),queue,cl::NullRange,cl::NDRange(10),cl::NullRange);
    //cl::make_kernel simple_add(program,"simple_add");
    cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer> simple_add(cl::Kernel(program,"simple_add"));
    cl::EnqueueArgs eargs(queue, cl::NullRange, cl::NDRange(10), cl::NullRange);
    simple_add(eargs, buffer_A,buffer_B,buffer_C).wait();

    // 1.1
    //simple_add(cl::Kernel(program,"simple_add"),queue,cl::NullRange,cl::NDRange(10),cl::NullRange);

    // 1.2
    //cl::make_kernel = simple_add(cl::Kernel(program,"simple_add"));
    //auto simple_add = cl::make_kernel(program, "simple_add");
    //cl::EnqueueArgs eargs(queue,cl::NullRange,cl::NDRange(10),cl::NullRange);
    //simple_add(eargs, buffer_A,buffer_B,buffer_C).wait();

    //cl::make_kernel simple_add(cl::Kernel(program, "simple_add"));
    //cl::EnqueueArgs eargs(queue, cl::NullRange, cl::NDRange(10), cl::NullRange);
    //simple_add(eargs, buffer_A, buffer_B, buffer_C).wait();
    //simple_add(buffer_A,buffer_B,buffer_C);

    //alternative way to run the kernel
    /*cl::Kernel kernel_add=cl::Kernel(program,"simple_add");
    //kernel_add.setArg(0,buffer_A);
    //kernel_add.setArg(1,buffer_B);
    //kernel_add.setArg(2,buffer_C);
    //queue.enqueueNDRangeKernel(kernel_add,cl::NullRange,cl::NDRange(10),cl::NullRange);
    //queue.enqueueNDRangeKernel(simple_add,cl::NullRange,cl::NDRange(10),cl::NullRange);
    //queue.finish();*/

    int C[10];
    //read result C from the device to array C
    queue.enqueueReadBuffer(buffer_C,CL_TRUE,0,sizeof(int)*10,C);

    std::cout<<" result: \n";
    for(int i=0;i<10;i++){
        std::cout<<C[i]<<" ";
    }
}
