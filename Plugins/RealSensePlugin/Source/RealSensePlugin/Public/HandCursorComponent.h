#pragma once

#include "RealSenseComponent.h"
#include "HandCursorComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHandCursorDataDelegate, FVector, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHandCursorDataLeftDelegate, FVector, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHandCursorDataRightDelegate, FVector, Data);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGestureClickDelegate, EBodySideType, BodySide);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGestureClockwiseCircleDelegate, EBodySideType, BodySide);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGestureCounterClockwiseCircleDelegate, EBodySideType, BodySide);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGestureHandClosingDelegate, EBodySideType, BodySide);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGestureHandOpeningDelegate, EBodySideType, BodySide);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFiredAlertDataDelegate, const TArray<EAlertType> &, AlertData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatusCodeDelegate, EStatus, StatusCode);


UCLASS(editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = RealSense) 
class REALSENSEPLUGIN_API UHandCursorComponent : public URealSenseComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "RealSense")
		FVector HandCursorData;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense")
		bool IsHandCursorDataValid;

	UPROPERTY(BlueprintAssignable, Category = "RealSense")
		FHandCursorDataDelegate OnHandCursorData;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense")
		FVector HandCursorDataLeft;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense")
		bool IsHandCursorDataLeftValid;

	UPROPERTY(BlueprintAssignable, Category = "RealSense")
		FHandCursorDataLeftDelegate OnHandCursorDataLeft;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense")
		FVector HandCursorDataRight;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense")
		bool IsHandCursorDataRightValid;

	UPROPERTY(BlueprintAssignable, Category = "RealSense")
		FHandCursorDataRightDelegate OnHandCursorDataRight;


	UPROPERTY(BlueprintAssignable, Category = "RealSense")
		FGestureClickDelegate OnGestureClick;

	UPROPERTY(BlueprintAssignable, Category = "RealSense")
		FGestureClockwiseCircleDelegate OnGestureClockwiseCircle;

	UPROPERTY(BlueprintAssignable, Category = "RealSense")
		FGestureCounterClockwiseCircleDelegate OnGestureCounterClockwiseCircle;

	UPROPERTY(BlueprintAssignable, Category = "RealSense")
		FGestureHandClosingDelegate OnGestureHandClosing;

	UPROPERTY(BlueprintAssignable, Category = "RealSense")
		FGestureHandOpeningDelegate OnGestureHandOpening;

	UPROPERTY(BlueprintAssignable, Category = "RealSense")
		FFiredAlertDataDelegate OnFiredAlertData;

	UPROPERTY(BlueprintReadOnly, Category = "RealSense")
		EStatus StatusCode;

	UPROPERTY(BlueprintAssignable, Category = "RealSense")
		FStatusCodeDelegate OnStatusCode;

	UHandCursorComponent();

	void InitializeComponent() override;
	
	void TickComponent(float DeltaTime, enum ELevelTick TickType, 
		               FActorComponentTickFunction *ThisTickFunction) override;

private:
	TArray<EAlertType> m_Alerts;
};
