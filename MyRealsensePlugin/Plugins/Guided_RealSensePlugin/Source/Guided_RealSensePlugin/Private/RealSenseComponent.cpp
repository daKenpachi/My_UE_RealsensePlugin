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


	try {

		pipeline = new rs2::pipeline();
		pipeline->start();
		cameraWorks = true;
	}
	catch (std::exception e) {
		UE_LOG(Guided_RealSensePlugin, Error, TEXT("Realsense initialization error: %s"), e.what());
	}

	try {
		rs2::frameset frames = pipeline->wait_for_frames();
		rs2::video_frame colorFrame = frames.get_color_frame();
		TextureFromVideo = UTexture2D::CreateTransient(colorFrame.get_width(), colorFrame.get_height());
		TextureFromVideo->AddToRoot();
		TextureFromVideo->UpdateResource();
		textureVideoRegion = new FUpdateTextureRegion2D(0,0,0,0, colorFrame.get_width(), colorFrame.get_height());
		MaterialToUpdateDynamic = UMaterialInstanceDynamic::Create(MaterialInstanceToUpdate, this);
	}
	catch (std::exception e) {
		UE_LOG(Guided_RealSensePlugin, Error, TEXT("Realsense video stream error: %s"), e.what());
		cameraWorks = false;
	}
}

bool URealSenseComponent::receiveFrame()
{
	try {
		rs2::frameset frames = pipeline->wait_for_frames();
		rs2::video_frame colorFrame = frames.get_color_frame();
		uint8* RawUndistortedLeftBGRA = (uint8*)(colorFrame.get_data());
		int UndistortedFrameIndex = colorFrame.get_frame_number();
		int UndistortedTimeIndex = colorFrame.get_frame_timestamp_domain();

		TextureFromVideo->UpdateTextureRegions(DBL_MAX_10_EXP, 1, textureVideoRegion, static_cast<uint32>(colorFrame.get_width() * sizeof(uint8) * 4), 
			sizeof(uint8) * 4, RawUndistortedLeftBGRA, texCleanUpFP);
	}
	catch (const rs2::error & e) {

		UE_LOG(Guided_RealSensePlugin, Error, TEXT("%s"), *FString(e.what()));
		cameraWorks = false;
	}
	catch (const std::exception& e) {

		UE_LOG(Guided_RealSensePlugin, Error, TEXT("%s"), *FString(e.what()));
		cameraWorks = false;
	}

	return cameraWorks;
}


// Called every frame
void URealSenseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (cameraWorks) {
		receiveFrame();
	}

	if (MediaTexture) {
		MediaTexture = Cast<UTexture>(TextureFromVideo);
	}

	if (MaterialInstanceToUpdate) {
		MaterialToUpdateDynamic->SetTextureParameterValue(TEXT("Video"), TextureFromVideo);

	}
}

