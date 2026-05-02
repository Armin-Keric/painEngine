#version 450

layout(location = 0) in vec2 aPos;

layout(std140, binding = 1) uniform ubo_Camera {
  mat4 model;
  mat4 view;
  mat4 proj;
} camera;

void main() {
  gl_Position = camera.proj * camera.view * camera.model * vec4(aPos, 0.0, 1.0);
}