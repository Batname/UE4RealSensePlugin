
#include "RealSenseComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/EngineUtils.h"

// Specifies that this component should be initialized and can tick.
URealSenseComponent::URealSenseComponent() 
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;

	CameraModel = ECameraModel::None;
	CameraFirmware = "0.0.0.0";

	ColorHorizontalFOV = 0.0f;
	ColorVerticalFOV = 0.0f;
	DepthHorizontalFOV = 0.0f;
	DepthVerticalFOV = 0.0f;

	globalRealSenseSession = nullptr;
}

// When initialized, this component will check if a RealSenseSessionManager actor 
// exists in the scene. If the actor exists, this component stores a reference to 
// it. If it does not, a new RealSenseSessionManager actor will be spawned, and a 
// reference to it will be saved.
void URealSenseComponent::InitializeComponent() 
{
	if (globalRealSenseSession == nullptr) {
		for (TActorIterator<ARealSenseSessionManager> Itr(GetWorld()); Itr; ++Itr) {
			globalRealSenseSession = (ARealSenseSessionManager*)*Itr;
		}
		if (globalRealSenseSession == nullptr) {
			RS_LOG(Log, "Creating RealSenseSessionManager Actor")
			globalRealSenseSession = GetWorld()->SpawnActor<ARealSenseSessionManager>(ARealSenseSessionManager::StaticClass());
		}
	}

	if (globalRealSenseSession)
	{
		globalRealSenseSession->EnableFeature(m_feature);
	}
}

// Queries the camera model, firmware, and field of view data from the RealSense 
// camera.
void URealSenseComponent::BeginPlay() 
{
	Super::BeginPlay();

	CameraModel = globalRealSenseSession->GetCameraModel();
	CameraFirmware = globalRealSenseSession->GetCameraFirmware();

	ColorHorizontalFOV = globalRealSenseSession->GetColorHorizontalFOV();
	ColorVerticalFOV = globalRealSenseSession->GetColorVerticalFOV();

	DepthHorizontalFOV = globalRealSenseSession->GetDepthHorizontalFOV();
	DepthVerticalFOV = globalRealSenseSession->GetDepthVerticalFOV();
}

void URealSenseComponent::EnableFeature()
{
	globalRealSenseSession->EnableFeature(m_feature);
}

void URealSenseComponent::DisableFeature()
{
	globalRealSenseSession->DisableFeature(m_feature);
}

void URealSenseComponent::StartCamera()
{
	if (!bIsColorCameraResolutionSet)
	{
		SetColorCameraResolution(DefaultColorResolution);
	}

	if (!bIsDepthCameraResolutionSet)
	{
		SetDepthCameraResolution(DefaultDepthResolution);
	}


	globalRealSenseSession->StartCamera();
}

void URealSenseComponent::StopCamera()
{
	globalRealSenseSession->StopCamera();
}

bool URealSenseComponent::IsCameraRunning()
{
	return globalRealSenseSession->IsCameraRunning();
}

FStreamResolution URealSenseComponent::GetColorCameraResolution() 
{
	return globalRealSenseSession->GetColorCameraResolution();
}

void URealSenseComponent::SetColorCameraResolution(EColorResolution resolution)
{
	// TODO, make it more clever way to install only once
	if (!bIsColorCameraResolutionSet && resolution != EColorResolution::UNDEFINED) {
		globalRealSenseSession->SetColorCameraResolution(resolution);
	}

	// Set resolution is set flag
	bIsColorCameraResolutionSet = true;
}

FStreamResolution URealSenseComponent::GetDepthCameraResolution()
{
	return globalRealSenseSession->GetDepthCameraResolution();
}

void URealSenseComponent::SetDepthCameraResolution(EDepthResolution resolution) 
{
	if (!bIsDepthCameraResolutionSet && resolution != EDepthResolution::UNDEFINED) {
		globalRealSenseSession->SetDepthCameraResolution(resolution);
	}

	// Set resolution is set flag
	bIsDepthCameraResolutionSet = true;
}

void URealSenseComponent::Set3DSegCameraResolution(E3DSegResolution resolution)
{
	if (resolution != E3DSegResolution::UNDEFINED) {
		globalRealSenseSession->Set3DSegCameraResolution(resolution);
	}
}

bool URealSenseComponent::IsStreamSetValid(EColorResolution ColorResolution, 
										   EDepthResolution DepthResolution) 
{
	return globalRealSenseSession->IsStreamSetValid(ColorResolution, DepthResolution);
}
