// This is a file that is not included in in source because it's not my code. TODO: replace it with something else.
//#include "shaders/Noise.cl"
#include "shaders/Noise2.cl"

// futhark
// Density-function example.

/**
 * Some wierd way to create density value.
 */
float createCube(float4 pos, float4 a1, float4 a2, float4 b1, float4 b2)
{
  if (pos.x < a1.x || pos.y < a1.y || pos.z > a1.z || pos.x > a2.x || pos.y > a2.y || pos.z < a2.z) return 1.0;
  // if (pos.x > b1.x && pos.y > b1.y && pos.z < b1.z && pos.x < b2.x && pos.y < b2.y && pos.z > a2.z) return 0.0;
  else return -1.0;
}

/**
 * Another wierd density value funcion. This won't create any ball now... 
 */
/******************************************************************************************************************************/

float ball(float3 f_pos, float3 ball_center, float radius) {
  //return f_pos.y;
  float final_value = 0.0f;
  float value = pow(ball_center.x - f_pos.x,2.0f) + pow(ball_center.y - f_pos.y,2.0f) + pow(ball_center.z - f_pos.z,2.0f) - pow(radius, 2.0f);  
  if (f_pos.y < -0.2f) return 1.0f;
  //if (fabs(f_pos.y) < 0.001f) return 0.4f;
  if (f_pos.y < 0.2f) return -1.0f;
  //else if (value - 0.4f < 0.2f) return 0.41f; 
  //else if (value - 0.4f < 0.1f) return 0.4f; 
  //else if (value - 0.4f < 0.0f) return 0.39f; 
  //else if (f_pos.y - 0.4f < 0.0f) return f_pos.y;
  return value; // f_pos.y + 0.4f;;
}

/******************************************************************************************************************************/

float maasto(float4 f_pos)
{
  
  float value =  SingleValueFractalRigidMulti3(0.2, 0.5, 4, 2, 2, f_pos.x*0.2, f_pos.y*0.2, f_pos.z*0.2);
//  float value =  GetValueFractal3(5.2, 2, 1.3, 4.0, 2, 0.3, 1,3, f_pos.x*0.2, f_pos.y*0.2, f_pos.z*0.2);
//  float value2 = GetValueFractal3(1.5, 1, 0.7, 0.5, 3, 0.5, 1,3, f_pos.x*0.25, f_pos.y*0.25, f_pos.z*0.25);
//	float value =  Noise_3d(f_pos.x*0.1, f_pos.y*0.1, f_pos.z*0.1); 
//  float value =  GetWhiteNoise3(3,f_pos.x*0.001, f_pos.y*0.001, f_pos.z*0.001); 
//	float value2 =  Noise_3d(f_pos.x*0.2, f_pos.y*0.2, f_pos.z*0.2); 
//  float result =  f_pos.y - 15*value + 15.0*value2;
//  return result;
//  return f_pos.y + value + 0.5*value2;
    float value2 = SinglePerlin3(1, 123, f_pos.x*0.2, f_pos.y*0.2, f_pos.z*0.2);
//float SinglePerlin3(int m_smoothing,
//    int seed,
//    float x, float y, float z)
  return f_pos.y + 3 * value + 5*value2;
}

float anti_maasto(float4 f_pos, float4 a1, float4 a2)
{
  float m = maasto(f_pos);
  float value;
  modf(m, &value);
  return value;
}

/**
 * Creates a "box".
 */
float outoLaatikko(float4 f_pos)
{
  float4 outer = (float4){2.0,2.0,58.0,0.0};
  float4 outer2 = (float4){115.0,115.0,5.0,0.0};
  float4 inner = (float4){1.95,1.95,57.95,0.0};
  float4 inner2 = (float4){114.95,114.95,4.95,0.0};
  
  float temp = f_pos.y + createCube(f_pos,outer,outer2,inner,inner2);

  outer = (float4){2.95,2.95,56.0,0.0};
  outer2 = (float4){2.95,2.95,5.1,0.0};
  inner = (float4){1.85,1.85,55.00,0.0};
  inner2 = (float4){1.85,1.85,4.05,0.0};

  float temp2 = createCube(f_pos,outer,outer2,inner,inner2);

  return temp - temp2;
}

/**
 *  Maching cubes pass1. Calculate all f(x,y,z) = v values for each position and save values to @output.
 */
__kernel void evalDensity(__global float* output, int x_offset, int y_offset, float block_size, float4 base_point)
{

  const int global_id = get_global_id(0);

  // The global position.
  const int global_id_x = get_global_id(0);
  const int global_id_y = get_global_id(1);
  const int global_id_z = get_global_id(2);

  // This point translated and scaled to the marching cubes area.
  const float4 this_point_global = (float4){global_id_x,global_id_y,global_id_z,0.0}*block_size + base_point;

  // Calculate the corresponding index for output.
  const finalID = global_id_x + x_offset * global_id_y + x_offset * y_offset * global_id_z; 

  // Save the density value to the output.
  //output[finalID] = outoLaatikko(this_point_global) + maasto(this_point_global) + 15 + ball(this_point_global, (float4){0.0,0.0,0.0,0.0}, 15.0);
  //output[finalID] = maasto(this_point_global);
  //output[finalID] = ball(this_point_global.xyz, (float3){0.0f,0.0f,0.0f},5.3f);
  output[finalID] = maasto(this_point_global);

}                                                                               
