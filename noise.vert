uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat4 normalMatrix;
uniform vec4 lightPos;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

attribute vec4 a_position;
attribute vec4 a_normal;
attribute vec2 a_texcoord;

varying float LightIntensity;
varying vec2 tex_f;
varying vec3 testData;

varying vec2 ModelPos;
uniform vec3 loc;

void main()
{
	vec3 P = vec3(mvMatrix * a_position);
	vec3 N = normalize((normalMatrix * vec4(a_normal.xyz,0.0)).xyz);
	vec3 L = normalize(lightPos.xyz - P);
	vec3 R = reflect(-L, N);
	vec3 V = normalize(-P);
	float Id = max(dot(L,N) , 0.0);
	float Is = (Id>0.0) ? pow(max(dot(R,V) , 0.0) , shininess) : 0.0;
	vec3 color = ambient + Id*diffuse + Is*specular;
	LightIntensity = length(color);
	tex_f = a_texcoord;
	gl_Position = mvpMatrix * a_position;
	ModelPos = loc.z*a_position.xy*4 - loc.xy;
	//testData = normalize((normalMatrix * vec4(a_normal.xyz,0.0)).xyz);
}
