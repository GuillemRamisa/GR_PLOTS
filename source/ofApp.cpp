#include "ofApp.h"
#include "VectorScope.hpp"


void ofApp::setup(){
    
// --------------------------------------------------------------------------------------------
//     ofSetLogLevel(OF_LOG_VERBOSE);
//     ofSetFrameRate(60);
// --------------------------------------------------------------------------------------------
    
    // vectorscope UI_name
    vectorscope_name = "vectorscope";
    
    // base image load as a standin
    t_image.allocate(512, 512, OF_IMAGE_COLOR_ALPHA);

    // base required setup for gui
    gui.setup(new GUI_theme(), true);
    
    // setup function to populate initial values of the vectorscope
    // included object constructor
    vectorscope.setup(t_image);
    
    // init setup for 3D cam
    cam.setFov(50);
    cam.setPosition(0, 0, 965);
    cam.setFarClip(10000);
    cam.setNearClip(10);
    
    // this function ensures that the source files used like the images and shaders
    // are taken from the app dir relative to the executable (portable app)
    ofSetDataPathRoot("");
    
}

// --------------------------------------------------------------------------------------------

ofImage ofApp::t_image_select(){
    
    // function to select from fragment shader generation implementation
    // this is used to get initial images to test
    // more shaders are availabe and more can be included
    if (t_image_index == 0) {
        
        t_image = draw_RGBA_CUBE((float[]){1.0f, 1.0f, 1.0f}, w_W, w_H, false);
        
    }else if(t_image_index == 1){
        
        t_image = draw_RGBA_BARS((float[]){1.0f, 1.0f, 1.0f}, w_W, w_H, false);
        
    }else if(t_image_index == 2){
        
        t_image = draw_RGBA_MACB_CLASSIC((float[]){1.0f, 1.0f, 1.0f}, w_W, w_H, false);
        
    }
    return t_image;
}

// --------------------------------------------------------------------------------------------

void ofApp::update(){
    
    //dynamic window sizing
    w_W = ofGetWindowWidth();
    w_H = ofGetWindowHeight();
    
    // update welcome limits
    w_Limit_L = (w_W/6);
    w_Limit_T = (w_H/6);
    
    // we need to clear the list of dragged files, so the 1-1 vector check can be done
    draggedFiles.clear();
    
    // initial image pixel population
    t_image = t_image_select();
    
    // update
    if (_image.size() == 0 ) {

        if(t_image_button){
            
            // image state
            is_image = true;
            
            // update
            vectorscope.update(t_image);
            
        }else if(!t_image_button){
            
            // image state
            is_image = false;
            
            // INITIAL SCREEN!!
            draw_Welcome();
        }
        
        vectorscope.is_loaded_image = false;

    }else{
        
        // image state
        is_image = true;
        
        
        if(t_image_track_key){
            vectorscope.is_loaded_image = false;
        }else{
            vectorscope.is_loaded_image = true;
        }

        for(int i = 0; i < _image.size(); i++){
            
            // update
            vectorscope.update(_image[i]);
            
        }
    }
    
    // update cam state
    if (vectorscope.view_mode3D){
        cam.enableMouseInput();
    }else{
        cam.disableMouseInput();
    }
    
    if(!is_image){
        ofHideCursor();
    }else{
        ofShowCursor();
    }
    
}

// --------------------------------------------------------------------------------------------

void ofApp::draw_Welcome(){
    
    // INITIAL SCREEN SETUP/DRAW
    draw_RGBA_GRAD_W((float[]){1.0f, 1.0f, 1.0f}, w_W, w_H,true);

    // this function enures the usage of of methods to scale, translate and rotate ofDrawBitmapString
    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
    
    ofPushMatrix();
    ofTranslate(w_Limit_L, w_Limit_T);
    ofScale(2);
    ofDrawBitmapStringHighlight("GR_PLOTS_v1.0.1", 0, 0, ofColor::black, ofColor(220));
    ofPopMatrix();

    ofSetColor(200);
    ofDrawBitmapString("You can use this software to plot images and get data from them.\n\nJust drop an image file into the marked area and it will plot it\nin a Vectorscope (for now, more plotting methods to come soon)\n\nYou can press the Return key in your keyboard to see it in 3D !!\n\n\nPress the z key and check the test images\nPress the 1 , 2 and 3 keys to flip between them\nPress the 0 key to return to the last loaded image (if any)\n\nPress the ? key to see more hotkeys and navigation controls" ,w_Limit_L,w_Limit_T+60);
    ofSetColor(255);

    ofSetColor(64,64,64,64);
    ofDrawRectRounded(w_Limit_L, w_Limit_T*3, w_Limit_L*4, w_Limit_T*2, 12);
    ofSetColor(255);

    ofNoFill();
    ofSetColor(64);
    ofDrawRectRounded(w_Limit_L, w_Limit_T*3, w_Limit_L*4, w_Limit_T*2, 12);
    ofSetColor(255);
    ofFill();
    
    float XX = (float)ofGetMouseX() / (float)ofGetWindowWidth();
    float YY = (float)ofGetMouseY() / (float)ofGetWindowHeight();
    XX -= 0.5;
    YY -= 0.7;
    
    ofPushMatrix();
    ofSetColor(16,16,16,255);
    ofTranslate(((w_W/2)-360/2)-(30*XX), (w_Limit_T*4)+(30*-YY));
    ofScale(1.5);
    ofDrawBitmapString("DRAG AND DROP YOUR IMAGES HERE", 0,0);
    ofSetColor(255);
    ofPopMatrix();

    ofPushMatrix();
    ofSetColor(128,128,128,255);
    ofTranslate(((w_W/2)-360/2), (w_Limit_T*4));
    ofScale(1.5);
    ofDrawBitmapString("DRAG AND DROP YOUR IMAGES HERE", 0,0);
    ofSetColor(255);
    ofPopMatrix();
}

// --------------------------------------------------------------------------------------------

void ofApp::save_image(){
    
    // this function saves the image to the same dir as the executable
    image_save_image.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    
    string filename = "../../../GR_PLOTS_image_" + ofToString(image_save_count, 3, '1') + ".png";
    
    image_save_image.save(filename);
    
}

// --------------------------------------------------------------------------------------------

void ofApp::draw_VectorScope(){
    
    // vectorscope draw funcctions
    if (!vectorscope.view_mode3D) {
        vectorscope.draw_translate();
    }
    
    // draw functions
    vectorscope.draw();
    vectorscope.draw_hud();
    
    // here the coordinates of the image have been reset for the source image!
    vectorscope.draw_source(vectorscope.is_loaded_image);
    vectorscope.draw_stats();
    vectorscope.draw_magnifier();
//    vectorscope.draw_cv(); // to be implemented in the future, now its too expensive and slow
    
    // init main gui draw loop function // DO NOT DELETE!!
    gui.begin();
    
    // init vectorscope gui
    vectorscope.gui(vectorscope_name);
    
    // end main gui draw loop function // DO NOT DELETE!!
    gui.end();
}

// --------------------------------------------------------------------------------------------

void ofApp::draw_VectorScope3D(){
    
    // vectorscope draw3D funcctions
    // 3D mode enabled with OF easy cam
    cam.begin();
    
    // Depth mode drawing method to bypass the draw order
    ofSetDepthTest(true);
    
    // cam view mode
    if (cam_orto) {
        cam.enableOrtho();
    }else{
        cam.disableOrtho();
    }
    
    // ImGui hovering gui
    if(ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)){
        cam.disableMouseInput();
    }else{
        cam.enableMouseInput();
    }
    
    // vectorscope 3D draw function
    vectorscope.draw3D();
    ofSetColor(255);
    vectorscope.draw_hud3D();
    
    // end depth composite and easy cam
    ofSetDepthTest(false);
    cam.end();
    
    // reset color and translate to center
    ofSetColor(255);
    ofTranslate(ofGetWindowWidth()/2, ofGetWindowHeight()/2);

    // here the coordinates of the image have been reset for the source image!
    vectorscope.draw_source(vectorscope.is_loaded_image);
    vectorscope.draw_stats();
    
// --------------------------------------------------------------------------------------------
//    to implement in the future with better performance, now its too slow
//    vectorscope.draw_cv();
// --------------------------------------------------------------------------------------------
    
    // init main gui draw loop function // DO NOT DELETE!!
    gui.begin();
    
    // init vectorscope gui
    vectorscope.gui(vectorscope_name);
    
    // end main gui draw loop function // DO NOT DELETE!!
    gui.end();
}

//--------------------------------------------------------------

void ofApp::draw(){
    
    if (_image.size() == 0){
        
        if(t_image_button){
            
            if (vectorscope.view_mode3D){
                
                draw_VectorScope3D();
                
            }else{
            
                draw_VectorScope();
            }

        }else if(!t_image_button){
            
            // INITIAL SCREEN!!
            draw_Welcome();
        }

    }else{
        
        if (vectorscope.view_mode3D) {
        
            draw_VectorScope3D();
        }else{
            
            
            draw_VectorScope();
        }
    }


//    for(int v = 0; v < _video.size(); v++){
//        _video[v].draw(0,0);
//    }

}

//--------------------------------------------------------------
void ofApp::exit(){}
//--------------------------------------------------------------

void ofApp::keyPressed(int key){

    // //  // USER-KEY events
    // global events
    switch (key) {
        case 'z':
            if (_image.size() == 0){
                t_image_button = !t_image_button;
                
                if(!t_image_button){
                    draw_Welcome();
                }
                
                break;
            }else{
                t_image_track_key = true;
                _image[0] = t_image_select();
                break;
            }
            
        case 'f':
            ofToggleFullscreen();
            break;
        
        // selection of image based on last dropped image from o_image if droped
        case '0':
            t_image_index = 0;
            
            t_image_track_key = true;
            
            if (_image.size() != 0){
                
                t_image_track_key = false;
            
                if(is_image){
                    _image[0] = o_image[0];
                    break;
                }else{
                    _image[0];
                }
            }else{
                break;
            }
            
        // selection of image based on user key
        case '1':
            t_image_index = 0;
            
            t_image_track_key = true;
            
            if (_image.size() != 0){
                if(is_image){
                    _image[0] = t_image_select();
                    break;
                }else{
                    _image[0];
                }
            }else{
                break;
            }
            
        case '2':
            
            t_image_track_key = true;
            
            if(is_image){
                // only active if its not in welcome
                t_image_index = 1;
            }
            if (_image.size() != 0){
                if(is_image){
                    _image[0] = t_image_select();
                    break;
                }else{
                    _image[0];
                }
            }else{
                break;
            }
            
        case '3':
            
            t_image_track_key = true;
            
            if(is_image){
                // only active if its not in welcome
                t_image_index = 2;
            }
            if (_image.size() != 0){
                if(is_image){
                    _image[0] = t_image_select();
                    break;
                }else{
                    _image[0];
                }
            }else{
                break;
            }
    }
    
    // with an image event
    if(is_image){
        switch (key) {
            case 'c':
                vectorscope.resetControls();
                break;

            case '?':
                vectorscope.help_key = !vectorscope.help_key;
                break;
                
            case 'h':
                vectorscope.button_hud = !vectorscope.button_hud;
                break;
            
            case 't':
                vectorscope.button_targets = !vectorscope.button_targets;
                break;
                
            case 's':
                vectorscope.button_source = !vectorscope.button_source;
                break;
                
            case 'i':
                vectorscope.button_stats = !vectorscope.button_stats;
                break;
                
            case 'a':
                vectorscope.button_bg = !vectorscope.button_bg;
                break;
                
            case 'm':
                vectorscope.m_button = !vectorscope.m_button;
                break;

            case 'p':
                // save screenshot image
                
                save_image();
                
                image_save_count++;
                
                break;

// -------------- -------------- -------------- -------------- -------------- -------------- --------------
//            removed for better implementation in the future (too slow...)
//            case 'y':
//                vectorscope.cv_button = !vectorscope.cv_button;
//                break;
// -------------- -------------- -------------- -------------- -------------- -------------- --------------
                
            // user input keys to display channel selection
            case 'r':
                if(vectorscope._channel == 0){
                    vectorscope._channel = 3;
                    break;
                }else{
                    vectorscope._channel = 0;
                    break;
                }
            case 'g':
                if(vectorscope._channel == 1){
                    vectorscope._channel = 3;
                    break;
                }else{
                    vectorscope._channel = 1;
                    break;
                }
            case 'b':
                if(vectorscope._channel == 2){
                    vectorscope._channel = 3;
                    break;
                }else{
                    vectorscope._channel = 2;
                    break;
                }

            // view mode 2D/3D
            case OF_KEY_RETURN:
                vectorscope.view_mode3D = !vectorscope.view_mode3D;
                break;
                
            case 'o':
                if(vectorscope.view_mode3D){
                    cam_orto = !cam_orto;
                    break;
                }else{
                    break;
                }
                
                
            default:
                break;
        }

    }else if (!is_image){
        // events when welcome
        false;
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    switch (button) {
        case OF_MOUSE_BUTTON_MIDDLE:
            if(!vectorscope.view_mode3D){
                vectorscope.draw_translate_reset();
                break;
            }else{
                break;
            }
        default:
            break;
    }
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){}
//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){}
//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}
//--------------------------------------------------------------

void ofApp::dragEvent(ofDragInfo dragInfo){
    
    // drag and drop image utilities
    // check for dropped data and load it to a vector
    if (dragInfo.files.size() > 0){
        _image.assign(dragInfo.files.size(), ofImage());
        for(int i = 0; i < dragInfo.files.size(); i++ ){
            _image[i].load(dragInfo.files[i]);
        }
    }
    // Original source dragged image vector storage
    if (dragInfo.files.size() > 0){
        o_image.assign(dragInfo.files.size(), ofImage());
        for(int i = 0; i < dragInfo.files.size(); i++ ){
            o_image[i].load(dragInfo.files[i]);
        }
    }

    // -------------- -------------- -------------- -------------- -------------- -------------- --------------
    
    // // // Method for storing value to know new dragged files for resize
    int numFilesBeforeDrag = draggedFiles.size();

    // Add the paths of the dragged files to the vector
    for (auto file : dragInfo.files) {
        draggedFiles.push_back(file);
    }

    // Check if new files were added during the drag operation
    if (draggedFiles.size() > numFilesBeforeDrag) {
        // if new file dragged, change temp checker
        t_image_track_key = false;;
    }
    
    // -------------- -------------- -------------- -------------- -------------- -------------- --------------
    
//    if (dragInfo.files.size() > 0){
//        _video.assign(dragInfo.files.size(), ofVideoPlayer());
//        for(int v = 0; v < dragInfo.files.size(); v++ ){
//            _video[v].loadAsync(dragInfo.files[v]);
//            _video[v].play();
//            _video[v].setVolume(0.0f);
//        }
//    }

    
}
