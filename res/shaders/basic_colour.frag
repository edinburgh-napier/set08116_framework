#version 410

uniform vec4 colour;

// Outgoing colour for the shader
layout (location = 0) out vec4 out_colour;

void main()
{
	// Set outgoing colour
	out_colour = colour;
}