/*
 * AutoPlayer
 * https://github.com/toddberreth/AutoPlayer
 *
 * Copyright 2015, Todd Berreth
 *
 * Licensed under the MIT license:
 * http://www.opensource.org/licenses/MIT
 */

#include "autoPlayerData.h"

autoPlayerData::autoPlayerData(){

	windowWidth = origWindowWidth = WINDOW_WIDTH_DEFAULT; windowHeight = origWindowHeight = WINDOW_HEIGHT_DEFAULT;
    bFullScreen = false; backgroundColor.set(0,0,0);
    
    playType = LOOP;
	
	bDisplayOn = bAudioOn = true; globalAlpha = 0; bShowData = false; globalVolume = 0;
	
	frameNumber = 0;
    
    playDurationSeconds = (float)PLAY_DURATION_MIN; playDurationFrames = (int)(PLAY_DURATION_MIN * FRAME_RATE);
	
	globalAlphaStep = globalVolumeStep = (float)1/(GLOBAL_TRANSITION*FRAME_RATE);
	
	videosPlaying = soundsPlaying = 0; videosPlayingMax = VIDEOS_PLAYING_MAX_DEFAULT; soundsPlayingMax = SOUNDS_PLAYING_MAX_DEFAULT;
	
	tilesH = tilesH = 1;
	
	inputType = OFF;
    
    message = "";
    
    randomAlphaMinTyp = RANDOM_ALPHA_MIN_TYP;   randomAlphaMaxTyp = RANDOM_ALPHA_MAX_TYP;
    randomSpeedMinTyp = RANDOM_SPEED_MIN_TYP;   randomSpeedMaxTyp = RANDOM_SPEED_MAX_TYP;
    randomVolumeMinTyp = RANDOM_VOLUME_MIN_TYP; randomVolumeMaxTyp = RANDOM_VOLUME_MAX_TYP;
    randomDurationMinTyp = RANDOM_DURATION_MIN_TYP; randomVolumeMaxTyp = RANDOM_DURATION_MAX_TYP;
    
    camData = new CamData();
    camData->setup();
}

autoPlayerData::~autoPlayerData(){}

void autoPlayerData::update(){
	
	frameNumber++;
	if ((frameNumber == playDurationFrames) && (playType == LOOP)) frameNumber = 0;
	if ((frameNumber == playDurationFrames) && (playType == ONCE)) exit(0);
	
    if (!bFullScreen){windowWidth = ofGetWindowWidth(); windowHeight = ofGetWindowHeight();}
    else {windowWidth = ofGetScreenWidth(); windowHeight = ofGetScreenHeight();}
	
	if (bDisplayOn) globalAlpha+=globalAlphaStep; else globalAlpha-=globalAlphaStep;
	globalAlpha = ofClamp(globalAlpha, 0, 1);
	
	if (bAudioOn) globalVolume+=globalVolumeStep; else globalVolume-=globalVolumeStep;
	globalVolume = ofClamp(globalVolume, 0, 1);
	
	if (bPlayTransition){
		if ((bDisplayOn) && (frameNumber == (playDurationFrames - (GLOBAL_TRANSITION*FRAME_RATE)))&& (playType != ENDLESS)) bDisplayOn = false;
		if ((!bDisplayOn) && (frameNumber == 0)) bDisplayOn = true;
		
		if ((bAudioOn) && (frameNumber == (playDurationFrames - (GLOBAL_TRANSITION*FRAME_RATE)))&& (playType != ENDLESS)) bAudioOn = false;
		if ((!bAudioOn) && (frameNumber == 0)) bAudioOn = true;
	}
    
    camData->update();
}

void autoPlayerData::draw(){

    glPushMatrix();
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); glOrtho(0, windowWidth, windowHeight, 0, -500.0, 500.0);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    
    ofSetColor(255, 0, 0, 255);
    
    ofPushMatrix();
    ofTranslate(0, 0, 10);
    ofNoFill();
    
    ofDrawBitmapString( "AutoPlayer", 30, 30);
    ofDrawBitmapString( title, 30, 50);
    ofDrawBitmapString( "message: " + ofToString(message), 30, windowHeight - 20);
    ofDrawBitmapString( "(f)ullframe/window", 30, windowHeight - 40);
    ofDrawBitmapString( "(a)udio/soundtrack", 30, windowHeight - 60); ofDrawBitmapString(":", 180, windowHeight - 60);
    if (bAudioOn) ofDrawBitmapString("ON", 220, windowHeight - 60); else ofDrawBitmapString("OFF", 220, windowHeight - 60);
    ofDrawBitmapString( "(s)how data", 30, windowHeight - 80);
    ofDrawBitmapString( "sounds playing: " + ofToString((float)soundsPlaying, 0), 30, windowHeight - 140);
    ofDrawBitmapString( "videos playing: " + ofToString((float)videosPlaying, 0), 30, windowHeight - 160);
    string thisInput; if (inputType == OFF) thisInput = "OFF"; else if (inputType == MOUSE) thisInput = "MOUSE"; else if (inputType == CAMERA) thisInput = "CAMERA";
    ofDrawBitmapString( "input type: " + thisInput, 30, windowHeight - 180);
    if (inputType == MOUSE)  ofDrawBitmapString( "x: " + ofToString(xPerc) + " y: " + ofToString(yPerc), 220, windowHeight - 180);
    ofDrawBitmapString( "fps: " + ofToString(ofGetFrameRate(), 0), windowWidth - 100, 30);
    ofDrawBitmapString( "frame number: " + ofToString(frameNumber, 0), windowWidth - 300, 30);
    ofDrawBitmapString( "seconds: " + ofToString((float)frameNumber/FRAME_RATE, 2), windowWidth - 450, 30);
    
    
    ofDrawBitmapString( ofToString(windowWidth) + ", " + ofToString(windowHeight) , windowWidth - 450, 100);
    ofPopMatrix();
    
    ofSetColor(255);
    
    glPopMatrix();
}

void autoPlayerData::mouseMoved(int x, int y ){xPerc = (float)x/windowWidth; yPerc = (float)y/windowHeight;}