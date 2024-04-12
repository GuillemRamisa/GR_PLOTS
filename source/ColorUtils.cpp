//
//  ColorUtils.cpp
//  GR_PLOTS_100
//
//  Created by Guillem Ramisa de Soto on 21/2/24.
//

#include "ColorUtils.hpp"


// -------------------- RED ------------------------------
void RGB_to_RRR(ofImage & ofimage)
{
    ofimage.setFromPixels(ofimage.getPixels().getChannel(0));
}

void RGB_to_RRR(ofPixels & ofpixels)
{
    ofpixels.getChannel(0);
}

void RGB_to_RRR(ofVec3f & vec3)
{
    vec3 = {vec3[0], vec3[0], vec3[0]};
}

// -------------------- GREEN ------------------------------
void RGB_to_GGG(ofImage & ofimage)
{
    ofimage.setFromPixels(ofimage.getPixels().getChannel(1));
}

void RGB_to_GGG(ofPixels & ofpixels)
{
    ofpixels.getChannel(1);
}

void RGB_to_GGG(ofVec3f & vec3)
{
    vec3 = {vec3[1], vec3[1], vec3[1]};
}


// -------------------- BLUE ------------------------------
void RGB_to_BBB(ofImage & ofimage)
{
    ofimage.setFromPixels(ofimage.getPixels().getChannel(2));
}

void RGB_to_BBB(ofPixels & ofpixels)
{
    ofpixels.getChannel(2);
}

void RGB_to_BBB(ofVec3f & vec3)
{
    vec3 = {vec3[2], vec3[2], vec3[2]};
}

/**
* /// OF_BLENDMODE_DISABLED        = 0,
* /// OF_BLENDMODE_ALPHA              = 1,
* /// OF_BLENDMODE_ADD                  = 2,
* /// OF_BLENDMODE_SUBTRACT      = 3,
* /// OF_BLENDMODE_MULTIPLY        = 4,
* /// OF_BLENDMODE_SCREEN          = 5
*/
ofImage multiplyImage1_factor255(ofImage& ofimage, float mult_factor, int channel, ofBlendMode blendMode) {
    
    ofimage.setFromPixels(ofimage.getPixels().getChannel(channel));
    ofEnableAlphaBlending();
    ofBlendMode(ofBlendMode);
    ofSetColor(mult_factor);
    return ofimage;
}

void multiplyImage3_factor255(ofImage& ofimage, float mult_factor[3], ofBlendMode blendMode) {
    
    ofEnableAlphaBlending();
    ofBlendMode(ofBlendMode);
    ofSetColor(mult_factor[0], mult_factor[2], mult_factor[2]);
//    return ofimage;
}

ofImage add_3_IMAGES(ofImage& ofimage1, ofImage& ofimage2, ofImage& ofimage3){
    
    ofImage resultImage;
    
    // Use OpenFrameworks pixel operations to add channels without a for loop
    ofPixels& pixels1 = ofimage1.getPixels();
    ofPixels& pixels2 = ofimage2.getPixels();
    ofPixels& pixels3 = ofimage3.getPixels();
    ofPixels resultPixels;// = resultImage.getPixels();
    resultPixels.allocate(ofimage1.getWidth(), ofimage1.getHeight(), OF_IMAGE_COLOR);

    resultPixels.setChannel(0, pixels1.getChannel(0));
    resultPixels.setChannel(1, pixels2.getChannel(0));
    resultPixels.setChannel(2, pixels3.getChannel(0));
    
    resultImage.setFromPixels(resultPixels);
//    resultImage.update();
    
    return resultImage;
}

// generic translator COLOR MATRIX
void IMAGE_COLOR_MATRIX_RGB(ofImage& _rgbImage, COLOR_TRANSFORM color_transform) {

    // Allocation of the image in case its not
    _rgbImage.allocate(_rgbImage.getWidth(), _rgbImage.getHeight(), _rgbImage.getImageType());
    
    // get ref of pixels from image
    ofPixels& rgbPixels = _rgbImage.getPixels();
    
    // create matrix struct
    color_Matrix3x3 mat;
    // define the matrix as a choice inisde the struct initialized using the id from the enum
    ofMatrix3x3& matrix_transform = mat.matriXXX[color_transform];
    
    // double loop to iterate image
    for (int y = 0; y < _rgbImage.getHeight(); y++) {
        for (int x = 0; x < _rgbImage.getWidth(); x++) {
            
            // Get RGB values per channel per pixel
            float r = _rgbImage.getColor(x, y).r;
            float g = _rgbImage.getColor(x, y).g;
            float b = _rgbImage.getColor(x, y).b;
            
            float xxx;
            float yyy;
            float zzz;
            
            if (color_transform == RGB_YCbCr_601)
            {
                // YCbCr 601
                xxx = 16.0f  + ( (matrix_transform.a * r)/255 ) + ( (matrix_transform.b * g)/255 ) + ( (matrix_transform.c * b)/255 );
                yyy = 128.0f - ( (matrix_transform.d * r)/255 ) - ( (matrix_transform.e * g)/255 ) + ( (matrix_transform.f * b)/255 );
                zzz = 128.0f + ( (matrix_transform.g * r)/255 ) - ( (matrix_transform.h * g)/255 ) - ( (matrix_transform.i * b)/255 );
                
                rgbPixels.setColor(x, y, ofColor(xxx, yyy, zzz, 255));
                
            }else if (color_transform == RGB_YCbCr_709)
            {
                // YCbCr 709
                xxx = ( matrix_transform.a * (r/255)) + (matrix_transform.b * (g/255)) + (matrix_transform.c * (b/255)) ;
                yyy = (-matrix_transform.d * (r/255)) - (matrix_transform.e * (g/255)) + (matrix_transform.f * (b/255)) + 0.5f;
                zzz = ( matrix_transform.g * (r/255)) - (matrix_transform.h * (g/255)) - (matrix_transform.i * (b/255)) + 0.5f;

                rgbPixels.setColor(x, y, ofColor(xxx * 255, yyy * 255, zzz * 255, 255));
                
            }
            
        }
    }
    _rgbImage.update();
}

// This function needs some check on conversion from 8/10/12 bit images
ofVec3f COLOR_MATRIX_PRIMS(ofVec3f xyz, COLOR_TRANSFORM color_transform){
    
    // create matrix struct
    color_Matrix3x3 mat;
    // define the matrix as a choice inisde the struct initialized using the id from the enum
    ofMatrix3x3& matrix_transform = mat.matriXXX[color_transform];
    
    ofVec3f _xyz;
    
    // Perform matrix multiplication for YCbCr conversion
    if (color_transform == RGB_YCbCr_601)
    {
        // YCbCr 601
        _xyz.x = 16.0f  + ( (matrix_transform.a * xyz.x)/255 ) + ( (matrix_transform.b * xyz.y)/255 ) + ( (matrix_transform.c * xyz.z)/255 );
        _xyz.y = 128.0f - ( (matrix_transform.d * xyz.x)/255 ) - ( (matrix_transform.e * xyz.y)/255 ) + ( (matrix_transform.f * xyz.z)/255 );
        _xyz.z = 128.0f + ( (matrix_transform.g * xyz.x)/255 ) - ( (matrix_transform.h * xyz.y)/255 ) - ( (matrix_transform.i * xyz.z)/255 );
        
        xyz = ofVec3f(_xyz.x, _xyz.y, _xyz.z);
        
    }else if (color_transform == RGB_YCbCr_709)
    {
        // YCbCr 709
        _xyz.x = ( matrix_transform.a * (xyz.x/255)) + (matrix_transform.b * (xyz.y/255)) + (matrix_transform.c * (xyz.z/255)) ;
        _xyz.y = (-matrix_transform.d * (xyz.x/255)) - (matrix_transform.e * (xyz.y/255)) + (matrix_transform.f * (xyz.z/255)) + 0.5f;
        _xyz.z = ( matrix_transform.g * (xyz.x/255)) - (matrix_transform.h * (xyz.y/255)) - (matrix_transform.i * (xyz.z/255)) + 0.5f;
        
        xyz = ofVec3f(_xyz.x * 255, _xyz.y * 255, _xyz.z * 255);
        
    }
    else if(color_transform == RGB_YCbCr_2020){
        // YCbCr 2020
        _xyz.x = ( matrix_transform.a * (xyz.x/255)) + (matrix_transform.b * (xyz.y/255)) + (matrix_transform.c * (xyz.z/255)) ;
        _xyz.y = (-matrix_transform.d * (xyz.x/255)) - (matrix_transform.e * (xyz.y/255)) + (matrix_transform.f * (xyz.z/255)) + 0.5f;
        _xyz.z = ( matrix_transform.g * (xyz.x/255)) - (matrix_transform.h * (xyz.y/255)) - (matrix_transform.i * (xyz.z/255)) + 0.5f;

        xyz = ofVec3f(_xyz.x * 255, _xyz.y * 255, _xyz.z * 255);
    }

    return xyz;
}

// IMAGe COLOR conversions
void RGB_to_YCbCr_BT601(ofImage& ofimage){

    IMAGE_COLOR_MATRIX_RGB(ofimage, RGB_YCbCr_601);
}

void RGB_to_YCbCr_BT709(ofImage& ofimage){

    IMAGE_COLOR_MATRIX_RGB(ofimage, RGB_YCbCr_709);
}


ofVec2f PSR_2D(ofVec2f _x_y, float pos_x, float pos_y, float scale_x, float scale_y, float rotation){
    // SRT
    
    // S
    _x_y.x *= scale_x;
    _x_y.y *= scale_y;

    // R
    float Radiants = rotation * (_pi/180.0f);
    
    ofVec2f x_y_;
    x_y_.x = _x_y.x * cos(Radiants) - _x_y.y * sin(Radiants);
    x_y_.y = _x_y.x * sin(Radiants) + _x_y.y * cos(Radiants);

    // T
    x_y_.x += pos_x;
    x_y_.y += pos_y;
    
    return x_y_;
}

ofVec3f ROT_3D(ofVec3f _x_y_z, ofVec3f rotation){
    
    float pi = 3.1415926535897932384626433832795f;
    
    ofVec3f R(rotation.x * (pi/180.0f),
              rotation.y * (pi/180.0f),
              rotation.z * (pi/180.0f)
              );
    
    // ////////////// order of rotation ZXY
    // //  Z ROT  // //
    ofVec3f ZX(
               _x_y_z.x *  cos(R.z),
               _x_y_z.y * -sin(R.z),
               0.0f);
    
    ofVec3f ZY( _x_y_z.x * sin(R.z),
                _x_y_z.y * cos(R.z),
                0.0f);
    
    ofVec3f ZZ( 0.0f,
                0.0f,
                _x_y_z.z);
    // Z sum
    ofVec3f ZR( (ZX.x + ZX.y + ZX.z),
                (ZY.x + ZY.y + ZY.z),
                (ZZ.x + ZZ.y + ZZ.z)
                );
    
    
     // ///  X ROT  // //
    ofVec3f XX( ZR.x,
                0.0f,
                0.0f);
    
    ofVec3f XY( 0.0f ,
                ZR.y *  cos(R.x) ,
                ZR.z * -sin(R.x) );
    
    ofVec3f XZ( 0.0f,
                ZR.y * sin(R.x) ,
                ZR.z * cos(R.x) );
     // X sum
    ofVec3f XR( (XX.x + XX.y + XX.z),
                (XY.x + XY.y + XY.z),
                (XZ.x + XZ.y + XZ.z)
               );
    
    
     // //  Y ROT  // //
    ofVec3f YX( XR.x * cos(R.y) ,
                0.0f,
                XR.z * sin(R.y) );
    
    ofVec3f YY( 0.0f,
                XR.y,
                0.0f);
    
    ofVec3f YZ( XR.x * -sin(R.y) ,
                0.0f,
                XR.z *  cos(R.y) );
    
    // Y sum
    ofVec3f _ROTATION(  (YX.x + YX.y + YX.z),
                        (YY.x + YY.y + YY.z),
                        (YZ.x + YZ.y + YZ.z)
                      );
    
     return _ROTATION;
}

ofColor XYZ_threshold(ofColor _xyz, int _threshold[2], float alpha_mult){
    
    ofColor xyz_;
//    float avg = ( ( _xyz.r + _xyz.g + _xyz.b ) / 3.0f );
    
    float RR = _xyz.r;
    float GG = _xyz.g;
    float BB = _xyz.b;
    float AA = _xyz.a * alpha_mult;
    
    (_xyz.r < _threshold[0] || _xyz.r > _threshold[1] || _xyz.g < _threshold[0] || _xyz.g > _threshold[1] || _xyz.b < _threshold[0] || _xyz.b > _threshold[1])
    ? xyz_ = ofColor(0.0f,0.0f,0.0f,0.0f) : xyz_= ofColor(RR,GG,BB,AA);
    
    return xyz_;
}
