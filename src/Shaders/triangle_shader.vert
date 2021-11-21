#version 330 core
layout (location = 0) in vec3 position; // vertex_position attribute
layout (location = 1) in vec2 tex_coords; // texture_coordinate attribute

out vec2 texCoords;

uniform mat4 model_tri;
uniform mat4 view_tri;
uniform mat4 orthographic_projection_tri;

void main()
{
	gl_Position = orthographic_projection_tri * view_tri * model_tri * vec4(position, 1.0);
}