#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main(int argc, char *argv[]){
	ofSetupOpenGL(SCREENW, SCREENH,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofApp *app = new ofApp();
	app->arguments = std::vector<string>(argv, argv + argc);
	ofRunApp(app);
}
