#pragma once

#include "ofMain.h"
//#include "ofxImGui.h"

// My gui theme
#include <GUI_theme.hpp>
#include <VectorScope.hpp>

class ofApp : public ofBaseApp{
    
public:
    void setup() override;
    void update() override;
    void draw() override;
    void exit() override;
    
    void keyPressed(int key) override;
    void keyReleased(int key) override;
    void mouseMoved(int x, int y ) override;
    void mouseDragged(int x, int y, int button) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
    void mouseEntered(int x, int y) override;
    void mouseExited(int x, int y) override;
    void windowResized(int w, int h) override;
    void dragEvent(ofDragInfo dragInfo) override;
    void gotMessage(ofMessage msg) override;
    
private:
    //------------------------------------------------------------
    // Welcome
    int w_Limit_L;
    int w_Limit_T;
    
    //------------------------------------------------------------
    // Objects
    const char*             vectorscope_name;
    VectorScope             vectorscope;
    
    //------------------------------------------------------------
    // vectorscope draw function
    void draw_Welcome();
    void draw_VectorScope();
    void draw_VectorScope3D();
    
    //------------------------------------------------------------
    // Open Frameworks Image/video temp Objects
    ofImage                 t_image;
    ofImage                 t_image_select();
    
    int                     t_image_index = 0;
    bool                    t_image_button = false;
    bool                    t_image_track_key;

    //------------------------------------------------------------
    // Open Frameworks objects
    ofxImGui::Gui           gui;
    ofEasyCam               cam;
    bool                    cam_orto        = true;
    bool                    is_image        = true;
    
    //------------------------------------------------------------
    // Open Frameworks Image Objects<vectors>
    vector<ofImage>         _image;
    vector<ofImage>        o_image;
//    vector<ofVideoPlayer>   _video;
    
    // tracking vector for new files
    vector<string>          draggedFiles;
    
    //------------------------------------------------------------
    // save screenshot
    void save_image();
    ofImage                 image_save_image;
    int                     image_save_count;
    bool                    image_save_state;
    
    //------------------------------------------------------------
    // Generate dynamic objects
    int                     w_W;
    int                     w_H;
    const int               box_base = 512; //not dynamic ...
    
};
