#ifndef MARCHING_CUBES_DATA
#define MARCHING_CUBES_DATA

#include <memory>
#include <glm/glm.hpp>
/**
 * A simple class for holding parameters for marching cubes algoritm.
 */
class Marching_Cubes_Data {

  public:
    
    Marching_Cubes_Data() {};
    ~Marching_Cubes_Data() {};

    /**
     * @param program_name is the name of the opencl-progam which is used to
     * run marching cubes. Is program is not found the process will be
     * interrupted. 
     * @param dimenxionX The x-dimension of the marhcing cubes area. Must be a positive integer. 
     * @param dimenxionY The y-dimension of the marhcing cubes area. Must be a positive integer. 
     * @param dimenxionZ The z-dimension of the marhcing cubes area. Must be a positive integer. 
     * @param block_size Must be a positive float. This specifies the cube side length.
     * @param isovalue The isovalue for mc.
     * @param base_position The left-bottom point of the marching cubes area.
     * @param total_count Pointer to a int value. The total count of data if
     * to this location.
     * @param return The result of marching cubes algorighm.
     */
    std::unique_ptr<glm::vec4[]> create(const std::string& program_name,
                                  int dimensionX,
                                  int dimensionY,
                                  int dimensionZ,
                                  float block_size,
                                  float isovalue,
                                  glm::vec4 base_position,
                                  int* total_count);

  private:

//    const std::string& pVertex_buffer_name;
//    const std::string& pProgram_name;
//    int pDimensionX;
//    int pDimensionY;
//    int pDimensionZ;
//    float pBlock_size;
//    glm::vec4 pBase_position;
//    bool pInitialized = false;
};

#endif
