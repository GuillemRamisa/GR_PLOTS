
#version 150

out vec4 outputColor;

uniform vec2 resolution;
uniform vec2 MP;

void main(){
//    float red   = gl_FragCoord.x / resolution.x;
//    float green = gl_FragCoord.y / resolution.y;
    float red;
    float green;
    float blue;
    float crux;
    float amb;

    vec2 center = vec2(MP.x, MP.y);
    vec2 direction = gl_FragCoord.xy - center;
    float distance = length(direction);
    float distanceNorm = distance / resolution.y;
    
    if (gl_FragCoord.x >= MP.x && gl_FragCoord.x <= (MP.x+2)) {
        green = 1.0;
    }else{
        green = 0.0;
    }
    
    if (gl_FragCoord.y >= MP.y && gl_FragCoord.y <= (MP.y+2)){
        red = 1.0;
    }else{
        red = 0.0;
    }

    blue = 1 - distanceNorm;
    crux = ((red + green)*0.05) * pow(blue,0.8457);
    
//    blue = ((blue/255) * 64);
    blue = ((blue/255) * 64);
    blue = clamp(blue,0.,1.);

    blue += (8./255.);
    blue = pow(blue,1.22);
//    blue += (16./255.);
//    blue += crux;
    
    float alpha = 1.0;

    outputColor =  vec4(blue, blue, blue, alpha);
}
