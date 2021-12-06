#version 330 core

out vec4 color;
in vec2 texCoords;

uniform sampler2D u_Sampler;

void main()
{
	color = texture(u_Sampler, texCoords);
	//if(color.r < 0.6f && color.g < 0.6f && color.b < 0.6f)
	//{
	//	discard;		
	//}
}