/*
 * gApp.cpp
 *
 *  Created on: May 6, 2020
 *      Author: noyan
 */

#include "gApp.h"


gApp::gApp() {
}

gApp::gApp(int argc, char **argv) : gBaseApp(argc, argv) {
}

gApp::~gApp() {
}

void gApp::setup() {
	osc.initialize("127.0.0.1", 7000, 7001);
	osc.setMessageCallback(std::bind(&gApp::oscMessageReceived, this, std::placeholders::_1));
	osc.setIntegerCallback(std::bind(&gApp::oscIntegerReceived, this, std::placeholders::_1));
	osc.setFloatCallback(std::bind(&gApp::oscFloatReceived, this, std::placeholders::_1));
	osc.setBoolCallback(std::bind(&gApp::oscBoolReceived, this, std::placeholders::_1));
}

void gApp::update() {
}

void gApp::oscMessageReceived(std::string message) {
	gLogi("gApp") << "oscMessageReceived:" << message;

	osc.sendMessage("Reply for " + message);
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

