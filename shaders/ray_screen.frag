#version 330

uniform sampler2D ray_texture;

in vec4 point_pos;
out vec3 color;

void main(){
	 //vec3 temp = texture(ray_texture,vec2(gl_FragCoord.x, gl_FragCoord.y)).rgb;
   //color = mix(temp,vec3(0.0,0.5,0.5),0.5); 
   //color = temp; 
   //color = vec3(gl_PointCoord.x,gl_PointCoord.y, 1.0); 
   //color = vec3(gl_FragCoord.x,gl_FragCoord.y, 0.0); 
	 vec3 temp = texture(ray_texture,vec2(point_pos.x, point_pos.y)).rgb;
	 //vec3 temp = texture(ray_texture,vec2(gl_FragCoord.x, gl_FragCoord.y)).rgb;
   color = temp; // point_pos.xyz;
}
