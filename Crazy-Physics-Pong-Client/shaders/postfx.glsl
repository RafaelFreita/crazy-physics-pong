#version 130

uniform sampler2D texture;

void main () {
	int x = int(gl_FragCoord.x);
	int y = int(gl_FragCoord.y);
	gl_FragColor = texelFetch(texture, ivec2(x,y), 0);
}