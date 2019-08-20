// futhark
// Density-function example.
float ball(float4 f_pos, float4 ball_center, float radius)
{
  return pow(ball_center.x - f_pos.x,2.0f) + pow(ball_center.y - f_pos.y,2.0f) + pow(ball_center.z - f_pos.z,2.0f) - pow(radius, 2.0f);  
}

float planeY(float4 f_pos)
{
  return f_pos.y;
}

/**
 *  Maching cubes pass1. Calculate all f(x,y,z) = v values for each position and save values to @output.
 */
__kernel void evalDensity(__constant int* iConstants, __constant float* fConstants,  __global float* output)
{

  const int global_id = get_global_id(0);

  // The global position.
  const int global_id_x = get_global_id(0);
  const int global_id_y = get_global_id(1);
  const int global_id_z = get_global_id(2);

  // The local position.
  const int local_id_x = get_local_id(0);
  const int local_id_y = get_local_id(1);
  const int local_id_z = get_local_id(2);

  // The base point of the whole marching cubes area.
  const float4 base_point = (float4){fConstants[0],fConstants[1],fConstants[2], 0.0};

  // This point translated and scaled to the marching cubes area.
  const float4 this_point_global = (float4){global_id_x,global_id_y,global_id_z,0.0}*fConstants[4] + base_point;

  const finalID = global_id_x + iConstants[0] * global_id_y + iConstants[0] * iConstants[1] * global_id_z; 

  //output[finalID] = (float4) {this_point_global.xyz, this_point_global.y};
  output[finalID] = this_point_global.y;
}                                                                               
