#version 440

// Cubemap texture
uniform samplerCube cubemap;

// Incoming 3D texture coordinate
layout (location = 0) in vec3 tex_coord;

// Outgoing colour
layout (location = 0) out vec4 colour;

void main()
{
	// ************************
	// Sample texture as normal
	// ************************
	colour = texture(cubemap, tex_coord);
}