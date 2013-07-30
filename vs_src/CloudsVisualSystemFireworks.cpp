//
//  CloudsVisualSystemFireworks.cpp
//

#include "CloudsVisualSystemFireworks.h"
#include "CloudsRGBDVideoPlayer.h"

//#include "CloudsRGBDVideoPlayer.h"
//#ifdef AVF_PLAYER
//#include "ofxAVFVideoPlayer.h"
//#endif

//These methods let us add custom GUI parameters and respond to their events
void CloudsVisualSystemFireworks::selfSetupGui(){
	
	customGui = new ofxUISuperCanvas("CUSTOM", gui);
	customGui->copyCanvasStyle(gui);
	customGui->copyCanvasProperties(gui);
	customGui->setName("Custom");
	customGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
	
	customGui->addSlider("minLifeSpan", .1, 10, &minLifeSpan);
	customGui->addSlider("maxLifeSpan", 1, 10, &maxLifeSpan);
	
	customGui->addSlider("minExplosionTime", .1, 2, &minExplosionTime);
	customGui->addSlider("maxExplosionTime", .5, 5, &maxExplosionTime);
	
	customGui->addSlider("minParticleVelocity", 0, 100, &minVel);
	customGui->addSlider("maxParticleVelocity", 0, 200, &maxVel );
	customGui->addSlider("maxFireworkVelocity", 1, 5, &maxFWVel );
	
	customGui->addSlider("particle gravity", -100, 100, &(particleGravity.y) );
	
	customGui->addSlider("firework gravity", -1, 1, &(fireworkGravity.y) );
	
	customGui->addSlider("camSpeed", -.1, 5, &camSpeed );
	
	
	
	
	//	customGui->addButton("Custom Button", false);
	customGui->addToggle("Custom Toggle", &customToggle);
	
	startColorSampler =  customGui->addImageSampler("birth color map", &colorSampleImage, (float)colorSampleImage.getWidth()/2, (float)colorSampleImage.getHeight()/2 );
	
	endColorSampler =  customGui->addImageSampler("death color map", &colorSampleImage, (float)colorSampleImage.getWidth()/2, (float)colorSampleImage.getHeight()/2 );
	
	
	//	customGui->addSlider("farClip", 100, 10000, &farClip);
	
	ofAddListener(customGui->newGUIEvent, this, &CloudsVisualSystemFireworks::selfGuiEvent);
	
	guis.push_back(customGui);
	guimap[customGui->getName()] = customGui;
}

void CloudsVisualSystemFireworks::selfGuiEvent(ofxUIEventArgs &e){
	if(e.widget->getName() == "Custom Button"){
		cout << "Button pressed!" << endl;
	}
	
	else if( e.widget->getName() == "birth color map"){
		
		ofxUIImageSampler* sampler = (ofxUIImageSampler *) e.widget;
		ofFloatColor col =  sampler->getColor();
		startColor.set( col.r, col.g, col.b, 1. );
		
		cout << startColor << endl;
	}
	else if( e.widget->getName() == "death color map"){
		
		ofxUIImageSampler* sampler = (ofxUIImageSampler *) e.widget;
		ofFloatColor col =  sampler->getColor();
		endColor.set( col.r, col.g, col.b, 1. );
	}
	//	else if( e.widget->getName() == "farClip"){
	//
	//		float clip = ((ofxUISlider *) e.widget)->getValue();
	//		camera.setFarClip( 500 );
	//		camera.setNearClip( 10 );
	//	}
}

//Use system gui for global or logical settings, for exmpl
void CloudsVisualSystemFireworks::selfSetupSystemGui(){
	
}

void CloudsVisualSystemFireworks::guiSystemEvent(ofxUIEventArgs &e){
	
}
//use render gui for display settings, like changing colors
void CloudsVisualSystemFireworks::selfSetupRenderGui(){
	
}

void CloudsVisualSystemFireworks::guiRenderEvent(ofxUIEventArgs &e){
	
}

// selfSetup is called when the visual system is first instantiated
// This will be called during a "loading" screen, so any big images or
// geometry should be loaded here
void CloudsVisualSystemFireworks::selfSetup(){
	
	/*
	 //	if(ofFile::doesFileExist(getVisualSystemDataPath() + "TestVideo/Jer_TestVideo.mov")){
	 //		getRGBDVideoPlayer().setup(getVisualSystemDataPath() + "TestVideo/Jer_TestVideo.mov",
	 //								   getVisualSystemDataPath() + "TestVideo/Jer_TestVideo.xml" );
	 //
	 //		getRGBDVideoPlayer().swapAndPlay();
	 //
	 //		for(int i = 0; i < 640; i += 2){
	 //			for(int j = 0; j < 480; j+=2){
	 //				simplePointcloud.addVertex(ofVec3f(i,j,0));
	 //			}
	 //		}
	 //
	 //		pointcloudShader.load(getVisualSystemDataPath() + "shaders/rgbdcombined");
	 //
	 //	}
	 //
	 
	 //	someImage.loadImage( getVisualSystemDataPath() + "images/someImage.png";
	 */
	
	FIREWORKS_NUM_PARTICLES = 200000;
	
	positions = new ofVec3f[ FIREWORKS_NUM_PARTICLES ];
	velocities = new ofVec3f[ FIREWORKS_NUM_PARTICLES ];
	lifeData = new ofFloatColor[ FIREWORKS_NUM_PARTICLES ];
	indices = new ofIndexType[ FIREWORKS_NUM_PARTICLES ];
	
	minVel = 0; maxVel = 200;
	
	float lifespan;
	float t = ofGetElapsedTimef();
	
	for (int i=0; i<FIREWORKS_NUM_PARTICLES; i++) {
		//		indices[i] = i;
		//		lifeData[i].r = 0;
		//		lifeData[i].g = .1;
	}
	
	explodeFireWorkAtRandomPoint();
	
	vbo.setVertexData( &positions[0], FIREWORKS_NUM_PARTICLES, GL_DYNAMIC_DRAW );
	vbo.setNormalData( &velocities[0], FIREWORKS_NUM_PARTICLES, GL_DYNAMIC_DRAW );
	vbo.setIndexData( &indices[0], FIREWORKS_NUM_PARTICLES, GL_DYNAMIC_DRAW );
	
	
	vbo.setColorData( &lifeData[0], FIREWORKS_NUM_PARTICLES, GL_DYNAMIC_DRAW );
	
	//	indexCount = 0;// FIREWORKS_NUM_PARTICLES;
	
	colorSampleImage.loadImage( getVisualSystemDataPath() + "GUI/defaultColorPalette.png" );
}

// selfPresetLoaded is called whenever a new preset is triggered
// it'll be called right before selfBegin() and you may wish to
// refresh anything that a preset may offset, such as stored colors or particles
void CloudsVisualSystemFireworks::selfPresetLoaded(string presetPath){
	
}

// selfBegin is called when the system is ready to be shown
// this is a good time to prepare for transitions
// but try to keep it light weight as to not cause stuttering
void CloudsVisualSystemFireworks::selfBegin(){
	
	//particle behavior
	fireworkGravity.set(0, -6 / 120., 0 );
	particleGravity.set( 0, 40, 0);
	minVel = 4;
	maxVel = 60;
	maxFWVel = 2.4;
	
	
	//shader
	shader.load(getVisualSystemDataPath() + "shaders/base.vert", getVisualSystemDataPath() + "shaders/base.frag");
	
	shader.begin();
	shader.setUniform3f( "gravity", particleGravity.x, particleGravity.y, particleGravity.z );
	shader.end();
	startColor.set( .9, .95, 1.95, 1 );
	endColor.set( .6, 1.3, .2, 1 );
	
	spriteImage.loadImage(getVisualSystemDataPath() + "images/sphereNormal.png");
	
	minLifeSpan = 2;
	maxLifeSpan = 4;
	
	//camera
	camSpeed = 2;
	
	//particle rendering
	bUpdateVbo = true;
	indexCount = 0;
	nextIndex = 0;
	
	nextFireworkExplosionTime = ofGetElapsedTimef() + 1;
	
	ofDisableArbTex();
	
	//	sprites["star"].loadImage(  getVisualSystemDataPath() + "images/star.png" );
	sprites["triangle"].loadImage(  getVisualSystemDataPath() + "images/triangle-sprite.png" );
	sprites["square"].loadImage(  getVisualSystemDataPath() + "images/square-sprite.png" );
	sprites["circle"].loadImage(  getVisualSystemDataPath() + "images/circle-sprite.png" );
	
	ofEnableArbTex();
	
	//	explodeFireWorkAtRandomPoint();
	
}

//do things like ofRotate/ofTranslate here
//any type of transformation that doesn't have to do with the camera
void CloudsVisualSystemFireworks::selfSceneTransformation(){
	
}

//normal update call
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
	
	nearClip = 10;
	farClip = 1000;
	camera.setFarClip( farClip );
	camera.setNearClip( nearClip );
	
	
	//particles
	bool updateIndices = false;
	float t = ofGetElapsedTimef();
	
	indexCount = 0;
	for(int i=0; i<FIREWORKS_NUM_PARTICLES; i++){
		//if the age + lifespan is less then the current time we want to draw
		if(lifeData[i].r + lifeData[i].g > t){
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
	for (int i = spawnTime.size()-1; i>=0; i--) {
		//if it's alive apply gravity
		if(spawnTime[i] + 2 > t ){
			spawnPos[i] += spawnVel[i];
			spawnVel[i] += fireworkGravity;
			spawnVel[i] *= .99;
			
			trailPoint( spawnPos[i], spawnVel[i].normalized() );
		}
		else{
			//			if(nextFireworkExplosionTime < t){
			//				debugSpheres.push_back( spawnPos[i] );
			//				cout << "debugSpheres.size(): "<< debugSpheres.size() << endl;
			//
			//				explodeFireWorkAtPoint( spawnPos[i] );
			//			}
			
			spawnPos.erase( spawnPos.begin()+i );
			spawnVel.erase( spawnVel.begin()+i );
			spawnTime.erase( spawnTime.begin()+i );
		}
	}
	
	if(	bUpdateVbo ){
		updateVbo();
	}
	
	if( nextFireworkExplosionTime < t ){
		
		explodeFireWorkAtRandomPoint();
	}
}

// selfDraw draws in 3D using the default ofEasyCamera
// you can change the camera by returning getCameraRef()
void CloudsVisualSystemFireworks::selfDraw(){
	
	//	ofPushMatrix();
	//	setupRGBDTransforms();
	//	pointcloudShader.begin();
	//	getRGBDVideoPlayer().setupProjectionUniforms(pointcloudShader);
	//	simplePointcloud.drawVertices();
	//	pointcloudShader.end();
	//	ofPopMatrix();
	
	
	
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
	//    mat->begin();
	
	ofPushStyle();
	
	ofEnableAlphaBlending();
	
	//	ofSetColor(255, 0, 0 );
	//	for (int i=0; i < spawnPos.size(); i++) {	ofSphere( spawnPos[i], 2 );	}
	//
	//	ofSetColor(255, 0, 0 );
	//	for (int i=0; i < debugSpheres.size(); i++) {	ofSphere( debugSpheres[i], 2 );	}
	
	
	ofColor( 255, 255, 255, 50 );
	
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_VERTEX_PROGRAM_POINT_SIZE );
	
	ofBlendMode( OF_BLENDMODE_ADD );
	
	ofDisableArbTex();
	ofEnablePointSprites();
	
	shader.begin();
	shader.setUniform1f( "time", ofGetElapsedTimef() );
	shader.setUniform1f( "nearClip", camera.getNearClip() );
	shader.setUniform1f( "farClip", camera.getFarClip() );
	shader.setUniform3f("cameraPosition", camPos.x, camPos.y, camPos.z );
	shader.setUniform4f("startColor", startColor.x, startColor.y, startColor.z, startColor.w );
	shader.setUniform4f("endColor", endColor.x, endColor.y, endColor.z, endColor.w );
	
	shader.setUniform3f( "gravity", particleGravity.x, particleGravity.y, particleGravity.z );
	
	shader.setUniformTexture("starMap", sprites["star"].getTextureReference(), 3 );
	shader.setUniformTexture("triangleMap", sprites["triangle"].getTextureReference(), 2 );
	shader.setUniformTexture("squareMap", sprites["square"].getTextureReference(), 1 );
	shader.setUniformTexture("circleMap", sprites["circle"].getTextureReference(), 0 );
	
	vbo.drawElements( GL_POINTS, indexCount );
	
	sprites["cicle"].unbind();
	sprites["square"].unbind();
	sprites["triangle"].unbind();
	sprites["star"].unbind();
	
	shader.end();
	
	ofPopStyle();
	
	//    mat->end();
    glDisable(GL_NORMALIZE);
    glDisable(GL_DEPTH_TEST);
	
	ofDisablePointSprites();
	ofEnableArbTex();
}


void CloudsVisualSystemFireworks::updateVbo()
{
	int total = FIREWORKS_NUM_PARTICLES;
	vbo.updateVertexData( &positions[0], total );
	vbo.updateNormalData( &velocities[0], total );
	vbo.updateColorData( &lifeData[0], total );
	
	vbo.updateIndexData( &indices[0], indexCount );
	
	bUpdateVbo = false;
}

void CloudsVisualSystemFireworks::trailPoint( ofVec3f point, ofVec3f vel, int count )
{
	for(int i=0; i<count; i++){
		emitFromPoint( point, vel, ofRandom(minLifeSpan, maxLifeSpan), ofGetElapsedTimef() );
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
	velocities[i] = ofVec3f(ofRandom( minVel, maxVel ),ofRandom( minVel, maxVel ),ofRandom( minVel, maxVel )) * rotQuat;
	lifeData[i].set( t, lifespan, 0, 0 );
	
	bUpdateVbo = true;
}

void CloudsVisualSystemFireworks::explodeFireWork( ofVec3f origin, ofVec3f vel )
{
	fireWorkExplosionTime = ofGetElapsedTimef();
	
	for (int i=0; i<30; i++) {
		spawnTime.push_back(fireWorkExplosionTime);
		spawnPos.push_back( origin );
		spawnVel.push_back( ofVec3f( ofRandom(-maxFWVel,maxFWVel), ofRandom(0,maxFWVel*2), ofRandom(-maxFWVel,maxFWVel) ) );
	}
	
}




void CloudsVisualSystemFireworks::explodeFireWorkAtPoint( ofVec3f point, float t )
{
	nextFireworkExplosionTime = t + ofRandom( minExplosionTime, maxExplosionTime );
	ofVec3f offset = point;
	explodeFireWork( camTarget + offset );
}

void CloudsVisualSystemFireworks::explodeFireWorkAtRandomPoint()
{
	float t = ofGetElapsedTimef();
	nextFireworkExplosionTime = t + ofRandom( minExplosionTime, maxExplosionTime );
	
	
	ofVec3f offset( ofRandom(-1, 1), ofRandom(-.1,.1), ofRandom(-1, 1));
	offset.normalize();
	offset *= 300;
	
	//	offset.x = abs( offset.x );
	//	offset.y = abs( offset.y );
	//	offset.z = abs( offset.z );
	
	
	int randFWType = ofRandom(0,5);
	switch (randFWType) {
		case 0:
			
			explodeFireWork( camTarget + offset );
			break;
			
		case 1:
			explodeFireWork( camTarget + offset );
			
			break;
			
		default:
			
			explodeFireWork( camTarget + offset );
			break;
	}
}

// draw any debug stuff here
void CloudsVisualSystemFireworks::selfDrawDebug(){
	
}
// or you can use selfDrawBackground to do 2D drawings that don't use the 3D camera
void CloudsVisualSystemFireworks::selfDrawBackground(){
	
	//turn the background refresh off
	//bClearBackground = false;
	
}
// this is called when your system is no longer drawing.
// Right after this selfUpdate() and selfDraw() won't be called any more
void CloudsVisualSystemFireworks::selfEnd(){
	
	//	simplePointcloud.clear();
	
    delete[] positions;
    delete[] velocities;
    delete[] baseVelocities;
    delete[] lifeData;
    delete[] indices;
	
	for (std::map<string, ofImage>::iterator it=sprites.begin(); it!=sprites.end(); ++it){
		it->second.clear();
	}
	
}
// this is called when you should clear all the memory and delet anything you made in setup
void CloudsVisualSystemFireworks::selfExit(){
	
}

//events are called when the system is active
//Feel free to make things interactive for you, and for the user!
void CloudsVisualSystemFireworks::selfKeyPressed(ofKeyEventArgs & args){
	
}
void CloudsVisualSystemFireworks::selfKeyReleased(ofKeyEventArgs & args){
	
}

void CloudsVisualSystemFireworks::selfMouseDragged(ofMouseEventArgs& data){
	
}

void CloudsVisualSystemFireworks::selfMouseMoved(ofMouseEventArgs& data){
	
}

void CloudsVisualSystemFireworks::selfMousePressed(ofMouseEventArgs& data){
	
}

void CloudsVisualSystemFireworks::selfMouseReleased(ofMouseEventArgs& data){
	
}