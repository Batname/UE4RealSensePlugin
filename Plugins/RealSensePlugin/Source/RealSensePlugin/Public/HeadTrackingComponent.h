#pragma once

#include "RealSenseComponent.h"
#include "HeadTrackingComponent.generated.h"

UCLASS(editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = RealSense) 
class REALSENSEPLUGIN_API UHeadTrackingComponent : public URealSenseComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "RealSense")
	int32 HeadCount;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense")
	FVector HeadPosition;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense")
	FRotator HeadRotation;

	UHeadTrackingComponent();

	void InitializeComponent() override;
	
	void TickComponent(float DeltaTime, enum ELevelTick TickType, 
		               FActorComponentTickFunction *ThisTickFunction) override;

};
