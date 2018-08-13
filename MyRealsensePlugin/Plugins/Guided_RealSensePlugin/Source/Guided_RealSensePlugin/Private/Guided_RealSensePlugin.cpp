// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Guided_RealSensePlugin.h"


THIRD_PARTY_INCLUDES_START
#include <ThirdParty/librealsense2/include/librealsense2/rs.hpp>
THIRD_PARTY_INCLUDES_END

#define LOCTEXT_NAMESPACE "FGuided_RealSensePluginModule"

void FGuided_RealSensePluginModule::StartupModule()
{
	UE_LOG(Guided_RealSensePlugin, Log, TEXT("Starting Realsense Plugin!"));

	CheckRealsenseCamera();
}

void FGuided_RealSensePluginModule::ShutdownModule()
{
	UE_LOG(Guided_RealSensePlugin, Log, TEXT("Shut down Realsense Plugin!"));
}

int FGuided_RealSensePluginModule::CheckRealsenseCamera()
{
	try {
		rs2::pipeline pipe;
		pipe.start();
		pipe.stop();
	}
	catch (std::exception e) {
		UE_LOG(Guided_RealSensePlugin, Error, TEXT("Initialization Error: %s"), *FString(e.what()));
		return -1;
	}
	return 0;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGuided_RealSensePluginModule, Guided_RealSensePlugin)