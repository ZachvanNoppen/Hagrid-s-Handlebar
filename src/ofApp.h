/*
	You may use this application to start your projects but I would recommend a couple ofg poinmts:
	- modify this code for your own needs (i.e. change controls, inputs, types of camera movements)
	- try to make this better organized/modular (i.e. using state machines, classes where it fits, and the config file paradigm we looked at in class)
	- try to load in a nother model. Not necessary but might be fun :)

	Current Debug[keyboard] controls
	- FPS controls, WASD translate sideways/forwards/backwards and arrow keys adjust rotation on y and x axes
*/

#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofEvents.h"
#include <string>

namespace config {
	static const std::string ARDUINO_DEVICE_NAME = "COM3";		//WIN
	//static const std::string ARDUINO_DEVICE_NAME = "/dev/cu.usbmodem144101";	//MAC
};

class ofApp : public ofBaseApp 
{
public:
	string str;
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
    
	//Accelerometer data
	float m_orientation[3];



	void setupArduino(const int & _version);
	void updateArduino();
	//void setupArduino();
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
	ofVec2f readString();
	void mousePressed(int x, int y, int button);

	void keyPressed(int key);

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
	bool keyboard_toggle;
	ofNode meshNode;

	ofCamera cam;
	float cameraOrbit;

	float m_acceleration;
	float pan;
	float tilt;


};

