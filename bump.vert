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
attribute vec2 a_tangent;

varying float LightIntensity;
varying vec3 testData;
varying vec2  ModelPos;
varying vec3  lightDirT;
varying vec3  eyeDirT;

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

	vec3 T = normalize((normalMatrix * vec4(a_tangent.xyz,0.0)).xyz);
	vec3 B = cross(N,T);
	lightPosT.x = dot(L, T);
	lightPosT.y = dot(L, B);
	lightPosT.z = dot(L, N);
	lightPosT = normalize(lightPosT);

	eyeDirT.x = dot(P, T);
	eyeDirT.y = dot(P, B);
	eyeDirT.z = dot(P, N);
	eyeDirT = normalize(eyeDirT);

	ModelPos = loc.z*a_position.xy*2 - loc.xy;
	gl_Position = mvpMatrix * a_position;
	testData = normalize((normalMatrix * vec4(a_normal.xyz,0.0)).xyz);
}
