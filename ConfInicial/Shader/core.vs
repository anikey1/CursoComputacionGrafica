#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

// Usamos el layout 1 para recibir el factor de brillo.
// Aunque en C++ enviamos 3 floats (el 4to, 5to y 6to), aquí solo declaramos
// un float, y OpenGL automáticamente tomará el primero de esos tres.
layout (location = 1) in float brightnessFactor;
out float brightness; // Variable de salida hacia el Fragment Shader

out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position =projection*view*model*vec4(position, 1.0f);
    ourColor = color;
    // Simplemente pasamos el valor que leímos del arreglo de vértices
    brightness = brightnessFactor;
}