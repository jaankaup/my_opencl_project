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
  return f_pos.y + 8.0 * value + 50*value2;
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

__kernel void ray_path(const float isovalue, const int width, const int height, __global float8* output, __global Camera* cam)
{
  const unsigned int x_coord = get_global_id(0);
  const unsigned int y_coord = get_global_id(1);
  const unsigned int global_x_dim = get_global_size(0);
  const unsigned int global_y_dim = get_global_size(1);


  //const float fx = (float)x_coord / (float)global_x_dim; /* x-coordinate in range [0,1] */
  //const float fy = (float)y_coord / (float)global_y_dim; /* y-coordinate in range [0,1] */
//  float3 n = normalize(cam->position - cam->view); 
//  float3 u = normalize(cross(cam->up,n));
//  float3 v = cross(n,u); // normalize(cross(cam->up,cam->view));

////	float3 rendercamview = -cam->view; rendercamview = normalize(rendercamview);
////	float3 rendercamup = cam->up; rendercamup = normalize(rendercamup);
////	float3 horizontalAxis = cross(rendercamview, rendercamup); horizontalAxis = normalize(horizontalAxis);
////	float3 verticalAxis = cross(horizontalAxis, rendercamview); verticalAxis = normalize(verticalAxis);
////
////	float3 middle = cam->position + rendercamview;
////  float fovX = 2.0; 
////  float fovY = 2.0; 
////	//float3 horizontal = horizontalAxis * tan(cam->fov.x * 0.5f * (PI / 180)); 
////	//float3 vertical   = verticalAxis * tan(cam->fov.y * -0.5f * (PI / 180)); 
////	float3 horizontal = horizontalAxis * tan(fovX * 0.5f * (PI / 180)); 
////	float3 vertical   = verticalAxis * tan(fovY * -0.5f * (PI / 180)); 
////
////	unsigned int x = x_coord;
////	unsigned int y = y_coord;
////
////	int pixelx = x_coord; 
////	int pixely = height - y_coord - 1;
////
////	float sx = (float)pixelx / (width - 1.0f);
////	float sy = (float)pixely / (height - 1.0f);
////	
////	float3 pointOnPlaneOneUnitAwayFromEye = middle + (horizontal * ((2 * sx) - 1)) + (vertical * ((2 * sy) - 1));
////	float3 point_on_plane = cam->position + ((pointOnPlaneOneUnitAwayFromEye - cam->position) * cam->focalDistance); /* cam->focalDistance */

  float3 right = normalize(cross(cam->view,cam->up));  
  float3 y = normalize(cross(cam->view, right));
//  float3 y = normalize(cross(-cam->view, right));

  float d = cam->focalDistance;  
  
  float3 u = (d * tan(cam->fov.x*0.5f)) * right; 
  float3 v = (d * tan(cam->fov.y*0.5f)) * y;

  float alpha = 2.0*((float)x_coord + 0.5f) / (float)global_x_dim - 1.0;
  float beta  = 1.0 - 2.0f*((float)y_coord + 0.5f) / (float)global_y_dim;

  float3 point_on_plane = alpha * u + beta * v;

//  cam->focalDistance = 1.0f;

////  float3 n = normalize(cam->position - cam->view);
////  float3 u = normalize(cross(cam->up, n));
////  float3 v = cross(n, u);
////
////  float aspect_ratio = (float)(global_x_dim) / (float)(global_y_dim);
////
////  float h = tan(cam->fov.x * 0.5) * 2 * cam->focalDistance;
////  float w = h * aspect_ratio; 
////
////  float3 center = cam->position - n * cam->focalDistance;
////  float3 left_bottom = center - u * w * 0.5f - v * h * 0.5f;
////  
////  float3 point_on_plane = left_bottom + u * x_coord * w / global_x_dim + v * y_coord * h / global_y_dim;

  Ray ray;
  //ray.origin = point_on_plane + cam->position; //cam->position; // + point_on_plane; //point_on_plane; // - cam->position; //cam->position;
  //ray.direction = normalize(point_on_plane - d*cam->view);
  //ray.direction = -normalize(point_on_plane - d*cam->view);
  ray.origin = point_on_plane + cam->position; //(float3){0.0f, 0.0f, 1.0f}; // + cam->view*d; //point_on_plane; //-d*cam->view; // point_on_plane; // cam->position; 
  ray.direction = normalize(point_on_plane + d*cam->view);
  //ray.direction = normalize(point_on_plane - cam->position);

//  float d = 1.0 / (tan(cam->fov.x/2));
//  
//  const float fx = (float)x_coord + 0.5; // (float)global_x_dim; /* x-coordinate in range [0,1] */
//  const float fy = (float)y_coord + 0.5; // (float)global_y_dim; /* y-coordinate in range [0,1] */
//  
//  Ray ray;
//  ray.origin = cam->position;
//  ray.direction = (float3){aspect_ratio * 2.0 * fx / (float)global_x_dim - 1, 
//                           2.0 * fy / (float)global_y_dim - 1,
//                           d};
  
  float3 intersection_point;
  float3 normal = (float3){0.0,0.0,0.0};
	float3 accum_color = (float3){0.0f, 0.0f, 0.0f};

  float step_size = 0.1f;
  float depth = 0.0f; // distance(point_on_plane , cam->position);

//  float value_now = maasto(getPoint(depth, &ray)); // The inital value.

  for (int i=0 ; i<800 ; i++)
  {
    float3 p = getPoint(depth, &ray); 
    //float value = maasto(p);
    //float value = ball(p, (float3){0.0f,0.0f,0.0f},5.3f);
    float value_maasto = maasto(p);
    //if (fabs(value) < 0.4f) {
    //  intersection_point = p;
    //  normal = calculate_normal_ball(p);
    //  float3 mask = (float3){1.0f,1.0f,1.0f};
    //  float emission = 0.9f;
    //  float3 color = (float3){0.0f,1.0f,0.0f};
	  //  float diffuseCoeffient = max(0.1f , dot(-normal,normalize(intersection_point - cam->position)));

    //  accum_color += emission * mask;
    //  accum_color *= color;
	  //  accum_color *= diffuseCoeffient;
    //  break;
    //}   
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
    //else if (value_maasto-40.0f > 0.0f) { depth += 19.9f; } 
    //else if (value_maasto-20.0f > 0.0f) { depth += 19.9f; } 
    //else if (value_maasto-6.0f > 0.0f) { depth += 5.9f; } 
    //else if (value_maasto-3.0f > 0.0f) { depth += 2.9f; } 
    //else if (value_maasto-1.0f > 0.0) { depth += 0.4f; } 
    //else if (value_maasto-0.6f > 0.0) { depth += 0.2f; } 
    depth += step_size;
    //if (depth > 400.0f) break;
    //step_size = step_size + 0.0001;
  }
  //float3 black = (float3){0.0f,0.0f,0.0f};
  if (accum_color.x == 0.0f && accum_color.y == 0.0f && accum_color.z == 0.0f) accum_color = (float3){0.0f,0.0f,0.0f};

  //float3 v;

//  const float fx_centered = (x_coord - 0.5) * aspect_ratio; /* Relative to the centered camera. */ 
//  const float fy_centered = fy - 0.5;                       /* Relative to the centered camera. */ 
//  
//  const float3 pixel_pos = (float3){fx_centered, - fy_centered, 0.0}; /* Screen pixel position on the screen. */
//
//  Ray ray;
//  ray.origin = cam->position;
//  ray.direction = normalize(pixel_pos - cam->position);

//  output[global_id_x] = (float8){
//                                  (float)(global_id_x + global_id_y * get_global_size(0)),
//                                  (float)(global_id_x),
//                                  (float)(global_id_y),
//                                  cam->up.x,
//                                  cam->up.y,
//                                  cam->up.z,
//                                  (float)get_global_size(0),
//                                  (float)get_global_size(1)};

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
  output[x_coord + global_x_dim * y_coord] = (float8){(float)x_coord,(float)y_coord,(float)global_x_dim,(float)global_y_dim,accum_color.x,accum_color.y,accum_color.z,normal.y};
}
