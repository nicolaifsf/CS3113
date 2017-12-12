
uniform sampler2D diffuse;
varying vec2 texCoordVar;

uniform float saturationAmount;

vec3 saturation_func(vec3 rgb, float adjustment)
{
    const vec3 W = vec3(0.2125, 0.7154, 0.1721);
    vec3 intensity = vec3(dot(rgb, W));
    return mix(intensity, rgb, adjustment);
}

void main() {
    vec4 finalColor;
    finalColor.rgb = saturation_func(texture2D(diffuse, texCoordVar).rgb, saturationAmount);
    finalColor.a = texture2D(diffuse, texCoordVar).a;
    gl_FragColor = finalColor;
    //gl_FragColor = texture2D(diffuse, texCoordVar);
}
