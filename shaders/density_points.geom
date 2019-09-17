#version 330

layout(points) in;

layout(triangle_strip, max_vertices = 30) out;
//layout(line_strip, max_vertices = 30) out;
//layout(points, max_vertices = 8) out;

in COLOR_OUT {
    vec3 color;
} gs_in[];

out vec3 fColor_in;

uniform mat4 MVP;
uniform int show_surface = 0;
//uniform float block_size = 1.0;
//uniform vec4 base_position;



//        v5                        v6
//         +------------------------+
//        /|                       /|
//       / |                      / |
//      /  |                     /  |
//     /   |                    /   |  
//    /    |                   /    |
//v1 +------------------------+ v2  |
//   |     |                  |     |
//   |     |                  |     |
//   |     |                  |     |
//   |  v4 +------------------|-----+ v7
//   |    /                   |    /
//   |   /                    |   /
//   |  /                     |  /    
//   | /                      | /
//   |/                       |/
//   +------------------------+
//  v0                       v3

void printCube(vec3 c)
{
  float pSize = 1.0; // block_size;
  float d = 0.005;
  vec3 theCOLOR = gs_in[0].color;

  vec3 p0 = c.xyz;
  vec3 p1 = c.xyz + vec3(0.0,   d, 0.0);
  vec3 p2 = c.xyz + vec3(d  ,   d, 0.0);
  vec3 p3 = c.xyz + vec3(d  , 0.0, 0.0);
  vec3 p4 = c.xyz + vec3(0.0, 0.0,  d);
  vec3 p5 = c.xyz + vec3(0.0,   d,  d);
  vec3 p6 = c.xyz + vec3(d  ,   d,  d);
  vec3 p7 = c.xyz + vec3(d  , 0.0,  d);

    // FRONT sideFront = v0 v1 v2 v3
    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p0,1.0);
    fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p1,1.0);
    fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p2,1.0);
    fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p3,1.0);
    fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p0,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    EndPrimitive();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p1,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p5,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p6,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p2,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p1,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    EndPrimitive();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p3,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p2,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p6,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p7,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p3,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    EndPrimitive();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p0,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p4,1.0);
      fColor_in = theCOLOR;
    
    EmitVertex();
    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p5,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p1,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p0,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    EndPrimitive();


    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p4,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p7,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p6,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p5,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p4,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    EndPrimitive();


    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p0,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p3,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p7,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p4,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p0,1.0);
      fColor_in = theCOLOR;
    EmitVertex();

    EndPrimitive();
}


void main(){
  vec3 pos = gl_in[0].gl_Position.xyz;
  if (show_surface != 0 && gs_in[0].color == vec3(0.0)) return;
  printCube(pos);
}

