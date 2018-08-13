// Fill out your copyright notice in the Description page of Project Settings.

#include "RealSenseComponent.h"
#include "IGuided_RealSensePlugin.h"
#include "Guided_RealSensePlugin.h"


// Sets default values for this component's properties
URealSenseComponent::URealSenseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URealSenseComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IGuided_RealSensePlugin::IsAvailable()) {
		int x = IGuided_RealSensePlugin::Get().CheckRealsenseCamera();
		UE_LOG(Guided_RealSensePlugin, Log, TEXT("CheckRealsenseCamera returned: %d"), x);
	}
	else {
		UE_LOG(Guided_RealSensePlugin, Warning, TEXT("IGuided_RealSensePlugin not available"));
	}
	
}


// Called every frame
void URealSenseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

