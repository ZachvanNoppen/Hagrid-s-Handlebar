#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowShape(1280, 720);

	//FIRMATA REQUIRES bitrate to be 57600
	m_board.connect(PORT, 57600);
	m_inputVal = 0;
	m_isSetup = false;

	//setup the event listener to call and setup the board
	ofAddListener(m_board.EInitialized, this, &ofApp::setupArduino);
	cout << "Starting" << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
	updateArduino();
}

//--------------------------------------------------------------
void ofApp::draw(){
	
}

void ofApp::setupArduino(const int & version) {
	m_isSetup = true;
	ofRemoveListener(m_board.EInitialized, this, &ofApp::setupArduino);

	m_board.sendAnalogPinReporting(STRETCH_ANALOG_PIN, ARD_ANALOG);
	m_board.sendAnalogPinReporting(PRESSURE_ANALOG_PIN, ARD_ANALOG);
	m_board.sendDigitalPinMode(PIN_PWN_INPUT, ARD_PWM);

	//Adding event listeners for all pins
	ofAddListener(m_board.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
	ofAddListener(m_board.EAnalogPinChanged, this, &ofApp::analogPinChanged);

	cout << "Arduino has completed setup" << endl;
}
void ofApp::updateArduino() {

}
void ofApp::digitalPinChanged(const int & pinNum) {

}
void ofApp::analogPinChanged(const int & pinNum) {
	//Getting the pin value
	if (pinNum == PRESSURE_ANALOG_PIN) {
		m_inputVal = m_board.getAnalog(pinNum);
		m_inputVal = ofMap(m_inputVal, 90, 700, 0, 1023);

		cout << "Pressure: " << m_inputVal << endl;
	}
	if (pinNum == STRETCH_ANALOG_PIN) {
		m_inputVal = m_board.getAnalog(pinNum);
		

		cout << "Stretch: " << m_inputVal << endl;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}
