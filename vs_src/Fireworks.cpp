//
//  CloudsVisualSystemFireworks.cpp
//
//  Created by Patricio Gonzalez Vivo on 7/23/13.
//
//

#include "Fireworks.h"

string CloudsVisualSystemFireworks::getSystemName()
{
	return "Fireworks";
}

void CloudsVisualSystemFireworks::selfSetup()
{
    
}

void CloudsVisualSystemFireworks::selfPresetLoaded(string presetPath){
	
}

void CloudsVisualSystemFireworks::selfBegin()
{
	
//	cout << "SelfBigin: "<<endl<<endl;
	
	shader.load("shaders/base.vert", "shaders/base.frag");
	
	shader.begin();
	shader.setUniform3f( "gravity", 0, -98, 0 );
	shader.end();
	
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
    
}


void CloudsVisualSystemFireworks::updateVbo(){
	int total = FIREWORKS_NUM_PARTICLES;
	vbo.updateVertexData( &positions[0], total );
	vbo.updateNormalData( &velocities[0], total );
	vbo.updateTexCoordData( &lifeData[0], total );
	
	vbo.updateIndexData( &indices[0], indexCount );
	
	bUpdateVbo = false;
}

void CloudsVisualSystemFireworks::trailPoint( ofVec3f point, ofVec3f vel, int count ){
	for(int i=0; i<count; i++){
		emitFromPoint( point, vel, ofRandom(1.5, 3), ofGetElapsedTimef() );
	}
}


void CloudsVisualSystemFireworks::emitFromPoint( ofVec3f point, ofVec3f dir, float lifespan, float t ){
	
	
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

void CloudsVisualSystemFireworks::explodeFireWork( ofVec3f origin, ofVec3f vel ){
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

void CloudsVisualSystemFireworks::selfUpdate()
{
    
	//camera
	camPos = camera.getPosition();
	camTarget = camera.getTarget().getPosition();
	
	ofVec3f cameraAim = (camTarget - camPos).normalize();
	
	camPos += cameraAim * 3;
	camTarget += cameraAim * 3;
	
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
		cout << indexCount << endl;
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
	ofVec3f camPos = camera.getPosition();
	shader.setUniform3f("cameraPosition", camPos.x, camPos.y, camPos.z );
	
	vbo.drawElements( GL_POINTS, indexCount-1 );
	
	shader.end();
	
	camera.end();
	
	ofPopStyle();

	//    mat->end();
    glDisable(GL_NORMALIZE);
    glDisable(GL_DEPTH_TEST);
    
}


void CloudsVisualSystemFireworks::selfSetupGuis()
{
    
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
		
		ofVec3f offset( ofRandom(-1, 1), ofRandom(-.1,.1), ofRandom(0, 1));
		offset.normalize();
		offset *= 300;
		explodeFireWork( camTarget + offset );
	}
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