#version 150

out vec4 outputColor;

uniform vec2 resolution;
uniform vec3 cha;

// 16 * 16 cube
// XYZ

void main(){
    float alpha = 1.0;

    
    float   red       = gl_FragCoord.x / resolution.x;
    float   green     = gl_FragCoord.x / resolution.x;
    float   blue      = (resolution.y - gl_FragCoord.y) / resolution.y;
    
    float global_size = 16;
    
    // red
    float r_size    = global_size/4;
    float r_sss     = 1.0f/r_size;
    
    red             = mod(red, r_sss) * r_size;
    red             = trunc(red * (r_size*4));
    red             = red/((r_size*4)-1);
    
    // blue
    float b_size    = global_size;
    float b_sss     = 1.0f/b_size;
    blue            = trunc(blue * b_size);
    blue            = blue/(b_size-1);
    
        
    // green
    float g_size    = global_size/4;
    float g_sss     = 1.0f/g_size;
    green           = trunc(green * g_size);
    green           = green/(g_size-1);
    green *= 0.2f;
    
    
    float green2 = (resolution.y-gl_FragCoord.y) / resolution.y;
    green2 = mod(green2, 1/global_size)*global_size;
    green2 = trunc(green2*4)/(4-1);
    
    green = green + (green2*0.8);
    
    outputColor =  vec4(red, green, blue, alpha);
}
