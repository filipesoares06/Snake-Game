#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	glEnable(GL_DEPTH_TEST);
	ofSetFrameRate(60);
	ofBackground(0.5, 0.5, 0.5);   //White background color is ofBackground(255, 255, 255).
	glLineWidth(5);
	
	gameMovement = new Movement();
}

//--------------------------------------------------------------
void ofApp::update() {
	ofSetWindowTitle("CG Game Project Running @ " + ofToString(int(ofGetFrameRate())));

	gameMovement->update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	gameMovement->draw();
}

//--------------------------------------------------------------
void ofApp::exit() {
	delete gameMovement;
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	gameMovement->keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	gameMovement->keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ) {

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

//Filipe Freire Soares 2020238986
