#include "ofApp.h"

using namespace kaleidome;
//--------------------------------------------------------------
void ofApp::setup() {
	// load settings
	parseSettings();

	ofSetWindowShape(setting.windowWidth, setting.windowHeight);
	ofBackground(255);
	ofSetColor(255);

	dome = new Kaleidome(1.0f, setting.subdivide);
	dome->setPosition(ofVec3f(0, 0, 0));
	dome->setScale(10);
	dome->setOrientation(ofVec3f(180.0f, 0.0f, 0.0f));

	/// texture settings
	if (setting.isCam) {
		// camera
		ofSetLogLevel(OF_LOG_VERBOSE);
		grabber.setVerbose(true);
		grabber.listDevices();
		grabber.setDeviceID(setting.camDeviceId);
		grabber.initGrabber(setting.cameraWidth, setting.cameraHeight);
	}
	else {
		// image file
		texture.loadImage(setting.filepath);
	}
	///

	domemaster.setup();
	domemaster.setMeshScale(1);
	domemaster.resize(setting.renderWidth, setting.renderHeight);
	domemaster.setCameraPosition(0, 0, 5.77);

	// inner rendering frame
	fbo.allocate(setting.renderWidth, setting.renderHeight);
}

//--------------------------------------------------------------
void ofApp::update() {
	if (setting.isCam)
		grabber.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	for (int i = 0; i < domemaster.renderCount; i++) {
		domemaster.begin(i);
		drawScene();
		domemaster.end(i);
	}
	fbo.begin(false);
	ofClear(0);
	domemaster.draw();
	fbo.end();
	fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::drawScene() {
	ofEnableNormalizedTexCoords();
	if (setting.isCam)
		texture.setFromPixels(grabber.getPixels());

	texture.bind();
	dome->draw();
	texture.unbind();

	ofDisableNormalizedTexCoords();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 's') {
		ofImage img;
		img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		char fileNameStr[255];
		string date = ofGetTimestampString();
		sprintf(fileNameStr, "S-%s.png", date.c_str());
		img.save(fileNameStr, OF_IMAGE_QUALITY_BEST);
		std::cout << "スクリーンを保存しました " << fileNameStr << std::endl;
	}
	if (key == 'f') {
		ofImage img;
		ofPixels pixels;
		fbo.readToPixels(pixels);
		img.setFromPixels(pixels);
		char fileNameStr[255];
		string date = ofGetTimestampString();
		sprintf(fileNameStr, "F-%s.png", date.c_str());
		img.save(fileNameStr, OF_IMAGE_QUALITY_BEST);
		std::cout << "フレームを保存しました" << fileNameStr << std::endl;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

//--------------------------------------------------------------
void ofApp::exit() {
	delete dome;
}

//--------------------------------------------------------------
bool ofApp::parseSettings() {
	bool success = json.open("sample.json");
	if (success) {
		std::cout << "Succeed to parse JSON" << std::endl;
		// windows size
		setting.windowWidth = json["windowSize"][0].asInt();
		setting.windowWidth = json["windowSize"][1].asInt();

		// inner render size
		setting.renderWidth = json["renderSize"][0].asInt();
		setting.renderHeight = json["renderSize"][1].asInt();

		//input
		setting.isCam = json["enableCamera"].asBool();
		setting.camDeviceId = json["cameraDeviceId"].asInt();
		setting.cameraWidth = json["cameraResolution"][0].asInt();
		setting.cameraHeight = json["cameraResolution"][1].asInt();
		setting.filepath = json["filePath"].asString();

		// dome setting
		setting.subdivide = json["subdivide"].asUInt();

	}
	else {
		std::cout << "Failed to parse JSON" << std::endl;
	}

	return success;
}