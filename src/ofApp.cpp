#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{

    m_font.load( "DIN.otf", 16 );
	//setup Arduino
    m_arduino.connect(config::ARDUINO_DEVICE_NAME, 57600);

	m_brake_val	= 0.0f;
	m_gas_val = 0.0f;

	// Listen for EInitialized notification. this indicates that
	// the arduino is ready to receive commands and it is safe to
	ofAddListener(m_arduino.EInitialized, this, &ofApp::setupArduino);
	m_bSetup = false;

	ofBackground(255, 255, 255);
	ofSetVerticalSync(true);

	bUsingMesh = false;

    //Set up the bike and camera so it is facing the right direction
	bikeControls.setOrientation(ofVec3f(0, 180, 0));
	bikeControls.setPosition(ofVec3f(0, 100, 0));
	cam.setPosition(ofVec3f(0, 30, 50));
	cam.setPosition(ofVec3f(0, 30, 50));

    //Loading in the bike model and setting up the orientation
	bike.loadModel("bike.3DS");
	bike.setRotation(0, 180, 1, 0, 0);
	bike.setScale(0.1, 0.1, 0.1);
	bike.setPosition(bikeControls.getGlobalPosition().x, bikeControls.getGlobalPosition().y, bikeControls.getGlobalPosition().z);
	curFileInfo = ".3ds";

    //Setting up the virtual world
	model.loadModel("gmae.3ds");
	model.setRotation(0, 180, 1, 0, 0);
	model.setScale(10.0, 10.0, 10.0);
	curFileInfo = ".3ds";

    //Setting up lighting
	light.setPosition(500, 0, 1500);

    //Initializing the location of the bike.
    //Note that the bikeControls variable inherits the attributes of the camera, and is then used to position the bike model
	bikeControls.setGlobalPosition(bikeControls.getGlobalPosition().x, bikeControls.getGlobalPosition().y - 120, bikeControls.getGlobalPosition().z - 20);
	bikeControls.setParent(cam);
	//set help text to display by default
	bHelpText = true;

    //Initializing orientation variables
	tilt = cam.getOrientationEulerDeg().x;
	pan = cam.getOrientationEulerDeg().y;
    m_panRead = 0;
    m_tiltRead = -40.0f;
	m_acceleration = 1.0f;

}

//--------------------------------------------------------------
void ofApp::update()
{
    updateArduino();

    
	if (m_bSetup) {
		m_controllerRot = readString();
		//map from 0 to 20 to 0 to 90
		m_tiltRead = ofMap(m_controllerRot.x,0.0f,20.0f,-180.0f,180.0f);
        m_panRead = ofMap(m_controllerRot.y, 0.0f, 20.0f, -180.0f, 180.0f);
	}
	//Added
	bike.setPosition(bikeControls.getGlobalPosition().x, bikeControls.getGlobalPosition().y, bikeControls.getGlobalPosition().z);

    //Only allow turning if NOT tilted up or down. 
    //This keep the camera positioned properly
    if (tilt == 0) {
        //Only panning if the controller is moved a certain amount
        //The hard coded values here are specific to the controller setup used
        if (m_panRead > 40.0) {
            //Pan the camera and pan the bike over the same amount to keep it in the center of the screen
            cam.panDeg(2);
            //Since the pan variable is seperate from the camera (only keeping track alongside camera rotation) it has to change if it gets too big/small
            if (pan >= 180) {
                pan = -180;
            }
            else {
                pan += 2;
            }
            bike.setRotation(1, pan, 0, 1, 0);
        }
        else if (m_panRead < -20.0) {
            cam.panDeg(-2);
            if (pan <= -180) {
                pan = 180;
            }
            else {
                pan -= 2;
            }
            bike.setRotation(1, pan, 0, 1, 0);
        }
            
    }
        //Tilting up and down
        //The hard coded values here are specific to the controller setup used
        if (m_tiltRead < -95.0) {
            //Limitting the amount the bike can tilt
            if (tilt < 30.0f) {
                tilt += 2;
                cam.tiltDeg(2);
            }
            bike.setRotation(1, -tilt, 1, 0, 0);
            bike.setRotation(1, pan, 0, 1, 0);

        }
        else if (m_tiltRead > -35.0) {
            //Limitting the amount the bike can tilt
            if (tilt > -30.0f) {
                cam.tiltDeg(-2);
                tilt -= 2;
            }
            bike.setRotation(1, -tilt, 1, 0, 0);
            bike.setRotation(1, pan, 0, 1, 0);
        }
        else {
            //If the bike is at rest, tilt back to the default position
            cam.tiltDeg(0 - tilt);
            tilt = 0;
        }
    
    //Getting the normalized vector that the bike will move along
    //Allows us to move in the direction the bike is facing
	ofVec3f dir;
	dir = cam.getLookAtDir();

	//Setting the accelleration
    m_acceleration += m_gas_val / 50.0f;
    if (m_acceleration < 1.0f) {
        m_acceleration = 1.0f;
    }
    //Moving the camera, which in turn moves the bike
	cam.move(dir.x * m_acceleration, dir.y, dir.z * m_acceleration);
    //Making the bike always move slightly forward, at idle
    if (m_brake_val < 1.0f) {
        m_brake_val = 1.0f;
    }
    //Slowing the bike down
    m_acceleration /= 1.01f * (m_brake_val);
}
//--------------------------------------------------------------
void ofApp::draw()
{
    //Setting up the environment and lighting
	ofEnableDepthTest();
	light.enable();

	cam.begin();
	ofColor(255, 255);
	//Drawing the model and world
    model.drawFaces();
    bike.drawFaces();

	cam.end();
	light.disable();
	ofDisableDepthTest();

	// display help text if it is enable
	if (bHelpText) {
		stringstream ss;
		ss << "FPS: " << ofToString(ofGetFrameRate(), 0) << endl << endl;
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
    //Lets the program know the arduino is connected
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

    //Wait until this is displayed in the console to start moving the controller
	cout << "Setup Complete" << endl;
	
}

//--------------------------------------------------------------
void ofApp::updateArduino() {

	// update the arduino, get any data or messages.
	// the call to m_arduino.update() is required
	m_arduino.update();
}

void ofApp::digitalPinChanged(const int & pinNum) {
    //No digital pins are used
}

void ofApp::analogPinChanged(const int & pinNum) {
    //Getting data from the arduino
    if ( pinNum == PIN_BRAKE_INPUT) {
        //Mapping the analog signal to usable data
        m_brake_val = m_arduino.getAnalog(pinNum);
        m_brake_val = (int)ofMap( m_brake_val, 0, 1024, 0, 255 );
        
    }
	if (pinNum == PIN_GAS_INPUT) {
        //Mapping the analog signal to usable  data
		m_gas_val = m_arduino.getAnalog(pinNum);
		m_gas_val = (int)ofMap(m_gas_val, 0, 1024, 0, 255);
	}
}


ofVec2f ofApp::readString() {
	ofVec2f orientation;
    //Reading the string value in the buffer sent from the arduino
	str = m_arduino.getString();
	std::string::size_type sz;
    //Grabbing the first number, and last number since their length will change
	std::string numX = str.substr(0, 4);
	std::string numY = str.substr(str.length()-6, str.length()-1);
    //Parsing each number to a float and returning the vector
	orientation.x = std::stof(numX, &sz);
	orientation.y = std::stof(numY, &sz);

	return orientation;
}

void ofApp::keyPressed(int key) {

	//Some keyboard controls for debugging

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