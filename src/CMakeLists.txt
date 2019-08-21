# Specify the minimum version for CMake # cmake -H. -Bbuild
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fopenmp")

# Debug.
if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    message("Debug Build")
    add_compile_definitions(DEBUG_PROJECT)

# Release. Tanne optimoinnit ym. 
elseif(${CMAKE_BUILD_TYPE} MATCHES Release)
    message("Release Build")
endif()

set (graphics_dir ${src_dir}/Graphics)
set (util_dir ${src_dir}/Utils)
set (opencl_dir ${src_dir}/OpenCL)
set (program_dir ${src_dir}/Program)
set (test_dir ${src_dir}/Tests)

set(graphics_src 
      ${graphics_dir}/window.cpp
      ${graphics_dir}/shader.cpp
      ${graphics_dir}/vertexbuffer.cpp
      ${graphics_dir}/vertexAttributes.cpp
      ${graphics_dir}/renderer.cpp
      ${graphics_dir}/texture.cpp
      ${graphics_dir}/camera.cpp
      ${graphics_dir}/textureData.cpp
      )

set(util_src 
      ${util_dir}/log.cpp
      ${util_dir}/misc.cpp
      ${util_dir}/Helper.cpp
      )

set(program_src 
      ${program_dir}/Program.cpp
      ${program_dir}/InputCache.cpp
      ${program_dir}/Marching_Cubes_data.cpp
      #      ${util_dir}/GlobalPropertyManager.cpp
      )

set(opencl_src 
  ${opencl_dir}/GPU_Device.cpp
  ${opencl_dir}/CL_Helper.cpp
      )

set (soil_src
  ${soil_dir}/image_DXT.c
  ${soil_dir}/image_helper.c
  ${soil_dir}/SOIL.c
  ${soil_dir}/stb_image_aug.c
  )

#set (tests_src
#  ${tests_dir}/TestEvents.c
#	)


set (headers ${CMAKE_SOURCE_DIR}/headers)
message("libraries : ${libraries}")
message("headers : ${headers}")

#	add_custom_command(TARGET hello 
#		           POST_BUILD
#			   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shadersNative ${CMAKE_BINARY_DIR}/bin/shaders
#			   COMMENT "Copying shaders."
#			   VERBATIM)
#
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") 

# MAKE_RUNTIME_OUTPUT_DIRECTORY or EXECUTABLE_OUTPUT_PATH. Where to create the executable. 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Library output path.
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# Tanne alki-kirjastot
#add_subdirectory(lib/math)

# Alikirjaston nimi, tiedostot ja tyyppi.
#add_library(math SHARED lib/math/operations.cpp)
#add_library(math STATIC lib/math/operations.cpp)

add_executable(hello ${src_dir}/hello.cpp ${util_src} ${graphics_src} ${opencl_src} ${program_src} ${soil_src})  

#Tanne linkitykset
target_link_libraries(hello ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} OpenCL::OpenCL ${Boost_LIBRARIES}) 
