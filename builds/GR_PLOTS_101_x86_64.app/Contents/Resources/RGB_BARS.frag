#version 150

out vec4 outputColor;

uniform vec2 resolution;
uniform vec3 cha;

// SMPTE COLOR BARS
// 100% AND 75%

void main(){
    float alpha = 1.0;
    
    float baseX = gl_FragCoord.x                / resolution.x;
    float baseY = (resolution.y-gl_FragCoord.y) / resolution.y;
    
    float   red     = baseX;
    float   green   = baseX;
    float   blue    = baseX;

    
    // green
    green = trunc(green * 2);
    green = clamp(1-green, 0., 1.);
    
    
    // blue
    blue = sin(blue * (3.14*6));
    blue = trunc(blue+1.);
    blue = clamp(1-blue, 0., 1.);
    
    // red
    float red2;
    red = gl_FragCoord.x < (resolution.x/6) ? 1. : 0.;
    red2 = (resolution.x-gl_FragCoord.x) < (resolution.x/6.) ? 1. : 0.;
    red2 = (1-red2) * clamp((1-green),0.,1.);
    red = clamp((red + red2),0.,1.);
    
    
    // mid row
    float row;
    float unit = (resolution.y/5);
    float topU = unit*2;
    float botU = unit*3;
    
    float TOP = (gl_FragCoord.y < topU ? 1. : 0.);
    float BOT = (gl_FragCoord.y > botU ? 1. : 0.);
    
    row =  TOP + BOT;
    row = clamp((1-row), 0., 1.);
    
    // top and bottom ramps
    float botRamp;
    botRamp = ( (resolution.y-botU)-(gl_FragCoord.y-botU) ) / (resolution.y-botU);
    botRamp = clamp((BOT * botRamp),0.,1.);

    float topRamp;
    topRamp = ( (resolution.y-botU)-((resolution.y-gl_FragCoord.y)-botU) ) / (resolution.y-botU);
    topRamp = clamp((1-topRamp),0.,1.);

    
    // segmentation
    float rowR = red    * row;
    float rowG = green  * row;
    float rowB = blue   * row;
    
    float topR = (red   + topRamp) * TOP;
    float topG = (green + topRamp) * TOP;
    float topB = (blue  + topRamp) * TOP;
    
    float botR = red    * botRamp;
    float botG = green  * botRamp;
    float botB = blue   * botRamp;
    
    // final color
    red     = topR + rowR + botR;
    green   = topG + rowG + botG;
    blue    = topB + rowB + botB;

    outputColor =  vec4(red, green, blue, alpha);
}
