//
//  VectorScope.cpp
//  GR_PLOTS_100
//
//  Created by Guillem Ramisa de Soto on 15/2/24.
//

#include "VectorScope.hpp"

VectorScope::VectorScope()
{
    resetControls();
    cout << "vectorscope COSNTRUCTED" << endl;
}

VectorScope::~VectorScope()
{
    cout << "vectorscope DESTRUCTED!" << endl;
}

void VectorScope::setup(ofImage& __image)
{
    ofClear(0.0f);
    
    // standin resolution for initial data population
    s_image = __image;
    d_image = __image;
    
    // source image size init
    s_W = s_image.getWidth();
    s_H = s_image.getHeight();
    
    /* need to call the crop values here to populate the initial solution at the setup stage
     otherwise it will load after the crop init so a 0 will be set for all values in the int[4]
    */
    CropRec_V[0] = 0;
    CropRec_V[1] = 0;
    CropRec_V[2] = s_W;
    CropRec_V[3] = s_W;
    
    // base dynamic
    // window dimentions need to be called here first manually so it can allocate dimentions
    // if not since setup is only called once it will allocate 0
    int w_W = ofGetWindowWidth();
    int w_H = ofGetWindowHeight();
    
    // magnifier objects allocation
    m_image.allocate(1, 1, OF_IMAGE_COLOR_ALPHA);
    m_texture.allocate(w_W, w_H, GL_RGBA, true);
    m_texture.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    
    
// -----------------------------------------------------------------------------------------------------------------------------
//    // to be implemented in the future, now its too expensive and slow memory wise
//    // cv allocations
//    cv_image.allocate(w_W, w_H, OF_IMAGE_COLOR_ALPHA);
//    cv_pixels.allocate(w_W, w_W, OF_PIXELS_RGB);
//    ofxCV_image_OUT.allocate(w_W,w_H);
// -----------------------------------------------------------------------------------------------------------------------------
    
    // ImGui dockable setup
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    // image sampling precision initial setup
    _precision = 3;
    
    // minMax data display pixel allocation
    p_MinMax_R.allocate(w_W, w_H, OF_PIXELS_GRAY);
    p_MinMax_G.allocate(w_W, w_H, OF_PIXELS_GRAY);
    p_MinMax_B.allocate(w_W, w_H, OF_PIXELS_GRAY);
    p_MinMax_V.allocate(w_W, w_H, OF_PIXELS_RGB);
    
}

void VectorScope::update(ofImage __image)
{
    // standin for resize aspect ratio
    o_image = __image;
    
    // resize image to unit size for sampling
    __image.getPixels().resize(s_W, s_H ,OF_INTERPOLATE_NEAREST_NEIGHBOR);
    
    // pass pixel and color data
    s_image = __image;  // src image
    d_image = __image;  // dst image
    
    // base dynamics calculations image
    s_W = s_image.getWidth();
    s_H = s_image.getHeight();
    
    // base dynamics calculations window
    w_W = ofGetWindowWidth();
    w_H = ofGetWindowHeight();
    
    minWin = min(w_W, w_H);
    maxWin = max(w_W, w_H);
    
    w_center[0] = w_W/2;
    w_center[1] = w_H/2;
    
    maxMax = max((maxWin - minWin)/2, (minWin - maxWin)/2); // max W or H win size
    
    w_Offset = w_W > w_H ? (w_H/2) : (w_W/2);
    
    // get mouse screen cordinates
    MX  = ofGetMouseX();
    MY  = ofGetMouseY();
    
    // -----------------------------------------------------------------------------------------
    //  An issue with the sampling allocatioon of the pixel data makes the .size() not dynamic
    //  the solution is too expensive, needs to be revisited in the future
    //  for now this values are constant based on the setup init values so the minmax and
    //  custom target draws are not NULL...
    
    if(button_minmax || MinMax_pixel_button){
        CropRec_V[0]    = 0;
        CropRec_V[1]    = 0;
        CropRec_V[2]    = s_W;
        CropRec_V[3]    = s_H;
        
        _precision      = 3;
        
    }else{
        // image crop values update/ populate
        draw_crop();
    }
    // -----------------------------------------------------------------------------------------
    
    
    if(button_stats == false){
        button_minmax = false;
    }
    
    // tint pixel custom color threshold
    for(int c = 0; c < 3 ; c++){
        if(custom_pixel_color[c] < 0){
            custom_pixel_color[c] = 0;
        }else if (custom_pixel_color[c] > 255){
            custom_pixel_color[c] = 255;
        }else{
            custom_pixel_color[c] = custom_pixel_color[c];
        }
    }
    
}

void VectorScope::draw_crop()
{
    // limit bounds of rec to avoid extra allocation
    // X // left = 0
    if (CropRec_V[0] > s_W){
        CropRec_V[0] = s_W;
    }else if(CropRec_V[0] < 0){
        CropRec_V[0] = 0;
    }else{
        CropRec_V[0];
    }
    // W // right = s_W
    if (CropRec_V[2] < 0){
        CropRec_V[2] = 0;
    }else if(CropRec_V[2] > s_W){
        CropRec_V[2] = s_W;
    }else{
        CropRec_V[2];
    }

    // Y // top = 0
    if (CropRec_V[1] > 0) {
        CropRec_V[1] = 0;
    }else if(CropRec_V[1] < -s_H){
        CropRec_V[1] = -s_H;
    }else{
        CropRec_V[1];
    }
    // H // bottom = s_H
    if (CropRec_V[3] > s_H) {
        CropRec_V[3] = s_H;
    }else if (CropRec_V[3] < 0){
        CropRec_V[3] = 0;
    }else{
        CropRec_V[3];
    }

    // rectangle creation to get values to use as crop
    CropRec.set(CropRec_V[0],CropRec_V[1],CropRec_V[2],CropRec_V[3]);
    
    // 4 walls of rectangle
    CropRecLFT = CropRec.getLeft();
    CropRecTOP = -CropRec.getTop();

    CropRecRGT = CropRec.getRight();
    CropRecBOT = CropRec.getBottom();
    
}

ofVec3f VectorScope::get_custom()
{
    ofVec3f _xyz;
    
    if(!cstm_from_sourceMag)
    {
        _xyz = ofVec3f(c_target_col[0]*255, c_target_col[1]*255, c_target_col[2]*255);
        
        if(tint_target)
        {
            ofSetColor(ofColor(c_target_col[0]*255, c_target_col[1]*255, c_target_col[2]*255, 255));
        }
        else
        {
            ofSetColor(ofColor(255));
        }
        
    }
    else if (is_insideSource() && button_source)
    {
        _xyz = ofVec3f(m_col_r, m_col_g, m_col_b);
        
        if(tint_target)
        {
            ofSetColor(ofColor(m_col_r, m_col_g, m_col_b, 255));
        }
        else
        {
            ofSetColor(ofColor(c_target_col[0]*255, c_target_col[1]*255, c_target_col[2]*255, 255));
        }
        
    }
    else if (!m_button)
    {
        
        cstm_from_sourceMag = false;
        _xyz = ofVec3f(c_target_col[0]*255, c_target_col[1]*255, c_target_col[2]*255);
        ofSetColor(ofColor(c_target_col[0]*255, c_target_col[1]*255, c_target_col[2]*255, 255));
    }
    else
    {
        
        _xyz = ofVec3f(c_target_col[0]*255, c_target_col[1]*255, c_target_col[2]*255);
        
        if(tint_target)
        {
            ofSetColor(ofColor(c_target_col[0]*255, c_target_col[1]*255, c_target_col[2]*255, 255));
        }
        else
        {
            ofSetColor(ofColor(255));
        }
    }
    return _xyz;
}

ofVec3f VectorScope::get_custom3D()
{
    ofVec3f _xyz;
    _xyz = ofVec3f(c_target_col[0]*255, c_target_col[1]*255, c_target_col[2]*255);
    
    if(tint_target)
    {
        ofSetColor(ofColor(c_target_col[0]*255, c_target_col[1]*255, c_target_col[2]*255, 255));
    }
    else
    {
        ofSetColor(ofColor(255));
    }
    return _xyz;
}

void VectorScope::draw_custom_target(ofVec3f& _xyz, float _radius)
{
    // color matrix
    if (matrix_item_current == 1) {
        _xyz = COLOR_MATRIX_PRIMS(_xyz, RGB_YCbCr_709);
        ___padM        = 0;
        ___rotM        = 0;

    }else if (matrix_item_current == 0){
        _xyz = COLOR_MATRIX_PRIMS(_xyz, RGB_YCbCr_601);
        ___padM        = 0;
        ___rotM        = 0;
        
    }else if (matrix_item_current == 2){
        _xyz = COLOR_MATRIX_PRIMS(_xyz, RGB_YCbCr_2020);
        ___padM        = 0;
        ___rotM        = 0;
    }
    
    // padding relative to screenspace
    int padd = ((minWin/2) * (_padding+___padM)) /100;

    // init 3 channel
    // map source values to screenspace
    float _x_ = ofMap(  _xyz.x, 0, _maxRange,   +(padd),    (minWin) -(padd)); // Y
    float _y_ = ofMap(  _xyz.y, 0, _maxRange,   +(padd),    (minWin) -(padd)); // Cb
    float _z_ = ofMap(  _xyz.z, 0, _maxRange,   +(padd),    (minWin) -(padd)); // Cr

    // Pre rotation and center allocation
    ofVec3f pr_xyz  = ROT_3D( ofVec3f((w_Offset) - _x_, (w_Offset) - _y_, (w_Offset) - _z_ ), ofVec3f(180.0f, 0, 0) );
    
    // 3D rotation matrix
    ofVec3f r_xyz  = ROT_3D( ofVec3f(pr_xyz.x, pr_xyz.y, pr_xyz.z ), ofVec3f(-_rotate3D[0], _rotate3D[1], _rotate3D[2] ) );

//            float _x = r_xyz.x+(w_H/2);
    float _y = r_xyz.y+(w_Offset);
    float _z = r_xyz.z+(w_Offset);
    
    // positioning in screenspace based on max size W/H
    _y = w_H > w_W ? _y + maxMax : _y;
    _z = w_H < w_W ? _z + maxMax : _z;
    
    // 2D PSR YCbCr AXIS compensation
    ofVec2f _x_y_ROT = PSR_2D(ofVec2f((w_W/2)-_z,(w_H/2)-_y), w_W/2, w_H/2, 1, -1, -90.0f);
    _z = _x_y_ROT.x;
    _y = _x_y_ROT.y;
    
    // set color from user
    ofNoFill();
    
    ofTranslate(-ofGetWindowWidth()/2, -ofGetWindowHeight()/2);
    
    // coords from transform
    ofDrawCircle(_z,_y, _radius);
    
    ofFill();
    ofSetColor(255);
    ofTranslate(ofGetWindowWidth()/2, ofGetWindowHeight()/2);
}

void VectorScope::draw_custom_target3D(ofVec3f& _xyz, float _radius)
{
    // color matrix
    if (matrix_item_current == 1) {
        _xyz = COLOR_MATRIX_PRIMS(_xyz, RGB_YCbCr_709);
        ___padM        = 0;
        ___rotM        = 0;

    }else if (matrix_item_current == 0){
        _xyz = COLOR_MATRIX_PRIMS(_xyz, RGB_YCbCr_601);
        ___padM        = 0;
        ___rotM        = 0;

    }else if (matrix_item_current ==2){
        _xyz = COLOR_MATRIX_PRIMS(_xyz, RGB_YCbCr_2020);
        ___padM        = 0;
        ___rotM        = 0;
    }

    // padding relative to screenspace
    int padd = ((minWin/2) * (_padding+___padM)) /100;

    // init 3 channel
    // map source values to screenspace
    float _x_ = ofMap(  _xyz.x, 0, _maxRange,   +(padd),    (minWin) -(padd)); // Y
    float _y_ = ofMap(  _xyz.y, 0, _maxRange,   +(padd),    (minWin) -(padd)); // Cb
    float _z_ = ofMap(  _xyz.z, 0, _maxRange,   +(padd),    (minWin) -(padd)); // Cr

    // Pre rotation and center allocation
    ofVec3f pr_xyz  = ROT_3D( ofVec3f((w_Offset) - _x_, (w_Offset) - _y_, (w_Offset) - _z_ ), ofVec3f(90.0f, 180.0f, 0 ) );
    
    // 3D rotation matrix
    ofVec3f r_xyz  = ROT_3D( ofVec3f(pr_xyz.x, pr_xyz.y, pr_xyz.z ), ofVec3f(_rotate3D[0], _rotate3D[2], _rotate3D[1] ) );

    float _x = r_xyz.x+(w_Offset);
    float _y = r_xyz.y+(w_Offset);
    float _z = r_xyz.z+(w_Offset);

    // positioning in screenspace based on max size W/H
    _x = w_H > w_W ? (_x-w_H/2) + maxMax : (_x-w_H/2);
    _y = w_H > w_W ? (_y-w_H/2) + maxMax : (_y-w_H/2);
    _z = w_H < w_W ? (_z-w_W/2) + maxMax : (_z-w_W/2);

    ofNoFill();
    
    ofSetSphereResolution(3);
    
    ofDrawSphere(_z, _y, _x, _radius);
    
    ofFill();
    ofSetColor(255);
}

void VectorScope::draw()
{
    ofClear(0.0f);
    ofSetColor(255.0f);
    ofSetDepthTest(false);
    
    // this allocations need to be in the draw loop NOT in the setup, so the allocations is dynamic
    // and not set once...
    
    // allocation of pixels in the buffer and channel selector
    s_pixels.allocate(w_W, w_H, OF_PIXELS_RGBA);
    
    // dst texture data allocation
    d_texture.allocate(w_W, w_H, GL_RGBA);
    
    // setters of initial color values for the bg and check box trigger
    if (!button_bg) {
        s_pixels.setColor(BG_opaque);
        ofBackground(0);
    }else{
        s_pixels.setColor(BG_transparent);
        ofBackground(50);
    }

    // Vectorscope Pixel population
    // classic image double loop

    for (int i = CropRec_V[0]; i < CropRec_V[2]; i+= _precision)
    {
        for (int j = -CropRec_V[1]; j < CropRec_V[3]; j+= _precision)
        {
            // get color data from pixel index
            s_col = s_image.getColor(i, j);
            d_col = s_image.getColor(i, j);
            
            // image threshold
            s_col_ = XYZ_threshold(s_col, Col_threshold, _alpha);
            d_col_ = XYZ_threshold(d_col, Col_threshold, _alpha);

            // vec3 contruct for matrix conversion
            ofVec3f _xyz = ofVec3f(s_col_.r, s_col_.g, s_col_.b);
            
            // s_col = _xyz
            // color matrix
            if (matrix_item_current == 1) {
                _xyz = COLOR_MATRIX_PRIMS(_xyz, RGB_YCbCr_709);
                ___padM        = 0;
                ___rotM        = 0;

            }else if (matrix_item_current == 0){
                _xyz = COLOR_MATRIX_PRIMS(_xyz, RGB_YCbCr_601);
                ___padM        = 0;
                ___rotM        = 0;
                
            }else if (matrix_item_current ==2){
                _xyz = COLOR_MATRIX_PRIMS(_xyz, RGB_YCbCr_2020);
                ___padM        = 0;
                ___rotM        = 0;
            }
            
            // padding relative to screenspace
            int padd = ((minWin/2) * (_padding+___padM)) /100;

            // init 3 channel
            // map source values to screenspace
            float _x_ = ofMap(  _xyz.x, 0, _maxRange,   +(padd),    (minWin) -(padd)); // Y
            float _y_ = ofMap(  _xyz.y, 0, _maxRange,   +(padd),    (minWin) -(padd)); // Cb
            float _z_ = ofMap(  _xyz.z, 0, _maxRange,   +(padd),    (minWin) -(padd)); // Cr
            
            // Pre rotation and center allocation
            ofVec3f pr_xyz  = ROT_3D( ofVec3f((w_Offset) - _x_, (w_Offset) - _y_, (w_Offset) - _z_ ), ofVec3f(180.0f, 0, 0) );
            
            // 3D rotation matrix
            ofVec3f r_xyz  = ROT_3D( ofVec3f(pr_xyz.x, pr_xyz.y, pr_xyz.z ), ofVec3f(-_rotate3D[0], _rotate3D[1], _rotate3D[2] ) );
            
//            float _x = r_xyz.x+(w_H/2);
            float _y = r_xyz.y+(w_Offset);
            float _z = r_xyz.z+(w_Offset);
            
            // positioning in screenspace based on max size W/H
            _y = w_H > w_W ? _y + maxMax : _y;
            _z = w_H < w_W ? _z + maxMax : _z;
            
            // 2D PSR YCbCr AXIS compensation
            ofVec2f _x_y_ROT = PSR_2D(ofVec2f((w_W/2)-_z,(w_H/2)-_y), w_W/2, w_H/2, 1, -1, -90.0f);
            _z = _x_y_ROT.x;
            _y = _x_y_ROT.y;
            
            if(button_minmax)
            {
                // populate minmax pixel objects R/G/B
                p_MinMax_R[i * w_W + j] = d_col_.r;
                p_MinMax_G[i * w_W + j] = d_col_.g;
                p_MinMax_B[i * w_W + j] = d_col_.b;
            }
            
            if(MinMax_pixel_button)
            {
                // Min/Max pixel
                p_MinMax_V.setColor(i,j,d_col_);
            }
            
            // pixel coordinates positioning
            // classic double foor loop with erode in a 2*2 with checks if out of bounds to avoid edge tiling
            // no other way found, need to revisit this in the future for better performance...
            
            if(!button_tint_pixels){
                d_col_ = ofColor(custom_pixel_color[0] * 255,
                                 custom_pixel_color[1] * 255,
                                 custom_pixel_color[2] * 255,
                                 255*_alpha);
            }
            
            // population without erode
            if (_erode==0) {
                if (_z >= 0 && _z < w_W && _y >= 0 && _y < w_H)
                {
                    s_pixels.setColor( _z, _y, d_col_ * _gain);
                }
            }else if (_erode!=0){

                // pixel coordinates positioning and erode
                for(int p = 0; p<=_erode ;p++){
                    for(int c = 0; c<=_erode ;c++){
                        if ( (_z+p) >= 0 && (_z+p) < w_W && (_y-c) >= 0 && (_y-c) < w_H)
                        {
                            s_pixels.setColor( _z+p, _y-c, d_col_ * _gain);
                        }
                        
                        if ( (_z+p) >= 0 && (_z+p) < w_W && (_y+c) >= 0 && (_y+c) < w_H)
                        {
                            s_pixels.setColor( _z+p, _y+c, d_col_ * _gain);
                        }
                        
                        if ( (_z-p) >= 0 && (_z-p) < w_W && (_y+c) >= 0 && (_y+c) < w_H)
                        {
                            s_pixels.setColor( _z-p, _y+c, d_col_ * _gain);
                        }
                        
                        if ( (_z-p) >= 0 && (_z-p) < w_W && (_y-c) >= 0 && (_y-c) < w_H)
                        {
                            s_pixels.setColor( _z-p, _y-c, d_col_ * _gain);
                        }
                    }
                }
            }
        }
    }
    
    // dst texture with pixels
    ofDisableArbTex();
    
    // dst texture allocation of pixel data
    // Channel selctor // R / G / B // ALL
    if (_channel == 3) {
        d_texture.allocate(s_pixels);
    }else{
        d_texture.allocate(s_pixels.getChannel(_channel));
        ofBackground(0);
    }
    
    // dst texture MIP mapping for scaling
    d_texture.enableMipmap();
    
    // min-max Mipmap nearest for single pixel interperetation, no smoothing nor filtering
    d_texture.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST); // GL_NEAREST // GL_LINEAR

    // PSR with anchor point at center
    d_texture.setAnchorPoint(w_center[0], w_center[1]);

    // translation by value
    ofTranslate((w_W/2)+_pos[0], (w_H/2)+_pos[1]);
    ofScale(_scale);
    ofRotateDeg((int)_rotate);
    
    // vectorscope orientation
    ofScale(1, -1);

//    ofEnableBlendMode(OF_BLEND);
    
// --------------------------------------------------------------------------------------------
//    premult function ---> diabled for alpha control
//    glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
// --------------------------------------------------------------------------------------------
    
    // store and draw a texture allows for GPU storage and less computation needed in the CPU
    // however the performance boost is limited by the texture class methods
    d_texture.draw(0, 0, w_W, w_H);
    d_texture.clear();

    if(button_targets)
    {
        // custom target draw
        if(c_target_col_b){
            // custom color target draw 2D
            ofVec3f _xyz_ = get_custom();
            draw_custom_target(_xyz_, c_target_col_rad);
        }
        
        // custom draw min max
        if(MinMax_pixel_button){
            // min pixel
            ofColor minColor = getMinPixel();
            ofVec3f _xyz_MIN = ofVec3f((int)minColor.r,(int)minColor.g,(int)minColor.b);
            if(tint_target_minMax){
                ofSetColor(minColor);
            }
            draw_custom_target(_xyz_MIN, c_col_radMinMax);

            // max pixel
            ofColor maxColor = getMaxPixel();
            ofVec3f _xyz_MAX = ofVec3f((int)maxColor.r,(int)maxColor.g,(int)maxColor.b);
            if(tint_target_minMax){
                ofSetColor(maxColor);
            }
            draw_custom_target(_xyz_MAX, c_col_radMinMax);
        }
    }
    // reset scale
    ofScale(1, -1);
}

void VectorScope::draw3D()
{
    ofClear(0.0f);
    ofSetColor(255.0f);
//    ofSetDepthTest(true);

    // allocation of pixels for Min/Max
    s_pixels.allocate(w_W, w_H, OF_PIXELS_RGBA);
    
    // setters of initial color values for the bg and check box trigger
    if (!button_bg) {
        ofBackground(0);
    }else{
        ofBackground(50);
    }
    
    // Vectorscope Pixel population
    // classic image double loop

    for (int i = CropRec_V[0]; i < CropRec_V[2]; i+= _precision)
    {
        for (int j = -CropRec_V[1]; j < CropRec_V[3]; j+= _precision)
        {
            // get color data from pixel index
            s_col = s_image.getColor(i, j);
            d_col = s_image.getColor(i, j);

            // image threshold
            s_col_ = XYZ_threshold(s_col, Col_threshold, _alpha);
            d_col_ = XYZ_threshold(d_col, Col_threshold, _alpha);
            
            // vec3 contruct for matrix conversion
            ofVec3f _xyz = ofVec3f(s_col_.r, s_col_.g, s_col_.b);

            // s_col = _xyz
            // color matrix
            if (matrix_item_current == 1) {
                _xyz = COLOR_MATRIX_PRIMS(_xyz, RGB_YCbCr_709);
                ___padM        = 0;
                ___rotM        = 0;

            }else if (matrix_item_current == 0){
                _xyz = COLOR_MATRIX_PRIMS(_xyz, RGB_YCbCr_601);
                ___padM        = 0;
                ___rotM        = 0;

            }else if (matrix_item_current ==2){
                _xyz = COLOR_MATRIX_PRIMS(_xyz, RGB_YCbCr_2020);
                ___padM        = 0;
                ___rotM        = 0;
            }

            // padding relative to screenspace
            int padd = ((minWin/2) * (_padding+___padM)) /100;

            // init 3 channel
            // map source values to screenspace
            float _x_ = ofMap(  _xyz.x, 0, _maxRange,   +(padd),    (minWin) -(padd)); // Y
            float _y_ = ofMap(  _xyz.y, 0, _maxRange,   +(padd),    (minWin) -(padd)); // Cb
            float _z_ = ofMap(  _xyz.z, 0, _maxRange,   +(padd),    (minWin) -(padd)); // Cr

            // Pre rotation and center allocation
            ofVec3f pr_xyz  = ROT_3D( ofVec3f((w_Offset) - _x_, (w_Offset) - _y_, (w_Offset) - _z_ ), ofVec3f(90.0f, 180.0f, 0 ) );
            
            // 3D rotation matrix
            ofVec3f r_xyz  = ROT_3D( ofVec3f(pr_xyz.x, pr_xyz.y, pr_xyz.z ), ofVec3f(_rotate3D[0], _rotate3D[2], _rotate3D[1] ) );

            float _x = r_xyz.x+(w_Offset);
            float _y = r_xyz.y+(w_Offset);
            float _z = r_xyz.z+(w_Offset);

            // positioning in screenspace based on max size W/H
            _x = w_H > w_W ? (_x-w_H/2) + maxMax : (_x-w_H/2);
            _y = w_H > w_W ? (_y-w_H/2) + maxMax : (_y-w_H/2);
            _z = w_H < w_W ? (_z-w_W/2) + maxMax : (_z-w_W/2);
            
            if(button_minmax || MinMax_pixel_button)
            {
                // populate minmax pixel objects
                p_MinMax_R[i * w_W + j] = d_col_.r;
                p_MinMax_G[i * w_W + j] = d_col_.g;
                p_MinMax_B[i * w_W + j] = d_col_.b;
            }
            
            if(MinMax_pixel_button)
            {
                // Min/Max pixel
                p_MinMax_V.setColor(i,j,d_col_);
            }
            
            
            if(!button_tint_pixels){
                ofSetColor(ofColor(custom_pixel_color[0],
                                   custom_pixel_color[1],
                                   custom_pixel_color[2],
                                   255*_alpha));
            }else{
                ofSetColor(d_col_);
            }
            // scene population with color and object
            ofDrawBox(_z, _y, _x, 1+(_erode*2));
        }
    }

    if(button_targets)
    {
        // custom target draw
        if(c_target_col_b){
            // custom color target draw 3D
            ofVec3f _xyz_3D = get_custom3D();
            draw_custom_target3D(_xyz_3D, c_target_col_rad);
            
        }
        
        // custom draw min max
        if(MinMax_pixel_button){
            // min pixel
            ofColor minColor = getMinPixel();
            ofVec3f _xyz_MIN = ofVec3f((int)minColor.r,(int)minColor.g,(int)minColor.b);
            if(tint_target_minMax){
                ofSetColor(minColor);
            }
            draw_custom_target3D(_xyz_MIN, c_col_radMinMax);
            
            // max pixel
            ofColor maxColor = getMaxPixel();
            ofVec3f _xyz_MAX = ofVec3f((int)maxColor.r,(int)maxColor.g,(int)maxColor.b);
            if(tint_target_minMax){
                ofSetColor(maxColor);
            }
            draw_custom_target3D(_xyz_MAX, c_col_radMinMax);
        }
    }
}

bool VectorScope::is_insideSource()
{
    bool is_in = false;
    
    float aspect_W;
    float aspect_H;
    
    if(s_aspect>1){
        aspect_W = s_aspect;
        aspect_H = 1.0f;
    }else if(s_aspect<1){
        aspect_W = 1.0f;
        aspect_H = 1+s_aspect;
    }else{
        aspect_W = 1.0f;
        aspect_H = 1.0f;
    }
    
    if (   MX > ((                             CropRec_V[0] - 1) *_source_scale) * aspect_W
        && MX < (                              CropRec_V[2]      *_source_scale) * aspect_W
        && MY < w_H - ( (d_image.getHeight() - CropRec_V[3])     *_source_scale) * aspect_H
        && MY > w_H - ( (d_image.getHeight() + CropRec_V[1] + 1) *_source_scale) * aspect_H
        ){
//        cout<<"IN"<<endl;
        return is_in = true;
    }else{
//        cout<<"OUT"<<endl;
        return is_in = false;
    }
}

void VectorScope::draw_source(bool is_loaded_image)
{
    // the compensation and reset of the coordinates need to be done in order to get a new anchor
    ofScale(1/_scale);
    if(!view_mode3D){
        ofRotateDeg((int)-_rotate);
    }
    ofTranslate(-_pos[0], -_pos[1]);
    d_image.setAnchorPercent(0,1);

    // safe methdo for removing 2D translation and scale when in 3D mode
    if (!view_mode3D) {
        ofScale(1/MouseScale);
        ofTranslate(-w_W/2, w_H/2);
        ofTranslate(-cumulativeOffset.x, -cumulativeOffset.y);
    }else{
        ofTranslate(-w_W/2, w_H/2);
    }
    
    // aspect ratio source image
    // dynamic scaling source image for correct aspect ratio
    s_aspect = (o_image.getWidth()/o_image.getHeight());
    
    if(is_loaded_image){
        if(s_aspect>1){
            ofScale(s_aspect,1);
        }else if(s_aspect<1){
            ofScale(1,1+s_aspect);
        }else{
            ofScale(1,1);
            s_aspect = 1;
        }
    }else if (!is_loaded_image){
        ofScale(1,1);
        s_aspect = 1;
    }
    
    if (button_source) {
        
        ofScale(_source_scale);
        
        // crop function to get the REC values
        
        // the crop function REQUIRES an anchor reset so all the coordinates start from 0,0 not 0,1 and the H is double inverted
        d_image.resetAnchor();
        // due to a repositioning of the anchor an offset is required
        ofTranslate(0,-s_W);
        
        // draw with subsction source image
        d_image_drawSubsection_x    = CropRecLFT;
        d_image_drawSubsection_y    = CropRecTOP;
        d_image_drawSubsection_w    = CropRecRGT-CropRecLFT-CropRecLFT;
        d_image_drawSubsection_h    = CropRecBOT;
        d_image_drawSubsection_sx   = CropRecLFT;
        d_image_drawSubsection_sy   = CropRecTOP;
        d_image_drawSubsection_sw   = CropRecRGT-CropRecLFT-CropRecLFT;
        d_image_drawSubsection_sh   = CropRecBOT;
        
        if(button_minmax || MinMax_pixel_button){
            d_image.draw(0, 0);
        }else{
            d_image.drawSubsection(
                                   d_image_drawSubsection_x,
                                   d_image_drawSubsection_y,
                                   
                                   d_image_drawSubsection_w,
                                   d_image_drawSubsection_h,
                                   
                                   d_image_drawSubsection_sx,
                                   d_image_drawSubsection_sy,
                                   
                                   d_image_drawSubsection_sw,
                                   d_image_drawSubsection_sh
                                   );
        }
        
        // translation compensation from the anchor point offset
        ofTranslate(0,s_W);
    }
    
    // dynamis scaling source image for correct aspect ratio
    if(is_loaded_image){
        if(s_aspect>1){
            ofScale(1/s_aspect,1);
        }else if(s_aspect<1){
            ofScale(1,1/(1+s_aspect));
        }else{
            ofScale(1,1);
        }
    }else if (!is_loaded_image){
        ofScale(1,1);
    }
}

ofColor VectorScope::getMaxPixel()
{
    // init value of intensity at 0
    maxIntensity = 0;
    
    // run pixel array
    for (int i = 0; i < p_MinMax_V.size(); i++) {
        // pass pixel data to of color to get components
        pixelColor_max = p_MinMax_V.getColor(i);
        
        // sum of RGB channels for each pixel to compare with others
        // this can be expanded to get different Max values
        // this is just the max sum, but coukld be the max luma/weighted pixel from coeficients
        int intensity_max = pixelColor_max.r + pixelColor_max.g + pixelColor_max.b;
        
        // filter max by getting max max
        if (intensity_max > maxIntensity) {
            maxIntensity = intensity_max;
            maxIntensityColor = pixelColor_max;
        }
    }
    
    return maxIntensityColor;
}

ofColor VectorScope::getMinPixel()
{
    // init value of intensity at 255*3 = Rmax+Gmax+Bmax
    minIntensity = 255*3;

    // run pixel array
    for (int i = 0; i < p_MinMax_V.size(); i++) {
        // pass pixel data to of color to get components
        pixelColor_min = p_MinMax_V.getColor(i);
        
        // check if pixel data is 0 on any channel
        if (pixelColor_min.r == 0 || pixelColor_min.g == 0 || pixelColor_min.b == 0) {
            continue;
        }
        
        // sum of RGB channels for each pixel to compare with others
        // this can be expanded to get different Min values
        // this is just the min sum, but coukld be the min luma/weighted pixel from coeficients
        int intensity_min = pixelColor_min.r + pixelColor_min.g + pixelColor_min.b;
        
        // filter max by getting min min
        if (intensity_min < minIntensity) {
            minIntensity = intensity_min;
            minIntensityColor = pixelColor_min;
        }
    }
    
    return minIntensityColor;
}

void VectorScope::draw_stats()
{
    
    // get min max R / G / B value for ANY pixel in image
    // no individual pixel is plotted as result of operation
    // to get the MAX_pixel / Min_pixel in image use getMinPixel() and getMaxPixel()
    if(button_minmax)
    {
        // ---------------------------------------------------------------------------------
        // MAX VALUE
        // p_maxV from std
        p_maxV_R = *std::max_element(p_MinMax_R.begin(), p_MinMax_R.end());
        p_maxV_G = *std::max_element(p_MinMax_G.begin(), p_MinMax_G.end());
        p_maxV_B = *std::max_element(p_MinMax_B.begin(), p_MinMax_B.end());
        // ---------------------------------------------------------------------------------
        
        // MIN VALUE - NON ZERO VALUE
        // Custom way to get non Zero values from iterator using a std::vector
        
        // Initialize a vector to store non-zero elements
        std::vector<int> Vp_minNoZero_R;
        std::vector<int> Vp_minNoZero_G;
        std::vector<int> Vp_minNoZero_B;
        
        // iterate through list of pixel values of .size()
        for (int i = 0; i < p_MinMax_R.size(); i++) {
            // check pixels value and add them to a vecor if NO zero value RED
            if (p_MinMax_R[i] != 0) {
                Vp_minNoZero_R.push_back(p_MinMax_R[i]);
            }

            // check pixels value and add them to a vecor if NO zero value GREEN
            if (p_MinMax_G[i] != 0) {
                Vp_minNoZero_G.push_back(p_MinMax_G[i]);
            }

            // check pixels value and add them to a vecor if NO zero value BLUE
            if (p_MinMax_B[i] != 0) {
                Vp_minNoZero_B.push_back(p_MinMax_B[i]);
            }
        }
        
        // check if vector NOT empty RED
        // p_minV_R from std::min_element, else list empty p_minV_G=0
        p_minV_R = (!Vp_minNoZero_R.empty()) ? *std::min_element(Vp_minNoZero_R.begin(), Vp_minNoZero_R.end()) : 0;
        // to avoid non Zero values allways, 1-1
        if (p_minV_R == 1) {
            p_minV_R -= 1;
        }

        // check if vector NOT empty GREEN
        // p_minV_G from std::min_element, else list empty p_minV_G=0
        p_minV_G = (!Vp_minNoZero_G.empty()) ? *std::min_element(Vp_minNoZero_G.begin(), Vp_minNoZero_G.end()) : 0;
        // to avoid non Zero values allways, 1-1
        if (p_minV_G == 1) {
            p_minV_G -= 1;
        }

        // check if vector NOT empty BLUE
        // p_minV_B from std::min_element, else list empty p_minV_G=0
        p_minV_B = (!Vp_minNoZero_B.empty()) ? *std::min_element(Vp_minNoZero_B.begin(), Vp_minNoZero_B.end()) : 0;
        // to avoid non Zero values allways, 1-1
        if (p_minV_B == 1) {
            p_minV_B -= 1;
        }
    }

//    cout << "Max Red: " << p_maxV_R << " //  Max Green: " << p_maxV_G << " //  Max Blue: " << p_maxV_B <<endl;
//    cout << "Min Red: " << p_minV_R << " //  Min Green: " << p_minV_G << " //  Min Blue: " << p_minV_B <<endl;
    
    if(button_source){
        ofScale(1/_source_scale);
    }
    ImVec2 v2 =  ImVec2(MX, MY);

    ofTranslate(w_W/2, -w_H/2);

    if (button_stats) {
        if(button_minmax){
            ofDrawBitmapString("Min/Max R: "  + ofToString(p_minV_R) + ", " + ofToString(p_maxV_R), w_W/2-180, w_H/2-140);
            ofDrawBitmapString("Min/Max G: "  + ofToString(p_minV_G) + ", " + ofToString(p_maxV_G), w_W/2-180, w_H/2-120);
            ofDrawBitmapString("Min/Max B: "  + ofToString(p_minV_B) + ", " + ofToString(p_maxV_B), w_W/2-180, w_H/2-100);
            
            ofSetColor(p_minV_R, p_minV_G, p_minV_B);
            ofDrawRectangle(w_W/2-180, w_H/2-180, 20, 20);
            
            ofSetColor(p_maxV_R, p_maxV_G, p_maxV_B);
            ofDrawRectangle(w_W/2-150, w_H/2-180, 20, 20);
            
            ofSetColor((p_maxV_R + p_maxV_G + p_maxV_B) / 3 );
            ofDrawRectangle(w_W/2-120, w_H/2-180, 20, 20);
            
            ofSetColor(255);
        }
        
        ofDrawBitmapString("x/y: " + ofToString(v2),w_W/2-180,w_H/2-60);
        ofDrawBitmapString("nPixels: " + ofToString((s_image.getWidth() * s_image.getHeight())/_precision),w_W/2-180,w_H/2-40);
        ofDrawBitmapString("Frame Rate: " + ofToString(ofGetFrameRate()),w_W/2-180,w_H/2-20);
    }
}

void VectorScope::draw_hud()
{
    // hud draw
    // Lines

    if (button_hud) {
        
        if(button_graticule)
        {
            // Vertical
            h_VL.setColor(ofColor(  custom_graticule_color[0] * _h_col,
                                    custom_graticule_color[1] * _h_col,
                                    custom_graticule_color[2] * _h_col
                                    ));
            h_VL.setStrokeWidth(_h_width);
            h_VL.lineTo(0,  minWin/2);
            h_VL.lineTo(0, -minWin/2);
            h_VL.draw();
            h_VL.clear();
            
            // horizontal
            h_HL.setColor(ofColor(  custom_graticule_color[0] * _h_col,
                                    custom_graticule_color[1] * _h_col,
                                    custom_graticule_color[2] * _h_col
                                    ));
            h_HL.setStrokeWidth(_h_width);
            h_HL.lineTo( minWin/2, 0);
            h_HL.lineTo(-minWin/2, 0);
            h_HL.draw();
            h_HL.clear();
            
            // circles population
            for(int l = 0; l < 4 ; l++){
                h_CI.setColor(ofColor(custom_graticule_color[0] * (_h_col* (0.25f+l*0.25f)),
                                      custom_graticule_color[1] * (_h_col* (0.25f+l*0.25f)),
                                      custom_graticule_color[2] * (_h_col* (0.25f+l*0.25f))
                                      ));
                h_CI.setFilled(false);
                h_CI.setStrokeWidth(_h_width);
                h_CI.setCircleResolution(h_rez);
                h_CI.circle(0, 0, minWin/2 * (0.25f+l*0.25f));
                h_CI.draw();
                h_CI.clear();
            }

            // lines 3 degrees
            for(int i = 0; i < 360; i+= 3 ){
                h_LINES2.setStrokeColor(ofColor(custom_graticule_color[0] * (_h_col*0.5f),
                                                custom_graticule_color[1] * (_h_col*0.5f),
                                                custom_graticule_color[2] * (_h_col*0.5f)
                                                ));
                
                h_LINES2.setStrokeWidth(1);
                float x =  (minWin/2) * cos(ofDegToRad(i));
                float y =  (minWin/2) * sin(ofDegToRad(i));
                // line segment length
                h_LINES2.lineTo(0, 0);
                h_LINES2.lineTo(0,  -10);
                // center rotation
                glm::vec3 axis2(0, 0, 1);
                h_LINES2.rotateDeg(i-90, axis2);
                // move to circle
                glm::vec2 move2(x, y);
                h_LINES2.translate(move2);
                // draw loop
                h_LINES2.draw();
                h_LINES2.clear();
            }

            // lines 10 degrees
            for(int i = 0; i < 360; i+= (360/40) ){
                h_LINES1.setStrokeColor(ofColor(custom_graticule_color[0] * _h_col,
                                                custom_graticule_color[1] * _h_col,
                                                custom_graticule_color[2] * _h_col
                                                ));
                h_LINES1.setStrokeWidth(1);
                float x =  (minWin/2)      * cos(ofDegToRad(i));
                float y =  (minWin/2)      * sin(ofDegToRad(i));
                // line segment length
                h_LINES1.lineTo(0, 0);
                h_LINES1.lineTo(0,  -25);
                // center rotation
                glm::vec3 axis(0, 0, 1);
                h_LINES1.rotateDeg(i-90, axis);
                // move to circle
                glm::vec2 move(x, y);
                h_LINES1.translate(move);
                // draw loop
                h_LINES1.draw();
                h_LINES1.clear();
            }
        }


        // custom USER line
        if(h_LINES_CU_b){
            h_LINES_CU.setColor(ofColor(200));
            h_LINES_CU.setStrokeWidth(2);
            float x_cC =  ((minWin/2)*h_LINES_CU_f) * cos(ofDegToRad(h_LINES_CU_v - 90));
            float y_cC =  ((minWin/2)*h_LINES_CU_f) * sin(ofDegToRad(h_LINES_CU_v - 90));
            h_LINES_CU.lineTo(0,    0);
            h_LINES_CU.lineTo(x_cC, y_cC);
            h_LINES_CU.draw();
            h_LINES_CU.clear();
        }

        if(button_IQ)
        {
            // custom line " 'skintone' line"
            // I ("In phase") and ("Quadrature") components
            for (int iq = 0; iq<4; iq++){
                h_LINES_IQ.setColor(ofColor(custom_graticule_color[0] * (_h_col_IQ* 0.33f),
                                            custom_graticule_color[1] * (_h_col_IQ* 0.33f),
                                            custom_graticule_color[2] * (_h_col_IQ* 0.33f)
                                            ));

                
                h_LINES_IQ.setStrokeWidth(2);
                float x_c =  (minWin/2) * cos(ofDegToRad(33 + (iq*90)));
                float y_c =  (minWin/2) * sin(ofDegToRad(33 + (iq*90)));
                h_LINES_IQ.lineTo(0,    0);
                h_LINES_IQ.lineTo(x_c,  y_c);
                h_LINES_IQ.scale(1, -1);
                h_LINES_IQ.draw();
                h_LINES_IQ.clear();
            }
        }
    }
    
    // draw targets function inside the hud
    if(button_targets){
        draw_targets();
    }
    
}

void VectorScope::draw_hud3D()
{
    // hud draw 3D
    // Lines

    if (button_hud) {
        
        if(button_graticule)
        {
            // Vertical Cr
            h_VL.setColor(ofColor(  custom_graticule_color[0] * _h_col,
                                    custom_graticule_color[1] * _h_col,
                                    custom_graticule_color[2] * _h_col
                                    ));
            h_VL.setStrokeWidth(_h_width);
            h_VL.lineTo(0,  minWin/2, -minWin/2);
            h_VL.lineTo(0, -minWin/2, -minWin/2);
            h_VL.draw();
            h_VL.clear();

            // horizontal Cb
            h_HL.setColor(ofColor(  custom_graticule_color[0] * _h_col,
                                    custom_graticule_color[1] * _h_col,
                                    custom_graticule_color[2] * _h_col
                                    ));
            h_HL.setStrokeWidth(_h_width);
            h_HL.lineTo( minWin/2, 0, -minWin/2);
            h_HL.lineTo(-minWin/2, 0, -minWin/2);
            h_HL.draw();
            h_HL.clear();

            // Depth Y
            h_DL.setColor(ofColor(  custom_graticule_color[0] * _h_col,
                                    custom_graticule_color[1] * _h_col,
                                    custom_graticule_color[2] * _h_col
                                    ));
            h_DL.setStrokeWidth(_h_width);
            h_DL.lineTo( 0, 0, minWin/2);
            h_DL.lineTo( 0, 0, -minWin/2);
            h_DL.draw();
            h_DL.clear();
            

            // circles population
            for(int l = 0; l < 4 ; l++){
                h_CI.setColor(ofColor(custom_graticule_color[0] * (_h_col* (0.25f+l*0.25f)),
                                      custom_graticule_color[1] * (_h_col* (0.25f+l*0.25f)),
                                      custom_graticule_color[2] * (_h_col* (0.25f+l*0.25f))
                                      ));
                h_CI.setFilled(false);
                h_CI.setStrokeWidth(_h_width);
                h_CI.setCircleResolution(h_rez);
                h_CI.circle(0, 0, 0, minWin/2 * (0.25f+l*0.25f));
                h_CI.draw();
                h_CI.clear();
            }

            // lines 3 degrees
            for(int i = 0; i < 360; i+= 3 ){
                h_LINES2.setStrokeColor(ofColor(custom_graticule_color[0] * (_h_col*0.5f),
                                                custom_graticule_color[1] * (_h_col*0.5f),
                                                custom_graticule_color[2] * (_h_col*0.5f)
                                                ));
                h_LINES2.setStrokeWidth(1);
                float x =  (minWin/2) * cos(ofDegToRad(i));
                float y =  (minWin/2) * sin(ofDegToRad(i));
                // line segment length
                h_LINES2.lineTo(0,      0,      0);
                h_LINES2.lineTo(0,      -10,    0);
                // center rotation
                glm::vec3 axis2(0, 0, 1);
                h_LINES2.rotateDeg(i-90, axis2);
                // move to circle
                glm::vec2 move2(x, y);
                h_LINES2.translate(move2);
                // draw loop
                h_LINES2.draw();
                h_LINES2.clear();
            }

            // lines 10 degrees
            for(int i = 0; i < 360; i+= (360/40) ){
                h_LINES1.setStrokeColor(ofColor(custom_graticule_color[0] * _h_col,
                                                custom_graticule_color[1] * _h_col,
                                                custom_graticule_color[2] * _h_col
                                                ));
                h_LINES1.setStrokeWidth(1);
                float x =  (minWin/2)      * cos(ofDegToRad(i));
                float y =  (minWin/2)      * sin(ofDegToRad(i));
                // line segment length
                h_LINES1.lineTo(0,      0,      0.01f);
                h_LINES1.lineTo(0,      -25,    0.01f);
                // center rotation
                glm::vec3 axis(0, 0, 1);
                h_LINES1.rotateDeg(i-90, axis);
                // move to circle
                glm::vec2 move(x, y);
                h_LINES1.translate(move);
                // draw loop
                h_LINES1.draw();
                h_LINES1.clear();
            }
        }
        
        if(button_box3dD)
        {
            // main box
            ofSetColor(_h_col_box3d);
            ofNoFill();
            ofDrawBox(minWin);
            ofFill();
            ofSetColor(255);
        }


        // custom USER line
        if(h_LINES_CU_b){
            h_LINES_CU.setColor(ofColor(200));
            h_LINES_CU.setStrokeWidth(2);
            float x_cC2 = ((minWin/2)*h_LINES_CU_f) * cos(ofDegToRad(-h_LINES_CU_v+90));
            float y_cC2 = ((minWin/2)*h_LINES_CU_f) * sin(ofDegToRad(-h_LINES_CU_v+90));
            h_LINES_CU.lineTo(0,     0,       .1);
            h_LINES_CU.lineTo(x_cC2, y_cC2,   .1+(h_LINES_CU_vz*(minWin/2)));
            h_LINES_CU.draw();
            h_LINES_CU.clear();
        }
        
        if(button_IQ)
        {
            // custom line " 'skintone' line"
            // I ("In phase") and ("Quadrature") components
            for (int iq = 0; iq<4; iq++){
                h_LINES_IQ.setColor(ofColor(custom_graticule_color[0] * (_h_col_IQ* 0.33f),
                                            custom_graticule_color[1] * (_h_col_IQ* 0.33f),
                                            custom_graticule_color[2] * (_h_col_IQ* 0.33f)
                                            ));
                h_LINES_IQ.setStrokeWidth(2);
                float x_c = (minWin/2) * cos(ofDegToRad(33 + (iq*90)));
                float y_c = (minWin/2) * sin(ofDegToRad(33 + (iq*90)));
                h_LINES_IQ.lineTo(0,    0,      0);
                h_LINES_IQ.lineTo(x_c,  y_c,    0);
                h_LINES_IQ.draw();
                h_LINES_IQ.clear();
            }
        }
    }
    
    // draw targets function inside the hud
    if(button_targets){
        draw_targets3D();
    }
}

void VectorScope::draw_targets()
{
    // prim struct call
    prims COLOR;
    
    for (int color = RED; color < NUM_COLORS; color++) {
        // ofColor definition and asignment based from struct
        // calls the constructor inside the struct from the prims method
        array<int, 3>& PRIM_COL = COLOR.PRIM[color];

        // color matrix
        if (matrix_item_current == 1) {
            
            target_V = COLOR_MATRIX_PRIMS(ofVec3f(PRIM_COL[0], PRIM_COL[1], PRIM_COL[2]), RGB_YCbCr_709);
            c_rotate    = 180.0f;
            c_scale[0]  = -1.0f         * (target_Factor/100.0f);
            c_scale[1]  = -1.0f         * (target_Factor/100.0f);
            
        }else if (matrix_item_current == 0){
            target_V = COLOR_MATRIX_PRIMS(ofVec3f(PRIM_COL[0], PRIM_COL[1], PRIM_COL[2]), RGB_YCbCr_601);
            c_rotate    = 180.0f;
            c_scale[0]  = -1.0f        * (target_Factor/100.0f);
            c_scale[1]  = -1.0f        * (target_Factor/100.0f);
            
        }else if(matrix_item_current ==2){
            target_V = COLOR_MATRIX_PRIMS(ofVec3f(PRIM_COL[0], PRIM_COL[1], PRIM_COL[2]), RGB_YCbCr_2020);
            c_rotate    = 180.0f;
            c_scale[0]  = -1.0f        * (target_Factor/100.0f);
            c_scale[1]  = -1.0f        * (target_Factor/100.0f);
            
        }
        
        float rect_padd = 6;
        
        target_V.x = ofMap(  target_V.x, 0,   _maxRange,  rect_padd,    (minWin)-rect_padd);
        target_V.y = ofMap(  target_V.y, 0,   _maxRange,  rect_padd,    (minWin)-rect_padd);
        target_V.z = ofMap(  target_V.z, 0,   _maxRange,  rect_padd,    (minWin)-rect_padd);
        
        ofVec3f OFFSET = ofVec3f(minWin/2, minWin/2, minWin/2);
        
        target_V -= OFFSET;
        
        target_V_2d = PSR_2D(ofVec2f(target_V.y, target_V.z), c_pos[0], c_pos[1], c_scale[0], -c_scale[1], c_rotate);
        
        int RRR = minWin/(30/target_size*30);
        
        
        if(button_tint_targets)
        {
            h_Rec.setColor(ofColor(PRIM_COL[0]*(target_gain/100),
                                   PRIM_COL[1]*(target_gain/100),
                                   PRIM_COL[2]*(target_gain/100) ));
        }else{
            h_Rec.setColor(ofColor(custom_target_color[0]*(target_gain/100)*255,
                                   custom_target_color[1]*(target_gain/100)*255,
                                   custom_target_color[2]*(target_gain/100)*255));
        }
        
        h_Rec.setFilled(false);
        h_Rec.setStrokeWidth(_h_width);
        
        ofVec2f p_BL = ofVec2f(-RRR, +RRR);
        ofVec2f p_BR = ofVec2f(+RRR, +RRR);
        ofVec2f p_TL = ofVec2f(-RRR, -RRR);
        ofVec2f p_TR = ofVec2f(+RRR, -RRR);
        
        h_Rec.lineTo(p_BL.x, p_BL.y);
        h_Rec.lineTo(p_BR.x, p_BR.y);
        h_Rec.lineTo(p_TR.x, p_TR.y);
        h_Rec.lineTo(p_TL.x, p_TL.y);
        h_Rec.close();
        
        float XX = target_V_2d.x;
        float YY = target_V_2d.y;
        
        float angleRad = atan2(YY, XX);
        float angleDeg = ofRadToDeg(angleRad);
        
        glm::vec3 ax(0, 0, 1);
        h_Rec.rotateDeg(angleDeg+90, ax);
        
        
        glm::vec2 p(XX , YY );
        h_Rec.translate(p);
        
        h_Rec.draw();
        h_Rec.clear();
    }
}

void VectorScope::draw_targets3D()
{
    
    // prim struct call
    prims COLOR;
    
    for (int color = RED; color < NUM_COLORS; color++) {
        // ofColor definition and asignment based from struct
        // calls the constructor inside the struct from the prims method
        array<int, 3>& PRIM_COL = COLOR.PRIM[color];

        // color matrix
        if (matrix_item_current == 1) {
            glColor4f(1, 1, 1, 1);
            target_V = COLOR_MATRIX_PRIMS(ofVec3f(PRIM_COL[0], PRIM_COL[1], PRIM_COL[2]), RGB_YCbCr_709);
            c_rotate    = 180.0f;
//            c_scale[0]  = -1.0f         * (target_Factor/100.0f);
//            c_scale[1]  = -1.0f         * (target_Factor/100.0f);
            
        }else if (matrix_item_current == 0){
            target_V = COLOR_MATRIX_PRIMS(ofVec3f(PRIM_COL[0], PRIM_COL[1], PRIM_COL[2]), RGB_YCbCr_601);
            c_rotate    = 180.0f;
//            c_scale[0]  = -1.0f        * (target_Factor/100.0f);
//            c_scale[1]  = -1.0f        * (target_Factor/100.0f);
            
        }else if(matrix_item_current ==2){
            target_V = COLOR_MATRIX_PRIMS(ofVec3f(PRIM_COL[0], PRIM_COL[1], PRIM_COL[2]), RGB_YCbCr_2020);
            c_rotate    = 180.0f;
//            c_scale[0]  = -1.0f        * (target_Factor/100.0f);
//            c_scale[1]  = -1.0f        * (target_Factor/100.0f);
            
        }
        
        
        float rect_padd = 6;
        
        target_V.x = ofMap(  target_V.x, 0,   _maxRange,  rect_padd,    (minWin)-rect_padd);
        target_V.y = ofMap(  target_V.y, 0,   _maxRange,  rect_padd,    (minWin)-rect_padd);
        target_V.z = ofMap(  target_V.z, 0,   _maxRange,  rect_padd,    (minWin)-rect_padd);
        
        ofVec3f OFFSET = ofVec3f(minWin/2, minWin/2, minWin/2);
        
        target_V -= OFFSET;
        
        ofVec3f r_xyz  = ROT_3D(target_V, ofVec3f(-90, -90, 0));
        
        ofNoFill();
        if(button_tint_targets){
            ofSetColor(ofColor(PRIM_COL[0]*(target_gain/100),
                               PRIM_COL[1]*(target_gain/100),
                               PRIM_COL[2]*(target_gain/100) ));
        }else{
            ofSetColor(ofColor(custom_target_color[0]*(target_gain/100)*255,
                               custom_target_color[1]*(target_gain/100)*255,
                               custom_target_color[2]*(target_gain/100)*255));
            
        }
        
        // LOCAL PSR... LOL
        float XX = r_xyz.x;
        float YY = r_xyz.y;
        
        float angleRad = atan2(YY, XX);
        float angleDeg = ofRadToDeg(angleRad);
        
        ofTranslate(r_xyz.x * (target_Factor/100.0f), r_xyz.y * (target_Factor/100.0f), r_xyz.z * (target_Factor/100.0f));
        ofRotateZDeg(angleDeg+90);
        ofDrawBox(0,0,0, minWin/(30/(target_size*2)*30));
        ofRotateZDeg(-(angleDeg+90));
        ofTranslate(-r_xyz.x * (target_Factor/100.0f), -r_xyz.y * (target_Factor/100.0f), -r_xyz.z * (target_Factor/100.0f));
        ofFill();

    }
}

void VectorScope::draw_magnifier()
{

    if(m_button)
    {
        if (MX>0 && MX<w_W && MY>0 && MY<w_H)
        {
            int sq = 180;
            // sample the pixel grabbed for data display
            // if this function is used after the new texture draw, it will use the datatype of the texture with wrong data
            // allocated a 1x1 pixel image to grab from mouse coordinates
            m_image.grabScreen(MX, MY, 1, 1);
            // initial color allocation for sampoling, otherwise if not set some random values from memory pop
            m_col = ofColor(0,0,0,0);
            // sample pixel at 0,0 = the one and only pixel in the image
            m_col = m_image.getColor(0, 0);
            m_col_r = m_col.r;
            m_col_g = m_col.g;
            m_col_b = m_col.b;
            
            // magnifier load data into the texture
            m_texture.loadScreenData(MX-(m_factor/2), MY-(m_factor/2), m_factor, m_factor);
            
            if(MX+sq > w_W){
                magnifierEdgeWindow_offsetX = -sq;
            }else{
                magnifierEdgeWindow_offsetX = 0;
            }
            
            if((MY+sq+60) > w_H){
                magnifierEdgeWindow_offsetY = -sq - 60;
            }else{
                magnifierEdgeWindow_offsetY = 0;
            }
            
            // display data loaded 'magnified'
            m_texture.draw((MX+magnifierEdgeWindow_offsetX)-(w_W/2), (MY+magnifierEdgeWindow_offsetY)-(w_H/2), sq, sq);
            
            // hud magnifiers
            // data square and frame
            m_p_data.setColor(15);
            m_p_data.setFilled(true);
            m_p_data.setStrokeColor(ofColor(40));
            m_p_data.setStrokeWidth(1);
            m_r_data.set(0, 0, sq, sq/3);
            m_p_data.rectangle(m_r_data);
            m_p_data.draw((MX+magnifierEdgeWindow_offsetX)-(w_W/2), (MY+magnifierEdgeWindow_offsetY)-(w_H/2) + sq);
            m_p_data.clear();

            // main frame
            m_p.setColor(100);
            m_p.setFilled(false);
            m_p.setStrokeColor(ofColor(100));
            m_p.setStrokeWidth(1);
            m_r.set(0,0, sq, sq);
            m_p.rectangle(m_r);
            m_p.draw((MX+magnifierEdgeWindow_offsetX)-(w_W/2), (MY+magnifierEdgeWindow_offsetY)-(w_H/2));
            m_p.clear();

            
            // main frame cross pixel sampler
            m_cross_pixel.setColor(ofColor(_h_col * 0.75f));
            m_cross_pixel.setStrokeWidth(1);
            
            int cL_H_p1[2] = {((MX+magnifierEdgeWindow_offsetX)+sq/2)  - w_W/2     ,  (MY+magnifierEdgeWindow_offsetY)      - w_H/2};
            int cL_H_p2[2] = {((MX+magnifierEdgeWindow_offsetX)+sq/2)  - w_W/2     , ((MY+magnifierEdgeWindow_offsetY)+sq)  - w_H/2};
            m_cross_pixel.lineTo(cL_H_p1[0], cL_H_p1[1]);
            m_cross_pixel.lineTo(cL_H_p2[0], cL_H_p2[1]);
            m_cross_pixel.draw();
            m_cross_pixel.clear();

            int cL_V_p1[2] = {((MX+magnifierEdgeWindow_offsetX))       - w_W/2     , ((MY+magnifierEdgeWindow_offsetY)+sq/2) - w_H/2};
            int cL_V_p2[2] = {((MX+magnifierEdgeWindow_offsetX)+sq)    - w_W/2     , ((MY+magnifierEdgeWindow_offsetY)+sq/2) - w_H/2};
            m_cross_pixel.lineTo(cL_V_p1[0], cL_V_p1[1]);
            m_cross_pixel.lineTo(cL_V_p2[0], cL_V_p2[1]);
            m_cross_pixel.draw();
            m_cross_pixel.clear();
            
            // single pixel draw ontop of cross pixel lines
            m_cross_pixel.setFilled(true);
            m_cross_pixel.setColor(ofColor(m_col_r, m_col_g, m_col_b, 255));
            //center rectangle fake fill
            m_cross_pixel_rect.set( (((MX+magnifierEdgeWindow_offsetX)+sq/2)-((sq/m_factor)/2)) - w_W/2,
                                   (((MY+magnifierEdgeWindow_offsetY)+sq/2)-((sq/m_factor)/2)) - w_H/2,
                                   (sq/m_factor),
                                   (sq/m_factor)
                                   );
            
            m_cross_pixel.rectangle(m_cross_pixel_rect);
            m_cross_pixel.draw();
            m_cross_pixel.clear();
            
            // center rectangle edge
            m_cross_pixel.setFilled(false);
            m_cross_pixel.setStrokeColor(ofColor(100));
            m_cross_pixel.setStrokeWidth(1);
            m_cross_pixel_rect.set( (((MX+magnifierEdgeWindow_offsetX)+sq/2)-((sq/m_factor)/2)) - w_W/2,
                                    (((MY+magnifierEdgeWindow_offsetY)+sq/2)-((sq/m_factor)/2)) - w_H/2,
                                    (sq/m_factor),
                                    (sq/m_factor)
                                   );
            
            m_cross_pixel.rectangle(m_cross_pixel_rect);
            m_cross_pixel.draw();
            m_cross_pixel.clear();
            
            // custom line "DISTANCE"
            int L_s_posX = cumulativeOffset.x;
            int L_s_posY = cumulativeOffset.y;
            LINE_CUSTOM_Dist.setColor(ofColor(_h_col * 0.75f));
            LINE_CUSTOM_Dist.setStrokeWidth(1);
            LINE_CUSTOM_Dist.lineTo(L_s_posX,  L_s_posY);
            LINE_CUSTOM_Dist.lineTo(MX - w_W/2, MY - w_H/2);
            LINE_CUSTOM_Dist.draw();
            LINE_CUSTOM_Dist.clear();
            float dist = ofDist(L_s_posX, L_s_posY, MX - w_W/2, MY - w_H/2);
            dist = (dist/MouseScale)/(w_H/2);
            ofDrawBitmapString("DIST: " + ofToString(dist*100),(MX+magnifierEdgeWindow_offsetX)-w_W/2 + 10   , (MY+magnifierEdgeWindow_offsetY)-w_H/2 + sq + 45);

            // data draw
            ofDrawBitmapString("R:" + ofToString(m_col_r),(MX+magnifierEdgeWindow_offsetX)-w_W/2 + 10    , (MY+magnifierEdgeWindow_offsetY)-w_H/2 + sq + 20);
            ofDrawBitmapString("G:" + ofToString(m_col_g),(MX+magnifierEdgeWindow_offsetX)-w_W/2 + 70    , (MY+magnifierEdgeWindow_offsetY)-w_H/2 + sq + 20);
            ofDrawBitmapString("B:" + ofToString(m_col_b),(MX+magnifierEdgeWindow_offsetX)-w_W/2 + 130   , (MY+magnifierEdgeWindow_offsetY)-w_H/2 + sq + 20);
        
        }else{
            false;
        }
        
    }
}

// --------------------------------------------------------------------------------------------------
// // to be implemented in the future, now its too expensive and slow
// // Successful implemented way to use openCV
//void VectorScope::draw_cv()
//{
//
//    if (cv_button) {
//
//        // grab image data and store it into image data(pixels)
//        cv_image.grabScreen(0, 0, w_W, w_H);
//
//        // the set image type HAS to be in the draw loop, otherwise it will fail
//        cv_image.setImageType(OF_IMAGE_COLOR);
//
//        // get pixels from image
//        cv_image.getTexture().readToPixels(cv_pixels);
//
//        // set cv image from pixels
//        ofxCV_image_OUT.setFromPixels(cv_pixels.getData(), w_W, w_H);
//
//        // *********************    CV WORK HERE        ********************* //
//
//        ofxCV_image_OUT.convertRgbToHsv();
//        ofxCV_image_OUT.dilate();
//        ofxCV_image_OUT.blurGaussian(9);
//
//        // *********************    END CV WORK HERE    ********************* //
//
//        // RENDER CV IMAGE
//        ofxCV_image_OUT.draw(-w_W/2, -w_H/2);
//    }
//
//}
// --------------------------------------------------------------------------------------------------

void VectorScope::gui(const char* name)
{
    // the const char* name type allows to have multiple gui with a string as ID...

// --------------------------------------------------------------------------------------------------
//        // disabled for now the main menu
//        // needs to be revsited in the future
//
//        // creates menubar and populate it
//        if (ImGui::BeginMainMenuBar()) {
//            if (ImGui::BeginMenu("File")) {
//                if (ImGui::MenuItem("Create")) {
//                }
//                if (ImGui::MenuItem("Open", "Ctrl+O")) {
//                    cout<<"Open"<<endl;
//                }
//                if (ImGui::MenuItem("Save", "Ctrl+S")) {
//                }
//                if (ImGui::MenuItem("Save as..")) {
//                }
//                ImGui::EndMenu();
//            }
//            ImGui::EndMainMenuBar();
//        }
//
// --------------------------------------------------------------------------------------------------


    // dockable space for any window pannel created
    // it needs a null pointer as an initialization for the main window, since open frameworks is not using a conventional opengl method
    ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

//    ImGui::SetNextWindowSize(ImVec2(400, 600)); // disabled so the size its dynamic
    if(help_key){
        ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(900,900));
        ImGui::Begin("Help", NULL, ImGuiWindowFlags_NoSavedSettings);
        if (ImGui::BeginTabBar("TABS", ImGuiTabBarFlags_Reorderable))
        {
            if (ImGui::BeginTabItem("HOTKEYS"))
            {

                ImGui::Spacing();
                ImGui::BulletText("HOTKEYS");
                ImGui::Text("    'p'          : save screencapture to current directory");
                ImGui::Text("    'f'          : switch fullscreen mode");
                ImGui::Text("    's'          : show source image");
                ImGui::Text("    'm'          : show magnifier tool (only 2D mode)");
                ImGui::Spacing();
                ImGui::Text("    'c'          : reset control values");
                ImGui::Text("    'a'          : transparent BG");
                ImGui::Text("    'i'          : image stats");
                ImGui::Text("    'h'          : HUD display");
                ImGui::Text("    't'          : targets display");
                ImGui::Text("    'r g b'      : display channels");
                ImGui::Spacing();
                ImGui::Separator();// ####################################################
                
                ImGui::Text("    'Return Key' : switch 2D and 3D mode");
                ImGui::Text("    'o'          : switch orthografic / perspective camera (3D)");
                ImGui::Spacing();
                ImGui::Separator();// ####################################################
                
                ImGui::Text("    'z'          : display test images mode");
                ImGui::Text("    '1 2 3'      : switch test images");
                ImGui::Text("    '0'          : display last loaded image");
                ImGui::Dummy(ImVec2(300,50));
                
                ImGui::EndTabItem();
            }
        
            if (ImGui::BeginTabItem("NAVIGATION CONTROLS"))
            {
                
                ImGui::Spacing();
                ImGui::BulletText("NAVIGATION CONTROLS");
                ImGui::Text("    2D Translate : Right Mouse click and drag");
                ImGui::Text("    2D Scale     : Middle mouse weel/zoon action in pad");
                ImGui::Text("    2D Rotate    : Knob in UI");
                ImGui::Text("    Reset View   : middle mouse click");
                ImGui::Spacing();
                ImGui::Separator();// ####################################################
                
                ImGui::Text("    3D Translate : Middle Mouse click and drag");
                ImGui::Text("    3D Scale     : Middle mouse weel/zoon action in pad");
                ImGui::Text("    3D Rotate    : Left Mouse click and orbit");
                ImGui::Text("    Reset View   : Left Mouse double click");
                
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Credits"))
            {
                ImGui::Text("GR_PLOTS_V1.0.1");
                ImGui::Text("2024 - Guillem Ramisa de Soto");
                ImGui::Text("Xcode Version 14.3.1 ");
                ImGui::Text("Made with OpenFrameworks:(of_v0.12.0_osx_release)");
                ImGui::Text("Made with dear imgui, v1.77 WIP");
                ImGui::Text("ofxOpenCv");
                ImGui::Text("OpenGL 3.2 | GLSL version 150");
                
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
            
        }
        ImGui::End();
 
    };
      
    // (ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar))
//    ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(200,200));
    if(ImGui::Begin(name, NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::Spacing();
        
        if(view_mode3D){
            ImGui::Text("VectorScope MODE: 3D");
        }else{
            ImGui::Text("VectorScope MODE: 2D");
        }
        
        
        if (ImGui::BeginTabBar("MAIN", ImGuiTabBarFlags_Reorderable)){
            if (ImGui::BeginTabItem("DISPLAY"))
            {

                const char* matrix_items[] = { "Rec-601", "Rec-709", "Rec-2020" };
                ImGui::Combo("YCbCr Matrix", &matrix_item_current, matrix_items, IM_ARRAYSIZE(matrix_items));
                if(ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip("Matrix coeficients conversion:\nRec 601  : SD-TV\nRec 709  : HD-TV\nRec 2020 : UHD-SDR\n\nAll conversions are done using linear matrix transforms. From intput to YCbCR\nNo correctios is done."); // text over the mouse
                }
                
                ImGui::Separator(); // ####################################################
        
                ImGui::Checkbox("HUD",                          &button_hud);
                ImGui::SameLine();
                ImGui::Checkbox("Targets",                      &button_targets);
                ImGui::SameLine();
                ImGui::Checkbox("Stats",                        &button_stats);
                
                ImGui::Checkbox("Source",                       &button_source);
                ImGui::SameLine();
                ImGui::Checkbox("Sampler",                      &m_button);
                
                if(button_stats){
                    ImGui::SameLine();
                    ImGui::Checkbox("Min/Max",                  &button_minmax);
                    if(ImGui::IsItemHovered())
                    {
                        ImGui::SetTooltip("displays the R / G / B min and max values of any pixel.\nDue to performance issues this disables precision and image crop"); // text over the mouse
                    }
                }
                
                ImGui::Spacing();
                ImGui::EndTabItem();
            }
            
            // ----------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------
            
            if (ImGui::BeginTabItem("PLOT"))
            {
                
//                ImGui::Separator(); // ####################################################

                if (ImGui::Button("Reset controls", ImVec2(140, 20)))
                {
                    resetControls();
                }
                if(ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip("Reset controls to init values"); // text over the mouse
                }
                
                ImGui::Separator(); // ####################################################
                
                ImGui::Bullet();
                ImGui::Text("SOURCE IMAGE");
                
                if(button_minmax || MinMax_pixel_button){
                    
                    // This allows for a style to be passed without having to call an etire style
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.25f);
                    // This allows to DISABLE/LOCK a knob
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                    ImGui::SliderInt("Precision",               &_precision,        32,     0);
                    // The knob style and active state needs to be called after
                    ImGui::PopItemFlag();
                    ImGui::PopStyleVar();
                }else{
                    ImGui::SliderInt("Precision",               &_precision,        32,     0);
                }
                if(ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip("1 / nPixel\n\nHiger value less precicion"); // text over the mouse
                }
                
                if(button_minmax || MinMax_pixel_button){
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.25f);
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                    ImGui::DragInt4("Source crop",              &CropRec_V[0], 0.5f);
                    ImGui::PopItemFlag();
                    ImGui::PopStyleVar();
                }else{
                    ImGui::DragInt4("Source crop",              &CropRec_V[0], 0.5f);
                }
                
                ImGui::DragFloat("Display scale",               &_source_scale, 0.005, 0.001, 2);

                ImGui::Separator(); // ####################################################

                ImGui::Bullet();
                ImGui::Text("PIXEL");
                
                ImGui::Checkbox("Pixel color from  source",     &button_tint_pixels);
                if(!button_tint_pixels){
                    ImGui::ColorEdit3("Pixel color",            custom_pixel_color);
                }
                ImGui::DragInt2("Pixel threshold",              &Col_threshold[0], 0.5);
                
                ImGui::SliderInt("Pixel dilate",                &_erode,            0,      4      );
                ImGui::SliderFloat("Pixel alpha",               &_alpha,            0.0f,   1.0f   );
                ImGui::SliderFloat("Pixel gain",                &_gain,             1,      4      );
// --------------------------------------------------------------------------------------------------
// Removed due to an incompatibility with openGL versions. to be fixed in the future??
//                ImGui::SliderFloat("hud width",         &_h_width,          2,      20      );
// --------------------------------------------------------------------------------------------------
                
                ImGui::Separator(); // ####################################################
                
                ImGui::Bullet();
                ImGui::Text("TRANSFORM");
                
                if(!view_mode3D){
                    ImGui::SliderFloat("Rotation 2D",           &_rotate,          -180,    180     );
                }
                ImGui::SliderFloat3("Rotation 3D",              &_rotate3D[0],     -180,    180     );
                ImGui::SliderInt("Padding",                     &_padding,          1,      70      );
             
                ImGui::Spacing();
                
                ImGui::EndTabItem();
            }
            
            // ----------------------------------------------------------------------------------------------------------------
            // ----------------------------------------------------------------------------------------------------------------
            
            if (ImGui::BeginTabItem("HUD"))
            {
                
                ImGui::Bullet();
                ImGui::Text("GRATICULE");
                ImGui::Checkbox("Draw graticule",               &button_graticule); ImGui::SameLine();
                ImGui::Checkbox("Draw IQ",                      &button_IQ);
                if(view_mode3D)
                {
                    ImGui::SameLine();
                    ImGui::Checkbox("Draw BOX",                 &button_box3dD);
                }
                
                if(button_graticule)
                {
                    ImGui::ColorEdit3("Graticule color",        custom_graticule_color);
                    ImGui::SliderFloat("Graticule gain",        &_h_col,            0,      255     );
                }
                
                if(button_IQ)
                {
                    ImGui::SliderFloat("Gain IQ",               &_h_col_IQ,         0,      255     );
                }
                
                if(view_mode3D){
                    if(button_box3dD)
                    {
                        ImGui::SliderFloat("Gain Box",          &_h_col_box3d,      0,      255     );
                    }
                }
                
                ImGui::Spacing();

                ImGui::Checkbox("Custom Line",                  &h_LINES_CU_b);
                if(h_LINES_CU_b){
                    ImGui::SliderFloat("Line angle",            &h_LINES_CU_v,     -180,    180     );
                    ImGui::SliderFloat("Line length",           &h_LINES_CU_f,      0.01,   1.5     );
                    if(view_mode3D){
                        ImGui::SliderFloat("Line depth",        &h_LINES_CU_vz,    -1,      1       );
                    }
                }
                
                ImGui::Separator(); // ####################################################
                
                ImGui::Bullet();
                ImGui::Text("TARGETS");
                ImGui::Checkbox("Target color from primaries",  &button_tint_targets);
                
                if(!button_tint_targets)
                {
                    ImGui::ColorEdit3("Target color",           custom_target_color);
                }
                ImGui::SliderFloat("Target gain",               &target_gain,       1,      100.0f  );
                ImGui::SliderFloat("Target size",               &target_size,       1,      100.0f  );
                ImGui::SliderFloat("Target distance %",         &target_Factor,     0,      100.0f  );
                
                ImGui::Spacing();
                
                ImGui::Checkbox("Min/Max Pixel target",         &MinMax_pixel_button);
                if(MinMax_pixel_button)
                {
                    ImGui::SameLine();
                    ImGui::Checkbox("Color from sample",        &tint_target_minMax);
                    if(ImGui::IsItemHovered())
                    {
                        ImGui::SetTooltip("Get color to draw custom target from min/max sampled pixel\nThis will allow you to identify the max/min R+G+B pixel in the image"); // text over the mouse
                    }
                    ImGui::SliderFloat("Min/Max target radius", &c_col_radMinMax,    0.1,    50        );
                }
                
                ImGui::Spacing();
                    
                ImGui::Checkbox("Custom Target",                &c_target_col_b);
                if(ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip("Draw a targets with custom color."); // text over the mouse
                }
                if(c_target_col_b){
                    ImGui::SameLine();
                    ImGui::Checkbox("tint target",              &tint_target);
                    if (m_button) {
                        if(!view_mode3D){
                            ImGui::SameLine();
                            ImGui::Checkbox("from sampler",     &cstm_from_sourceMag);
                            if(ImGui::IsItemHovered())
                            {
                                ImGui::SetTooltip("Get color to draw custom target from magnifier sampled pixel"); // text over the mouse
                            }
                        }
                    }
    
                    ImGui::SliderFloat("Marker radius",         &c_target_col_rad,         0.1,    50              );
                    ImGui::ColorPicker3("Color",                c_target_col,   ImGuiColorEditFlags_HSV             |
                                                                                ImGuiColorEditFlags_PickerHueWheel  |
                                                                                ImGuiColorEditFlags_DisplayRGB      |
                                                                                ImGuiColorEditFlags_DisplayHex      |
                                                                                ImGuiColorEditFlags_DisplayHSV      |
                                                                                ImGuiColorEditFlags_NoAlpha         |
                                                                                ImGuiColorEditFlags_NoTooltip);
                }
                
                ImGui::Spacing();
                
                ImGui::EndTabItem();
            }
            
        ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

void VectorScope::resetControls()
{
    
//    _pos[0]             = 0.0f;
//    _pos[1]             = 0.0f;
//    _pos[2]             = 0.0f;
    
    _alpha                          = 1.0f;
    
    _precision                      = 3;
    
    _scale                          = 1;
    _rotate                         = 0;
    _rotate3D[0]                    = 0.0f;
    _rotate3D[1]                    = 0.0f;
    _rotate3D[2]                    = 0.0f;
    
    _erode                          = 0;
    
    _channel                        = 3;
    
    _gain                           = 1;
    
    _h_col                          = 100;
    _h_col_IQ                       = 100;
    _h_col_box3d                    = 64;
    
    target_size                     = 30;
    target_gain                     = 100.0f;
    target_Factor                   = 75.0f;
    
    _h_width                        = 2;

    m_factor                        = 9;

    _source_scale                   = 1;
    
    cumulativeOffset.x              = 0;
    cumulativeOffset.y              = 0;
    MouseScale                      = 1.0f;
    
    CropRec_V[0]                    = 0;
    CropRec_V[1]                    = 0;
    CropRec_V[2]                    = s_W;
    CropRec_V[3]                    = s_W;
    
    Col_threshold[0]                = 0;
    Col_threshold[1]                = 255;
    
    _padding                        = 0;
    
    h_LINES_CU_v                    = 33.0f;
    h_LINES_CU_vz                   = 0.0f;
    h_LINES_CU_f                    = 1.0f;
    
    c_target_col[0]                        = 0;
    c_target_col[1]                        = 0.5;
    c_target_col[2]                        = 1;
    
    c_target_col_rad                       = 15;
    
    custom_pixel_color[0]           = 1.0f;
    custom_pixel_color[1]           = 1.0f;
    custom_pixel_color[2]           = 1.0f;
    
    custom_graticule_color[0]       = 1.0f;
    custom_graticule_color[1]       = 1.0f;
    custom_graticule_color[2]       = 1.0f;

    custom_target_color[0]          = 1.0f;
    custom_target_color[1]          = 1.0f;
    custom_target_color[2]          = 1.0f;
    c_col_radMinMax                 = 20;
    
    
    
}

void VectorScope::draw_translate_reset()
{
    cumulativeOffset.x = 0.0f;
    cumulativeOffset.y = 0.0f;
    MouseScale = 1.0f;
}

void VectorScope::draw_translate()
{
    if(!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
    {
        currentMousePos = ImGui::GetMousePos();
        
        // Check if the mouse is pressed
        if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
            // Calculate the offset based on the change in mouse position
            ImVec2 offset = ImVec2(currentMousePos.x - lastMousePos.x, currentMousePos.y - lastMousePos.y);
            
            // Accumulate translation offset during dragging
            cumulativeOffset.x += offset.x;
            cumulativeOffset.y += offset.y;
            
        }else if(ImGui::GetIO().MouseWheel != 0.0f) {
            // Adjust the scale factor based on the mouse wheel delta // multiplier added so its not using intiger values
            MouseScale += ImGui::GetIO().MouseWheel * 0.01f;
            
            // Min max scale
            MouseScale = ofClamp(MouseScale, 0.1f, 5.0f);
        }
        
        // Apply cumulative translation
        ofTranslate(cumulativeOffset.x, cumulativeOffset.y);
        lastMousePos = currentMousePos;
        
        // Apply cumulative scale
        ofTranslate(ofGetWindowWidth()/2, ofGetWindowHeight()/2);
        ofScale(MouseScale);
        ofTranslate(-ofGetWindowWidth()/2, -ofGetWindowHeight()/2);
        
    }else{
        // keep values without change
        ofTranslate(cumulativeOffset.x, cumulativeOffset.y);
        
        ofTranslate(ofGetWindowWidth()/2, ofGetWindowHeight()/2);
        ofScale(MouseScale);
        ofTranslate(-ofGetWindowWidth()/2, -ofGetWindowHeight()/2);
    }
}
