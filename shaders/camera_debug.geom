#version 330

layout(points) in;

//layout(triangle_strip, max_vertices = 30) out;
layout(line_strip, max_vertices = 30) out;
//layout(points, max_vertices = 8) out;

in DATA_OUT {
  vec4 a_out;
  vec4 b_out;
  vec4 c_out;
  vec4 d_out;
} gs_in[];

out vec3 fColor_in;

uniform mat4 MVP;
uniform vec3 camera_position;
uniform vec3 camera_front;

//  output[x_coord + global_x_dim * y_coord] = (float16){(float)ray.origin.x,          // a.x
//                                                       (float)ray.origin.y,          // a.y
//                                                       (float)ray.origin.z,          // a.z
//                                                       (float)ray.direction.x,       // a.w
//                                                       (float)ray.direction.y,       // b.x
//                                                       (float)ray.direction.z,       // b.y
//                                                       (float)intersection_point.x,  // b.z
//                                                       (float)intersection_point.y,  // b.w
//                                                       (float)intersection_point.z,  // c.x
//                                                       (float)accum_color.x,         // c.y
//                                                       (float)accum_color.y,         // c.z
//                                                       (float)accum_color.z,         // c.w
//                                                       (float)v.x,                   // d.x
//                                                       (float)v.y,                   // d.y
//                                                       (float)v.z,                   // d.z
//                                                       666.0f                        // d.w
//                                                       };

void printRays()
{
  float pSize = 1.0; // block_size;
  vec3 theCOLOR = vec3(1.0,0.2,0.2); // gs_in[0].color;
  
  vec4 a_out = gs_in[0].a_out;
  vec4 b_out = gs_in[0].b_out;
  vec4 c_out = gs_in[0].c_out;

  vec3 ray_direction = vec3(a_out.w,b_out.x,b_out.y);
  vec3 intersection_point = vec3(b_out.z,b_out.w,c_out.x);

  vec4 p0 = MVP * vec4(a_out.xyz, 1.0);
  vec4 p1 = MVP * vec4(a_out.xyz + min(30.0f,length(a_out.xyz-intersection_point)) * ray_direction, 1.0);

//                                                       (float)intersection_point.x,  // b.z
//                                                       (float)intersection_point.y,  // b.w
//                                                       (float)intersection_point.z,  // c.x
  if (c_out.y == 0.1 && c_out.z == 0.1 && c_out.w == 0.1) return;
  gl_Position = p0;
  fColor_in = vec3(c_out.y,c_out.z,c_out.w);
  EmitVertex();

  gl_Position = p1;
  fColor_in = vec3(c_out.y,c_out.z,c_out.w);
  EmitVertex();

  EndPrimitive();

//  gl_Position = MVP * vec4(camera_position,1.0);
//  fColor_in = vec3(0.0,1.0,0.0);
//  EmitVertex();
//
//  gl_Position = MVP * vec4(camera_front,1.0);
//  fColor_in = vec3(0.0,1.0,0.0);
//  EmitVertex();
//
//  EndPrimitive();

//  float pSize = 1.0; // block_size;
//  vec3 theCOLOR = vec3(1.0,0.2,0.2); // gs_in[0].color;
//  
//  vec4 a_out = gs_in[0].a_out;
//  vec4 b_out = gs_in[0].b_out;
//
//  vec4 p0 = vec4(a_out.xyz, 1.0);
//  vec4 p1 = (vec4(a_out.xyz, 1.0) + vec4(1.0,1.0,1.0,0.0)); // vec4(a_out.w, b_out.x, b_out.y, 0.0));
//
//  gl_Position = p0;
//  fColor_in = theCOLOR;
//  EmitVertex();
//
//  gl_Position = p1;
//  fColor_in = theCOLOR;
//  EmitVertex();
//
//  EndPrimitive();
//
//  gl_Position = vec4(camera_position,1.0);
//  fColor_in = vec3(0.0,1.0,-6.0);
//  EmitVertex();
//
//  gl_Position = vec4(camera_position.1.0) + vec4(camera_front,0.0);
//  fColor_in = vec3(0.0,1.0,-6.0);
//  EmitVertex();
//
//  EndPrimitive();
}


void main(){
  printRays();
}

