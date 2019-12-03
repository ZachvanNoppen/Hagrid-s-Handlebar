#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	keyboard_toggle = false;
    m_font.load( "DIN.otf", 16 );

	//setup Arduino
	// replace the string below with the serial port for your Arduino board
	// you can get this from the Arduino application (Tools Menu -> Port) 
	//m_arduino.connect("COM4", 57600);
    m_arduino.connect(config::ARDUINO_DEVICE_NAME, 57600);

	m_brake_val	= 0.0f;
	m_gas_val = 0.0f;

	// Listen for EInitialized notification. this indicates that
	// the arduino is ready to receive commands and it is safe to
	// call setupArduino()
	ofAddListener(m_arduino.EInitialized, this, &ofApp::setupArduino);
	//setupArduino();
	m_bSetup = false;

	ofBackground(255, 255, 255);

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
	cam.setPosition(ofVec3f(0, 30, 50));
	cam.setPosition(ofVec3f(0, 30, 50));

	bike.loadModel("bike.3DS");
	bike.setRotation(0, 180, 1, 0, 0);
	bike.setScale(0.1, 0.1, 0.1);
	bike.setPosition(bikeControls.getGlobalPosition().x, bikeControls.getGlobalPosition().y, bikeControls.getGlobalPosition().z);
	curFileInfo = ".3ds";

	model.loadModel("gmae.3ds");
	model.setRotation(0, 180, 1, 0, 0);
	model.setScale(10.0, 10.0, 10.0);
	curFileInfo = ".3ds";

	light.setPosition(500, 0, 1500);

	//cameraOrbit = 0;
	//cam.setDistance(500);
	bikeControls.setGlobalPosition(bikeControls.getGlobalPosition().x, bikeControls.getGlobalPosition().y - 120, bikeControls.getGlobalPosition().z - 20);
	bikeControls.setParent(cam);
	//set help text to display by default
	bHelpText = true;

	tilt = cam.getOrientationEulerDeg().x;
	pan = cam.getOrientationEulerDeg().y;

	m_acceleration = 1.0f;


}

//--------------------------------------------------------------
void ofApp::update()
{
    updateArduino();

	if (m_bSetup) {
		m_controllerRot = readString();
		//map from 0 to 20 to 0 to 90
		tilt = ofMap(m_controllerRot.y,0.0f,20.0f,-180.0f,180.0f);
		pan = ofMap(m_controllerRot.x, 0.0f, 20.0f, -180.0f, 180.0f);
        //tilt = ofMap(m_controllerRot.y, 0.0f, 20.0f, -1.0f, 1.0f);
        //pan = ofMap(m_controllerRot.x, 0.0f, 20.0f, -1.0f, 1.0f);
		ofVec3f orient(-pan, tilt+TILT_CORRECTION, 0.0f);
        //ofQuaternion orient(pan, tilt, 0.0f, 0.0f);
		cam.setOrientation(orient);
        //bikeControls.setOrientation(orient);
	}
	//Added
	bike.setPosition(bikeControls.getGlobalPosition().x, bikeControls.getGlobalPosition().y, bikeControls.getGlobalPosition().z);

	//tilt = cam.getOrientationEulerDeg().x;
	//float rot = cam.getOrientationEulerDeg().z;
    ofVec3f bikeOrient = cam.getOrientationEulerDeg();
	bike.setRotation(1, bikeOrient.y, 0, 1, 0);
	bike.setRotation(1, bikeOrient.x, 1, 0, 0);
	//bike.setRotation(1, rot, 0, 0, 1);

	ofVec3f dir;
	dir = cam.getLookAtDir();
	//when using the keyboard controlls
	if (keyboard_toggle) {
		//m_acceleration = 1.0;
	}
	else {
		m_acceleration = m_gas_val / 10.0f;
	}
	cam.move(dir.x * m_acceleration, dir.y, dir.z * m_acceleration);
    m_acceleration /= 1.01f;// *(m_brake_val / 20.0f);
}
//--------------------------------------------------------------
void ofApp::draw()
{
	ofEnableDepthTest();

	light.enable();

	cam.begin();
	ofColor(255, 255);
	if (bUsingMesh) {
		// draws the ply file loaded into the mesh is you pressed 6
		meshNode.transformGL();
		mesh.draw();
		meshNode.restoreTransformGL();
	}
	else {
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
	if (bHelpText) {
		stringstream ss;
		ss << bikeControls.getOrientationEulerDeg().x << endl;
		ss << bike.getRotationAngle(0) << endl;
		ss << "FPS: " << ofToString(ofGetFrameRate(), 0) << endl << endl;
		ss << "(1/2/3/4/5/6): load different file types" << endl;
		ss << "Current file info: " + curFileInfo << endl;
		if (bUsingMesh) {
			ss << "Use ofEasyCam mouse and key controls to navigate." << endl << endl;
		}
		ss << "(h): Toggle help." << endl;
		ofDrawBitmapString(ss.str().c_str(), 20, 20);
	}
    
    // Draw sensor input values
    m_font.drawString( "Sensor Value: " + ofToString( m_brake_val ), 530, 105 );
	m_font.drawString("Sensor Value: " + ofToString(m_gas_val), 800, 105);

    // remap our flex values (can check on arduino sketch as values will always vary between sensors)
    float radius = ofMap( m_brake_val, 0, 255, 20, 150 );

	float stretch_radius = ofMap(m_gas_val, 0, 255, 20, 150);
	
	
}

//--------------------------------------------------------------
void ofApp::setupArduino(const int & _version)
{
	cout << "Setup" << endl;
    /**
     Look under examples/communication/firmata for more examples ..
     **/
    
    m_bSetup = true;
    
	// remove listener because we don't need it anymore
	ofRemoveListener(m_arduino.EInitialized, this, &ofApp::setupArduino);

	// print firmware name and version to the console
	ofLogNotice() << m_arduino.getFirmwareName();
	ofLogNotice() << "firmata v" << m_arduino.getMajorFirmwareVersion() << "." << m_arduino.getMinorFirmwareVersion();

	//analog input
	m_arduino.sendAnalogPinReporting(PIN_BRAKE_INPUT, ARD_ANALOG);
	m_arduino.sendAnalogPinReporting(PIN_GAS_INPUT, ARD_ANALOG);
    //PMW/digital output
    //m_arduino.sendDigitalPinMode(PIN_PWM_OUTPUT, ARD_PWM);
    
    ofAddListener(m_arduino.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
    ofAddListener(m_arduino.EAnalogPinChanged, this, &ofApp::analogPinChanged);

	cout << "Setup Complete" << endl;
	
}

//--------------------------------------------------------------
void ofApp::updateArduino() {

	// update the arduino, get any data or messages.
	// the call to m_arduino.update() is required
	m_arduino.update();
	
	

}

void ofApp::digitalPinChanged(const int & pinNum) {
    //std::cout  << "digital pin: " + ofToString(pinNum) + " : " + ofToString(m_arduino.getDigital(pinNum)) << std::endl;
}

void ofApp::analogPinChanged(const int & pinNum) {
    //std::cout  << "analog pin: " + ofToString(pinNum) + " : " + ofToString(m_arduino.getAnalog(pinNum)) << std::endl;
    if ( pinNum == PIN_BRAKE_INPUT) {
        
        //get analog value
        m_brake_val = m_arduino.getAnalog(pinNum);
        m_brake_val = (int)ofMap( m_brake_val, 0, 1024, 0, 255 );
        
    }
	if (pinNum == PIN_GAS_INPUT) {
		m_gas_val = m_arduino.getAnalog(pinNum);
		m_gas_val = (int)ofMap(m_gas_val, 0, 1024, 0, 255);
	}
}


ofVec2f ofApp::readString() {
	ofVec2f orientation;
	str = m_arduino.getString();
	std::string::size_type sz;

	std::string numX = str.substr(0, 4);
	std::string numY = str.substr(str.length()-6, str.length()-1);


	orientation.x = std::stof(numX, &sz);
	orientation.y = std::stof(numY, &sz);


	return orientation;
}

void ofApp::mousePressed(int x, int y, int button) {
	
}

void ofApp::keyPressed(int key) {

	//Toggle the keyboard controlls
	if (key == 'k') {
		keyboard_toggle = true;
	}
	//COMMENT OUT JUST FOR DEBUG
	//keyboard_toggle = true;

        if (key == OF_KEY_UP) {
            m_acceleration += 5.0f;
            cout << m_acceleration << endl;

        }
        else if (key == OF_KEY_DOWN) {

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
        }
        else if (key == OF_KEY_LEFT) {
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
	
	
}