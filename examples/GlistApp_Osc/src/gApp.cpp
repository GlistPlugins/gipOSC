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
	osc.setMessageCallback(std::bind(&gApp::oscMessageReceived, this, std::placeholders::_1));
	osc.setIntegerCallback(std::bind(&gApp::oscIntegerReceived, this, std::placeholders::_1));
	osc.setFloatCallback(std::bind(&gApp::oscFloatReceived, this, std::placeholders::_1));
	osc.setBoolCallback(std::bind(&gApp::oscBoolReceived, this, std::placeholders::_1));
	datasent = false;

	GameCanvas *cnv = new GameCanvas(this);
	appmanager->setCurrentCanvas(cnv);
}

void gApp::update() {
	if(!datasent) {
		std::string messagetosend = "Hello OSC!";
		osc.sendMessage(messagetosend);
		gLogi("gApp") << "Sent message:" << messagetosend;

		int integertosend = 25;
		osc.sendInteger(integertosend);
		gLogi("gApp") << "Sent integer:" << integertosend;

		float floattosend = 1.25f;
		osc.sendFloat(floattosend);
		gLogi("gApp") << "Sent float:" << floattosend;

		bool booltosend = true;
		osc.sendBool(booltosend);
		gLogi("gApp") << "Sent bool:" << booltosend;

		datasent = true;
	}
}

void gApp::oscMessageReceived(std::string message) {
	gLogi("gApp") << "oscMessageReceived:" << message;
}

void gApp::oscIntegerReceived(int value) {
	gLogi("gApp") << "oscIntegerReceived:" << value;
}

void gApp::oscFloatReceived(float value) {
	gLogi("gApp") << "oscFloatReceived:" << value;
}

void gApp::oscBoolReceived(bool value) {
	gLogi("gApp") << "oscBoolReceived:" << value;
}

