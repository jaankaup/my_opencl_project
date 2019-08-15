// futhark
// Density-function example.
float ball(float4 f_pos, float4 ball_center, float radius)
{
  return pow(ball_center.x - f_pos.x,2.0f) + pow(ball_center.y - f_pos.y,2.0f) + pow(ball_center.z - f_pos.z,2.0f) - pow(radius, 2.0f);  
}

// The kernel. 

__kernel void eval_density(__global float* base_values,
                                    int x_dimension,
                                    int y_dimension,
                                    int z_dimension,
                                    float block_size,
                                    float4 start_point,
                                    int n)
{
  const int x_offset = x_dimension + 2;
  const int y_offset = x_offset * (y_dimension + 2);

  const int global_id = get_global_id(0);

  const float x_coord = ((global_id % x_offset) - 1) * block_size;
  const float y_coord = (((global_id / x_offset) % (y_dimension + 2)) - 1) * block_size;
  const float z_coord = (((global_id / y_offset) % (z_dimension + 2)) - 1) * block_size;

  const float4 b_center;
  b_center.x = 0.0f;
  b_center.y = 0.0f;
  b_center.z = 0.0f;
  b_center.w = 0.0f;

  if(global_id < n) { 

     // Calculate the values for each f(x,y,z) = v.
     // Store the values in base_values.
     
     //base_values[global_id] = base_points[global_id];      
     float4 result = { x_coord, y_coord, z_coord, 0.0f };
     base_values[global_id] = ball(result, b_center, 0.3f);      
  }
}                                                                               
