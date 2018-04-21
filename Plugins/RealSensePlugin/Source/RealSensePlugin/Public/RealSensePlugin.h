#pragma once


#include "IRealSensePlugin.h"

class FRealSensePlugin : public IRealSensePlugin
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};