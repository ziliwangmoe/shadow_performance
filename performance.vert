attribute vec4 a_position;
varying vec2 pos;
void main()
{
	gl_Position = a_position;
	pos = a_position.xy;
}
