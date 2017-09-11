#pragma once

#include "ofMain.h"
#include "ofxDomemaster.h"
#include "ofxJSON.h"
#include "Kaleidome.h"
#include "setting.h"

#define FILENAME "christmas.png"
#define SCREENW 1024
#define SCREENH 1024
#define KALEIDOW 4096
#define KALEIDOH 4096

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void drawScene();
	void exit();

	bool parseSettings();

	std::vector<string> arguments;
	ofxJSONElement json;
	Setting setting;

	ofxDomemaster domemaster;
	kaleidome::Kaleidome *dome;
	ofImage texture;

	bool isCam;
	int camId = 0;
	ofVideoGrabber grabber;

	ofFbo fbo;
};
