/*
 * gApp.h
 *
 *  Created on: May 6, 2020
 *      Author: noyan
 */

#ifndef EXAMPLES_TESTEXAMPLE_GAPP_H_
#define EXAMPLES_TESTEXAMPLE_GAPP_H_

#include "gBaseApp.h"
#include "gipOSC.h"


class gApp : public gBaseApp {
public:
	gApp();
	~gApp();

	void setup();
	void update();

	void oscMessageReceived(std::string message);
	void oscIntegerReceived(int value);
	void oscFloatReceived(float value);
	void oscBoolReceived(bool value);

private:
	gipOSC osc;
	bool datasent;
};

#endif /* EXAMPLES_TESTEXAMPLE_GAPP_H_ */
