/****************************************************************************
 * Copyright (c) 2014 Nitra Games Ltd., Istanbul, Turkey                    *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, distribute with modifications, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the Software is    *
 * furnished to do so, subject to the following conditions:                 *
 *                                                                          *
 * The above copyright notice and this permission notice should not be      *
 * deleted from the source form of the Software.                            *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
 * IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,   *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR    *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR    *
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
 *                                                                          *
 * Except as contained in this notice, the name(s) of the above copyright   *
 * holders shall not be used in advertising or otherwise to promote the     *
 * sale, use or other dealings in this Software without prior written       *
 * authorization.                                                           *
 ****************************************************************************/

/*
 * gipOSC.h
 *
 *  Created on: Mar 3, 2023
 *      Author: Noyan Culum
 */

#ifndef SRC_GIPOSC_H_
#define SRC_GIPOSC_H_

#include "gBaseComponent.h"
#include "osc/OscOutboundPacketStream.h"
#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "ip/UdpSocket.h"

/*
 * A GlistEngine component that implements app to app communication
 * using Open Sound Control (OSC) packet manipulation library.
 *
 * The apps communicate via UDP packets sent from one to other.
 *
 * The app can act as sender or receiver. Developers should use
 * sendMEssage() and receiveMessage() functions to generate the data
 * transmission.
 */
class gipOSC : public gBaseComponent {
public:
	static const int DEFAULTPORT_REMOTE = 7000;
	static const int DEFAULTPORT_LOCAL = 7000;
	static const int OUTPUT_BUFFER_SIZE = 65535;
	static const unsigned int INPUT_BUFFER_SIZE = 65535;

	gipOSC();
	virtual ~gipOSC();

	bool initialize(std::string remoteReceiverIp, int remoteReceiverPort = DEFAULTPORT_REMOTE, int localListenerPort = DEFAULTPORT_LOCAL);
	void sendMessage(std::string message);
	std::string receiveMessage();

private:
	class gipOscPackListener : public osc::OscPacketListener {
	public:
		gipOscPackListener();
		virtual ~gipOscPackListener();
		void ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint);
	};

	std::string remoteip;
	int remoteport;
	int localport;
	UdpTransmitSocket* transmitSocket;
	char* buffer, *receivebuffer;
	static std::string receivedmessage;
	gipOscPackListener listener;
	UdpListeningReceiveSocket* s;
	UdpReceiveSocket* receiveSocket;
	IpEndpointName localendpoint;
};

#endif /* SRC_GIPOSC_H_ */
