//
//  emptyVisualSystem.h
//
//  Created by Patricio Gonzalez Vivo on 7/23/13.
//
//

#pragma once

#include "CloudsVisualSystem.h"
#define FIREWORKS_NUM_PARTICLES 100000

class CloudsVisualSystemFireworks : public CloudsVisualSystem {
public:
    
    string getSystemName();
    
    void selfSetup();
    void selfSetupGuis();
    
	void selfPresetLoaded(string presetPath);
	
    void selfAutoMode();
    void selfUpdate();
    void selfDrawBackground();
    void selfDrawDebug();
    void selfSceneTransformation();
    void selfDraw();
    void selfExit();
    void selfBegin();
	void selfEnd();
    
    void selfKeyPressed(ofKeyEventArgs & args);
    void selfKeyReleased(ofKeyEventArgs & args);
    
    void selfMouseDragged(ofMouseEventArgs& data);
    void selfMouseMoved(ofMouseEventArgs& data);
    void selfMousePressed(ofMouseEventArgs& data);
    void selfMouseReleased(ofMouseEventArgs& data);
	
	void selfSetupTimeline();
    
    void selfSetupGui();
    void selfGuiEvent(ofxUIEventArgs &e);
    
    void selfSetupSystemGui();
    void guiSystemEvent(ofxUIEventArgs &e);
    
    void selfSetupRenderGui();
    void guiRenderEvent(ofxUIEventArgs &e);
	
	//END TEMPLATE
	
	ofCamera* getCameraRef();
	
	
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
	ofVec2f* lifeData;
	ofIndexType* indices;
	int indexCount, nextIndex;
	
	bool bUpdateVbo;
	
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
	

    
protected:
    
    //  Your Stuff
    //
};
