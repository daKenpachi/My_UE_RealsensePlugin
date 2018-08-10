#pragma once

#include "ModuleManager.h"

class MyRealSenseImpl : public IModuleInterface
{
public:
	void StartupModule();
	void ShutdownModule();
};
	