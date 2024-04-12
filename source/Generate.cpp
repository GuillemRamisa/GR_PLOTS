//
//  Generate.cpp
//  GR_PLOTS_100
//
//  Created by Guillem Ramisa de Soto on 18/3/24.
//

#include "Generate.hpp"

#include <iostream>

// data structures standins for rendering
ofTexture       t;
ofFbo           t_fbo;
ofShader        shader;

ofImage         t_image;
ofPixels        t_p;

void draw_RGBA_pixel(int x, int y, int r, int g, int b, int a, int w_W, int w_H){
    // dynamic allocation of data
    t.allocate(w_W, w_H, GL_RGBA);
    t_p.allocate(w_W, w_H, GL_RGBA);
    
    t_p.setColor(x, y, ofColor(r,g,b,a));
    
    t.loadData(t_p);
    
    t.draw(0,0, w_W, w_H);
}


ofImage draw_RGBA_GRAD(float channels[3], int w_W, int w_H, bool draw){
    
    
    // dynamic allocation of data ... quite a journey lol...
    t_fbo.allocate(w_W, w_H, GL_RGBA);
    t.allocate(w_W, w_H, GL_RGBA);
    t_p.allocate(w_W, w_H, GL_RGBA);
    t_image.allocate(w_W, w_H, OF_IMAGE_COLOR_ALPHA);

    shader.load( "planar_shader_vert_base.vert", "RGB_GRAD.frag");
    
    // framebuffer start
    t_fbo.begin();
    
    // shader start
    shader.begin();

    // uniform data passed to the shader
    shader.setUniform2f("resolution", w_W, w_H);
    shader.setUniform3f("cha", channels[0], channels[1], channels[2]);
    
    // standin object to allocate the shader projection
    ofDrawRectangle(0,ofGetWindowHeight()-w_H, w_W, w_H);

    // shader start
    shader.end();
    
    // framebuffer end
    t_fbo.end();
    
    // texture data grab
    t = t_fbo.getTexture();
    
    // pixles texture grab
    t.readToPixels(t_p);
    
    // image pixels grab
    t_image.setFromPixels(t_p);
    
    if (draw) {
        t_image.draw(0,0);
    }
    return t_image;
    
}


ofImage draw_RGBA_GRAD_W(float channels[3], int w_W, int w_H, bool draw){
    
    
    // dynamic allocation of data ... quite a journey lol...
    t_fbo.allocate(w_W, w_H, GL_RGBA);
    t.allocate(w_W, w_H, GL_RGBA);
    t_p.allocate(w_W, w_H, GL_RGBA);
    t_image.allocate(w_W, w_H, OF_IMAGE_COLOR_ALPHA);

    shader.load( "planar_shader_vert_base.vert", "RGB_GRAD_W.frag");

//    shader.setupShaderFromFile(GL_VERTEX_SHADER, "planar_shader_vert_base.vert");
//    shader.setupShaderFromFile(GL_FRAGMENT_SHADER,"RGB_GRAD_W.frag");
//    shader.linkProgram();
    
    
    // framebuffer start
    t_fbo.begin();
    
    // shader start
    shader.begin();

    // uniform data passed to the shader
    shader.setUniform2f("resolution", w_W, w_H);
    shader.setUniform2f("MP", ofGetMouseX(), ofGetMouseY());
    
    // standin object to allocate the shader projection
    ofDrawRectangle(0,ofGetWindowHeight()-w_H, w_W, w_H);

    // shader start
    shader.end();
    
    // framebuffer end
    t_fbo.end();
    
    // texture data grab
    t = t_fbo.getTexture();
    
    // pixles texture grab
    t.readToPixels(t_p);
    
    // image pixels grab
    t_image.setFromPixels(t_p);
    
    if (draw) {
        t_image.draw(0,0);
    }
    return t_image;
    
}


ofImage draw_RGBA_CUBE(float channels[3], int w_W, int w_H, bool draw){
    
    int fac = 3;
    
    w_W = w_W / fac;
    w_H = w_H / fac;
    
    // dynamic allocation of data ... quite a journey lol...
    t_fbo.allocate(w_W, w_H, GL_RGBA);
    t.allocate(w_W, w_H, GL_RGBA);
    t_p.allocate(w_W, w_H, GL_RGBA);
    t_image.allocate(w_W, w_H, OF_IMAGE_COLOR_ALPHA);

    shader.load( "planar_shader_vert_base.vert", "RGB_CUBE.frag");
    
    // framebuffer start
    t_fbo.begin();
    
    // shader start
    shader.begin();

    // uniform data passed to the shader
    shader.setUniform2f("resolution", w_W, w_H);
    shader.setUniform3f("cha", channels[0], channels[1], channels[2]);
    
    // standin object to allocate the shader projection
    ofDrawRectangle(0,(ofGetWindowHeight()/fac)-w_H, w_W, w_H);

    // shader start
    shader.end();
    
    // framebuffer end
    t_fbo.end();
    
    // texture data grab
    t = t_fbo.getTexture();
    
    // pixles texture grab
    t.readToPixels(t_p);
    
    // image pixels grab
    t_image.setFromPixels(t_p);
    
    if (draw) {
        t_image.draw(0,0);
    }
    return t_image;
    
}

ofImage draw_RGBA_BARS(float channels[3], int w_W, int w_H, bool draw){
    
    int fac = 3;
    
    w_W = w_W / fac;
    w_H = w_H / fac;
    
    // dynamic allocation of data ... quite a journey lol...
    t_fbo.allocate(w_W, w_H, GL_RGBA);
    t.allocate(w_W, w_H, GL_RGBA);
    t_p.allocate(w_W, w_H, GL_RGBA);
    t_image.allocate(w_W, w_H, OF_IMAGE_COLOR_ALPHA);

    shader.load( "planar_shader_vert_base.vert", "RGB_BARS.frag");
    
    // framebuffer start
    t_fbo.begin();
    
    // shader start
    shader.begin();

    // uniform data passed to the shader
    shader.setUniform2f("resolution", w_W, w_H);
    shader.setUniform3f("cha", channels[0], channels[1], channels[2]);
    
    // standin object to allocate the shader projection
    ofDrawRectangle(0,(ofGetWindowHeight()/fac)-w_H, w_W, w_H);

    // shader start
    shader.end();
    
    // framebuffer end
    t_fbo.end();
    
    // texture data grab
    t = t_fbo.getTexture();
    
    // pixles texture grab
    t.readToPixels(t_p);
    
    // image pixels grab
    t_image.setFromPixels(t_p);
    
    if (draw) {
        t_image.draw(0,0);
    }
    return t_image;
    
}

ofImage draw_RGBA_MACB_2016(float channels[3], int w_W, int w_H, bool draw){
    
    int fac = 1;
    
    w_W = w_W / fac;
    w_H = w_H / fac;
    
    // dynamic allocation of data ... quite a journey lol...
    t_fbo.allocate(w_W, w_H, GL_RGBA);
    t.allocate(w_W, w_H, GL_RGBA);
    t_p.allocate(w_W, w_H, GL_RGBA);
    t_image.allocate(w_W, w_H, OF_IMAGE_COLOR_ALPHA);

    shader.load( "planar_shader_vert_base.vert", "RGB_MACB.frag");
    
    // framebuffer start
    t_fbo.begin();
    
    // shader start
    shader.begin();

    // uniform data passed to the shader
    shader.setUniform2f("resolution", w_W, w_H);
    shader.setUniform3f("cha", channels[0], channels[1], channels[2]);
    
    // standin object to allocate the shader projection
    ofDrawRectangle(0,(ofGetWindowHeight()/fac)-w_H, w_W, w_H);

    // shader start
    shader.end();
    
    // framebuffer end
    t_fbo.end();
    
    // texture data grab
    t = t_fbo.getTexture();
    
    // pixles texture grab
    t.readToPixels(t_p);
    
    // image pixels grab
    t_image.setFromPixels(t_p);
    
    if (draw) {
        t_image.draw(0,0);
    }
    return t_image;
    
}

ofImage draw_RGBA_MACB_CLASSIC(float channels[3], int w_W, int w_H, bool draw){
    
    int fac = 3;
    
    w_W = w_W / fac;
    w_H = w_H / fac;
    
    // dynamic allocation of data ... quite a journey lol...
    t_fbo.allocate(w_W, w_H, GL_RGBA);
    t.allocate(w_W, w_H, GL_RGBA);
    t_p.allocate(w_W, w_H, GL_RGBA);
    t_image.allocate(w_W, w_H, OF_IMAGE_COLOR_ALPHA);

    shader.load( "planar_shader_vert_base.vert", "RGB_MACA.frag");
    
    // framebuffer start
    t_fbo.begin();
    
    // shader start
    shader.begin();

    // uniform data passed to the shader
    shader.setUniform2f("resolution", w_W, w_H);
    shader.setUniform3f("cha", channels[0], channels[1], channels[2]);
    
    // standin object to allocate the shader projection
    ofDrawRectangle(0,(ofGetWindowHeight()/fac)-w_H, w_W, w_H);

    // shader start
    shader.end();
    
    // framebuffer end
    t_fbo.end();
    
    // texture data grab
    t = t_fbo.getTexture();
    
    // pixles texture grab
    t.readToPixels(t_p);
    
    // image pixels grab
    t_image.setFromPixels(t_p);
    
    if (draw) {
        t_image.draw(0,0);
    }
    return t_image;
    
}
