//
//  CloudsVisualSystemFireworks.cpp
//
//  Created by Lars Berhg
//
//

#include "CloudsVisualSystemFireworks.h"

string CloudsVisualSystemFireworks::getSystemName()
{
	return "Fireworks";
}

void CloudsVisualSystemFireworks::selfSetup()
{
    
}

void CloudsVisualSystemFireworks::selfPresetLoaded(string presetPath)
{
	
}

void CloudsVisualSystemFireworks::selfBegin()
{
	//shader
	shader.load(getVisualSystemDataPath() + "shaders/base.vert", getVisualSystemDataPath() + "shaders/base.frag");
	
	shader.begin();
	shader.setUniform3f( "gravity", 0, -98, 0 );
	shader.end();
	startColor.set( .9, .95, 1.95, 1 );
	endColor.set( .6, 1.3, .2, 1 );
	
	spriteImage.loadImage(getVisualSystemDataPath() + "images/sphereNormal.png");
	
	
	
	//camera
	camSpeed = 4;
	
	
	//particle rendering 
	bUpdateVbo = true;
	indexCount = 0;
	nextIndex = 0;
	
	positions = new ofVec3f[ FIREWORKS_NUM_PARTICLES ];
	velocities = new ofVec3f[ FIREWORKS_NUM_PARTICLES ];
	baseVelocities = new ofVec3f[ FIREWORKS_NUM_PARTICLES ];
	lifeData = new ofVec2f[ FIREWORKS_NUM_PARTICLES ];
	indices = new ofIndexType[ FIREWORKS_NUM_PARTICLES ];
	
	float lifespan;
	float t = ofGetElapsedTimef();
	
	for (int i=0; i<FIREWORKS_NUM_PARTICLES; i++) {
		
		positions[i].set( ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300) );
		
		ofVec3f vel;
		vel.set( ofRandom(-1,1),ofRandom(-.5,0),ofRandom(-1,1) );
		vel.normalize();
		vel *= ofRandom( 50, 150 );
		
		velocities[i] = vel;
		baseVelocities[i] = vel;
		
		lifeData[i].set( ofGetElapsedTimef(), 0 );
		
		indices[i] = i;
		
	}
	
	vbo.setVertexData( &positions[0], FIREWORKS_NUM_PARTICLES, GL_DYNAMIC_DRAW );
	vbo.setNormalData( &velocities[0], FIREWORKS_NUM_PARTICLES, GL_DYNAMIC_DRAW );
	vbo.setTexCoordData( &lifeData[0], FIREWORKS_NUM_PARTICLES, GL_DYNAMIC_DRAW );
	vbo.setIndexData( &indices[0], FIREWORKS_NUM_PARTICLES, GL_DYNAMIC_DRAW );
	
	indexCount = FIREWORKS_NUM_PARTICLES;
	
	nextFireworkExplosionTime = 1;
    
}

void CloudsVisualSystemFireworks::selfUpdate()
{
    
	//camera
	camPos = camera.getPosition();
	camTarget = camera.getTarget().getPosition();
	
	ofVec3f cameraAim = (camTarget - camPos).normalize();
	
	camPos += cameraAim * camSpeed;
	camTarget += cameraAim * camSpeed;
	
	camera.setPosition( camPos );
	camera.getTarget().setPosition( camTarget );
	
	
	//particles
	bool updateIndices = false;
	float t = ofGetElapsedTimef();
	
	indexCount = 0;
	for(int i=0; i<FIREWORKS_NUM_PARTICLES; i++){
		//if the age + lifespan is less then the current time we want to draw
		if(lifeData[i].x + lifeData[i].y > t){
			indices[indexCount] = i;
			indexCount++;
			updateIndices = true;
		}
	}
	
	if(updateIndices){
		vbo.updateIndexData( indices, indexCount );
//		cout << indexCount << endl;
	}
	
	
	//fireworks...
	ofVec3f gravity(0, -9.8 / 120., 0 );
	for (int i = spawnTime.size()-1; i>=0; i--) {
		if(spawnTime[i] + 2 > t ){
			spawnPos[i] += spawnVel[i];
			spawnVel[i] += gravity;
			spawnVel[i] *= .99;
			
			trailPoint( spawnPos[i], spawnVel[i].normalized() );
			
		}else{
			spawnPos.erase( spawnPos.begin()+i );
			spawnVel.erase( spawnVel.begin()+i );
			spawnTime.erase( spawnTime.begin()+i );
		}
	}
	
	if(	bUpdateVbo ){
		updateVbo();
	}
	
	if( nextFireworkExplosionTime < t ){
		nextFireworkExplosionTime = t + ofRandom( .1, 1);
		
		explodeFireWorkAtRandomPoint();
	}
	
}

void CloudsVisualSystemFireworks::selfDraw()
{
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
	//    mat->begin();
	
	ofPushStyle();
	
	ofBackgroundGradient( ofColor(70,75,100), ofColor(0,0,0), OF_GRADIENT_CIRCULAR );
	
	ofEnableAlphaBlending();
	
	camera.begin();
	
	//	ofSetColor(255, 0, 0 );
	//	for (int i=0; i < spawnPos.size(); i++) {	ofSphere( spawnPos[i], 3 );	}
	
	ofColor( 255, 255, 255, 50 );
	
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_VERTEX_PROGRAM_POINT_SIZE );
	glEnable( GL_POINT_SIZE );
	
	ofBlendMode( OF_BLENDMODE_ADD );
	
	shader.begin();
	shader.setUniform1f( "time", ofGetElapsedTimef() );
	shader.setUniform3f("cameraPosition", camPos.x, camPos.y, camPos.z );
	shader.setUniform4f("startColor", startColor.x, startColor.y, startColor.z, startColor.w );
	shader.setUniform4f("endColor", endColor.x, endColor.y, endColor.z, endColor.w );
	shader.setUniformTexture("map", spriteImage, 0 );
	
	vbo.drawElements( GL_POINTS, indexCount-1 );
	
	shader.end();
	
	camera.end();
	
	ofPopStyle();
	
	//    mat->end();
    glDisable(GL_NORMALIZE);
    glDisable(GL_DEPTH_TEST);
    
}


void CloudsVisualSystemFireworks::updateVbo()
{
	int total = FIREWORKS_NUM_PARTICLES;
	vbo.updateVertexData( &positions[0], total );
	vbo.updateNormalData( &velocities[0], total );
	vbo.updateTexCoordData( &lifeData[0], total );
	
	vbo.updateIndexData( &indices[0], indexCount );
	
	bUpdateVbo = false;
}

void CloudsVisualSystemFireworks::trailPoint( ofVec3f point, ofVec3f vel, int count )
{
	for(int i=0; i<count; i++){
		emitFromPoint( point, vel, ofRandom(1.5, 3), ofGetElapsedTimef() );
	}
}

void CloudsVisualSystemFireworks::emitFromPoint( ofVec3f point, ofVec3f dir, float lifespan, float t )
{
	
	
	int i = nextIndex;
	nextIndex ++;
	if( nextIndex > FIREWORKS_NUM_PARTICLES ) nextIndex = 0;
	
	//TODO: pass in quat rather then dir
	ofQuaternion rotQuat;
	rotQuat.makeRotate( ofVec3f(0,1,0), dir);
	
	positions[i] = point;
	velocities[i] = baseVelocities[i] * rotQuat;
	lifeData[i].set( t, lifespan );
	
	
	bUpdateVbo = true;
	
}

void CloudsVisualSystemFireworks::explodeFireWork( ofVec3f origin, ofVec3f vel )
{
	fireWorkExplosionTime = ofGetElapsedTimef();
	
	float maxVel = 4;
	for (int i=0; i<10; i++) {
		spawnTime.push_back(fireWorkExplosionTime);
		spawnPos.push_back( origin );
		spawnVel.push_back( ofVec3f( ofRandom(-maxVel, maxVel), ofRandom(0, maxVel*2), ofRandom(-maxVel, maxVel) ) );
	}
	
}

void CloudsVisualSystemFireworks::selfEnd()
{
    
}

ofCamera* CloudsVisualSystemFireworks::getCameraRef()
{
	return &camera;
}

void CloudsVisualSystemFireworks::selfExit()
{
    delete[] positions;
    delete[] velocities;
    delete[] baseVelocities;
    delete[] lifeData;
    delete[] indices;
}

void CloudsVisualSystemFireworks::selfSetupSystemGui()
{
   
}

void CloudsVisualSystemFireworks::selfSetupRenderGui()
{
    
}

void CloudsVisualSystemFireworks::guiSystemEvent(ofxUIEventArgs &e)
{
    
}

void CloudsVisualSystemFireworks::selfKeyPressed(ofKeyEventArgs & args){
    
}

void CloudsVisualSystemFireworks::selfSetupGuis()
{
    
	fireworksRenderGui = new ofxUISuperCanvas("FW - rendering", gui);
    fireworksRenderGui->copyCanvasStyle(gui);
    fireworksRenderGui->copyCanvasProperties(gui);
	
    fireworksRenderGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
	fireworksRenderGui->addSlider("fwStartColor.r", 0, 1., &startColor.x );
	fireworksRenderGui->addSlider("fwStartColor.g", 0, 1., &startColor.y );
	fireworksRenderGui->addSlider("fwStartColor.b", 0, 1., &startColor.z );
	fireworksRenderGui->addSlider("fwStartColor.a", 0, 1., &startColor.w );
	fireworksRenderGui->addSlider("fwEndColor.r", 0, 1., &endColor.x );
	fireworksRenderGui->addSlider("fwEndColor.g", 0, 1., &endColor.y );
	fireworksRenderGui->addSlider("fwEndColor.b", 0, 1., &endColor.z );
	fireworksRenderGui->addSlider("fwEndColor.a", 0, 1., &endColor.w );
	
	fireworksBehaviorGui = new ofxUISuperCanvas("FW - behavior", gui);
	fireworksBehaviorGui->addSpacer( 100, 2);
	
    fireworksBehaviorGui->copyCanvasStyle(gui);
    fireworksBehaviorGui->copyCanvasProperties(gui);
	
	fireworksRenderGui->addSlider("camera speed", 1, 10, &camSpeed );
	
	
	ofAddListener(fireworksRenderGui->newGUIEvent, this, &CloudsVisualSystemFireworks::selfGuiEvent);
	
    guis.push_back(fireworksRenderGui);
    guimap[fireworksRenderGui->getName()] = fireworksRenderGui;
}

void CloudsVisualSystemFireworks::selfSetupTimeline(){
	
}

void CloudsVisualSystemFireworks::selfAutoMode()
{
    
}

void CloudsVisualSystemFireworks::selfDrawBackground()
{
    
}

void CloudsVisualSystemFireworks::selfDrawDebug()
{
    
}

void CloudsVisualSystemFireworks::selfSceneTransformation()
{
    
}

void CloudsVisualSystemFireworks::selfKeyReleased(ofKeyEventArgs & args)
{
	
 	if(args.key == ' '){
		explodeFireWorkAtRandomPoint();
	}
}

void CloudsVisualSystemFireworks::explodeFireWorkAtRandomPoint()
{
	ofVec3f offset( ofRandom(-1, 1), ofRandom(-.1,.1), ofRandom(0, 1));
	offset.normalize();
	offset *= 300;
	explodeFireWork( camTarget + offset );
}

void CloudsVisualSystemFireworks::selfMouseDragged(ofMouseEventArgs& data)
{
    
}

void CloudsVisualSystemFireworks::selfMouseMoved(ofMouseEventArgs& data)
{
    
}

void CloudsVisualSystemFireworks::selfMousePressed(ofMouseEventArgs& data)
{
    
}

void CloudsVisualSystemFireworks::selfMouseReleased(ofMouseEventArgs& data)
{
    
}

void CloudsVisualSystemFireworks::selfSetupGui()
{
    
}

void CloudsVisualSystemFireworks::selfGuiEvent(ofxUIEventArgs &e)
{
    
}

void CloudsVisualSystemFireworks::guiRenderEvent(ofxUIEventArgs &e)
{
    
}