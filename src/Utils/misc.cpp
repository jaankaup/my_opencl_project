#include "misc.h"

std::string addPadding(const std::string& str, const uint32_t count, const char paddingChar)
{
  std::string result = str;
  if (count > str.size())
  {
    result.insert(0,count - str.size(), paddingChar);
  }
  return result;
}

void logGLM(const std::string& name, const glm::vec2& object)
{
  Log::getDebug().log("% = (%,%)",name,std::to_string(object.x),std::to_string(object.y));
}
   
void logGLM(const std::string& name, const glm::vec3& object)
{
  Log::getDebug().log("% = (%,%,%)",name,std::to_string(object.x),std::to_string(object.y),std::to_string(object.z));
}

void logGLM(const std::string& name, const glm::vec4& object)
{
  Log::getDebug().log("% = (%,%,%,%)",name,std::to_string(object.x),std::to_string(object.y),std::to_string(object.z),std::to_string(object.w));
}

void logGLM(const std::string& name, const glm::mat3& object)
{
  std::string padded = addPadding("",name.size(),' ');
  Log::getDebug().log("% = (%,%,%)",name   ,std::to_string(object[0][0]),std::to_string(object[0][1]),std::to_string(object[0][2]));
  Log::getDebug().log("%   (%,%,%)",padded ,std::to_string(object[1][0]),std::to_string(object[1][1]),std::to_string(object[1][2]));
  Log::getDebug().log("%   (%,%,%)",padded ,std::to_string(object[2][0]),std::to_string(object[2][1]),std::to_string(object[2][2]));
}

void logGLM(const std::string& name, const glm::mat4& object)
{
  std::string padded = addPadding("",name.size(),' ');
  Log::getDebug().log("% = (%,%,%,%)",name   ,std::to_string(object[0][0]),std::to_string(object[0][1]),std::to_string(object[0][2]),std::to_string(object[0][3]));
  Log::getDebug().log("%   (%,%,%,%)",padded ,std::to_string(object[1][0]),std::to_string(object[1][1]),std::to_string(object[1][2]),std::to_string(object[1][3]));
  Log::getDebug().log("%   (%,%,%,%)",padded ,std::to_string(object[2][0]),std::to_string(object[2][1]),std::to_string(object[2][2]),std::to_string(object[2][3]));
  Log::getDebug().log("%   (%,%,%,%)",padded ,std::to_string(object[3][0]),std::to_string(object[3][1]),std::to_string(object[3][2]),std::to_string(object[3][3]));
}

TextureData createPerlin3D(const int width, const int height, const int depth)
{
  int size = width*height*depth*4;
  TextureData td(size,width,height,depth);
  
  auto data = td.getData();

  MyRandom<double> mr;
  mr.setDistribution(0,250000.0);

  siv::PerlinNoise pn(mr());
  siv::PerlinNoise pn2(mr());
  siv::PerlinNoise pn3(mr());
  siv::PerlinNoise pn4(mr());

  const double fx = width; 
  const double fy = height;
  const double fz = depth; 

  unsigned int xOffset = 4;
  unsigned int yOffset = 4 * width;
  unsigned int zOffset = 4 * width * height;
  unsigned int lkm = 0;
  for (int z = 0; z < depth ; z++) {
  for (int y = 0; y < height ; y++){
  for (int x = 0; x < width ; x++) {
	unsigned int position = x*xOffset + y*yOffset + z*zOffset;
    data[position] = pn.noise0_1(x / fx, y / fy, z / fz) * 255;
    data[position+1] = pn2.noise0_1(x / fx, y / fy, z / fz) * 255;
    data[position+2] = pn3.noise0_1(x / fx, y / fy, z / fz) * 255; 
    data[position+3] = pn4.noise0_1(x / fx, y / fy, z / fz) * 255;
	lkm++;
  }}};

  return std::move(td);
}

TextureData createPerlin3D_rough(const int width, const int height, const int depth)
{
  int size = width*height*depth*4;
  TextureData td(size,width,height,depth);
  
  auto data = td.getData();

  MyRandom<double> mr;
  mr.setDistribution(0,250000.0);

  siv::PerlinNoise pn(mr());
  siv::PerlinNoise pn2(mr());
  siv::PerlinNoise pn3(mr());
  siv::PerlinNoise pn4(mr());

  const double fx = width; // / frequency;
  const double fy = height; // / frequency;
  const double fz = depth; // / frequency;

  MyRandom<int> mr2;
  mr2.setDistribution(0,255);

  unsigned int xOffset = 4;
  unsigned int yOffset = 4 * width;
  unsigned int zOffset = 4 * width * height;
  unsigned int lkm = 0;
  for (int z = 0; z < depth ; z++) {
  for (int y = 0; y < height ; y++){
  for (int x = 0; x < width ; x++) {
	  unsigned int position = x*xOffset + y*yOffset + z*zOffset;
    data[position] = pn.octaveNoise0_1(x/fx, y/fy, z/fz, 2) * 255;
    data[position+1] = pn2.octaveNoise0_1(x/fx, y/fy, z/fz, 2) * 255;
    data[position+2] = pn3.octaveNoise0_1(x / fx, y / fy, z / fz, 2) * 255;
    data[position+3] = pn4.octaveNoise0_1(x / fx, y / fy, z / fz, 2) * 255;
	lkm++;
  }}};

  return std::move(td);
}

int getLeft(const int index, const int width, const int height)
{
  if (index % width == 0) return -1;  
  return index - 1;
};

int getRight(const int index, const int width, const int height)
{
  if (index % width == width-1) return -1;  
  return index + 1;
};

int getUp(const int index, const int width, const int height)
{
  if (index - width < 0) return -1;  
  return index - width;
};

int getBottom(const int index, const int width, const int height)
{
  if (index + width > width*height - 1) return -1;  
  return index + width;
};

int getUpLeft(const int index, const int width, const int height)
{
  if (getLeft(index,width,height) == -1 || getUp(index,width,height) == -1) return -1;
  return index - width - 1;
};

int getUpRight(const int index, const int width, const int height)
{
  if (getRight(index,width,height) == -1 || getUp(index,width,height) == -1) return -1;
  return index - width + 1;
};

int getBottomLeft(const int index, const int width, const int height)
{
  if (getBottom(index,width,height) == -1 || getLeft(index,width,height) == -1) return -1;
  return index + width - 1;
};

int getBottomRight(const int index, const int width, const int height)
{
  if (getBottom(index,width,height) == -1 || getRight(index,width,height) == -1) return -1;
  return index + width + 1;
};
