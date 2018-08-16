#pragma once


#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"


DECLARE_LOG_CATEGORY_EXTERN(RealSenseLog, Log, All);

/**
* The public interface to the Point Cloud Renderer module.
* @Author Valentin Kraft
*/
class IRealSenseModule : public IModuleInterface
{

public:

	/**
	* Singleton-like access to this module's interface.  This is just for convenience!
	* Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	*
	* @return Returns singleton instance, loading the module on demand if needed
	*/
	static inline IRealSenseModule& Get()
	{
		return FModuleManager::LoadModuleChecked< IRealSenseModule >("RealSenseModule");
	}

	/**
	* Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	*
	* @return True if the module is loaded and ready to use
	*/
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("RealSenseModule");
	}

	virtual int CheckRealsenseCamera() = 0;
};