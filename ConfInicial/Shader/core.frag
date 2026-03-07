#version 330 core
out vec4 color;

in vec3 localPos;

uniform vec3 objectColor;

void main()
{
    vec3 base = objectColor;
    float shade = 1.0;

    // Detectar qué cara del cubo estamos viendo
    float ax = abs(localPos.x);
    float ay = abs(localPos.y);
    float az = abs(localPos.z);

    if (ay >= ax && ay >= az)
    {
        // Cara superior o inferior
        if (localPos.y > 0.0)
            shade = 1.08;   // arriba ligeramente más clara
        else
            shade = 0.72;   // abajo más oscura
    }
    else if (ax >= ay && ax >= az)
    {
        // Lados izquierda/derecha
        if (localPos.x > 0.0)
            shade = 0.88;
        else
            shade = 0.80;
    }
    else
    {
        // Frente y atrás
        if (localPos.z > 0.0)
            shade = 0.96;
        else
            shade = 0.84;
    }

    color = vec4(base * shade, 1.0);
}