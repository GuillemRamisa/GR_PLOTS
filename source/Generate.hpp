//
//  Generate.hpp
//  GR_PLOTS_100
//
//  Created by Guillem Ramisa de Soto on 18/3/24.
//

#pragma once

#ifndef Generate_hpp
#define Generate_hpp

#include <stdio.h>
#include "ofMain.h"


/**
* Draw pixel on screen.
*
* @param x posX
* @param y posY
* @param r Red val
* @param g Green val
* @param b Blue val
* @param a Alpha val
* @param w_W window Width
* @param w_H window Height
* @return void draw
*/
void draw_RGBA_pixel(int x, int y, int r, int g, int b, int a, int w_W, int w_H);

/**
* Draw RGBA grade.
*
* @param channels display channels
* @param w_W window Width
* @param w_H window Height
* @param draw bool if draw or only load and store
* @return ofImage
*/
ofImage draw_RGBA_GRAD(float channels[3], int w_W, int w_H, bool draw);

/**
 * Draw RGBA grade V2
*
* @param channels display channels
* @param w_W window Width
* @param w_H window Height
* @param draw bool if draw or only load and store
* @return ofImage
*/
ofImage draw_RGBA_GRAD_W(float channels[3], int w_W, int w_H, bool draw);

/**
 * Draw "CMS" color cube style.
*
* @param channels display channels
* @param w_W window Width
* @param w_H window Height
* @param draw bool if draw or only load and store
* @return ofImage
*/
ofImage draw_RGBA_CUBE(float channels[3], int w_W, int w_H, bool draw);

/**
 * Draw color bars.
*
* @param channels display channels
* @param w_W window Width
* @param w_H window Height
* @param draw bool if draw or only load and store
* @return ofImage
*/
ofImage draw_RGBA_BARS(float channels[3], int w_W, int w_H, bool draw);

/**
 * Draw macbeth color chart 2016.
*
* @param channels display channels
* @param w_W window Width
* @param w_H window Height
* @param draw bool if draw or only load and store
* @return ofImage
*/
ofImage draw_RGBA_MACB_2016(float channels[3], int w_W, int w_H, bool draw);

/**
* Draw macbeth color chart classic
*
* @param channels display channels
* @param w_W window Width
* @param w_H window Height
* @param draw bool if draw or only load and store
* @return ofImage
*/
ofImage draw_RGBA_MACB_CLASSIC(float channels[3], int w_W, int w_H, bool draw);






#endif /* Generate_hpp */
