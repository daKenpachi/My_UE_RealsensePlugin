// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "RealSenseModule.h"


THIRD_PARTY_INCLUDES_START
#include <ThirdParty/librealsense2/include/librealsense2/rs.hpp>
THIRD_PARTY_INCLUDES_END

#define LOCTEXT_NAMESPACE "FRealSenseModule"


DEFINE_LOG_CATEGORY(RealSenseLog);



void FRealSenseModule::StartupModule()
{
	UE_LOG(RealSenseLog, Log, TEXT("Starting Realsense Module!"));

	CheckRealsenseCamera();
}

void FRealSenseModule::ShutdownModule()
{
	UE_LOG(RealSenseLog, Log, TEXT("Shut down Realsense Module!"));
}

int FRealSenseModule::CheckRealsenseCamera()
{
	try {
		rs2::pipeline pipe;
		pipe.start();
		pipe.stop();
	}
	catch (std::exception e) {
		UE_LOG(RealSenseLog, Error, TEXT("Initialization Error: %s"), *FString(e.what()));
		return -1;
	}
	return 0;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRealSenseModule, RealSenseLog)