/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2013-2015 Intel Corporation. All Rights Reserved.

*******************************************************************************/

/** @file PXCPersonTrackingModule.h
    Defines the PXCPersonTrackingModule interface, which gives access to the person module's configuration and output data.
 */
#pragma once
#include "pxcsensemanager.h"

class PXCPersonTrackingConfiguration;
class PXCPersonTrackingData;

/**
	@Class PXCPersonTrackingModule 
*/
class PXCPersonTrackingModule : public PXCBase 
{
public:
	PXC_CUID_OVERWRITE(PXC_CUID_PERSON_TRACKING_MODULE);

	/** 
		@brief returns current configuration.
	*/
	virtual PXCPersonTrackingConfiguration* PXCAPI QueryConfiguration() = 0;

	/** 
		@brief returns latest available output data.
	*/
	virtual PXCPersonTrackingData* PXCAPI QueryOutput() = 0;
};
