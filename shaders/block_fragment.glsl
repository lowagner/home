#version 120

uniform sampler2D sampler;
uniform sampler2D sky_sampler;
uniform float timer;
uniform float daylight;
uniform int ortho;

varying vec3 fragment_glaze;
varying vec2 fragment_uv;
varying float fragment_ao;
varying float fragment_light;
varying float fog_factor;
varying float fog_height;
varying float diffuse;

const float pi = 3.14159265;

void main() {
    vec3 color;
    bool cloud;
    float alpha = 1;
    if (fragment_glaze[0] == 0.99 && fragment_glaze[1] == 0.99 && fragment_glaze[2] == 1.0) {
        // water
        float sint = sin(20*pi*timer);
        float cost = cos(20*pi*timer);
        color = vec3(texture2D(sampler, fragment_uv + (0.5*0.0625)*vec2(1+cost, 1-sint)));
        cloud = false;
        //alpha = 0.75 * 0.5*(1+fragment_light);
    } else {
        color = vec3(texture2D(sampler, fragment_uv));
        cloud = color == vec3(1.0, 1.0, 1.0);
        if (cloud && bool(ortho)) {
            discard;
        }
        //alpha = 1;
    }

    if (color == vec3(1.0, 0.0, 1.0)) {
        discard;
    }
    float value = (fragment_glaze[0] + fragment_glaze[1] + fragment_glaze[2])/3.0;
    color = (1.0-value)*fragment_glaze + value*color;

    float df = cloud ? 1.0 - diffuse * 0.2 : diffuse;
    float ao = cloud ? 1.0 - (1.0 - fragment_ao) * 0.2 : fragment_ao;
    ao = min(1.0, ao + fragment_light);
    df = min(1.0, df + fragment_light);
    value = min(1.0, daylight + fragment_light);
    vec3 light_color = vec3(value * 0.3 + 0.2);
    vec3 ambient = vec3(value * 0.3 + 0.2);
    vec3 light = ambient + light_color * df;

    color = clamp(color * light * ao, vec3(0.0), vec3(1.0));
    vec3 sky_color = vec3(texture2D(sky_sampler, vec2(timer, fog_height)));
    color = mix(color, sky_color, fog_factor);
    gl_FragColor = vec4(color, alpha);
}
