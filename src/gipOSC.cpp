/*
 * gipOSC.cpp
 *
 *  Created on: 7 Mar 2022
 *      Author: Noyan
 */

#include "gipOSC.h"
#include <fcntl.h>

std::string gipOSC::receivedmessage = "";


gipOSC::gipOSC() {
	remoteip = "127.0.0.1";
	remoteport = DEFAULTPORT_REMOTE;
	localport = DEFAULTPORT_LOCAL;
	receivedmessage = "";
}

gipOSC::~gipOSC() {
	if(transmitSocket != nullptr) delete transmitSocket;
	if(buffer != nullptr) delete buffer;
	if(s != nullptr) delete s;
}

bool gipOSC::initialize(std::string remoteReceiverIp, int remoteReceiverPort, int localListenerPort) {
	remoteip = remoteReceiverIp;
	remoteport = remoteReceiverPort;
	localport = localListenerPort;
	buffer = new char[OUTPUT_BUFFER_SIZE];
	receivebuffer = new char[OUTPUT_BUFFER_SIZE];

	try{
		transmitSocket = new UdpTransmitSocket(IpEndpointName(remoteip.c_str(), remoteport));
	} catch(std::exception &e){
		gLoge("gipOSC") << "Sender socket is not created!";
		if(transmitSocket != nullptr){
			delete transmitSocket;
			transmitSocket = nullptr;
		}
		return false;
	}
	gLogi("gipOSC") << "Sender socket is created!";

	try{
		localendpoint = IpEndpointName(IpEndpointName::ANY_ADDRESS, localport);
		receiveSocket = new UdpReceiveSocket(localendpoint);
//		s = new UdpListeningReceiveSocket(IpEndpointName(IpEndpointName::ANY_ADDRESS, localport), &listener);
	} catch(std::exception &e){
		gLoge("gipOSC") << "Receiver socket is not created!";
		if(s != nullptr){
			delete s;
			s = nullptr;
		}
		return false;
	}
	gLogi("gipOSC") << "Receiver socket is created!";

	int flags = fcntl(receiveSocket->Socket(), F_GETFL, 0);
	if (flags == -1) gLogi("gipOSC") << "No unblocking!";
	flags = false ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
	fcntl(receiveSocket->Socket(), F_SETFL, flags);


	return true;
}

void gipOSC::sendMessage(std::string message) {
    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    p << osc::BeginBundleImmediate << osc::BeginMessage("1") << message.c_str() << osc::EndMessage << osc::EndBundle;
    transmitSocket->Send(p.Data(), p.Size());
}

std::string gipOSC::receiveMessage() {
//    s->Run();
//    s->RunUntilSigInt();

//	gLogi("gipOSC") << "receiveMessage 1, isbound:" << receiveSocket->IsBound();
	std::size_t size = receiveSocket->ReceiveFrom(localendpoint, receivebuffer, INPUT_BUFFER_SIZE);
//	gLogi("gipOSC") << "receiveMessage 2";
	if(size > 0) listener.ProcessPacket(receivebuffer, size, localendpoint);
//	gLogi("gipOSC") << "receiveMessage 3";

	std::string rm = receivedmessage;
	receivedmessage = "";
	return rm;
}

gipOSC::gipOscPackListener::gipOscPackListener() {

}

gipOSC::gipOscPackListener::~gipOscPackListener() {

}

void gipOSC::gipOscPackListener::ProcessMessage(const osc::ReceivedMessage &m,
		const IpEndpointName &remoteEndpoint) {
    (void) remoteEndpoint; // suppress unused parameter warning

    try {
        if(std::strcmp(m.AddressPattern(), "1") == 0) {
            osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
            const char *a4;
            args >> a4 >> osc::EndMessage;
            receivedmessage = std::string(a4);

//            std::cout << "received '1' message with arguments:" << receivedmessage << "\n";
        }
    } catch(osc::Exception& e) {
        std::cout << "error while parsing message";
    }
}

