#pragma once

#include "CoreMinimal.h"
#include "RealSenseHand.generated.h"

USTRUCT(BlueprintType)
struct FRealSenseJoint
{
	GENERATED_BODY()

	/** The confidence score of the tracking data, ranging from 0 to 100. */
	UPROPERTY(BlueprintReadOnly, Category = "RealSense confidence")
	int32 Confidence;

	/** The geometric position in world coordinates(cm). */
	UPROPERTY(BlueprintReadOnly, Category = "RealSense PositionWorld")
	FVector PositionWorld;

	/** A quaternion representing the local 3D orientation(relative to the parent joint) from the joint's parent to the joint. */
	UPROPERTY(BlueprintReadOnly, Category = "RealSense LocalRotation")
	FQuat LocalRotation;

	/** A quaternion representing the global 3D orientation(relative to the camera) from the joint's parent to the joint. */
	UPROPERTY(BlueprintReadOnly, Category = "RealSense GlobalOrientation")
	FQuat GlobalOrientation;

	/** The direction and magnitude of the joint motion in X / Y / Z axes. */
	UPROPERTY(BlueprintReadOnly, Category = "RealSense Speed")
	FVector Speed;

	FRealSenseJoint()
		: PositionWorld(0.f), LocalRotation(0.f, 0.f, 0.f, 0.f)
		, GlobalOrientation(0.f, 0.f, 0.f, 0.f), Speed(0.f), Confidence(0)
	{

	}

};

USTRUCT(BlueprintType)
struct FRealSenseHand
{
	GENERATED_BODY()

	/** Is current hand left */
	UPROPERTY(BlueprintReadOnly, Category = "RealSense bIsLeft")
	bool bIsLeft;

	/** Is current hand right */
	UPROPERTY(BlueprintReadOnly, Category = "RealSense bIsRight")
	bool bIsRight;

	/** Id of hand */
	UPROPERTY(BlueprintReadOnly, Category = "RealSense ID")
	int32 ID;

	/** Is hand data was tracking, it is true in case hand is tracking */
	UPROPERTY(BlueprintReadOnly, Category = "RealSense bIsTracking")
	bool bIsTracking;

	/** the center of mass in the 3D world coordinates. */
	UPROPERTY(BlueprintReadOnly, Category = "RealSense HandMassCenterWorld")
	FVector HandMassCenterWorld;

	/** Quaternion representing the global 3D orientation of palm joint point. */
	UPROPERTY(BlueprintReadOnly, Category = "RealSense PalmOrientation")
	FQuat PalmOrientation;

	/** The radius of the minimal circle that contains the hand's palm in the world space. */
	UPROPERTY(BlueprintReadOnly, Category = "RealSense PalmRadiusWorld")
	float PalmRadiusWorld;

	/**  The hand openness value from 0 (all fingers completely folded) to 100 (all fingers fully spread). */
	UPROPERTY(BlueprintReadOnly, Category = "RealSense Openness")
	int32 Openness;

	/** if there is any tracked joint(s). */
	UPROPERTY(BlueprintReadOnly, Category = "RealSense Hand")
	bool bIsHasTrackedJoints;

	/** Aleets array */
	UPROPERTY(BlueprintReadOnly, Category = "RealSense Alerts")
	TArray<int> FiredAlertData;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Hand joint wirst")
		FRealSenseJoint JointWirst;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense The palm center joint")
		FRealSenseJoint JointCenter;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Thumb")
		FRealSenseJoint JointThumbBase;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Thumb")
		FRealSenseJoint JointThumbJT1;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Thumb")
		FRealSenseJoint JointThumbJT2;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Thumb")
		FRealSenseJoint JointThumbTIP;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Index")
		FRealSenseJoint JointIndexBase;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Index")
		FRealSenseJoint JointIndexJT1;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Index")
		FRealSenseJoint JointIndexJT2;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Index")
		FRealSenseJoint JointIndexTIP;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Middle")
		FRealSenseJoint JointMiddleBase;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Middle")
		FRealSenseJoint JointMiddleJT1;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Middle")
		FRealSenseJoint JointMiddleJT2;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Middle")
		FRealSenseJoint JointMiddleTIP;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Ring")
		FRealSenseJoint JointRingBase;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Ring")
		FRealSenseJoint JointRingJT1;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Ring")
		FRealSenseJoint JointRingJT2;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Ring")
		FRealSenseJoint JointRingTIP;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Pinky")
		FRealSenseJoint JointPinkyBase;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Pinky")
		FRealSenseJoint JointPinkyJT1;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Pinky")
		FRealSenseJoint JointPinkyJT2;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense Pinky")
		FRealSenseJoint JointPinkyTIP;

	FRealSenseHand()
		: bIsLeft(false), bIsRight(false), bIsTracking(false), ID(0)
		, HandMassCenterWorld(0.f), PalmRadiusWorld(0.f), Openness(0)
		, bIsHasTrackedJoints(false)
	{

	}
};