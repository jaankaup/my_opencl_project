#include "texture.h"

Texture::Texture()
{
}

Texture::~Texture()
{
    if (pId != 0) glDeleteTextures(1,&pId);
}

void Texture::init(const TextureType t)
{
    GLuint i = 0;
    glGenTextures(1,&i);
    pType = t;
    pId = i;
}

void Texture::create3D(const TextureData& td)
{
  use(0);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, td.getWidth(), td.getHeight(), td.getDepth(), 0, GL_RGBA, GL_UNSIGNED_BYTE, td.getData());
  pTextureSize = td.getDataSize()*sizeof(uint8_t); // Onkohan oikein?
}

std::tuple<std::unique_ptr<float[]>,unsigned int> Texture::getTextureData()
{
  auto result = std::unique_ptr<GLfloat[]>(new GLfloat[pTextureSize]);
  glGetTexImage(GL_TEXTURE_3D, 0, GL_RGBA, GL_FLOAT, result.get());
//  glGetNamedBufferSubData(pId, 0, pData_size, result.get());
//        unsigned int pTextureSize = 0;
  return std::make_tuple(std::move(result), pTextureSize);
}


/* For more information: https://open.gl/textures */
void Texture::create(const std::string& fileloc)
{
    Log::getDebug().log("Texture::create(%): Creating texture from file.",fileloc);
    int width, height;
    unsigned char* image = SOIL_load_image(fileloc.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

    if (image == 0)
    {
        Log::getError().log("Error: Texture::create(%)", fileloc);
        Log::getError().log("%",SOIL_last_result());
    }

    use(0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
}

void Texture::bind() const
{
  switch (pType)
  {
    case TextureType::d1:
      glBindTexture(GL_TEXTURE_1D,pId);
      break;
    case TextureType::d2:
      glBindTexture(GL_TEXTURE_2D,pId);
      break;
    case TextureType::d3:
      glBindTexture(GL_TEXTURE_3D,pId);
      break;
  }
}

void Texture::use(const GLuint unit) const
{
  glActiveTexture(GL_TEXTURE0 + unit);
  bind();
}

TextureType Texture::getTextureType() const
{
  return pType;
}

void Texture::create_tritable_texture()
{
    Log::getDebug().log("Texture::create_tritable_texture()");
    int width =  1280;
    auto data = new uint8_t[3840] {
   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 , // Case: 0
    0,  8,  3, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,    // Case: 1
    0,  1,  9, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,    // Case: 2
    1,  8,  3,  9,  8,  1, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    1,  2, 10, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    0,  8,  3,  1,  2, 10, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    9,  2, 10,  0,  2,  9, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    2,  8,  3,  2, 10,  8, 10,  9,  8, 255, 255, 255, 255, 255, 255 ,
    3, 11,  2, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    0, 11,  2,  8, 11,  0, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    1,  9,  0,  2,  3, 11, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    1, 11,  2,  1,  9, 11,  9,  8, 11, 255, 255, 255, 255, 255, 255 ,
    3, 10,  1, 11, 10,  3, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    0, 10,  1,  0,  8, 10,  8, 11, 10, 255, 255, 255, 255, 255, 255 ,
    3,  9,  0,  3, 11,  9, 11, 10,  9, 255, 255, 255, 255, 255, 255 ,
    9,  8, 10, 10,  8, 11, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    4,  7,  8, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    4,  3,  0,  7,  3,  4, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    0,  1,  9,  8,  4,  7, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    4,  1,  9,  4,  7,  1,  7,  3,  1, 255, 255, 255, 255, 255, 255 ,
    1,  2, 10,  8,  4,  7, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    3,  4,  7,  3,  0,  4,  1,  2, 10, 255, 255, 255, 255, 255, 255 ,
    9,  2, 10,  9,  0,  2,  8,  4,  7, 255, 255, 255, 255, 255, 255 ,
    2, 10,  9,  2,  9,  7,  2,  7,  3,  7,  9,  4, 255, 255, 255 ,
    8,  4,  7,  3, 11,  2, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
   11,  4,  7, 11,  2,  4,  2,  0,  4, 255, 255, 255, 255, 255, 255 ,
    9,  0,  1,  8,  4,  7,  2,  3, 11, 255, 255, 255, 255, 255, 255 ,
    4,  7, 11,  9,  4, 11,  9, 11,  2,  9,  2,  1, 255, 255, 255 ,
    3, 10,  1,  3, 11, 10,  7,  8,  4, 255, 255, 255, 255, 255, 255 ,
    1, 11, 10,  1,  4, 11,  1,  0,  4,  7, 11,  4, 255, 255, 255 ,
    4,  7,  8,  9,  0, 11,  9, 11, 10, 11,  0,  3, 255, 255, 255 ,
    4,  7, 11,  4, 11,  9,  9, 11, 10, 255, 255, 255, 255, 255, 255 ,
    9,  5,  4, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    9,  5,  4,  0,  8,  3, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    0,  5,  4,  1,  5,  0, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    8,  5,  4,  8,  3,  5,  3,  1,  5, 255, 255, 255, 255, 255, 255 ,
    1,  2, 10,  9,  5,  4, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    3,  0,  8,  1,  2, 10,  4,  9,  5, 255, 255, 255, 255, 255, 255 ,
    5,  2, 10,  5,  4,  2,  4,  0,  2, 255, 255, 255, 255, 255, 255 ,
    2, 10,  5,  3,  2,  5,  3,  5,  4,  3,  4,  8, 255, 255, 255 ,
    9,  5,  4,  2,  3, 11, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    0, 11,  2,  0,  8, 11,  4,  9,  5, 255, 255, 255, 255, 255, 255 ,
    0,  5,  4,  0,  1,  5,  2,  3, 11, 255, 255, 255, 255, 255, 255 ,
    2,  1,  5,  2,  5,  8,  2,  8, 11,  4,  8,  5, 255, 255, 255 ,
   10,  3, 11, 10,  1,  3,  9,  5,  4, 255, 255, 255, 255, 255, 255 ,
    4,  9,  5,  0,  8,  1,  8, 10,  1,  8, 11, 10, 255, 255, 255 ,
    5,  4,  0,  5,  0, 11,  5, 11, 10, 11,  0,  3, 255, 255, 255 ,
    5,  4,  8,  5,  8, 10, 10,  8, 11, 255, 255, 255, 255, 255, 255 ,
    9,  7,  8,  5,  7,  9, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    9,  3,  0,  9,  5,  3,  5,  7,  3, 255, 255, 255, 255, 255, 255 ,
    0,  7,  8,  0,  1,  7,  1,  5,  7, 255, 255, 255, 255, 255, 255 ,
    1,  5,  3,  3,  5,  7, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    9,  7,  8,  9,  5,  7, 10,  1,  2, 255, 255, 255, 255, 255, 255 ,
   10,  1,  2,  9,  5,  0,  5,  3,  0,  5,  7,  3, 255, 255, 255 ,
    8,  0,  2,  8,  2,  5,  8,  5,  7, 10,  5,  2, 255, 255, 255 ,
    2, 10,  5,  2,  5,  3,  3,  5,  7, 255, 255, 255, 255, 255, 255 ,
    7,  9,  5,  7,  8,  9,  3, 11,  2, 255, 255, 255, 255, 255, 255 ,
    9,  5,  7,  9,  7,  2,  9,  2,  0,  2,  7, 11, 255, 255, 255 ,
    2,  3, 11,  0,  1,  8,  1,  7,  8,  1,  5,  7, 255, 255, 255 ,
   11,  2,  1, 11,  1,  7,  7,  1,  5, 255, 255, 255, 255, 255, 255 ,
    9,  5,  8,  8,  5,  7, 10,  1,  3, 10,  3, 11, 255, 255, 255 ,
    5,  7,  0,  5,  0,  9,  7, 11,  0,  1,  0, 10, 11, 10,  0 ,
   11, 10,  0, 11,  0,  3, 10,  5,  0,  8,  0,  7,  5,  7,  0 ,
   11, 10,  5,  7, 11,  5, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
   10,  6,  5, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    0,  8,  3,  5, 10,  6, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    9,  0,  1,  5, 10,  6, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    1,  8,  3,  1,  9,  8,  5, 10,  6, 255, 255, 255, 255, 255, 255 ,
    1,  6,  5,  2,  6,  1, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    1,  6,  5,  1,  2,  6,  3,  0,  8, 255, 255, 255, 255, 255, 255 ,
    9,  6,  5,  9,  0,  6,  0,  2,  6, 255, 255, 255, 255, 255, 255 ,
    5,  9,  8,  5,  8,  2,  5,  2,  6,  3,  2,  8, 255, 255, 255 ,
    2,  3, 11, 10,  6,  5, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
   11,  0,  8, 11,  2,  0, 10,  6,  5, 255, 255, 255, 255, 255, 255 ,
    0,  1,  9,  2,  3, 11,  5, 10,  6, 255, 255, 255, 255, 255, 255 ,
    5, 10,  6,  1,  9,  2,  9, 11,  2,  9,  8, 11, 255, 255, 255 ,
    6,  3, 11,  6,  5,  3,  5,  1,  3, 255, 255, 255, 255, 255, 255 ,
    0,  8, 11,  0, 11,  5,  0,  5,  1,  5, 11,  6, 255, 255, 255 ,
    3, 11,  6,  0,  3,  6,  0,  6,  5,  0,  5,  9, 255, 255, 255 ,
    6,  5,  9,  6,  9, 11, 11,  9,  8, 255, 255, 255, 255, 255, 255 ,
    5, 10,  6,  4,  7,  8, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    4,  3,  0,  4,  7,  3,  6,  5, 10, 255, 255, 255, 255, 255, 255 ,
    1,  9,  0,  5, 10,  6,  8,  4,  7, 255, 255, 255, 255, 255, 255 ,
   10,  6,  5,  1,  9,  7,  1,  7,  3,  7,  9,  4, 255, 255, 255 ,
    6,  1,  2,  6,  5,  1,  4,  7,  8, 255, 255, 255, 255, 255, 255 ,
    1,  2,  5,  5,  2,  6,  3,  0,  4,  3,  4,  7, 255, 255, 255 ,
    8,  4,  7,  9,  0,  5,  0,  6,  5,  0,  2,  6, 255, 255, 255 ,
    7,  3,  9,  7,  9,  4,  3,  2,  9,  5,  9,  6,  2,  6,  9 ,
    3, 11,  2,  7,  8,  4, 10,  6,  5, 255, 255, 255, 255, 255, 255 ,
    5, 10,  6,  4,  7,  2,  4,  2,  0,  2,  7, 11, 255, 255, 255 ,
    0,  1,  9,  4,  7,  8,  2,  3, 11,  5, 10,  6, 255, 255, 255 ,
    9,  2,  1,  9, 11,  2,  9,  4, 11,  7, 11,  4,  5, 10,  6 ,
    8,  4,  7,  3, 11,  5,  3,  5,  1,  5, 11,  6, 255, 255, 255 ,
    5,  1, 11,  5, 11,  6,  1,  0, 11,  7, 11,  4,  0,  4, 11 ,
    0,  5,  9,  0,  6,  5,  0,  3,  6, 11,  6,  3,  8,  4,  7 ,
    6,  5,  9,  6,  9, 11,  4,  7,  9,  7, 11,  9, 255, 255, 255 ,
   10,  4,  9,  6,  4, 10, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    4, 10,  6,  4,  9, 10,  0,  8,  3, 255, 255, 255, 255, 255, 255 ,
   10,  0,  1, 10,  6,  0,  6,  4,  0, 255, 255, 255, 255, 255, 255 ,
    8,  3,  1,  8,  1,  6,  8,  6,  4,  6,  1, 10, 255, 255, 255 ,
    1,  4,  9,  1,  2,  4,  2,  6,  4, 255, 255, 255, 255, 255, 255 ,
    3,  0,  8,  1,  2,  9,  2,  4,  9,  2,  6,  4, 255, 255, 255 ,
    0,  2,  4,  4,  2,  6, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    8,  3,  2,  8,  2,  4,  4,  2,  6, 255, 255, 255, 255, 255, 255 ,
   10,  4,  9, 10,  6,  4, 11,  2,  3, 255, 255, 255, 255, 255, 255 ,
    0,  8,  2,  2,  8, 11,  4,  9, 10,  4, 10,  6, 255, 255, 255 ,
    3, 11,  2,  0,  1,  6,  0,  6,  4,  6,  1, 10, 255, 255, 255 ,
    6,  4,  1,  6,  1, 10,  4,  8,  1,  2,  1, 11,  8, 11,  1 ,
    9,  6,  4,  9,  3,  6,  9,  1,  3, 11,  6,  3, 255, 255, 255 ,
    8, 11,  1,  8,  1,  0, 11,  6,  1,  9,  1,  4,  6,  4,  1 ,
    3, 11,  6,  3,  6,  0,  0,  6,  4, 255, 255, 255, 255, 255, 255 ,
    6,  4,  8, 11,  6,  8, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    7, 10,  6,  7,  8, 10,  8,  9, 10, 255, 255, 255, 255, 255, 255 ,
    0,  7,  3,  0, 10,  7,  0,  9, 10,  6,  7, 10, 255, 255, 255 ,
   10,  6,  7,  1, 10,  7,  1,  7,  8,  1,  8,  0, 255, 255, 255 ,
   10,  6,  7, 10,  7,  1,  1,  7,  3, 255, 255, 255, 255, 255, 255 ,
    1,  2,  6,  1,  6,  8,  1,  8,  9,  8,  6,  7, 255, 255, 255 ,
    2,  6,  9,  2,  9,  1,  6,  7,  9,  0,  9,  3,  7,  3,  9 ,
    7,  8,  0,  7,  0,  6,  6,  0,  2, 255, 255, 255, 255, 255, 255 ,
    7,  3,  2,  6,  7,  2, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    2,  3, 11, 10,  6,  8, 10,  8,  9,  8,  6,  7, 255, 255, 255 ,
    2,  0,  7,  2,  7, 11,  0,  9,  7,  6,  7, 10,  9, 10,  7 ,
    1,  8,  0,  1,  7,  8,  1, 10,  7,  6,  7, 10,  2,  3, 11 ,
   11,  2,  1, 11,  1,  7, 10,  6,  1,  6,  7,  1, 255, 255, 255 ,
    8,  9,  6,  8,  6,  7,  9,  1,  6, 11,  6,  3,  1,  3,  6 ,
    0,  9,  1, 11,  6,  7, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    7,  8,  0,  7,  0,  6,  3, 11,  0, 11,  6,  0, 255, 255, 255 ,
    7, 11,  6, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    7,  6, 11, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    3,  0,  8, 11,  7,  6, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    0,  1,  9, 11,  7,  6, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    8,  1,  9,  8,  3,  1, 11,  7,  6, 255, 255, 255, 255, 255, 255 ,
   10,  1,  2,  6, 11,  7, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    1,  2, 10,  3,  0,  8,  6, 11,  7, 255, 255, 255, 255, 255, 255 ,
    2,  9,  0,  2, 10,  9,  6, 11,  7, 255, 255, 255, 255, 255, 255 ,
    6, 11,  7,  2, 10,  3, 10,  8,  3, 10,  9,  8, 255, 255, 255 ,
    7,  2,  3,  6,  2,  7, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    7,  0,  8,  7,  6,  0,  6,  2,  0, 255, 255, 255, 255, 255, 255 ,
    2,  7,  6,  2,  3,  7,  0,  1,  9, 255, 255, 255, 255, 255, 255 ,
    1,  6,  2,  1,  8,  6,  1,  9,  8,  8,  7,  6, 255, 255, 255 ,
   10,  7,  6, 10,  1,  7,  1,  3,  7, 255, 255, 255, 255, 255, 255 ,
   10,  7,  6,  1,  7, 10,  1,  8,  7,  1,  0,  8, 255, 255, 255 ,
    0,  3,  7,  0,  7, 10,  0, 10,  9,  6, 10,  7, 255, 255, 255 ,
    7,  6, 10,  7, 10,  8,  8, 10,  9, 255, 255, 255, 255, 255, 255 ,
    6,  8,  4, 11,  8,  6, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    3,  6, 11,  3,  0,  6,  0,  4,  6, 255, 255, 255, 255, 255, 255 ,
    8,  6, 11,  8,  4,  6,  9,  0,  1, 255, 255, 255, 255, 255, 255 ,
    9,  4,  6,  9,  6,  3,  9,  3,  1, 11,  3,  6, 255, 255, 255 ,
    6,  8,  4,  6, 11,  8,  2, 10,  1, 255, 255, 255, 255, 255, 255 ,
    1,  2, 10,  3,  0, 11,  0,  6, 11,  0,  4,  6, 255, 255, 255 ,
    4, 11,  8,  4,  6, 11,  0,  2,  9,  2, 10,  9, 255, 255, 255 ,
   10,  9,  3, 10,  3,  2,  9,  4,  3, 11,  3,  6,  4,  6,  3 ,
    8,  2,  3,  8,  4,  2,  4,  6,  2, 255, 255, 255, 255, 255, 255 ,
    0,  4,  2,  4,  6,  2, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    1,  9,  0,  2,  3,  4,  2,  4,  6,  4,  3,  8, 255, 255, 255 ,
    1,  9,  4,  1,  4,  2,  2,  4,  6, 255, 255, 255, 255, 255, 255 ,
    8,  1,  3,  8,  6,  1,  8,  4,  6,  6, 10,  1, 255, 255, 255 ,
   10,  1,  0, 10,  0,  6,  6,  0,  4, 255, 255, 255, 255, 255, 255 ,
    4,  6,  3,  4,  3,  8,  6, 10,  3,  0,  3,  9, 10,  9,  3 ,
   10,  9,  4,  6, 10,  4, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    4,  9,  5,  7,  6, 11, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    0,  8,  3,  4,  9,  5, 11,  7,  6, 255, 255, 255, 255, 255, 255 ,
    5,  0,  1,  5,  4,  0,  7,  6, 11, 255, 255, 255, 255, 255, 255 ,
   11,  7,  6,  8,  3,  4,  3,  5,  4,  3,  1,  5, 255, 255, 255 ,
    9,  5,  4, 10,  1,  2,  7,  6, 11, 255, 255, 255, 255, 255, 255 ,
    6, 11,  7,  1,  2, 10,  0,  8,  3,  4,  9,  5, 255, 255, 255 ,
    7,  6, 11,  5,  4, 10,  4,  2, 10,  4,  0,  2, 255, 255, 255 ,
    3,  4,  8,  3,  5,  4,  3,  2,  5, 10,  5,  2, 11,  7,  6 ,
    7,  2,  3,  7,  6,  2,  5,  4,  9, 255, 255, 255, 255, 255, 255 ,
    9,  5,  4,  0,  8,  6,  0,  6,  2,  6,  8,  7, 255, 255, 255 ,
    3,  6,  2,  3,  7,  6,  1,  5,  0,  5,  4,  0, 255, 255, 255 ,
    6,  2,  8,  6,  8,  7,  2,  1,  8,  4,  8,  5,  1,  5,  8 ,
    9,  5,  4, 10,  1,  6,  1,  7,  6,  1,  3,  7, 255, 255, 255 ,
    1,  6, 10,  1,  7,  6,  1,  0,  7,  8,  7,  0,  9,  5,  4 ,
    4,  0, 10,  4, 10,  5,  0,  3, 10,  6, 10,  7,  3,  7, 10 ,
    7,  6, 10,  7, 10,  8,  5,  4, 10,  4,  8, 10, 255, 255, 255 ,
    6,  9,  5,  6, 11,  9, 11,  8,  9, 255, 255, 255, 255, 255, 255 ,
    3,  6, 11,  0,  6,  3,  0,  5,  6,  0,  9,  5, 255, 255, 255 ,
    0, 11,  8,  0,  5, 11,  0,  1,  5,  5,  6, 11, 255, 255, 255 ,
    6, 11,  3,  6,  3,  5,  5,  3,  1, 255, 255, 255, 255, 255, 255 ,
    1,  2, 10,  9,  5, 11,  9, 11,  8, 11,  5,  6, 255, 255, 255 ,
    0, 11,  3,  0,  6, 11,  0,  9,  6,  5,  6,  9,  1,  2, 10 ,
   11,  8,  5, 11,  5,  6,  8,  0,  5, 10,  5,  2,  0,  2,  5 ,
    6, 11,  3,  6,  3,  5,  2, 10,  3, 10,  5,  3, 255, 255, 255 ,
    5,  8,  9,  5,  2,  8,  5,  6,  2,  3,  8,  2, 255, 255, 255 ,
    9,  5,  6,  9,  6,  0,  0,  6,  2, 255, 255, 255, 255, 255, 255 ,
    1,  5,  8,  1,  8,  0,  5,  6,  8,  3,  8,  2,  6,  2,  8 ,
    1,  5,  6,  2,  1,  6, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    1,  3,  6,  1,  6, 10,  3,  8,  6,  5,  6,  9,  8,  9,  6 ,
   10,  1,  0, 10,  0,  6,  9,  5,  0,  5,  6,  0, 255, 255, 255 ,
    0,  3,  8,  5,  6, 10, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
   10,  5,  6, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
   11,  5, 10,  7,  5, 11, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
   11,  5, 10, 11,  7,  5,  8,  3,  0, 255, 255, 255, 255, 255, 255 ,
    5, 11,  7,  5, 10, 11,  1,  9,  0, 255, 255, 255, 255, 255, 255 ,
   10,  7,  5, 10, 11,  7,  9,  8,  1,  8,  3,  1, 255, 255, 255 ,
   11,  1,  2, 11,  7,  1,  7,  5,  1, 255, 255, 255, 255, 255, 255 ,
    0,  8,  3,  1,  2,  7,  1,  7,  5,  7,  2, 11, 255, 255, 255 ,
    9,  7,  5,  9,  2,  7,  9,  0,  2,  2, 11,  7, 255, 255, 255 ,
    7,  5,  2,  7,  2, 11,  5,  9,  2,  3,  2,  8,  9,  8,  2 ,
    2,  5, 10,  2,  3,  5,  3,  7,  5, 255, 255, 255, 255, 255, 255 ,
    8,  2,  0,  8,  5,  2,  8,  7,  5, 10,  2,  5, 255, 255, 255 ,
    9,  0,  1,  5, 10,  3,  5,  3,  7,  3, 10,  2, 255, 255, 255 ,
    9,  8,  2,  9,  2,  1,  8,  7,  2, 10,  2,  5,  7,  5,  2 ,
    1,  3,  5,  3,  7,  5, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    0,  8,  7,  0,  7,  1,  1,  7,  5, 255, 255, 255, 255, 255, 255 ,
    9,  0,  3,  9,  3,  5,  5,  3,  7, 255, 255, 255, 255, 255, 255 ,
    9,  8,  7,  5,  9,  7, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    5,  8,  4,  5, 10,  8, 10, 11,  8, 255, 255, 255, 255, 255, 255 ,
    5,  0,  4,  5, 11,  0,  5, 10, 11, 11,  3,  0, 255, 255, 255 ,
    0,  1,  9,  8,  4, 10,  8, 10, 11, 10,  4,  5, 255, 255, 255 ,
   10, 11,  4, 10,  4,  5, 11,  3,  4,  9,  4,  1,  3,  1,  4 ,
    2,  5,  1,  2,  8,  5,  2, 11,  8,  4,  5,  8, 255, 255, 255 ,
    0,  4, 11,  0, 11,  3,  4,  5, 11,  2, 11,  1,  5,  1, 11 ,
    0,  2,  5,  0,  5,  9,  2, 11,  5,  4,  5,  8, 11,  8,  5 ,
    9,  4,  5,  2, 11,  3, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    2,  5, 10,  3,  5,  2,  3,  4,  5,  3,  8,  4, 255, 255, 255 ,
    5, 10,  2,  5,  2,  4,  4,  2,  0, 255, 255, 255, 255, 255, 255 ,
    3, 10,  2,  3,  5, 10,  3,  8,  5,  4,  5,  8,  0,  1,  9 ,
    5, 10,  2,  5,  2,  4,  1,  9,  2,  9,  4,  2, 255, 255, 255 ,
    8,  4,  5,  8,  5,  3,  3,  5,  1, 255, 255, 255, 255, 255, 255 ,
    0,  4,  5,  1,  0,  5, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    8,  4,  5,  8,  5,  3,  9,  0,  5,  0,  3,  5, 255, 255, 255 ,
    9,  4,  5, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    4, 11,  7,  4,  9, 11,  9, 10, 11, 255, 255, 255, 255, 255, 255 ,
    0,  8,  3,  4,  9,  7,  9, 11,  7,  9, 10, 11, 255, 255, 255 ,
    1, 10, 11,  1, 11,  4,  1,  4,  0,  7,  4, 11, 255, 255, 255 ,
    3,  1,  4,  3,  4,  8,  1, 10,  4,  7,  4, 11, 10, 11,  4 ,
    4, 11,  7,  9, 11,  4,  9,  2, 11,  9,  1,  2, 255, 255, 255 ,
    9,  7,  4,  9, 11,  7,  9,  1, 11,  2, 11,  1,  0,  8,  3 ,
   11,  7,  4, 11,  4,  2,  2,  4,  0, 255, 255, 255, 255, 255, 255 ,
   11,  7,  4, 11,  4,  2,  8,  3,  4,  3,  2,  4, 255, 255, 255 ,
    2,  9, 10,  2,  7,  9,  2,  3,  7,  7,  4,  9, 255, 255, 255 ,
    9, 10,  7,  9,  7,  4, 10,  2,  7,  8,  7,  0,  2,  0,  7 ,
    3,  7, 10,  3, 10,  2,  7,  4, 10,  1, 10,  0,  4,  0, 10 ,
    1, 10,  2,  8,  7,  4, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    4,  9,  1,  4,  1,  7,  7,  1,  3, 255, 255, 255, 255, 255, 255 ,
    4,  9,  1,  4,  1,  7,  0,  8,  1,  8,  7,  1, 255, 255, 255 ,
    4,  0,  3,  7,  4,  3, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    4,  8,  7, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    9, 10,  8, 10, 11,  8, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    3,  0,  9,  3,  9, 11, 11,  9, 10, 255, 255, 255, 255, 255, 255 ,
    0,  1, 10,  0, 10,  8,  8, 10, 11, 255, 255, 255, 255, 255, 255 ,
    3,  1, 10, 11,  3, 10, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    1,  2, 11,  1, 11,  9,  9, 11,  8, 255, 255, 255, 255, 255, 255 ,
    3,  0,  9,  3,  9, 11,  1,  2,  9,  2, 11,  9, 255, 255, 255 ,
    0,  2, 11,  8,  0, 11, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    3,  2, 11, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    2,  3,  8,  2,  8, 10, 10,  8,  9, 255, 255, 255, 255, 255, 255 ,
    9, 10,  2,  0,  9,  2, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    2,  3,  8,  2,  8, 10,  0,  1,  8,  1, 10,  8, 255, 255, 255 ,
    1, 10,  2, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    1,  3,  8,  9,  1,  8, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    0,  9,  1, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 ,
    0,  3,  8, 255, 255, 255, 255 ,255 ,255, 255 ,255 ,255, 255 ,255 ,255 ,
   255, 255, 255, 255, 255, 255, 255 ,255 ,255, 255 ,255 ,255, 255 ,255 ,255 };

    use(1);


    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB8, width, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    delete[] data;
}

