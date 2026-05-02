#version 450
layout(location=0) in vec2 aPos;
layout (location = 1) uniform ubo_Camera {
  mat4 model;
  mat4 view;
  mat4 proj;
};

void main() {
  gl_Position = model * view * proj * vec4(aPos, 0.0, 1.0);
}
