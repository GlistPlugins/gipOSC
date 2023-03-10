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

private:
	gipOSC osc;
	int messagesentno;
};

#endif /* EXAMPLES_TESTEXAMPLE_GAPP_H_ */
