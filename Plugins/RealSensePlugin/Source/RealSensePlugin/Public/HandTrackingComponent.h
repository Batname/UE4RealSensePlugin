// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RealSenseComponent.h"
#include "RealSenseHand.h"
#include "HandTrackingComponent.generated.h"

/**
 * 
 */
UCLASS(editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = RealSense)
class REALSENSEPLUGIN_API UHandTrackingComponent : public URealSenseComponent
{
	GENERATED_BODY()

public:
	UHandTrackingComponent();

	virtual void InitializeComponent() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
public:
	/** Left hand real sense data structure */
	UPROPERTY(BlueprintReadOnly, Category = "RealSense")
	FRealSenseHand RealSenseHandLeft;

	/** Right hand real sense data structure */
	UPROPERTY(BlueprintReadOnly, Category = "RealSense")
	FRealSenseHand RealSenseHandRight;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense HandCount")
	int32 HandCount;
};
