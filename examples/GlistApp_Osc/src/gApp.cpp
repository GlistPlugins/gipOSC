/*
 * gApp.cpp
 *
 *  Created on: May 6, 2020
 *      Author: noyan
 */

#include "gApp.h"
#include "GameCanvas.h"


gApp::gApp() {
}

gApp::~gApp() {
}

void gApp::setup() {
	osc.initialize("127.0.0.1");
	messagesentno = 0;
	GameCanvas *cnv = new GameCanvas(this);
	appmanager->setCurrentCanvas(cnv);
}

void gApp::update() {
	messagesentno++;
	if(messagesentno < 3 || messagesentno == 10) {
		std::string messagetosend = "OSC message " + gToStr(messagesentno);
		osc.sendMessage(messagetosend);
		gLogi("gApp") << "Sent message:" << messagetosend;
	}
	std::string receivedmessage = osc.receiveMessage();
	if(receivedmessage != "") gLogi("gApp") << "Received message:" << receivedmessage;
}
