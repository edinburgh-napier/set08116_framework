#version 410

// MVP transformation matrix
uniform mat4 MVP;

// Incoming position
layout (location = 0) in vec3 position;

// Outgoing 3D texture coordinate
layout (location = 0) out vec3 tex_coord;

void main()
{
	// Calculate screen space position
	gl_Position = MVP * vec4(position, 1.0);

	// *******************************
	// Set outgoing texture coordinate
	// *******************************
	tex_coord = position;
}