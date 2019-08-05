#ifndef HELPERF_H
#define HELPERF_H

#include <iostream>
#include <string>
#include <string>
#include <fstream>
#include <SDL2/SDL.h>

/* Helper functions. */

namespace Helper {


/* Loads a text content from a given file location. Return the content as a
 * string. Throws an exception if the function fails. */
extern std::string loadSource(const std::string& fileLoc);

extern bool initSDL();

};

#endif
