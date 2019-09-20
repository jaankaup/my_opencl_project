#version 330

in vec4 fCoords;
in vec4 nCoords;

out vec3 color;

//uniform mat4 viewMatrix;
//uniform vec3 cameraPosition;

void main(){
  //gl_FragCoord = vec4(fCoords.x, fCoords.y, 0.0, 1.0);   
  //gl_PointCoord = vec2(fCoords.x/1024.0,fCoords.y/1024.0);
  //color = vec3((1.0 + nCoords.x)*0.5, (1.0 + nCoords.y) * 0.5, (1.0 + nCoords.z) * 0.5); // nCoords.xyz;
  //color = vec3((1.0 + nCoords.x) * 0.5, (1.0 + nCoords.y) * 0.5, (1.0 + nCoords.z) * 0.5); // nCoords.xyz;
  float gamma = 0.8;
  vec3 final_color = mix(vec3(nCoords.x, nCoords.y, nCoords.z), vec3(1.0,0.0,0.0),(1+nCoords.w)*0.2); // nCoords.xyz;
  color = pow(final_color, vec3(1.0/gamma));
  //color = vec3(0.5); // nCoords.xyz;
}
