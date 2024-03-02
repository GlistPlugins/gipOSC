/*
 * gipOSC.cpp
 *
 *  Created on: 7 Mar 2022
 *      Author: Noyan
 */

#include "gipOSC.h"
#if not defined(WIN32)
#include <fcntl.h>
#else
#include <winsock.h>
#endif


gipOSC::gipOSC() {
	remoteip = "127.0.0.1";
	remoteport = DEFAULTPORT_REMOTE;
	localport = DEFAULTPORT_LOCAL;
	transmitsocket = nullptr;
	receivebuffer = nullptr;
	receivesocket = nullptr;
	buffer = nullptr;
}

gipOSC::~gipOSC() {
	if(transmitsocket != nullptr) delete transmitsocket;
	if(buffer != nullptr) delete buffer;
	if(receivesocket != nullptr) delete receivesocket;
	if(receivebuffer != nullptr) delete receivebuffer;
}

bool gipOSC::initialize(std::string remoteReceiverIp, int remoteReceiverPort, int localListenerPort) {
	remoteip = remoteReceiverIp;
	remoteport = remoteReceiverPort;
	localport = localListenerPort;
	buffer = new char[OUTPUT_BUFFER_SIZE];
	receivebuffer = new char[OUTPUT_BUFFER_SIZE];

	try{
		transmitsocket = new UdpTransmitSocket(IpEndpointName(remoteip.c_str(), remoteport));
	} catch(std::exception &e){
		gLoge("gipOSC") << "Sender socket is not created!";
		if(transmitsocket != nullptr){
			delete transmitsocket;
			transmitsocket = nullptr;
		}
		return false;
	}
	gLogi("gipOSC") << "Sender socket is created!";

	try{
		localendpoint = IpEndpointName(IpEndpointName::ANY_ADDRESS, localport);
		receivesocket = new UdpReceiveSocket(localendpoint);
	} catch(std::exception &e){
		gLoge("gipOSC") << "Receiver socket is not created!";
		return false;
	}
	gLogi("gipOSC") << "Receiver socket is created!";

#if not defined(WIN32)
	int flags = fcntl(receivesocket->Socket(), F_GETFL, 0);
	if (flags < 0) { // error getting flags
		gLogw("gipOSC") << "No unblocking, cannot get flags!";
	} else {
		flags |= O_NONBLOCK;
		if (fcntl(receivesocket->Socket(), F_SETFL, flags) < 0) {
			gLogw("gipOSC") << "No unblocking, cannot set flags!";
		}
	}
#else
	unsigned long on = 1;
	if (0 != ioctlsocket(receivesocket->Socket(), FIONBIO, &on)) {
		gLogw("gipOSC") << "No unblocking!";
	}
#endif

	return true;
}

void gipOSC::setMessageCallback(std::function<void(std::string)> messageCallback) {
	listener.setMessageCallback(messageCallback);
}

void gipOSC::setIntegerCallback(std::function<void(int)> integerCallback) {
	listener.setIntegerCallback(integerCallback);
}

void gipOSC::setFloatCallback(std::function<void(float)> floatCallback) {
	listener.setFloatCallback(floatCallback);
}

void gipOSC::setBoolCallback(std::function<void(bool)> boolCallback) {
	listener.setBoolCallback(boolCallback);
}


void gipOSC::update() {
	receive();
}

void gipOSC::sendMessage(std::string message) {
	if(transmitsocket == nullptr) {
		gLoge("gipOSC") << "Transmit socket is not ready!";
		return;
	}

    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    p << osc::BeginBundleImmediate << osc::BeginMessage("/s") << message.c_str() << osc::EndMessage << osc::EndBundle;
    transmitsocket->Send(p.Data(), p.Size());
}

void gipOSC::sendInteger(int value) {
	if(transmitsocket == nullptr) {
		gLoge("gipOSC") << "Transmit socket is not ready!";
		return;
	}

    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    p << osc::BeginBundleImmediate << osc::BeginMessage("/i") << value << osc::EndMessage << osc::EndBundle;
    transmitsocket->Send(p.Data(), p.Size());
}

void gipOSC::sendFloat(float value) {
	if(transmitsocket == nullptr) {
		gLoge("gipOSC") << "Transmit socket is not ready!";
		return;
	}

    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    p << osc::BeginBundleImmediate << osc::BeginMessage("/f") << value << osc::EndMessage << osc::EndBundle;
    transmitsocket->Send(p.Data(), p.Size());
}

void gipOSC::sendBool(bool value) {
	if(transmitsocket == nullptr) {
		gLoge("gipOSC") << "Transmit socket is not ready!";
		return;
	}

    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    p << osc::BeginBundleImmediate << osc::BeginMessage("/B") << value << osc::EndMessage << osc::EndBundle;
    transmitsocket->Send(p.Data(), p.Size());
}


void gipOSC::receive() {
	if(receivesocket == nullptr) {
		gLoge("gipOSC") << "Receiver socket is not ready!";
		return;
	}

	std::size_t size = receivesocket->ReceiveFrom(localendpoint, receivebuffer, INPUT_BUFFER_SIZE);
	if(size > 0) listener.ProcessPacket(receivebuffer, size, localendpoint);
}

gipOSC::gipOscPackListener::gipOscPackListener() {
	messagecallback = nullptr;
	integercallback = nullptr;
	floatcallback = nullptr;
	boolcallback = nullptr;
}

gipOSC::gipOscPackListener::~gipOscPackListener() {

}

void gipOSC::gipOscPackListener::ProcessMessage(const osc::ReceivedMessage &m,
		const IpEndpointName &remoteEndpoint) {
    (void) remoteEndpoint; // suppress unused parameter warning

    try {
        if(std::strcmp(m.AddressPattern(), "/s") == 0) {
            osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
            const char *a4;
            args >> a4 >> osc::EndMessage;
            if(messagecallback != nullptr) messagecallback(a4);
        } else if(std::strcmp(m.AddressPattern(), "/i") == 0) {
            osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
            int i4;
            args >> i4 >> osc::EndMessage;
            if(integercallback != nullptr) integercallback(i4);
        } else if(std::strcmp(m.AddressPattern(), "/f") == 0) {
            osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
            float f4;
            args >> f4 >> osc::EndMessage;
            if(floatcallback != nullptr) floatcallback(f4);
        } else if(std::strcmp(m.AddressPattern(), "/B") == 0) {
            osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
            bool b4;
            args >> b4 >> osc::EndMessage;
            if(boolcallback != nullptr) boolcallback(b4);
        } else {
            osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
            const char *a4;
            args >> a4 >> osc::EndMessage;
            if(messagecallback != nullptr) messagecallback(a4);
        }
    } catch(osc::Exception& e) {
        std::cout << "error while parsing message";
    }
}

void gipOSC::gipOscPackListener::setMessageCallback(std::function<void(std::string)> messageCallback) {
	messagecallback = messageCallback;
}

void gipOSC::gipOscPackListener::setIntegerCallback(std::function<void(int)> integerCallback) {
	integercallback = integerCallback;
}

void gipOSC::gipOscPackListener::setFloatCallback(std::function<void(float)> floatCallback) {
	floatcallback = floatCallback;
}

void gipOSC::gipOscPackListener::setBoolCallback(std::function<void(bool)> boolCallback) {
	boolcallback = boolCallback;
}


