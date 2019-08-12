#include "Program/Program.h"

int main(int argc, char* argv[])
{

  Program::MainProgram prog;
  if (!prog.initialize()) return 1;
  prog.start();
  return 0;
}
