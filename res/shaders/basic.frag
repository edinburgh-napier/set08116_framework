#version 410

// Incoming vertex colour
layout (location = 0) in vec4 vetex_colour;

// Outgoing pixel colour
layout (location = 0) out vec4 out_colour;

void main()
{
	// Simply set outgoing colour
	out_colour = vetex_colour;
}