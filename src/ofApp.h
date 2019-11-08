#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		
		void setupArduino(const int & version);
		void updateArduino();
		void digitalPinChanged(const int & pinNum);
		void analogPinChanged(const int & pinNum);

		//Constants
		static const int PRESSURE_ANALOG_PIN = 5;
		static const int STRETCH_ANALOG_PIN = 0;
		
		static const int PIN_PWN_INPUT = 9; //Just generic pins to use
		const string PORT = "COM5";
		//
		ofArduino m_board;
		int m_inputVal;
		bool m_isSetup;

};
