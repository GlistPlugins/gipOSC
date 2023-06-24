cmake_minimum_required (VERSION 3.10.2)

##### PLUGIN NAME #####
set(pluginname gipOSC)


##### PLUGIN PATHS #####
set(PLUGIN_DIR ${PLUGINS_DIR}/${pluginname})
set(PLUGIN_LIBS_DIR ${PLUGIN_DIR}/libs)

if(WIN32)
 set(IpPath win32)
else()
 set(IpPath posix)
endif(WIN32)


##### PLUGIN SOURCES #####
list(APPEND PLUGIN_SRCS
			${PLUGIN_DIR}/src/gipOSC.cpp
			${PLUGIN_LIBS_DIR}/src/osc/OscPrintReceivedElements.cpp
			${PLUGIN_LIBS_DIR}/src/osc/OscReceivedElements.cpp
			${PLUGIN_LIBS_DIR}/src/osc/OscOutboundPacketStream.cpp
			${PLUGIN_LIBS_DIR}/src/osc/OscTypes.cpp
			${PLUGIN_LIBS_DIR}/src/ip/IpEndpointName.cpp
			${PLUGIN_LIBS_DIR}/src/ip/${IpPath}/NetworkingUtils.cpp
			${PLUGIN_LIBS_DIR}/src/ip/${IpPath}/UdpSocket.cpp
)


##### PLUGIN HEADERS #####
list(APPEND PLUGIN_INCLUDES
			${PLUGIN_DIR}/src
			${PLUGIN_LIBS_DIR}/include
			${PLUGIN_LIBS_DIR}/src
)


##### PLUGIN DEPENDENCY LOCATIONS (IF ANY) #####
if(WIN32)
	list(APPEND PLUGIN_LINKDIRS
				${PLUGIN_LIBS_DIR}/lib/win32
				${PLUGIN_LIBS_DIR}/bin
	)
	
	#set(PLUGIN_LIBDIRS ${PLUGIN_LIBS_DIR}/lib/win32)
	list(APPEND PLUGIN_LIBDIRS
				${PLUGIN_LIBS_DIR}/lib/win32
	)
elseif(LINUX)
	#Linux libs here
elseif(APPLE)
	#Macos libs here
endif(WIN32)


##### PLUGIN DEPENDENCY LINKING (IF ANY) #####					
if(WIN32)
	list(APPEND PLUGIN_LINKLIBS
				#Lib names
				winmm
				ws2_32
	)
elseif(LINUX)
	list(APPEND PLUGIN_LINKLIBS
				#Lib names
	)
elseif(APPLE)
	list(APPEND PLUGIN_LINKLIBS
				#Lib names
	)
endif(WIN32)
