//
// CLOUDS Interactive Documentary
//
// VISUAL SYSTEMS
//
// Welcome to the EMPTY CloudsVisualSystem
//
//
//

#pragma once

#include "CloudsVisualSystem.h"


//TODO: rename this to your own visual system
class CloudsVisualSystemFireworks : public CloudsVisualSystem {
public:
    
	
	//TODO: Change this to the name of your visual system
	//This determines your data path so name it at first!
	//ie getVisualSystemDataPath() uses this
    string getSystemName(){
		return "EmptySystem";
	}
	
	//These methods let us add custom GUI parameters and respond to their events
    void selfSetupGui();
    void selfGuiEvent(ofxUIEventArgs &e);
    
	//Use system gui for global or logical settings, for exmpl
    void selfSetupSystemGui();
    void guiSystemEvent(ofxUIEventArgs &e);
    
	//use render gui for display settings, like changing colors
    void selfSetupRenderGui();
    void guiRenderEvent(ofxUIEventArgs &e);
	
	// selfSetup is called when the visual system is first instantiated
	// This will be called during a "loading" screen, so any big images or
	// geometry should be loaded here
    void selfSetup();
	
	// selfBegin is called when the system is ready to be shown
	// this is a good time to prepare for transitions
	// but try to keep it light weight as to not cause stuttering
    void selfBegin();
	
	// selfPresetLoaded is called whenever a new preset is triggered
	// it'll be called right before selfBegin() and you may wish to
	// refresh anything that a preset may offset, such as stored colors or particles
	void selfPresetLoaded(string presetPath);
    
	//do things like ofRotate/ofTranslate here
	//any type of transformation that doesn't have to do with the camera
    void selfSceneTransformation();
	
	//normal update call
	void selfUpdate();
	
	// selfDraw draws in 3D using the default ofEasyCamera
	// you can change the camera by returning getCameraRef()
    void selfDraw();
	
	// use this to draw the point cloud
	void selfDrawRGBD();
	
    // draw any debug stuff here
	void selfDrawDebug();
	
	//draws through RGBD camera;
	//	void selfDrawRGBD();
	
	// or you can use selfDrawBackground to do 2D drawings that don't use the 3D camera
	void selfDrawBackground();
	
	// this is called when your system is no longer drawing.
	// Right after this selfUpdate() and selfDraw() won't be called any more
	void selfEnd();
	
	// this is called when you should clear all the memory and delet anything you made in setup
    void selfExit();
	
	//events are called when the system is active
	//Feel free to make things interactive for you, and for the user!
    void selfKeyPressed(ofKeyEventArgs & args);
    void selfKeyReleased(ofKeyEventArgs & args);
    
    void selfMouseDragged(ofMouseEventArgs& data);
    void selfMouseMoved(ofMouseEventArgs& data);
    void selfMousePressed(ofMouseEventArgs& data);
    void selfMouseReleased(ofMouseEventArgs& data);
	
	
    // if you use a custom camera to fly through the scene
	// you must implement this method for the transitions to work properly
	//	ofCamera& getCameraRef(){
	//		return myCustomCamera;
	//	}
	
	
	ofCamera& getCameraRef(){
		//		return cloudsCamera;
		return camera;
	}
	
	
	void updateVbo();
	void explodeFireWork( ofVec3f origin=ofVec3f(), ofVec3f vel=ofVec3f() );
	void emitFromPoint( ofVec3f point, ofVec3f dir, float lifespan=ofRandom(1.,3.), float t=ofGetElapsedTimef() );
	
	void trailPoint( ofVec3f point, ofVec3f vel = ofVec3f(), int count = 10 );
	
	//camera
	ofEasyCam camera;
	ofShader shader;
	ofVbo vbo;
	ofVec3f camPos;
	ofVec3f camTarget;
	
	//particles
	ofVec3f* positions;
	ofVec3f* velocities;
	ofVec3f* baseVelocities;
	ofFloatColor* lifeData;
	ofIndexType* indices;
	int indexCount, nextIndex;
	float minExplosionTime, maxExplosionTime;
	
	bool bUpdateVbo;
	ofVec3f fireworkGravity, particleGravity;
	float minVel, maxVel;
	float maxFWVel;
	
	//firework explosion stuff
	vector<ofVec3f> spawnPos;
	vector<ofVec3f> spawnVel;
	vector<float> spawnTime;
	vector <float> startTimes;
	float fireWorkExplosionTime;
	
	//render attributes
	ofVec4f startColor, endColor;
	
	ofxUISuperCanvas* fireworksRenderGui;
	ofxUISuperCanvas* fireworksBehaviorGui;
	
	float camSpeed;
	
	ofImage spriteImage;
	
	float nextFireworkExplosionTime;
	void explodeFireWorkAtRandomPoint();
	void explodeFireWorkAtPoint(ofVec3f point, float t=ofGetElapsedTimef() );
	
	ofVec3f nextExplosion;
	
protected:
	
	ofxUISuperCanvas* customGui;
	bool customToggle;
	float customFloat1;
	float customFloat2;
	
	float farClip, nearClip;
	
	ofImage someImage;
	ofShader pointcloudShader;
	ofVboMesh simplePointcloud;
	
	int FIREWORKS_NUM_PARTICLES;
	
	map<string, ofImage> sprites;
	
	float minLifeSpan, maxLifeSpan;
	ofImage colorSampleImage;
	ofxUIImageSampler* startColorSampler;
	ofxUIImageSampler* endColorSampler;
	
	vector <ofVec3f> debugSpheres;
};
