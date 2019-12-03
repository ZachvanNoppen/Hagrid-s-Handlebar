#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class ofApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);

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
		float tilt;
};

