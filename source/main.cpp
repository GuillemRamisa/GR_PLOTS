#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
    ofGLWindowSettings settings;
    settings.setGLVersion(3, 2);
    
    settings.setSize(900, 800);
    settings.title = "GR_PLOTS_101";
//	settings.windowMode = OF_WINDOW; //can also be OF_WINDOWOF_FULLSCREEN

	auto window = ofCreateWindow(settings);

	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();

}
