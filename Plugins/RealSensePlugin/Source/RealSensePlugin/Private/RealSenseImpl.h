#pragma once

#include "AllowWindowsPlatformTypes.h"
#include <future>
#include <assert.h>
#include "HideWindowsPlatformTypes.h"

#include "CoreMisc.h"
#include "RealSenseTypes.h"
#include "RealSenseUtils.h"
#include "RealSenseBlueprintLibrary.h"
#include "PXCSenseManager.h"
#include "pxcfacemodule.h"
#include "pxc3dseg.h"
#include "pxchandcursormodule.h"
#include "pxccursordata.h"
#include "pxccursorconfiguration.h"

#include "pxchandmodule.h"
#include "pxchanddata.h"
#include "pxchandconfiguration.h"

#include "RealSenseHand.h"


// Stores all relevant data computed from one frame of RealSense camera data.
// Advice: Use this structure in a multiple-buffer configuration to share 
// RealSense data between threads.
// Example usage: 
//   Thread 1: Process camera data and populate background_frame
//             Swap background_frame with mid_frame
//   Thread 2: Swap mid_frame with foreground_frame
//             Read data from foreground_frame
struct RealSenseDataFrame {
	uint64 number;  // Stores an ID for the frame based on its occurrence in time
	TArray<uint8> colorImage;  // Container for the camera's raw color stream data
	TArray<uint16> depthImage;  // Container for the camera's raw depth stream data
	TArray<uint8> scanImage;  // Container for the scan preview image provided by the 3DScan middleware

	int headCount;
	FVector headPosition;
	FRotator headRotation;

	FVector cursorData;
	bool isCursorDataValid;
	FVector cursorDataLeft;
	bool isCursorDataLeftValid;
	FVector cursorDataRight;
	bool isCursorDataRightValid;

	// Hand tracking data
	int32 HandCount;
	FRealSenseHand RealSenseHandLeft;
	FRealSenseHand RealSenseHandRight;

	bool gestureClick;
	bool gestureClockwiseCircle;
	bool gestureCounterClockwiseCircle;
	bool gestureHandClosing;
	bool gestureHandOpening;

	PXCCursorData::BodySideType bodySideClick;
	PXCCursorData::BodySideType bodySideClockwiseCircle;
	PXCCursorData::BodySideType bodySideCounterClockwiseCircle;
	PXCCursorData::BodySideType bodySideHandClosing;
	PXCCursorData::BodySideType bodySideHandOpening;

	TArray<int> firedAlertData;

	int statusCode;

	RealSenseDataFrame() : number(0), headCount(0), 
		headPosition(0.0f), headRotation(0.0f), 
		cursorData(0.0f), isCursorDataValid(false),
		cursorDataLeft(0.0f), isCursorDataLeftValid(false),
		cursorDataRight(0.0f), isCursorDataRightValid(false),
		gestureClick(false), gestureClockwiseCircle(false), gestureCounterClockwiseCircle(false),
		gestureHandClosing(false), gestureHandOpening(false),
		HandCount(0), RealSenseHandLeft(), RealSenseHandRight(),
		bodySideClick(PXCCursorData::BodySideType::BODY_SIDE_UNKNOWN),
		bodySideClockwiseCircle(PXCCursorData::BodySideType::BODY_SIDE_UNKNOWN),
		bodySideCounterClockwiseCircle(PXCCursorData::BodySideType::BODY_SIDE_UNKNOWN),
		bodySideHandClosing(PXCCursorData::BodySideType::BODY_SIDE_UNKNOWN),
		bodySideHandOpening(PXCCursorData::BodySideType::BODY_SIDE_UNKNOWN),
		statusCode(0)
		{}
};

// Implements the functionality of the Intel(R) RealSense(TM) SDK and associated
// middleware modules as used by the RealSenseSessionManager Actor class.
//
// NOTE: Some function declarations do not have a function comment because the 
// comment is written in RealSenseSessionManager.h. 
class RealSenseImpl {
public:
	// Creates a new RealSense Session and queries physical device information.
	RealSenseImpl();

	// Terminates the camera processing thread and releases handles to Core SDK objects.
	~RealSenseImpl();

	// Performs RealSense Core and Middleware processing based on the enabled
	// feature set.
	//
	// This function is meant to be run on its own thread to minimize the 
	// performance impact on the game thread.
	void CameraThread();

	void StartCamera();

	void StopCamera();

	// Swaps the data frames to load the latest processed data into the 
	// foreground frame.
	void SwapFrames();

	inline bool IsCameraThreadRunning() const { return bCameraThreadRunning; }

	// Core SDK Support

	void EnableMiddleware();

	void EnableFeature(RealSenseFeature feature);

	void DisableFeature(RealSenseFeature feature);

	inline bool IsCameraConnected() const { return (senseManager->IsConnected() != 0); }

	inline float GetColorHorizontalFOV() const { return colorHorizontalFOV; }

	inline float GetColorVerticalFOV() const { return colorVerticalFOV; }

	inline float GetDepthHorizontalFOV() const { return depthHorizontalFOV; }

	inline float GetDepthVerticalFOV() const { return depthVerticalFOV; }

	const ECameraModel GetCameraModel() const;

	const FString GetCameraFirmware() const;

	inline FStreamResolution GetColorCameraResolution() const { return colorResolution; }

	inline int32 GetColorImageWidth() const { return colorResolution.width; }

	inline int32 GetColorImageHeight() const { return colorResolution.height; }

	void SetColorCameraResolution(EColorResolution resolution);

	inline FStreamResolution GetDepthCameraResolution() const { return depthResolution; }

	inline int32 GetDepthImageWidth() const { return depthResolution.width; }

	inline int32 GetDepthImageHeight() const { return depthResolution.height; }

	void SetDepthCameraResolution(EDepthResolution resolution);

	void Set3DSegCameraResolution(E3DSegResolution resolution);

	bool IsStreamSetValid(EColorResolution ColorResolution, EDepthResolution DepthResolution) const;

	inline const uint8* GetColorBuffer() const { return fgFrame->colorImage.GetData(); }

	inline const uint16* GetDepthBuffer() const { return fgFrame->depthImage.GetData(); }

	// 3D Scanning Module Support 

	void ConfigureScanning(EScan3DMode scanningMode, bool bSolidify, bool bTexture);

	void SetScanningVolume(FVector boundingBox, int32 resolution);

	void StartScanning();

	void StopScanning();

	void SaveScan(EScan3DFileFormat saveFileFormat, const FString& filename);
	
	inline bool IsScanning() const { return (p3DScan->IsScanning() != 0); }

	inline FStreamResolution GetScan3DResolution() const { return scan3DResolution; }

	inline int32 GetScan3DImageWidth() const { return scan3DResolution.width; }

	inline int32 GetScan3DImageHeight() const { return scan3DResolution.height; }

	inline const uint8* GetScanBuffer() const { return fgFrame->scanImage.GetData(); }

	inline bool HasScan3DImageSizeChanged() const { return bScan3DImageSizeChanged; }

	inline bool HasScanCompleted() const { return bScanCompleted; }

	// Head Tracking Support
	inline int GetHeadCount() const { return fgFrame->headCount; }
	inline FVector GetHeadPosition() const { return fgFrame->headPosition; }
	inline FRotator GetHeadRotation() const { return fgFrame->headRotation; }

	inline FVector GetCursorData() const { return fgFrame->cursorData; }
	inline bool IsCursorDataValid() const { return fgFrame->isCursorDataValid; }
	inline FVector GetCursorDataLeft() const { return fgFrame->cursorDataLeft; }
	inline bool IsCursorDataLeftValid() const { return fgFrame->isCursorDataLeftValid; }
	inline FVector GetCursorDataRight() const { return fgFrame->cursorDataRight; }
	inline bool IsCursorDataRightValid() const { return fgFrame->isCursorDataRightValid; }

	inline bool IsGestureClick() const { return fgFrame->gestureClick; }
	inline bool IsGestureClockwiseCircle() const { return fgFrame->gestureClockwiseCircle; }
	inline bool IsGestureCounterClockwiseCircle() const { return fgFrame->gestureCounterClockwiseCircle; }
	inline bool IsGestureHandClosing() const { return fgFrame->gestureHandClosing; }
	inline bool IsGestureHandOpening() const { return fgFrame->gestureHandOpening; }

	inline int GetBodySideClick() const { return fgFrame->bodySideClick; }
	inline int GetBodySideClockwiseCircle() const { return fgFrame->bodySideClockwiseCircle; }
	inline int GetBodySideCounterClockwiseCircle() const { return fgFrame->bodySideCounterClockwiseCircle; }
	inline int GetBodySideHandClosing() const { return fgFrame->bodySideHandClosing; }
	inline int GetBodySideHandOpening() const { return fgFrame->bodySideHandOpening; }

	inline const TArray<int> GetFiredAlertData() const { return fgFrame->firedAlertData; }

	inline int GetStatusCode() const { return fgFrame->statusCode; }

	int StatusCodeSDK(int statusCode);

	// Hand tracking accessor
	inline int32 GetHandCount() const { return fgFrame->HandCount; }
	inline FRealSenseHand GetRealSenseHandLeft() { return fgFrame->RealSenseHandLeft; }
	inline FRealSenseHand GetRealSenseHandRight() { return fgFrame->RealSenseHandRight; }
	

private:
	// Core SDK handles

	struct RealSenseDeleter {
		void operator()(PXCSession* s) { s->Release(); }
		void operator()(PXCSenseManager* sm) { sm->Release(); }
		void operator()(PXCCapture* c) { c->Release(); }
		void operator()(PXCCapture::Device* d) { d->Release(); }
		void operator()(PXC3DScan* sc) { ; }
		void operator()(PXCFaceModule* sc) { ; }
		void operator()(PXC3DSeg* s) { ; }
		void operator()(PXCHandCursorModule* sc) { ; }
		void operator()(PXCHandModule* sc) { ; }
	};

	std::unique_ptr<PXCSession, RealSenseDeleter> session;
	std::unique_ptr<PXCSenseManager, RealSenseDeleter> senseManager;
	std::unique_ptr<PXCCapture, RealSenseDeleter> capture;
	std::unique_ptr<PXCCapture::Device, RealSenseDeleter> m_device;

	PXCCapture::DeviceInfo deviceInfo;
	pxcStatus m_status;  // Status ID used by RSSDK functions

	// SDK Module handles

	std::unique_ptr<PXC3DScan, RealSenseDeleter> p3DScan;
	std::unique_ptr<PXCFaceModule, RealSenseDeleter> pFace;
	std::unique_ptr<PXC3DSeg, RealSenseDeleter> p3DSeg;
	std::unique_ptr<PXCHandCursorModule, RealSenseDeleter> pHandCursor;
	std::unique_ptr<PXCHandModule, RealSenseDeleter> pHandModule;


	// Feature set constructed as the logical OR of RealSenseFeatures
	uint32 RealSenseFeatureSet;

	std::atomic_bool bCameraStreamingEnabled;
	std::atomic_bool bScan3DEnabled;
	std::atomic_bool bFaceEnabled;
	std::atomic_bool bSeg3DEnabled;
	std::atomic_bool bHandCursorEnabled;
	std::atomic_bool bHandTrackingEnabled;


	// Camera processing members

	std::thread cameraThread;
	std::atomic_bool bCameraThreadRunning;

	std::unique_ptr<RealSenseDataFrame> fgFrame;
	std::unique_ptr<RealSenseDataFrame> midFrame;
	std::unique_ptr<RealSenseDataFrame> bgFrame;

	// Mutex for locking access to the midFrame
	std::mutex midFrameMutex;

	// Core SDK members

	FStreamResolution colorResolution;
	FStreamResolution depthResolution;

	float colorHorizontalFOV;
	float colorVerticalFOV;
	float depthHorizontalFOV;
	float depthVerticalFOV;

	// 3D Scan members

	FStreamResolution scan3DResolution;

	PXC3DScan::FileFormat scan3DFileFormat;
	FString scan3DFilename;

	std::atomic_bool bScanStarted;
	std::atomic_bool bScanStopped;
	std::atomic_bool bReconstructEnabled;
	std::atomic_bool bScanCompleted;
	std::atomic_bool bScan3DImageSizeChanged;

	// Face Module members

	PXCFaceConfiguration* faceConfig;
	PXCFaceData* faceData;

	// Helper Functions

	void UpdateScan3DImageSize(PXCImage::ImageInfo info);

private:
	void WriteHandData(FRealSenseHand& RealSenseHand, PXCHandData::IHand* IHand, bool bIsLeft);
	void WriteJointData(FRealSenseJoint& RealSenseJoint, PXCHandData::JointData& JointData);
	void WriteAlertData(const PXCHandData *handData, FRealSenseHand& RealSenseHand, PXCHandData::AlertData& AlertData, const pxcUID& HandId);

};
