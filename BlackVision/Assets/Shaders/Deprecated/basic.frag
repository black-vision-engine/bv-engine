#version 400

in vec3 Color;
out vec4 FragColor;

void main() {
    FragColor = vec4(Color, 0.5);
}
