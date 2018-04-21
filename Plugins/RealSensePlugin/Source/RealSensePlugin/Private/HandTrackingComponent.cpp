// Fill out your copyright notice in the Description page of Project Settings.

#include "HandTrackingComponent.h"


UHandTrackingComponent::UHandTrackingComponent()
	: HandCount(0)
{
	m_feature = RealSenseFeature::HAND_TRACKING; 
}

void UHandTrackingComponent::InitializeComponent()
{
	Super::InitializeComponent();

	UE_LOG(LogTemp, Warning, TEXT("UHandTrackingComponent::InitializeComponent")); 
}

void UHandTrackingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	if (globalRealSenseSession->IsCameraRunning() == false)
	{
		return;
	}

	//HandCount = globalRealSenseSession->GetHandCount();

	RealSenseHandLeft = globalRealSenseSession->GetRealSenseHandLeft();
	RealSenseHandRight = globalRealSenseSession->GetRealSenseHandRight();
}
