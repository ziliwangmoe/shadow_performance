uniform mat4 mvpMatrix;
attribute vec4 a_position;
attribute vec2 a_texcoord;
varying vec2 texCoor;
void main()
{
	gl_Position = a_position;
	gl_Position.xy=gl_Position.xy*1.0;
	texCoor = a_texcoord;
	if(a_position.z<=0){
		gl_Position.z =0.5;
	}else{
		gl_Position.z =5.0;
	}
}
