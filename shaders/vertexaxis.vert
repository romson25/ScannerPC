attribute vec3 position;
attribute vec3 color;

uniform mat4 mvp;

varying vec3 myColor;

void main(void)
{
    gl_Position = mvp * vec4(position, 1.0);
    myColor = color;
}

