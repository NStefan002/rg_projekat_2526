//#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 tex_coords;
out vec3 normal;
out vec3 frag_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    frag_pos = vec3(model * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(model))) * aNormal;
    tex_coords = aTexCoords;
    gl_Position = projection * view * vec4(frag_pos, 1.0);
}

//#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 tex_coords;
in vec3 normal;
in vec3 frag_pos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 view_pos;

// Directional light
struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionalLight dir_light;

// Point light
struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    bool enabled;
};
uniform PointLight point_light;

vec3 calc_dir_light(DirectionalLight light, vec3 norm, vec3 view_dir, vec3 diff_tex, vec3 spec_tex) {
    vec3 light_dir = normalize(-light.direction);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);
    vec3 ambient = light.ambient  * diff_tex;
    vec3 diffuse = light.diffuse  * diff * diff_tex;
    vec3 specular = light.specular * spec * spec_tex;
    return ambient + diffuse + specular;
}

vec3 calc_point_light(PointLight light, vec3 norm, vec3 view_dir, vec3 diff_tex, vec3 spec_tex) {
    vec3 light_dir = normalize(light.position - frag_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);
    float dist = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
    vec3 ambient = light.ambient  * diff_tex  * attenuation;
    vec3 diffuse = light.diffuse  * diff * diff_tex  * attenuation;
    vec3 specular = light.specular * spec * spec_tex * attenuation;
    return ambient + diffuse + specular;
}

void main() {
    vec3 norm = normalize(normal);
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 diff_tex = texture(texture_diffuse1, tex_coords).rgb;
    vec3 spec_tex = texture(texture_specular1, tex_coords).rgb;

    vec3 result = calc_dir_light(dir_light, norm, view_dir, diff_tex, spec_tex);
    if (point_light.enabled) {
        result += calc_point_light(point_light, norm, view_dir, diff_tex, spec_tex);
    }
    FragColor = vec4(result, 1.0);
}
