#ifndef HELPERF_H
#define HELPERF_H

#include <iostream>
#include <string>
#include <string>
#include <fstream>
#include <SDL2/SDL.h>

/* Helper functions. */

namespace Helper {

/**
 * Loads a text content from a given file location. Return the content as a
 * string. Throws an exception if the function fails. */
extern std::string loadSource(const std::string& fileLoc);

/**
 * Initialized sd_video. @param return The result of the initialization. 
 */
extern bool initSDL();

/**
 * Return the marching cubes triTable in float[] array format. 
 * @param return A unique_ptr to triTable.
 */
//extern float* getTritable();

};

#endif
