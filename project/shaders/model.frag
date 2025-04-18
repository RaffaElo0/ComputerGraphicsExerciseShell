#version 330 core

// === Fragment Shader ===
// Performs simple Phong-like lighting using a single directional light and a diffuse texture

out vec4 FragColor;     // Final pixel color output

in vec2 TexCoords;      // Interpolated texture coordinates
in vec3 FragPos;        // Interpolated world-space position
in vec3 Normal;         // Interpolated world-space normal

uniform sampler2D texture_diffuse1; // Diffuse texture sampler (texture unit 0)

// Lighting uniforms
uniform vec3 lightDir;   // Directional light direction (world space)
uniform vec3 lightColor; // Color/intensity of the light
uniform vec3 viewPos;    // Camera/world-space position (for specular, if needed)

void main() {
    // Sample the diffuse texture color
    vec3 color = texture(texture_diffuse1, TexCoords).rgb;

    // --- Ambient component ---
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor * color;

    // --- Diffuse component ---
    vec3 norm = normalize(Normal);                // Normalize interpolated normal
    vec3 lightDirection = normalize(-lightDir);   // Invert to get direction from fragment to light
    float diff = max(dot(norm, lightDirection), 0.0); // Lambert's cosine law
    vec3 diffuse = diff * lightColor * color;

    // Sum components (no specular in this simple example)
    vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0);
}
