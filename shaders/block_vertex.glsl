#version 120

uniform mat4 matrix;
uniform vec3 camera;
uniform float fog_distance;
uniform int ortho;

attribute vec3 position;
attribute vec3 normal;
attribute vec4 uv;
attribute vec3 color_glaze;

varying vec3 under_water;
varying vec3 fragment_glaze;
varying vec2 fragment_uv;
varying float fragment_ao;
varying float fragment_light;
varying float fog_factor;
varying float fog_height;
varying float diffuse;

const float pi = 3.14159265;
const vec3 light_direction = normalize(vec3(-1.0, 1.0, -1.0));

void main() {
    gl_Position = matrix * vec4(position, 1.0);
    fragment_glaze = color_glaze;
    fragment_uv = uv.xy;
    fragment_ao = 0.3 + (1.0 - uv.z) * 0.7;
    fragment_light = uv.w;
    diffuse = max(0.0, dot(normal, light_direction));
    if (bool(ortho)) {
        fog_factor = 0.0;
        fog_height = 0.0;
    }
    else {
        float camera_distance = distance(camera, position);
        if (fog_distance < 0) {
            fog_factor = pow(clamp(camera_distance / (-0.5 - fog_distance), 0.0, 1.0), 4.0); 
            under_water = vec3(0.0, 0.0, 0.5);
        }
        else {
            fog_factor = pow(clamp(camera_distance / fog_distance, 0.0, 1.0), 4.0);
            under_water = vec3(1.0, 1.0, 1.0);
        }
        float dy = position.y - camera.y;
        float dx = distance(position.xz, camera.xz);
        fog_height = (atan(dy, dx) + pi / 2) / pi;
    }
}
