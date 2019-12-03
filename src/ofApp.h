
#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofEvents.h"
#include <string>

namespace config {
    //Change this to whatever COM Port your arduino is attached to
	static const std::string ARDUINO_DEVICE_NAME = "COM3";		//WIN
	//static const std::string ARDUINO_DEVICE_NAME = "/dev/cu.usbmodem144101";	//MAC
};

class ofApp : public ofBaseApp 
{
public:
	
    static const int PIN_BRAKE_INPUT = 3;
	static const int PIN_GAS_INPUT	 = 0;
    float TILT_CORRECTION = -10.0f;


    void setup();
    void update();
    void draw();

	//Arduino stuff
	ofArduino				m_arduino;
    ofTrueTypeFont          m_font;
	int					    m_brake_val;	//sensor value
	int					    m_gas_val;		//sensor value
	bool					m_bSetup;       //is Arduinio initialized yet
    string                  str;
	//Accelerometer data

	void setupArduino(const int & _version);
	void updateArduino();
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
	ofVec2f readString();
	void keyPressed(int key);

    //bike and model variables
	ofVec2f m_controllerRot;
	ofNode bikeControls;
	ofxAssimpModelLoader model;
	ofxAssimpModelLoader bike;
	ofVec3f bikeRotation;

	ofLight	light;

	// stores the info on the current file.
	string curFileInfo;

	//boolean to toggle help text
	bool bHelpText;

	// for demonstrating loading in models directly into a mesh without using ofxAssimpModelLoader
	ofVboMesh mesh;
	bool bUsingMesh;
	ofNode meshNode;

	ofCamera cam;
	float cameraOrbit;

	float m_acceleration;
	float pan;
    float m_panRead;
	float tilt;
    float m_tiltRead;

};

