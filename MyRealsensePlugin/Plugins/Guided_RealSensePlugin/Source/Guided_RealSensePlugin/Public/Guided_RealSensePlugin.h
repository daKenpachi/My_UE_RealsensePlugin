// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "../Public/IGuided_RealsensePlugin.h"


DECLARE_LOG_CATEGORY_EXTERN(Guided_RealSensePlugin, Log, All);
DEFINE_LOG_CATEGORY(Guided_RealSensePlugin);

class FGuided_RealSensePluginModule : public IGuided_RealSensePlugin
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule();
	virtual void ShutdownModule();

	int CheckRealsenseCamera();
};
