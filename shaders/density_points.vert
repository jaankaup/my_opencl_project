#version 330

layout(location = 0) in float density_value;

uniform mat4 MVP;
uniform vec4 base_position;
uniform float block_size;
uniform int x_offset;
uniform int y_offset;
uniform int z_offset;
uniform int show_surface = 0;

//varying out vec3 fColor_in;

out COLOR_OUT {
    vec3 color;
} vs_out;

void main()
{
  int index = gl_VertexID;
  float x = int(mod(index,(x_offset*4))); 
  float y = int(mod(index / (x_offset*4),(y_offset*4))); 
  float z = int(mod(index / (x_offset * y_offset * 16), z_offset*4)); 
  gl_Position = vec4((vec4(x,y,z,0.0) * block_size + base_position).xyz,1.0);
  float clamped = (5 + clamp(density_value,-5,5))/10.0;
  vec3 temp_color = vec3(0.0);;
  if (show_surface == 0) temp_color = mix(vec3(0.0,0.0,0.5), vec3(0.5,0.0,0.0), clamped) ;
  if (abs(density_value) <= 0.1) {
    temp_color = vec3(1.0,1.0,1.0); mix(vec3(0.0,1.0,1.0), vec3(1.0,1.0,0.0), (2+clamped)/4.0);
  }
  vs_out.color = temp_color;
}
