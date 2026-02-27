#version 330 core
in vec3 ourColor;

out vec4 color;

in float brightness; // Recibimos el factor desde el Vertex Shader
uniform vec3 pastelColor; 

void main()
{
	//color = vec4(ourColor, 1.0f);
	vec3 finalColor = pastelColor * brightness;

    color = vec4(finalColor, 1.0f);
}