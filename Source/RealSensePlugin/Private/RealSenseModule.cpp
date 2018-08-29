// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "RealSenseModule.h"
#include <string>

THIRD_PARTY_INCLUDES_START
#include <ThirdParty/librealsense2/include/librealsense2/rs.hpp>
THIRD_PARTY_INCLUDES_END

#define LOCTEXT_NAMESPACE "FRealSenseModule"


DEFINE_LOG_CATEGORY(RealSenseLog);



void FRealSenseModule::StartupModule()
{
	UE_LOG(RealSenseLog, Log, TEXT("Starting Realsense Module!"));

	if (CheckRealsenseCamera() != 0) {
		UE_LOG(RealSenseLog, Warning, TEXT("No Realsense Device was detected!"));
	}
}

void FRealSenseModule::ShutdownModule()
{
	UE_LOG(RealSenseLog, Log, TEXT("Shut down Realsense Module!"));
}

int FRealSenseModule::CheckRealsenseCamera()
{
	int retVal = -1;
	// find a realsense camera connected to the computer
	try {
		using namespace rs2;
		context ctx;
		device_list devices = ctx.query_devices();

		for (device dev : devices) {
			std::string name = "Unknown Device";
			if (dev.supports(RS2_CAMERA_INFO_NAME)) {
				name = dev.get_info(RS2_CAMERA_INFO_NAME);
			}
			UE_LOG(RealSenseLog, Log, TEXT("Found Camera device: %s"), *FString(name.c_str()));
			if (name.find("RealSense") != std::string::npos) {
				UE_LOG(RealSenseLog, Log, TEXT("%s is a Realsense Device!"), *FString(name.c_str()));
				retVal = 0;
			}
		}

	}
	catch (std::exception e) {
		UE_LOG(RealSenseLog, Error, TEXT("Initialization Error: %s"), *FString(e.what()));
		
	}
	return retVal;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRealSenseModule, RealSenseLog)