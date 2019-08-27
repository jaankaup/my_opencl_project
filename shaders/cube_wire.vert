#version 330

layout(location = 0) in float hopohopo;

uniform vec4 base_position;
uniform float block_size;
uniform int x_offset;
uniform int y_offset;
uniform int z_offset;
uniform int index;

void main()
{
//  float group_id = mod(index,64.0); 
//  float group_x = mod(group_id, x_offset);
//  float group_y = mod(group_id, x_offset*y_offset);
//  float group_z = mod(group_id, x_offset*y_offset*z_offset);
  //float x = mod(index,x_offset) * block_size + base_position.x; 
  //float y = mod(index,x_offset * y_offset) * block_size + base_position.y; 
  //float z = mod(index,z_offset * y_offset * z_offset) * block_size + base_position.z; 
  //float x = mod(index,x_offset) * block_size + base_position.x; 
  //float y = mod(index,x_offset * y_offset) * block_size + base_position.y; 
  //float z = mod(index,x_offset * y_offset * z_offset) * block_size + base_position.z; 
  //float joop = index - group_id * 64;

  float x = mod(index,x_offset)                           * block_size + base_position.x; 
  float y = mod(index / x_offset,y_offset)                *  block_size + base_position.y; 
  float z = mod(index / (x_offset * y_offset), z_offset)  * block_size + base_position.z; 

  //float x = mod(index,4.0*x_offset) * block_size + base_position.x; 
  //float y = mod(index,4.0*x_offset*y_offset) * y_offset * block_size + base_position.y; 
  //float z = mod(index,4.0*x_offset*y_offset * z_offset) * block_size + base_position.z; 
  gl_Position =  vec4(x,y,z,1.0);
  //gl_Position =  vec4(0.0,0.0,0.0 + hopohopo,1);
  //gl_Position =  vec4(float(index)*0.01,float(index)*0.01,float(index)*0.01 + hopohopo,1);
}
