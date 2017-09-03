#include "ofApp.h"

using namespace kaleidome;
//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetColor(255);

	dome = new Kaleidome(1.0f,2);
	dome->setPosition(ofVec3f(0, 0, 0));
	dome->setScale(10);
	dome->setOrientation(ofVec3f(180.0f, 0.0f, 0.0f));


	texture.loadImage("christmas.png");

	domemaster.setup();
	domemaster.setCameraPosition(0, 0, 5.5);

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	for (int i = 0; i < domemaster.renderCount; i++) {
		domemaster.begin(i);
		drawScene();
		domemaster.end(i);
	}

	domemaster.draw();
}

//--------------------------------------------------------------
void ofApp::drawScene() {
	ofEnableNormalizedTexCoords();
	texture.bind();
	dome->draw();
	texture.unbind();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::exit() {
	delete dome;
}