#include "ofApp.h"

/*
Loads in the same 3D penguin model in various file types. Demonstrates how to load in a model using both ofMesh and ofxAssimpModelLoader.
*/
 
//--------------------------------------------------------------
void ofApp::setup(){	
	ofBackground(255,255,255);
		
	ofSetVerticalSync(true);
    
    // load the first model
    //model.loadModel("penguin.dae", 20);
    // model info
    //curFileInfo = ".dae";
    
	
    // this loads in the model directly into a mesh
    // ofMesh can only read in .plys that are in the ascii and not in the binary format.
    //bUsingMesh = false;
    //mesh.load("penguin.ply");
	//meshNode.setOrientation(glm::angleAxis(ofDegToRad(-90.f), glm::vec3{1.f, 0.f, 0.f}));
    
	// you can create as many rotations as you want
	// choose which axis you want it to effect
	// you can update these rotations later on
	// these rotation set the model so it is oriented correctly

	bUsingMesh = false;

	bikeControls.setOrientation(ofVec3f(0, 180, 0));
	bikeControls.setPosition(ofVec3f(0, 100, 0));
	cam.setPosition(ofVec3f(0, 100, 70));

	bike.loadModel("bike.3DS");
	bike.setRotation(0, 180, 1, 0, 0);
	bike.setScale(0.1, 0.1, 0.1);
	bike.setPosition(bikeControls.getGlobalPosition().x, bikeControls.getGlobalPosition().y, bikeControls.getGlobalPosition().z);
	curFileInfo = ".3ds";
	
	model.loadModel("gmae.3ds");
	model.setRotation(0, 180, 1, 0, 0);
	model.setScale(1.0, 1.0, 1.0);
	curFileInfo = ".3ds";

    light.setPosition(500, 0, 1500);

	//cameraOrbit = 0;
	//cam.setDistance(500);
	bikeControls.setGlobalPosition(bikeControls.getGlobalPosition().x, bikeControls.getGlobalPosition().y-120, bikeControls.getGlobalPosition().z - 20);
	bikeControls.setParent(cam);
    //set help text to display by default
    bHelpText  = true;

	tilt = cam.getOrientationEulerDeg().x;
	pan = cam.getOrientationEulerDeg().y;

	m_acceleration = 1.0f;
}

//--------------------------------------------------------------
void ofApp::update(){
	//cameraOrbit += ofGetLastFrameTime() * 20.; // 20 degrees per second;
	//cam.orbitDeg(cameraOrbit, 0., cam.getDistance(), {0., 0., 0.});
	bike.setPosition(bikeControls.getGlobalPosition().x, bikeControls.getGlobalPosition().y, bikeControls.getGlobalPosition().z);

	tilt = cam.getOrientationEulerDeg().x;
	//float rot = cam.getOrientationEulerDeg().z;
	bike.setRotation(1, tilt, 1, 0, 0);
	bike.setRotation(1, pan, 0, 1, 0);
	//bike.setRotation(1, rot, 0, 0, 1);

	ofVec3f dir;
	dir = cam.getLookAtDir();
	cam.move(dir.x * m_acceleration, dir.y, dir.z * m_acceleration);
	m_acceleration /= 1.05f;

}

//--------------------------------------------------------------
void ofApp::draw(){
	// so the model isn't see through.
	ofEnableDepthTest();
	
    light.enable();

	cam.begin(); 
	ofColor(255,255);
	if (bUsingMesh){
		// draws the ply file loaded into the mesh is you pressed 6
		meshNode.transformGL();
		mesh.draw();
		meshNode.restoreTransformGL();
	} else {
		// draws all the other file types which are loaded into model.
		model.drawFaces();
		//bike.setPosition(bikeControls.getGlobalPosition().x, bikeControls.getGlobalPosition().y, bikeControls.getGlobalPosition().z);
		bike.drawFaces();
    }

	//bikeControls.setGlobalPosition(bikeControls.getGlobalPosition().x, bikeControls.getGlobalPosition().y, bikeControls.getGlobalPosition().z - 90);
	ofSetColor(0, 0, 255);
	ofBoxPrimitive tempBike(10, 10, 10);
	tempBike.setPosition(bikeControls.getGlobalPosition());
	tempBike.setGlobalOrientation(bikeControls.getGlobalOrientation());
	tempBike.draw();

	//ofDrawBox(bikeControls.getGlobalPosition(), 20);
	cam.end();
    
    light.disable();

	ofDisableDepthTest();
    
    // display help text if it is enable
    if(bHelpText) {
        stringstream ss;
		ss << bikeControls.getOrientationEulerDeg().x << endl;
		ss << bike.getRotationAngle(0) << endl;
        ss << "FPS: " << ofToString(ofGetFrameRate(),0) << endl << endl;
        ss << "(1/2/3/4/5/6): load different file types"<<endl;
        ss << "Current file info: " + curFileInfo <<endl;
        if(bUsingMesh){
            ss << "Use ofEasyCam mouse and key controls to navigate."<< endl <<endl;
        }
        ss <<"(h): Toggle help."<<endl;
        ofDrawBitmapString(ss.str().c_str(), 20, 20);
    }
}
 
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	

	if (key == OF_KEY_UP) {
		m_acceleration += 5.0f;
		cout << m_acceleration << endl;

	}else if (key == OF_KEY_DOWN) {
		
		m_acceleration -= 10.0f;
	}


	if (key == OF_KEY_RIGHT) {
		cam.panDeg(-2);
		if (pan <= -180) {
			pan = 180;
		}
		else {
			pan -= 2;
		}
	}else if(key == OF_KEY_LEFT) {
		cam.panDeg(2);
		if (pan >= 180) {
			pan = -180;
		}
		else {
			pan += 2;
		}
	}
	if (key == 'w') {
		cam.tiltDeg(2);
	}
	if (key == 's') {
		cam.tiltDeg(-2);
	}
    //switch (key){
		
		/*
            case '1':
                bUsingMesh = false;
				model.loadModel("penguin.dae");
				model.setRotation(0, 180, 1, 0, 0);
				model.setScale(0.9, 0.9, 0.9);
				cam.setDistance(700);
                curFileInfo = ".dae";
                break;
            case '2':
                bUsingMesh = false;
				model.loadModel("penguin.3ds");
				model.setRotation(0, 180, 1, 0, 0);
				model.setScale(0.9, 0.9, 0.9);
				cam.setDistance(700);
				curFileInfo = ".3ds";
                break;
            case '3':
                bUsingMesh = false;
				model.loadModel("penguin.ply");
				model.setRotation(0, 90, 1, 0, 0);
				model.setScale(0.9, 0.9, 0.9);
				cam.setDistance(700);
                curFileInfo = ".ply";
                break;
            case '4':
                bUsingMesh = false;
				model.loadModel("gmae.obj");
				model.setRotation(0, 90, 1, 0, 0);
				model.setScale(0.9, 0.9, 0.9);
				cam.setDistance(700);
                curFileInfo = ".obj";
                break;
            case '5':
                bUsingMesh = false;
				model.loadModel("penguin.stl");
				model.setRotation(0, 90, 1, 0, 0);
				model.setScale(0.9, 0.9, 0.9);
				cam.setDistance(700);
                curFileInfo = ".stl";
                break;
            case '6':
				bUsingMesh = true;
				cam.setDistance(40);
                curFileInfo = ".ply loaded directly into ofmesh";
                break;
            case 'h':
                //toggle help text
                bHelpText = !bHelpText;
                break;
            default:
                break;
		*/
    //}
}