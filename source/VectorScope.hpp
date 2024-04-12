//
//  VectorScope.hpp
//  GR_PLOTS_100
//
//  Created by Guillem Ramisa de Soto on 15/2/24.
//

#pragma once

#ifndef VectorScope_hpp
#define VectorScope_hpp

#include <stdio.h>
#include <ofMain.h>

#include "ofxImGui.h"
// this internal is imported to get more control over imgui objects like style font and disable
#include "imgui_internal.h"

//#include "ofxOpenCv.h" // to implement in the future now its too slow

#include <Generate.hpp>
#include <ColorUtils.hpp>

// for pixel averaging and numerical utils
#include <algorithm>
#include <numeric>


class VectorScope
{

public:
    
    // constructor
    VectorScope();

    // destructor
    ~VectorScope();
    
    // public functions and members
    void setup(ofImage& __image);
    void update(ofImage __image);
    void gui(const char* name);
    void resetControls();

    void pre_draw();
    
    void draw();
    void draw3D();

    void draw_crop();
    
    void draw_hud();
    void draw_hud3D();
    void draw_targets();
    void draw_targets3D();

    bool is_insideSource();
    void draw_source(bool is_loaded_image);

    void draw_stats();
    void draw_translate();
    void draw_translate_reset();
    void draw_magnifier();
//    void draw_cv(); // to be implemented in the future, now its too expensive and slow
    void draw_custom_target(ofVec3f& _xyz, float _radius);
    void draw_custom_target3D(ofVec3f& _xyz, float _radius);
    
    ofColor getMaxPixel();
    ofColor getMinPixel();
    

    // path
    const char*             _path;
    
    // chanel to display at draw
    int                     _channel;
    
    // Vectorscope Pixel population
    int                     _precision;
    float                   _alpha = 255.0f;
    float                   _pos[3];
    float                   _scale;
    float                   _rotate;
    float                   _rotate3D[3];
    int                     _erode;
    float                   _gain;
    int                     _padding;
    
    // keys and buttons functionalitis for interaction
    bool                    help_key            = false;
    bool                    button_tint_pixels  = true;
    bool                    view_mode3D         = false;
    bool                    button_bg           = false;
    bool                    button_hud          = true;
    bool                    button_targets      = true;
    bool                    button_tint_targets = true;
    bool                    tint_target_minMax = true;
    bool                    button_source       = false;
    bool                    button_minmax       = false;
    bool                    button_stats        = true;
    bool                    m_button            = false;
    bool                    cstm_from_sourceMag = false;
    bool                    tint_target         = true;
//    bool                    cv_button           = false;
    bool                    translate_button    = false;
    bool                    is_loaded_image     = false;
    bool                    MinMax_pixel_button = false;
    bool                    button_graticule    = true;
    bool                    button_IQ           = true;
    bool                    button_box3dD       = true;
    
private:
    
    // const
    int                     s_W;
    int                     s_H;
    
    // base dynamics on update function
    int                     w_W;
    int                     w_H;
    float                   minWin;
    float                   maxWin;
    int                     w_center[2];
    float                   maxMax;
    int                     MX;
    int                     MY;
    float                   s_aspect;
    int                     w_Offset;
    
    int                     d_W = 900;
    int                     d_H = 900;
    int                     d_center[2] = { d_W/2 , d_H/2 };
    
    int                     _maxRange = 255;
    
    float                   custom_pixel_color[3];
    float                   custom_graticule_color[3];
    float                   custom_target_color[3];

    float                   _source_scale = 1;
    
    float                   _h_col;
    float                   _h_col_IQ;
    float                   _h_col_box3d;
    
    float                   _h_width;
    
    
    // tempo
    int                     ___rotM;
    int                     ___padM;
    
    float                   pos_base[2] = {0.0f, 0.0f};
    
    ImVec2                  lastMousePos;
    ImVec2                  cumulativeOffset = ImVec2(0.0f, 0.0f);
    float                   MouseScale = 1.0f;
    
    ImVec2                  currentMousePos;
    float                   currentMouseScale = 1.0f;
    
    int                     h_rez = 64;
    int                     AAAAAA= 20;
    
    bool                     _matrix = true;
    int                      matrix = 0;
    
    
    // controls utils
    float                   c_pos[3];
    float                   c_scale[2] = {1.0f,1.0f};
    float                   c_rotate;
    
    int                     matrix_item_current = 1;
    
    int                     Col_threshold[2];
    
    // image utils
    ofImage                 s_image;
    ofImage                 d_image;
    ofPixels                s_pixels;
    ofImage                 o_image;

    // texture where the pixels computed will be alocated and displayed
    ofTexture               d_texture;

    // color utils
    ofColor                 s_col;
    ofColor                 d_col;
    
    // image threshold
    ofColor                 s_col_;
    ofColor                 d_col_;
    
    ofColor                 BG_opaque       = ofColor(0,0,0,255);
    ofColor                 BG_transparent  = ofColor(0,0,0,0);
    
    //crop rec...
    ofRectangle             CropRec;
    int                     CropRec_V[4];
    int                     CropRecLFT;
    int                     CropRecRGT;
    int                     CropRecTOP;
    int                     CropRecBOT;
    int                     d_image_drawSubsection_x;
    int                     d_image_drawSubsection_y;
    int                     d_image_drawSubsection_w;
    int                     d_image_drawSubsection_h;
    int                     d_image_drawSubsection_sx;
    int                     d_image_drawSubsection_sy;
    int                     d_image_drawSubsection_sw;
    int                     d_image_drawSubsection_sh;
    
    // Min Max pixel value for data print
    ofPixels                p_MinMax_R;
    float                   p_maxV_R;
    float                   p_minV_R;

    ofPixels                p_MinMax_G;
    float                   p_maxV_G;
    float                   p_minV_G;

    ofPixels                p_MinMax_B;
    float                   p_maxV_B;
    float                   p_minV_B;
    
    ofPixels                p_MinMax_V;
    float                   p_maxV_V;
    float                   p_minV_V;

    int                     maxIntensity;
    int                     minIntensity;
    
    ofColor                 maxIntensityColor;
    ofColor                 minIntensityColor;
    
    ofColor                 pixelColor_min;
    ofColor                 pixelColor_max;

// -----------------------------------------------------------------------------------------------------------------------------
//    // for future impementation
//    ofVideoPlayer           s_video;
// -----------------------------------------------------------------------------------------------------------------------------
    
    // magnifier
    ofImage                 m_image;
    ofColor                 m_col;
    ofTexture               m_texture;
    
    ofPath                  m_p;
    ofRectangle             m_r;
    ofPath                  m_p_data;
    ofRectangle             m_r_data;
    int                     m_factor;
    ofPath                  LINE_CUSTOM_Dist;
    ofPath                  m_cross_pixel;
    ofRectangle             m_cross_pixel_rect;
    int                     m_col_r;
    int                     m_col_g;
    int                     m_col_b;
    int                     magnifierEdgeWindow_offsetX; // for dynamoc positioning X
    int                     magnifierEdgeWindow_offsetY; // for dynamoc positioning Y
    
    
// -----------------------------------------------------------------------------------------------------------------------------
//    // for future implementation, now its too slow
//    // cv
//    ofImage                 cv_image;           // source image that grabs the pixels
//    ofPixels                cv_pixels;          // source pixels that gets the data from the texture() from the source image
//    ofxCvColorImage         ofxCV_image_OUT;    // ofxCV image
// -----------------------------------------------------------------------------------------------------------------------------
    
    //custom color image
    ofImage                 cCol_image;
    
    ofVec3f                 get_custom();
    ofVec3f                 get_custom3D();
    
    
    // hud custom paths
    ofPath                  h_VL;
    ofPath                  h_HL;
    ofPath                  h_DL; // only 3D line
    
    ofPath                  h_CI;
    
    ofPath                  h_LINES1;
    ofPolyline              h_LINES_1;
    ofPath                  h_LINES2;
    ofPath                  h_LINES_IQ;
    ofPath                  h_LINES_CU;
    float                   h_LINES_CU_v    = 33.0f;
    float                   h_LINES_CU_vz   = 0.0f;
    float                   h_LINES_CU_f    = 1.0f;
    bool                    h_LINES_CU_b    = false;
    ofVec3f                 target_V;
    ofVec2f                 target_V_2d;
    float                   target_size = 30;
    
    
    // custom color for custom target
    float                   c_target_col[3];
    bool                    c_target_col_b         = false;
    float                   c_target_col_rad       = 25;

    float                   c_col_radMinMax = 25; //  min max

    // target factor
    float                    target_Factor = 75.0f;
    float                    target_gain   = 100.0f;
    
    // path template for rect constructor
    ofPath                  h_Rec;
    
    // rect objects
    ofRectangle             RED_rec;
    ofRectangle             GREEN_rec;
    ofRectangle             BLUE_rec;
    ofRectangle             CYAN_rec;
    ofRectangle             MAGENTA_rec;
    ofRectangle             YELLOW_rec;
    
    
};

#endif /* VectorScope_hpp */
