#version 120

uniform sampler2D sampler;
uniform sampler2D sky_sampler;
uniform float timer;
uniform float daylight;
uniform int ortho;

varying vec3 under_water;
varying vec3 fragment_glaze;
varying vec2 fragment_uv;
varying float fragment_ao;
varying float fragment_light;
varying float fog_factor;
varying float fog_height;
varying float diffuse;

const float pi = 3.14159265;

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
    vec3 color;
    bool cloud;
    float value;
    if (fragment_glaze.z < 0.0) { 
        // water
        float sint = sin(20*pi*timer);
        float cost = cos(20*pi*timer);
        color = vec3(texture2D(sampler, fragment_uv + (0.5*0.0625)*vec2(1+cost, 1-sint)));

        // rotate hue using HSV stuff
        vec3 fragment_HSV = rgb2hsv(color).xyz;
        fragment_HSV.x = mod(fragment_HSV.x + fragment_glaze.x, 1.0);
        fragment_HSV.y *= fragment_glaze.y;
        fragment_HSV.z *= -fragment_glaze.z;
        fragment_HSV.yz = clamp(fragment_HSV.yz, vec2(0.0), vec2(1.0));
        color = hsv2rgb(fragment_HSV);

        cloud = false;
    } else {
        color = vec3(texture2D(sampler, fragment_uv));
        cloud = color == vec3(1.0, 1.0, 1.0);
        if (cloud) {
            if (bool(ortho)) {
                discard;
            }
        }
        else if (color == vec3(1.0, 0.0, 1.0)) {
            discard;
        }
        else
        {
            // rotate hue using HSV stuff
            vec3 fragment_HSV = rgb2hsv(color).xyz;
            fragment_HSV.x = mod(fragment_HSV.x + fragment_glaze.x, 1.0);
            fragment_HSV.yz *= fragment_glaze.yz;
            fragment_HSV.yz = clamp(fragment_HSV.yz, vec2(0.0), vec2(1.0));
            color = hsv2rgb(fragment_HSV);
        }
    }

    float df = cloud ? 1.0 - diffuse * 0.2 : diffuse;
    float ao = cloud ? 1.0 - (1.0 - fragment_ao) * 0.2 : fragment_ao;
    ao = min(1.0, ao + fragment_light);
    df = min(1.0, df + fragment_light);
    value = min(1.0, daylight + fragment_light);
    vec3 light_color = vec3(value * 0.3 + 0.2);
    vec3 ambient = vec3(value * 0.3 + 0.2);
    vec3 light = ambient + light_color * df;

    color = clamp(color * light * ao * under_water, vec3(0.0), vec3(1.0));
    vec3 sky_color = vec3(texture2D(sky_sampler, vec2(timer, fog_height)));
    color = mix(color, sky_color, fog_factor);
    gl_FragColor = vec4(color, 1.0);
}
