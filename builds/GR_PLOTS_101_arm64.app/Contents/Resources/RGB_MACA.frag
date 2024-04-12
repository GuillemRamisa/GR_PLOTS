
#version 150

out vec4 outputColor;

uniform vec2 resolution;

// color poatches from colour-science --> DATA_COLORCHECKER1976
/*
 Reference *ColorChecker Classic* sample labels.
 */

vec3 col_patches[24] = vec3[](
    vec3(0.4325, 0.3788, 0.1034),
    vec3(0.4191, 0.3748, 0.3525),
    vec3(0.2761, 0.3004, 0.1847),
    vec3(0.3700, 0.4501, 0.1335),
    vec3(0.3020, 0.2877, 0.2324),
    vec3(0.2856, 0.3910, 0.4174),
    vec3(0.5291, 0.4075, 0.3117),
    vec3(0.2339, 0.2155, 0.1140),
    vec3(0.5008, 0.3293, 0.1979),
    vec3(0.3326, 0.2556, 0.0644),
    vec3(0.3989, 0.4998, 0.4435),
    vec3(0.4962, 0.4428, 0.4358),
    vec3(0.2040, 0.1696, 0.0579),
    vec3(0.3270, 0.5033, 0.2307),
    vec3(0.5709, 0.3298, 0.1268),
    vec3(0.4694, 0.4732, 0.6081),
    vec3(0.4177, 0.2704, 0.2007),
    vec3(0.2151, 0.3037, 0.1903),
    vec3(0.3488, 0.3628, 0.9129),
    vec3(0.3451, 0.3596, 0.5885),
    vec3(0.3446, 0.3590, 0.3595),
    vec3(0.3438, 0.3589, 0.1912),
    vec3(0.3423, 0.3576, 0.0893),
    vec3(0.3439, 0.3565, 0.0320)
);


vec3 xyY_to_XYZ(vec3 xyY){
    
    float x = xyY.x;
    float y = xyY.y;
    float Y = xyY.z;
    
    // xyY to XYZ
    float _X = x * Y / y;
    
    float _Y = Y;
    
    float _Z = (1.0 - x - y ) * Y / y;
    
    // XYZ to bt709
    float _X_ =  ( 3.24096894    * _X)   -   ( 1.53738272     * _Y)   -   ( 0.49861062      * _Z);
    float _Y_ =  (-0.96924359    * _X)   +   ( 1.87596726     * _Y)   +   ( 0.04155508      * _Z);
    float _Z_ =  ( 0.05563008    * _X)   -   ( 0.20397694     * _Y)   +   ( 1.05697143      * _Z);

    vec3 XYZ = vec3(_X_, _Y_, _Z_);
    
    return pow(XYZ, vec3(1/2.2));
}


// rectangle constructor
vec3 rec(vec2 recPos, vec2 recSize, vec3 col){
    
    float U = gl_FragCoord.x;
    float V = gl_FragCoord.y;
    
    
    float posX = recPos.x;
    float posY = recPos.y;
    
    float recW = recSize.x;
    float recH = recSize.y;
    
    
    if (U > posX && U < (posX+recW)
        &&
        V > posY && V < (posY+recH)){
        
        col = vec3(col.x, col.y, col.z);
        
    }else{
        
        col = vec3(0.0,0.0,0.0);
        
    }
    
    return clamp(col, vec3(0.0), vec3(1.0));
    
}




void main(){

    float alpha = 1.0;

    float HOR_SLICES = 6.0f;
    float VER_SLICES = 4.0f;


    vec2 siz = vec2( (resolution.x/HOR_SLICES), (resolution.y/VER_SLICES));
    
    vec3 col_patch = vec3(0.0);
    
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 6; ++x) {
            col_patch += rec(vec2(siz.x * float(x), siz.y * float(y)), siz, xyY_to_XYZ(col_patches[y * 6 + x]));
        }
    }
    outputColor = vec4(col_patch, alpha);
}
