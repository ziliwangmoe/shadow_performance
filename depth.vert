uniform mat4 MVPMatrix;
attribute vec4 a_position;
void main()
{
	gl_Position = MVPMatrix * a_position;
}