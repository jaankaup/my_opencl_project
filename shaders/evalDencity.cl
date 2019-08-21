#include "shaders/Noise.cl"

// futhark
// Density-function example.

float createCube(float4 pos, float4 a1, float4 a2, float4 b1, float4 b2)
{
  if (pos.x < a1.x || pos.y < a1.y || pos.z > a1.z || pos.x > a2.x || pos.y > a2.y || pos.z < a2.z) return 3.0;
  if (pos.x > b1.x && pos.y > b1.y && pos.z < b1.z && pos.x < b2.x && pos.y < b2.y && pos.z > a2.z) return 0.0;
  else return -3.0;
}

float ball(float4 f_pos, float4 ball_center, float radius)
{
  //return fmin(fmax(pow(ball_center.x - f_pos.x,2.0f + pow(ball_center.y - f_pos.y,2.0f) + pow(ball_center.z - f_pos.z,2.0f) - pow(radius, 12.0f)), 2.0f),-12.0f);  
  return fmin(2.0f,fmax(-2.0f,pow(ball_center.x - f_pos.x,2.0f + pow(ball_center.y - f_pos.y,2.0f) + pow(ball_center.z - f_pos.z,2.0f) - pow(radius, 2.0f))));  
}

float maasto(float4 f_pos)
{

	float value =  Noise_3d(f_pos.x*0.1, f_pos.y*0.1, f_pos.z*0.1); 
	float value2 =  Noise_3d(f_pos.x*0.02, f_pos.y*0.02, f_pos.z*0.02); 
  //float i_part = 0.0f;
  //float f_part = modf(80.0*value2, &i_part);
  //return f_pos.y - i_part + ball(f_pos, (float4){0.0,0.0,0.0,0.0},5.0);
  ////return f_pos.y - ball(f_pos, (float4){0.0,0.0,0.0,0.0},45.0);
  return f_pos.y - 8*value + 20.0*value2 - 25;
  //return f_pos.y + sin(f_pos.x)*3 - 5*cos(f_pos.z) + 28*value - 155;
	//return map256( value );
}

float outoLaatikko(float4 f_pos)
{
  //return createCube(f_pos,(float4){1.5,0.0,2.5,0.0},(float4){3.5,2.5,1.5,0.0});
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

  // The local position. We don't need these values now.
  //  const int local_id_x = get_local_id(0);
  //  const int local_id_y = get_local_id(1);
  //  const int local_id_z = get_local_id(2);

  // This point translated and scaled to the marching cubes area.
  const float4 this_point_global = (float4){global_id_x,global_id_y,global_id_z,0.0}*block_size + base_point;

  const finalID = global_id_x + x_offset * global_id_y + x_offset * y_offset * global_id_z; 

  // Save the density value to the output.
  output[finalID] = outoLaatikko(this_point_global) + maasto(this_point_global) + 15 + ball(this_point_global, (float4){0.0,0.0,0.0,0.0}, 15.0);
  //output[finalID] = maasto(this_point_global);
}                                                                               
