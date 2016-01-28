uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat4 normalMatrix;
uniform vec4 lightPos;

attribute vec4 a_position;
attribute vec4 a_normal;
attribute vec2 a_texcoord;
attribute vec4 a_tangent;

varying vec3 testData;
varying vec2 ModelPos;
varying vec3 lightDirT;
varying vec3 eyeDirT;

uniform vec3 loc;


void main()
{
	vec3 P = vec3(mvMatrix * a_position);
	vec3 N = normalize((normalMatrix * vec4(a_normal.xyz,0.0)).xyz);
	vec3 L = normalize(lightPos.xyz - P);
	vec3 V = normalize(-P);
	vec3 T = normalize((normalMatrix * vec4(a_tangent.xyz,0.0)).xyz);
	vec3 B = cross(N,T);

	lightDirT.x = dot(L, B);
	lightDirT.y = dot(L, T);
	lightDirT.z = dot(L, N);
	lightDirT = normalize(lightDirT);

	eyeDirT.x = dot(V, B);
	eyeDirT.y = dot(V, T);
	eyeDirT.z = dot(V, N);
	eyeDirT = normalize(eyeDirT);

	ModelPos = loc.z*a_position.xy*4 - loc.xy;
	gl_Position = mvpMatrix * a_position;
	//testData = normalize((normalMatrix * vec4(a_normal.xyz,0.0)).xyz);
}
