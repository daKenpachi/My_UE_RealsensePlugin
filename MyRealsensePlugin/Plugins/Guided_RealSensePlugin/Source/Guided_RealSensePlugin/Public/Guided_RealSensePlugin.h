// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "../Public/IGuided_RealsensePlugin.h"


class FGuided_RealSensePluginModule : public IGuided_RealSensePlugin
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule();
	virtual void ShutdownModule();

	int CheckRealsenseCamera();
};
