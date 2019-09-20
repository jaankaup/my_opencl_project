#version 330

layout(points) in;
out vec4 point_pos;

layout(triangle_strip, max_vertices = 5) out;

void printScreen()
{

    gl_Position = vec4(-1.0f,-1.0,0.0,1.0);
    point_pos = vec4(0.0f,0.0,0.0,1.0);
    EmitVertex();

    gl_Position = vec4(1.0f,-1.0,0.0,1.0);
    point_pos = vec4(1.0f,0.0,0.0,1.0);
    EmitVertex();

    gl_Position = vec4(1.0f,1.0,0.0,1.0);
    point_pos = vec4(1.0f,1.0,0.0,1.0);
    EmitVertex();

    gl_Position = vec4(-1.0f,1.0,0.0,1.0);
    point_pos = vec4(0.0f,1.0,0.0,1.0);
    EmitVertex();

    gl_Position = vec4(-1.0f,-1.0,0.0,1.0);
    point_pos = vec4(0.0f,0.0,0.0,1.0);
    EmitVertex();

    EndPrimitive();
}

void main() {
  printScreen();
}

