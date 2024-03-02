/*
 * gApp.h
 *
 *  Created on: May 6, 2020
 *      Author: noyan
 */

#ifndef GAPP_H_
#define GAPP_H_

#include "gBaseApp.h"
#include "gipOSC.h"


class gApp : public gBaseApp {
public:
	gApp();
	gApp(int argc, char **argv);
	~gApp();

	void setup();
	void update();

	void oscMessageReceived(std::string message);
	void oscIntegerReceived(int value);
	void oscFloatReceived(float value);
	void oscBoolReceived(bool value);


private:
	gipOSC osc;
};

#endif /* GAPP_H_ */
