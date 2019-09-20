#include "shaders/Noise2.cl"

__constant float PI = 3.14159265359f;

typedef struct Camera{
	float3 position;		
	float3 view;			
	float3 up;			
	float2 resolution;	
	float2 fov;		
	float apertureRadius;
	float focalDistance;
} Camera;

typedef struct Ray {
  float3 origin;
  float3 direction;
} Ray;

union Color{ float c; uchar4 components; };

/******************************************************************************************************************************/

float ball(float3 f_pos, float3 ball_center, float radius) {
  return pow(ball_center.x - f_pos.x,2.0f) + pow(ball_center.y - f_pos.y,2.0f) + pow(ball_center.z - f_pos.z,2.0f) - pow(radius, 2.0f);  
}

/******************************************************************************************************************************/

float maasto(float3 f_pos)
{
  
//  float value =  SingleValueFractalRigidMulti3(0.2, 0.5, 4, 2, 2, f_pos.x*0.2, f_pos.y*0.2, f_pos.z*0.2);
//  float value =  GetValueFractal3(5.2, 2, 1.3, 4.0, 2, 0.3, 1,3, f_pos.x*0.2, f_pos.y*0.2, f_pos.z*0.2);
//  float value2 = GetValueFractal3(1.5, 1, 0.7, 0.5, 3, 0.5, 1,3, f_pos.x*0.25, f_pos.y*0.25, f_pos.z*0.25);
//	float value =  Noise_3d(f_pos.x*0.1, f_pos.y*0.1, f_pos.z*0.1); 
//  float value =  GetWhiteNoise3(3,f_pos.x*0.001, f_pos.y*0.001, f_pos.z*0.001); 
//	float value2 =  Noise_3d(f_pos.x*0.2, f_pos.y*0.2, f_pos.z*0.2); 
//  float result =  f_pos.y - 15*value + 15.0*value2;
//  return result;
//  return f_pos.y + value + 0.5*value2;
    float value2 = SinglePerlin3(1, 123, f_pos.x*0.01, f_pos.y*0.01, f_pos.z*0.01);
    float value = SinglePerlin3(3, 5, f_pos.x*0.1, f_pos.y*0.1, f_pos.z*0.1);
//float SinglePerlin3(int m_smoothing,
//    int seed,
//    float x, float y, float z)
  return f_pos.y + 8.0 * value + 150*value2;
}

/******************************************************************************************************************************/

float3 calculate_normal_ball(float3 pos)
{
  float3 grad;
  float right =   ball((float3){pos.x+0.01f, pos.y,pos.z}, (float3){-1.0f,0.0f,0.0f},0.4f);
  float left =    ball((float3){pos.x-0.01f, pos.y,pos.z}, (float3){-1.0f,0.0f,0.0f},0.4f);
  float up =      ball((float3){pos.x, pos.y+0.01f,pos.z}, (float3){-1.0f,0.0f,0.0f},0.4f);
  float down =    ball((float3){pos.x, pos.y-0.01f,pos.z}, (float3){-1.0f,0.0f,0.0f},0.4f);
  float z =       ball((float3){pos.x, pos.y,pos.z-0.01f}, (float3){-1.0f,0.0f,0.0f},0.4f);
  float z_minus = ball((float3){pos.x, pos.y,pos.z+0.01f}, (float3){-1.0f,0.0f,0.0f},0.4f);

  //float right =   maasto((float3){pos.x+0.01f, pos.y,pos.z});
  //float left =    maasto((float3){pos.x-0.01f, pos.y,pos.z});
  //float up =      maasto((float3){pos.x, pos.y+0.01f,pos.z});
  //float down =    maasto((float3){pos.x, pos.y-0.01f,pos.z});
  //float z =       maasto((float3){pos.x, pos.y,pos.z-0.01f});
  //float z_minus = maasto((float3){pos.x, pos.y,pos.z+0.01f});
  //return (float3){0.0f,0.0f,0.0f};
  grad.x = right - left;
  grad.y = up - down;
  grad.z = z - z_minus;
  return normalize(grad); // Normalize the result. 
}

/******************************************************************************************************************************/

float3 calculate_normal_maasto(float3 pos)
{
  float3 grad;

  float offset = 0.5f;
  float right =   maasto((float3){pos.x+offset, pos.y,pos.z});
  float left =    maasto((float3){pos.x-offset, pos.y,pos.z});
  float up =      maasto((float3){pos.x, pos.y+offset,pos.z});
  float down =    maasto((float3){pos.x, pos.y-offset,pos.z});
  float z =       maasto((float3){pos.x, pos.y,pos.z-offset});
  float z_minus = maasto((float3){pos.x, pos.y,pos.z+offset});
  grad.x = right - left;
  grad.y = up - down;
  grad.z = z - z_minus;
  return normalize(grad); // Normalize the result. 
}
/******************************************************************************************************************************/

float3 getPoint(float distance, Ray* ray)
{
  return ray->origin + distance * ray->direction; 
}

/******************************************************************************************************************************/

__kernel void ray_path(__write_only image2d_t output, __global Camera* cam)
{
  const unsigned int x_coord = get_global_id(0);
  const unsigned int y_coord = get_global_id(1);
  const unsigned int global_x_dim = get_global_size(0);
  const unsigned int global_y_dim = get_global_size(1);



  float3 right = normalize(cross(cam->view,cam->up));  
  float3 y = normalize(cross(cam->view, right));

  float d = cam->focalDistance;  
  
  float3 u = (d * tan(cam->fov.x*0.5f)) * right; 
  float3 v = (d * tan(cam->fov.y*0.5f)) * y;

  float alpha = 2.0*((float)x_coord + 0.5f) / (float)global_x_dim - 1.0;
  float beta  = 1.0 - 2.0f*((float)y_coord + 0.5f) / (float)global_y_dim;

  float3 point_on_plane = alpha * u + beta * v;

  Ray ray;
  ray.origin = point_on_plane + cam->position; //(float3){0.0f, 0.0f, 1.0f}; // + cam->view*d; //point_on_plane; //-d*cam->view; // point_on_plane; // cam->position; 
  ray.direction = normalize(point_on_plane + d*cam->view);

  float3 intersection_point;
  float3 normal = (float3){0.0,0.0,0.0};
	float3 accum_color = (float3){0.0f, 0.0f, 0.0f};

  float step_size = 0.15f;
  float depth = 0.0f;

  for (int i=0 ; i<1600 ; i++)
  {
    float3 p = getPoint(depth, &ray); 
    float value_maasto = maasto(p);
    if (fabs(value_maasto) < 0.4f) {
      intersection_point = p;
      normal = calculate_normal_maasto(p);
      float3 mask = (float3){1.0f,1.0f,1.0f};
      float emission = 0.9f;
      float3 color = (float3){0.0f,1.0f,0.0f};
	    float diffuseCoeffient = max(0.1f , dot(-normal,normalize(intersection_point - cam->position)));

      accum_color += emission * mask;
      accum_color *= color;
	    accum_color *= diffuseCoeffient;
      break;
    }   

    depth += step_size;
  }
  
  //if (accum_color.x == 0.0f && accum_color.y == 0.0f && accum_color.z == 0.0f) accum_color = (float3){0.0f,0.0f,0.0f};

////  output[x_coord + global_x_dim * y_coord] = (float16){(float)ray.origin.x,          // a.x
////                                                       (float)ray.origin.y,          // a.y
////                                                       (float)ray.origin.z,          // a.z
////                                                       (float)ray.direction.x,       // a.w
////                                                       (float)ray.direction.y,       // b.x
////                                                       (float)ray.direction.z,       // b.y
////                                                       (float)intersection_point.x,  // b.z
////                                                       (float)intersection_point.y,  // b.w
////                                                       (float)intersection_point.z,  // c.x
////                                                       (float)accum_color.x,         // c.y
////                                                       (float)accum_color.y,         // c.z
////                                                       (float)accum_color.z,         // c.w
////                                                       (float)v.x,                   // d.x
////                                                       (float)v.y,                   // d.y
////                                                       (float)v.z,                   // d.z
////                                                       666.0f                        // d.w
////                                                       };
  //output[x_coord + global_x_dim * y_coord] = (float8){(float)x_coord,(float)y_coord,(float)global_x_dim,(float)global_y_dim,accum_color.x,accum_color.y,accum_color.z,normal.y};

	union Color fcolor;
	fcolor.components = (uchar4){
		(unsigned char)(accum_color.x * 255),
		(unsigned char)(accum_color.y * 255),
		(unsigned char)(accum_color.z * 255),
		1};
//	fcolor.components = (uchar4){
//		1,133,133,255};
  //output[x_coord + global_x_dim * y_coord] = fcolor.c; // fcolor.c;
  //write_imagef(output, (int2){x_coord,y_coord}, (float4){accum_color.x,accum_color.y, accum_color.z, 1.0f}); // fcolor.c); // fcolor.c);
  float4 output_color;
  output_color.x = accum_color.x;
  output_color.y = accum_color.y;
  output_color.z = accum_color.z;
  output_color.w = 1.0f;
  write_imagef(output, (int2){x_coord,y_coord}, output_color); // fcolor.c); // fcolor.c);
}
