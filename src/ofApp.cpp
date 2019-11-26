#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetWindowShape(1280, 720);
    
    m_font.load( "DIN.otf", 16 );

	//setup Arduino
	// replace the string below with the serial port for your Arduino board
	// you can get this from the Arduino application (Tools Menu -> Port) 
	//m_arduino.connect("COM4", 57600);
    m_arduino.connect(config::ARDUINO_DEVICE_NAME, 57600);

	m_pressure_val	= 0.0f;
	m_stretch_val = 0.0f;

	// Listen for EInitialized notification. this indicates that
	// the arduino is ready to receive commands and it is safe to
	// call setupArduino()
	ofAddListener(m_arduino.EInitialized, this, &ofApp::setupArduino);
	//setupArduino();


	m_bSetup = false;
}

//--------------------------------------------------------------
void ofApp::update()
{
    updateArduino();
	
}
//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground( 255, 0, 130 );
    
    ofEnableAlphaBlending();
    ofSetColor( 0, 0, 0, 127 );
    ofDrawRectangle( 510, 15, 275, 150 );
    ofDisableAlphaBlending();
    
    ofSetColor( 255, 255, 255 );
    
    // Draw sensor input values
    m_font.drawString( "Sensor Value: " + ofToString( m_pressure_val ), 530, 105 );
	m_font.drawString("Sensor Value: " + ofToString(m_stretch_val), 800, 105);

    // remap our flex values (can check on arduino sketch as values will always vary between sensors)
    float radius = ofMap( m_pressure_val, 0, 255, 20, 150 );
    ofDrawCircle( 640, 400, radius );

	float stretch_radius = ofMap(m_stretch_val, 0, 255, 20, 150);
	ofDrawCircle(900, 400, stretch_radius);

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
	m_arduino.sendAnalogPinReporting(PIN_PRESSURE_INPUT, ARD_ANALOG);
	m_arduino.sendAnalogPinReporting(PIN_STRETCH_INPUT, ARD_ANALOG);
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
	readString();

}

void ofApp::digitalPinChanged(const int & pinNum) {
    //std::cout  << "digital pin: " + ofToString(pinNum) + " : " + ofToString(m_arduino.getDigital(pinNum)) << std::endl;
}

void ofApp::analogPinChanged(const int & pinNum) {
    //std::cout  << "analog pin: " + ofToString(pinNum) + " : " + ofToString(m_arduino.getAnalog(pinNum)) << std::endl;
    if ( pinNum == PIN_PRESSURE_INPUT) {
        
        //get analog value
        m_pressure_val = m_arduino.getAnalog(pinNum);
        m_pressure_val = (int)ofMap( m_pressure_val, 0, 1024, 0, 255 );
        
    }
	if (pinNum == PIN_STRETCH_INPUT) {
		m_stretch_val = m_arduino.getAnalog(pinNum);
		m_stretch_val = (int)ofMap(m_stretch_val, 0, 1024, 0, 255);
	}
}

//!!convert voltage signal into a readable result distance in cm (convenience function for for distance sensor - from IR sensor specs)
float ofApp::getIRDistance(int & val)
{
	if (val < 16)
	{
		val = 16;
	}

	return 2076.0f / (val - 11.0f);
}

void ofApp::readString() {
	str = m_arduino.getString();
	std::string::size_type sz;

	std::string numX = str.substr(0, 4);
	std::string numY = str.substr(str.length()-6, str.length()-1);

	float x = std::stof(numX, &sz);
	float y = std::stof(numY, &sz);


	cout << "X: " << x << endl;
	cout << "Y: " << y << endl;
}

void ofApp::mousePressed(int x, int y, int button) {
	
}