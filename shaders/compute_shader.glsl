#version 430 core
layout (local_size_x = 256) in; // Workgroup size, adjust as needed

struct Particle {
    vec3 position;
    vec3 velocity;
    float mass;
};

layout(std430, binding = 0) buffer ParticlesBuffer {
    Particle particles[];
};

uniform float deltaTime;
uniform float G; // Gravitational constant

void main() {
    uint index = gl_GlobalInvocationID.x;
    if (index >= particles.length()) return;

    vec3 force = vec3(0.0);
    for (uint j = 0; j < particles.length(); ++j) {
        if (index != j) {
            vec3 direction = particles[j].position - particles[index].position;
            float distance = length(direction) + 0.01f; // avoid division by zero
            float forceMagnitude = (G * particles[index].mass * particles[j].mass) / (distance * distance);
            force += forceMagnitude * normalize(direction);
        }
    }

    // Update velocity and position
    particles[index].velocity += force / particles[index].mass;
    particles[index].position += particles[index].velocity * deltaTime;
}
