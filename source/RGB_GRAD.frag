#version 150

out vec4 outputColor;

uniform vec2 resolution;
uniform vec3 cha;

void main(){
    
    float red   = gl_FragCoord.x / resolution.x;
    float green = gl_FragCoord.y / resolution.y;
    float blue;
    
    vec2 center = vec2(resolution.x/2, resolution.y/2);
    vec2 direction = gl_FragCoord.xy - center;
    float distance = length(direction);
    float distanceNorm = distance / resolution.x;
    
    blue = 1 - distanceNorm;
    
    green = 1 - green;
    
    red     = red     * cha.x;
    green   = green   * cha.y;
    blue    = blue    * cha.z;

    
    float alpha = 1.0;
    
    outputColor =  vec4(red, green, blue, alpha);
}
