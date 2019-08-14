// Density-function example.
float ball(float3 f_pos, float3 ball_center, float radius)
{
  return pow(ball_center.x - f_pos.x,2.0f) + pow(ball_center.y - f_pos.y,2.0f) + pow(ball_center.z - f_pos.z,2.0f) - pow(radius, 2.0f);  
}

// The kernel. 
__kernel void eval_density(__global float3* base_points, __global float* base_values, int n)
{
  const int global_id = get_global_id(0);
  const float3 b_center;
  b_center.x = 0.0f;
  b_center.y = 0.0f;
  b_center.z = 0.0f;

  if(global_id < n) { 

     // Calculate the values for each f(x,y,z) = v.
     // Store the values in base_values.
     
     //base_values[global_id] = base_points[global_id];      
     base_values[global_id] = ball(base_points[global_id], b_center, 0.3f);      
  }
}                                                                               
