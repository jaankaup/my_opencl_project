#version 330

// The input data is a point. A coordinate for base cube corner. 
layout(points) in;

// The output will be a triangle. 
layout(triangle_strip, max_vertices = 15) out;
//layout(line_strip, max_vertices = 30) out;
//layout(points, max_vertices = 8) out;

// Output vertex normal.
//out vec3 fNormalIn;
out vec3 fColorIn;

uniform float block_size;
uniform vec4 base_position;
uniform mat4 MVP;
uniform float x_offset;
uniform float y_offset;

struct Cube
{
  vec4 v0;
  vec4 v1;
  vec4 v2;
  vec4 v3;
  vec4 v4;
  vec4 v5;
  vec4 v6;
  vec4 v7;
};

void printCube(Cube c, vec3 theColor)
{
  float pSize = 1.0;
  float maskColor = mask / 255.0;
  
  // FRONT sideFront = v0 v1 v2 v3
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v0.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v1.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v2.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v3.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v0.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  EndPrimitive();
  
  // TOP sideFront = v0 v5 v6 v2
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v1.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v5.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v6.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v2.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v1.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  EndPrimitive();
  
  // RIGHT sideRight = v3 v2 v6 v7
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v3.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v2.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v6.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v7.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v3.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  EndPrimitive();
  
  // LEFT sideLeft = v0 v1 v5 v4
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v0.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v1.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v5.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v4.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v0.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  EndPrimitive();
  
  // BACK sideBack = v4 v5 v6 v7
  
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v4.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v5.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v6.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v7.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v4.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  EndPrimitive();
  
  // BOTTOM sideBottom = v0 v4 v7 v3
  
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v0.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v4.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v7.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v3.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  gl_PointSize = pSize;
  gl_Position =  MVP * vec4(c.v0.xyz,1.0);
  fColorIn = theColor; 
  
  EmitVertex();
  EndPrimitive();
}

void main() {

  float d = block_size;
  vec3 p0 = gl_in[0].gl_Position.xyz*d;
  vec3 p1 = gl_in[0].gl_Position.xyz*d + vec3(0.0 ,   d , 0.0);
  vec3 p2 = gl_in[0].gl_Position.xyz*d + vec3(d   ,   d , 0.0);
  vec3 p3 = gl_in[0].gl_Position.xyz*d + vec3(d   , 0.0 , 0.0);
  vec3 p4 = gl_in[0].gl_Position.xyz*d + vec3(0.0 , 0.0 , d);
  vec3 p5 = gl_in[0].gl_Position.xyz*d + vec3(0.0 ,   d , d);
  vec3 p6 = gl_in[0].gl_Position.xyz*d + vec3(d   ,   d , d);
  vec3 p7 = gl_in[0].gl_Position.xyz*d + vec3(d   , 0.0 , d);
}
