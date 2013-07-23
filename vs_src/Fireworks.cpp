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
    
}

void CloudsVisualSystemFireworks::selfEnd()
{
    
}

void CloudsVisualSystemFireworks::selfExit()
{
    
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
    
}

void CloudsVisualSystemFireworks::selfDraw()
{
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    mat->begin();

    
    
    mat->end();
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