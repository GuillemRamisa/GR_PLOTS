//
//  ColorUtils.hpp
//  GR_PLOTS_100
//
//  Created by Guillem Ramisa de Soto on 21/2/24.
//

#pragma once

#ifndef ColorUtils_hpp
#define ColorUtils_hpp

#include <stdio.h>
#include <ofMain.h>

#define _pi 3.1415926535897932384626433832795f

// constants

// enum defining index of color prims
// here we define a last elemnt used to track the end of the list
enum COLOR_PRIMS
{
    RED         = 0,
    GREEN       = 1,
    BLUE        = 2,
    CYAN        = 3,
    MAGENTA     = 4,
    YELLOW      = 5,
//    BLACK       = 6,
//    WHITE       = 7,
    NUM_COLORS
};

// struct of prims colors
struct prims{
    
    // declaration of type of prim and assignation on how many
    // the last element of the enum class is used to specify the number of elements
    // note that they need to match
    array<int, 3> PRIM[NUM_COLORS];
    
    // constructor of struct with defined type
    // index is based on enum colors
    prims()
    {
        PRIM[RED]       = { 255,     0,      0};
        PRIM[GREEN]     = { 0,       255,    0};
        PRIM[BLUE]      = { 0,       0,      255};

        PRIM[CYAN]      = { 0,       255,    255};
        PRIM[MAGENTA]   = { 255,     0,      255};
        PRIM[YELLOW]    = { 255,     255,    0};
    }
};

/**
* Planar transformation 2D Position - Scale - Rotation.
*
* @param ofVec2f source vector value to be rotated
* @param pos_x position offset X
* @param pos_y position offset Y
* @param scale_x Scale value in X
* @param scale_y Scale value in Y
* @param rotation rotation value in ANGLES
* @return ofVec2f
* @note the axis of rotation is set from the 'center = 0,0'
* @note Actually is SRT not PSR
*/
ofVec2f PSR_2D(ofVec2f _x_y, float pos_x, float pos_y, float scale_x, float scale_y, float rotation);

/**
* 3D Rotation.
*
* @param ofVec3f source vector value to be rotated
* @param ofVec3f rotation vector
* @return ofVec3f
* @note the axis of rotation is set from the 'center = 0,0'
*/
ofVec3f ROT_3D(ofVec3f _x_y_z, ofVec3f rotation);



/**
* color transformation from RGB to RRR.
*
* @param ofimage this gets a ofImage as input and resturns an ofImage.
* @return ofImage
* @note returns the same ofImage, but: ofImage -> ofPixel[0] -> ofImage
*/
void RGB_to_RRR(ofImage& ofimage);

/**
* color transformation from RGB to RRR.
*
* @param ofimage this gets a oPixels as input and resturns an ofImage.
* @return ofImage
* @note returns the same ofImage, but: ofImage -> ofPixel[0] -> ofImage
*/
void RGB_to_RRR(ofPixels& ofpixels);

/**
* color transformation from RGB to RRR.
*
* @param ofimage this gets a ofVec3f as input and resturns an ofImage.
* @return ofImage
* @note returns the same ofImage, but: ofImage -> ofPixel[0] -> ofImage
*/
void RGB_to_RRR(ofVec3f& vec3);



void RGB_to_GGG(ofImage& ofimage);
void RGB_to_GGG(ofPixels& ofpixels);
void RGB_to_GGG(ofVec3f& vec3);

void RGB_to_BBB(ofImage& ofimage);
void RGB_to_BBB(ofPixels& ofpixels);
void RGB_to_BBB(ofVec3f& vec3);


// color conversions BT709
void RGB_to_YCbCr_BT709(ofImage& ofimage);
void YCbCr_to_RGB_BT709(ofImage& ofimage);

// color conversions BT601
void RGB_to_YCbCr_BT601(ofImage& ofimage);
void YCbCr_to_RGB_BT601(ofImage& ofimage);


// Enum pair of color transform -> matrixID
enum COLOR_TRANSFORM
{
    RGB_YCbCr_709   = 0,
    RGB_YCbCr_601   = 1,
    RGB_YCbCr_2020  = 2,
    RGB_YCbCr_COUNT,
    
};

void IMAGE_COLOR_MATRIX_RGB(ofImage& _rgbImage, COLOR_TRANSFORM color_transform);

ofVec3f COLOR_MATRIX_PRIMS(ofVec3f xyz, COLOR_TRANSFORM color_transform);

/**
* color matrix transforms from RGB to XXX
*
* @return ofMatrix3x3 with its components
*
* @note To initialize: color_Matrix3x3 mat;
* @note mat.matrices[RGB_YCbCr_709t].a; ---> 0.2126f
**/
struct color_Matrix3x3{
    
    // list of matrix objects
    // note that this number needs to match the elements in the enum list used!
    ofMatrix3x3 matriXXX[RGB_YCbCr_COUNT];
    
    // initialize the color matrices by enum element
    color_Matrix3x3()
    {
        matriXXX[RGB_YCbCr_709].set(0.2126f, 0.7152f, 0.0722f, 0.1146f, 0.3854f, 0.5f, 0.5f, 0.4542f, 0.0458);
        matriXXX[RGB_YCbCr_601].set(65.481f, 128.553f, 24.966f, 37.797f, 74.203f, 112.0f, 112.0f, 93.788f, 18.214f);
        matriXXX[RGB_YCbCr_2020].set(0.2627f, 0.678f, 0.0593f, 0.13963f, 0.36037f, 0.5f, 0.5f, 0.45979f, 0.04021f);
    }
    
    
};



ofColor XYZ_threshold(ofColor _xyz, int threshold[2],  float alpha_mult);


#endif /* ColorUtils_hpp */
