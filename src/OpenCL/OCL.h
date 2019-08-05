#ifndef OCL_H
#define OCL_H

#include <iostream>
#include <CL/cl.hpp>

class OCL 
{
  public:

    bool init();
    void createProgram();
    std::string loadSource(const std::string& fileLoc);

  private:
    cl::Device pDevice;
    cl::Context pContext;
};

#endif
