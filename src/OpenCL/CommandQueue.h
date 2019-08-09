#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H

#include "GPU_Device.h"

class CommandQueue
{
  public:

    bool create(GPU_Device* device);

  private:

    cl::CommandQueue pQueue;

};

#endif
