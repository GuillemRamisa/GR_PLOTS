#version 150

out vec4 outputColor;

uniform vec2 resolution;

// color poatches from colour-science --> DATA_TE226_V2_CIE_XYZ
/*
 Reference *ColorChecker SG* data from *X-Rite (2016)* and matching the
 ColorChecker SG* edition after November 2014.
 */

vec3 col_patches[45] = vec3[](
    vec3(0.1278, 0.1074, 0.0726), // dark_skin
    vec3(0.4945, 0.4484, 0.3586), // light_skin
    vec3(0.1459, 0.1690, 0.2925), // blue_sky
    vec3(0.0714, 0.1243, 0.0254), // foliage
    vec3(0.4470, 0.4039, 0.7304), // blue_flower
    vec3(0.3921, 0.5420, 0.6113), // bluish_green
    vec3(0.4574, 0.3628, 0.0624), // orange
    vec3(0.2979, 0.3180, 0.8481), // purplish_blue
    vec3(0.3884, 0.2794, 0.1886), // moderate_red
    vec3(0.1324, 0.0796, 0.3824), // purple
    vec3(0.3399, 0.5786, 0.1360), // yellow_green
    vec3(0.5417, 0.4677, 0.0644), // orange_yellow
    vec3(0.0859, 0.0361, 0.4728), // blue
    vec3(0.1000, 0.2297, 0.0530), // green
    vec3(0.3594, 0.1796, 0.0197), // red
    vec3(0.5236, 0.5972, 0.0368), // yellow
    vec3(0.4253, 0.2050, 0.5369), // magenta
    vec3(0.4942, 0.6119, 1.0304), // cyan
    vec3(0.2646, 0.2542, 0.1631), // patch_19
    vec3(0.7921, 0.7560, 0.5988), // patch_20
    vec3(0.4409, 0.4004, 0.3366), // patch_21
    vec3(0.1546, 0.3395, 0.1016), // patch_22
    vec3(0.3182, 0.3950, 0.5857), // patch_23
    vec3(0.5920, 0.5751, 0.9892), // patch_24
    vec3(0.4287, 0.2583, 0.0444), // patch_25
    vec3(0.4282, 0.5757, 0.4770), // patch_26
    vec3(0.1697, 0.1294, 0.7026), // patch_27
    vec3(0.2143, 0.1564, 0.1908), // patch_28
    vec3(0.1659, 0.3876, 0.3945), // patch_29
    vec3(0.1869, 0.1093, 0.7069), // patch_30
    vec3(0.3316, 0.1596, 0.1714), // patch_31
    vec3(0.8298, 0.8910, 0.5199), // patch_32
    vec3(0.1412, 0.1758, 0.4643), // patch_33
    vec3(0.0153, 0.0668, 0.0694), // patch_34
    vec3(0.6053, 0.5088, 0.1593), // patch_35
    vec3(0.4217, 0.4459, 0.3173), // patch_36
    vec3(0.9505, 1.0000, 1.0888), // white
    vec3(0.8331, 0.8801, 0.9576), // neutral_87
    vec3(0.6050, 0.6401, 0.6958), // neutral_63
    vec3(0.4119, 0.4358, 0.4724), // neutral_44
    vec3(0.2638, 0.2798, 0.3018), // neutral_28
    vec3(0.1405, 0.1489, 0.1598), // neutral_15
    vec3(0.0628, 0.0665, 0.0701), // neutral_7
    vec3(0.0190, 0.0202, 0.0202), // neutral_2
    vec3(0.0000, 0.0001, 0.0000)  // neutral_01
);


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

    float HOR_SLICES = 9.0f;
    float VER_SLICES = 5.0f;


    vec2 siz = vec2( (resolution.x/HOR_SLICES), (resolution.y/VER_SLICES));
    
    vec3 col_patch;
    
    col_patch = rec(vec2(siz.x * 0,siz.y * 0), siz, col_patches[0]);
    
    col_patch += rec(vec2(siz.x * 1,siz.y *  0), siz, col_patches[1]);
    col_patch += rec(vec2(siz.x * 2,siz.y *  0), siz, col_patches[2]);
    col_patch += rec(vec2(siz.x * 3,siz.y *  0), siz, col_patches[3]);
    col_patch += rec(vec2(siz.x * 4,siz.y *  0), siz, col_patches[4]);
    col_patch += rec(vec2(siz.x * 5,siz.y *  0), siz, col_patches[5]);
    col_patch += rec(vec2(siz.x * 6,siz.y *  0), siz, col_patches[6]);
    col_patch += rec(vec2(siz.x * 7,siz.y *  0), siz, col_patches[7]);
    col_patch += rec(vec2(siz.x * 8,siz.y *  0), siz, col_patches[8]);
    
    col_patch += rec(vec2(siz.x * 0,siz.y *  1), siz, col_patches[9]);
    col_patch += rec(vec2(siz.x * 1,siz.y *  1), siz, col_patches[10]);
    col_patch += rec(vec2(siz.x * 2,siz.y *  1), siz, col_patches[11]);
    col_patch += rec(vec2(siz.x * 3,siz.y *  1), siz, col_patches[12]);
    col_patch += rec(vec2(siz.x * 4,siz.y *  1), siz, col_patches[13]);
    col_patch += rec(vec2(siz.x * 5,siz.y *  1), siz, col_patches[14]);
    col_patch += rec(vec2(siz.x * 6,siz.y *  1), siz, col_patches[15]);
    col_patch += rec(vec2(siz.x * 7,siz.y *  1), siz, col_patches[16]);
    col_patch += rec(vec2(siz.x * 8,siz.y *  1), siz, col_patches[17]);

    col_patch += rec(vec2(siz.x * 0,siz.y *  2), siz, col_patches[18]);
    col_patch += rec(vec2(siz.x * 1,siz.y *  2), siz, col_patches[19]);
    col_patch += rec(vec2(siz.x * 2,siz.y *  2), siz, col_patches[20]);
    col_patch += rec(vec2(siz.x * 3,siz.y *  2), siz, col_patches[21]);
    col_patch += rec(vec2(siz.x * 4,siz.y *  2), siz, col_patches[22]);
    col_patch += rec(vec2(siz.x * 5,siz.y *  2), siz, col_patches[23]);
    col_patch += rec(vec2(siz.x * 6,siz.y *  2), siz, col_patches[24]);
    col_patch += rec(vec2(siz.x * 7,siz.y *  2), siz, col_patches[25]);
    col_patch += rec(vec2(siz.x * 8,siz.y *  2), siz, col_patches[26]);
    
    col_patch += rec(vec2(siz.x * 0,siz.y *  3), siz, col_patches[27]);
    col_patch += rec(vec2(siz.x * 1,siz.y *  3), siz, col_patches[28]);
    col_patch += rec(vec2(siz.x * 2,siz.y *  3), siz, col_patches[29]);
    col_patch += rec(vec2(siz.x * 3,siz.y *  3), siz, col_patches[30]);
    col_patch += rec(vec2(siz.x * 4,siz.y *  3), siz, col_patches[31]);
    col_patch += rec(vec2(siz.x * 5,siz.y *  3), siz, col_patches[32]);
    col_patch += rec(vec2(siz.x * 6,siz.y *  3), siz, col_patches[33]);
    col_patch += rec(vec2(siz.x * 7,siz.y *  3), siz, col_patches[34]);
    col_patch += rec(vec2(siz.x * 8,siz.y *  3), siz, col_patches[35]);

    col_patch += rec(vec2(siz.x * 0,siz.y *  4), siz, col_patches[36]);
    col_patch += rec(vec2(siz.x * 1,siz.y *  4), siz, col_patches[37]);
    col_patch += rec(vec2(siz.x * 2,siz.y *  4), siz, col_patches[38]);
    col_patch += rec(vec2(siz.x * 3,siz.y *  4), siz, col_patches[39]);
    col_patch += rec(vec2(siz.x * 4,siz.y *  4), siz, col_patches[40]);
    col_patch += rec(vec2(siz.x * 5,siz.y *  4), siz, col_patches[41]);
    col_patch += rec(vec2(siz.x * 6,siz.y *  4), siz, col_patches[42]);
    col_patch += rec(vec2(siz.x * 7,siz.y *  4), siz, col_patches[43]);
    col_patch += rec(vec2(siz.x * 8,siz.y *  4), siz, col_patches[44]);
    
    outputColor = vec4(col_patch, alpha);
}
