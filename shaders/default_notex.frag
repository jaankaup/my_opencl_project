#version 330

in vec3 vFrag_in;
in vec3 nFrag_in;
in vec3 nFrag_in_flat;

out vec3 color;

struct Light {
       vec3 position;
       vec3 color;
       vec3 materialSpecularColor;
       float ambientCoeffience;
       float materialShininess;
       float attentuationFactor;
};

uniform sampler2D diffuseTexture;
uniform sampler2D diffuseTexture2;
uniform mat4 M;
uniform vec3 cameraPosition;
uniform mat3 normalMatrix;
uniform Light lights[8];

void main(){
        //color = vFrag_in;
        //color = vec3(0.2f,1.0f,0.9f);
        //color = vec3(vFrag_in);
        // Verteksin paikka maailmassa.
	vec3 vPositionCamera = (M * vec4(vFrag_in,1.0f)).xyz;

	// Normaali. Jos mallia on venytetty ja liikuteltu, niin normaali täyy kertoa normaali matriisilla.
	vec3 normal = normalize(normalMatrix * nFrag_in);
	normal = nFrag_in;

	// Valon suunta verteksiin näen.
	vec3 lightDirection = normalize(lights[0].position - vPositionCamera);
	
	// Valon teho.
	float diffuseCoeffient = max(0.0 , dot(normal,lightDirection));
	
	// Valon pävastainen suuntavektori. Tarvitaan reflect funtiota varten.
	vec3 incidenceVector = -lightDirection;

	// Laskee heijastuneen valon suunnan.
	vec3 reflectionVector = reflect(incidenceVector, normal);

        // Kameran suunta suhteessa verteksiin.
        vec3 cameraDirection = normalize(cameraPosition - vPositionCamera);

        // Valon intensiteetti suhteessa kameraan. Tarvitaan siis heijastuksen laskemiseen.
        float cosAngle = max(0.0, dot(cameraDirection,reflectionVector));

        // Heijastus intensiteetti. Jos pinta on pois pä valon suunnasta, niin täöintensiteetti on 0.0;
        float specularCoeffient = 0.0;
        if (diffuseCoeffient > 0.0)
           specularCoeffient = pow(cosAngle, lights[0].materialShininess);

        // Heijastunvä.
        vec3 specularComponent = specularCoeffient * lights[0].materialSpecularColor * lights[0].color;

        // Pinnan vä.
        float x = vFrag_in.x;
        float y = vFrag_in.y;
        float z = vFrag_in.z;

	//vec3 surfaceColor = texture(diffuseTexture,vec2((x+y)/2.0,(y+z)/2.0)).rgb;
  vec2 coord1 = vFrag_in.xy;
  vec2 coord2 = vFrag_in.xz;
  vec2 coord3 = vFrag_in.zz;
	vec3 surfaceColor_grass = texture(diffuseTexture, (coord1 + coord2 + coord3)/3.0).rgb;
	vec3 surfaceColor_rock =  texture(diffuseTexture2,(0.1*(coord1 + coord2 + coord3)/3.0)).rgb;

	vec3 surfaceColor = mix(surfaceColor_rock, surfaceColor_grass, clamp(0.4*nFrag_in_flat.x + 0.6*nFrag_in_flat.y,0.0,1.0));
	//vec3 surfaceColor = vec3(0.0,1.0,0.8);;
	//vec3 surfaceColor = mix(surfaceColor_rock, surfaceColor_grass, nFrag_in.x);
	//vec3 surfaceColor = texture(diffuseTexture,vec2((x+y)/2.0,(y+z)/2.0)).rgb;
	//vec3 surfaceColor = texture(diffuse3DTexture,vFrag_in).rgb;
	//vec3 surfaceColor = vec3(0.1,0.4,0.1);

	// Ambient vä.
        vec3 ambient = lights[0].ambientCoeffience * lights[0].color * surfaceColor;

        // Pinnan vä ottaen huomioon valon teho ja vä.
        vec3 diffuseComponent = diffuseCoeffient * lights[0].color * surfaceColor;

        // Etäyys verteksistäaloon.
        float distanceToLight = distance(vPositionCamera,lights[0].position);

        // Valon vaimennuskerroin.
        float attentuation = 1.0 / (1.0 + lights[0].attentuationFactor * pow(distanceToLight,2.0));

        // Lopullinen vä.
        //color = ambient + attentuation * (diffuseComponent + specularComponent);
        //color = ambient;
	vec3 temp_color = ambient + attentuation * (diffuseComponent + specularComponent);
        color = temp_color;
//        float ix;
//        float iy;
//        float iz;
//        modf(4*temp_color.x,ix);
//        modf(4*temp_color.y,iy);
//        modf(4*temp_color.z,iz);
//        color = vec3(ix/4.0, iy/4.0, iz/4.0); 
//        //color = vec3(ix/4.0 + temp_color.x/10.0, iy/4.0+temp_color.y/4.0 + temp_color.x/4.0, iz/10.0+temp_color.z/4.0 + temp_color.x/4.0); 
//	color = mix(temp_color,vec3(0.2,0.8,0.2) ,smoothstep(0.0,1.0,(ix+iy+iz)/12));
}
