#version 330

layout(points) in;

//layout(triangle_strip, max_vertices = 30) out;
layout(line_strip, max_vertices = 30) out;
//layout(points, max_vertices = 8) out;

out vec3 color_in;

uniform mat4 MVP;
uniform float block_size = 1.0;
uniform vec4 base_position;

//in gl_PerVertex
//{
//  vec4 gl_position;
//  float gl_PointSize;
//  float gl_ClipDistance;
//} gl_in[];

//void printCubeCorners(vec4 c)
//{
//    float pSize = 5.0;
//
//    gl_PointSize = pSize;
//	  gl_Position =  MVP * (vec4(c.v0.xyz,1.0));
//          if (densityValue)
//          {
//            fColorIn = vec3(texture(diffuse3DTexture,c.v0.xyz).w);
//          }
//          else { fColorIn = texture(diffuse3DTexture,c.v0.xyz).xyz; }
//          EmitVertex();
//
//    gl_PointSize = pSize;
//	  gl_Position =  MVP * (vec4(c.v1.xyz,1.0));
//          if (densityValue)
//          {
//            fColorIn = vec3(texture(diffuse3DTexture,c.v1.xyz).w);
//          }
//          else { fColorIn = texture(diffuse3DTexture,c.v1.xyz).xyz; }
//          EmitVertex();
//    gl_PointSize = pSize;
//	  gl_Position =  MVP * (vec4(c.v2.xyz,1.0));
//          if (densityValue)
//          {
//            fColorIn = vec3(texture(diffuse3DTexture,c.v2.xyz).w);
//          }
//          else { fColorIn = texture(diffuse3DTexture,c.v2.xyz).xyz; }
//          EmitVertex();
//    gl_PointSize = pSize;
//	  gl_Position =  MVP * (vec4(c.v3.xyz,1.0));
//          if (densityValue)
//          {
//            fColorIn = vec3(texture(diffuse3DTexture,c.v3.xyz).w);
//          }
//          else { fColorIn = texture(diffuse3DTexture,c.v3.xyz).xyz; }
//          EmitVertex();
//    gl_PointSize = pSize;
//	  gl_Position =  MVP * (vec4(c.v4.xyz,1.0));
//          if (densityValue)
//          {
//            fColorIn = vec3(texture(diffuse3DTexture,c.v4.xyz).w);
//          }
//          else { fColorIn = texture(diffuse3DTexture,c.v4.xyz).xyz; }
//          EmitVertex();
//    gl_PointSize = pSize;
//	  gl_Position =  MVP * (vec4(c.v5.xyz,1.0));
//          if (densityValue)
//          {
//            fColorIn = vec3(texture(diffuse3DTexture,c.v5.xyz).w);
//          }
//          else { fColorIn = texture(diffuse3DTexture,c.v5.xyz).xyz; }
//          EmitVertex();
//    gl_PointSize = pSize;
//	  gl_Position =  MVP * (vec4(c.v6.xyz,1.0));
//          if (densityValue)
//          {
//            fColorIn = vec3(texture(diffuse3DTexture,c.v6.xyz).w);
//          }
//          else {fColorIn = texture(diffuse3DTexture,c.v6.xyz).xyz; }
//          EmitVertex();
//    gl_PointSize = pSize;
//	  gl_Position =  MVP * (vec4(c.v7.xyz,1.0));
//          if (densityValue)
//          {
//            fColorIn = vec3(texture(diffuse3DTexture,c.v7.xyz).w);
//          }
//          else {fColorIn = texture(diffuse3DTexture,c.v7.xyz).xyz; }
//          EmitVertex();
// 
//          EndPrimitive();
//}

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
  float d = block_size; // block_size;
  vec3 theCOLOR = vec3(1.0,0.0,0.0);

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
    color_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p1,1.0);
    color_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p2,1.0);
    color_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p3,1.0);
    color_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p0,1.0);
    color_in = theCOLOR;
    EmitVertex();

    EndPrimitive();

    // TOP sideFront = v1 v5 v6 v2
    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p1,1.0);
    color_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p5,1.0);
    color_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p6,1.0);
    color_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p2,1.0);
    color_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p1,1.0);
    color_in = theCOLOR;
    EmitVertex();

    EndPrimitive();

    // RIGHT sideRight = v3 v2 v6 v7
    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p3,1.0);
    color_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p2,1.0);
    color_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p6,1.0);
    color_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p7,1.0);
    color_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p3,1.0);
    color_in = theCOLOR;
    EmitVertex();

    EndPrimitive();

    // LEFT sideLeft = v0 v1 v5 v4
    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p0,1.0);
    color_in = vec3(0.0,1.0,0.0); //theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p4,1.0);
    color_in = vec3(0.0,1.0,0.0); //theCOLOR;
    
    EmitVertex();
    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p5,1.0);
    color_in = vec3(0.0,1.0,0.0); //theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p1,1.0);
    color_in = vec3(0.0,1.0,0.0); //theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p0,1.0);
    color_in = vec3(0.0,1.0,0.0); //theCOLOR;
    EmitVertex();

    EndPrimitive();

    // BACK sideBack = v4 v5 v6 v7

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p4,1.0);
    color_in = vec3(0.0,0.0,1.0); //theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p7,1.0);
    color_in = vec3(0.0,0.0,1.0); //theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p6,1.0);
    color_in = vec3(0.0,0.0,1.0); //theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p5,1.0);
    color_in = vec3(0.0,0.0,1.0); //theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p4,1.0);
    color_in = vec3(0.0,0.0,1.0); //theCOLOR;
    EmitVertex();

    EndPrimitive();

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

    // BOTTOM sideBottom = v0 v4 v7 v3

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p0,1.0);
    color_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p3,1.0);
    color_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p7,1.0);
    color_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p4,1.0);
    color_in = theCOLOR;
    EmitVertex();

    gl_PointSize = pSize;
    gl_Position =  MVP * vec4(p0,1.0);
    color_in = theCOLOR;
    EmitVertex();

    EndPrimitive();
}


void main(){
  vec4 pos = vec4(gl_in[0].gl_Position.xyz * block_size + base_position.xyz, 0.0);
  printCube(pos.xyz);
}

