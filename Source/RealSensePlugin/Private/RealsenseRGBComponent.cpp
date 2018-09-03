// Fill out your copyright notice in the Description page of Project Settings.

#include "RealsenseRGBComponent.h"


// Sets default values for this component's properties
URealsenseRGBComponent::URealsenseRGBComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	pipeline = new rs2::pipeline();
}


bool URealsenseRGBComponent::ReceiveFrame()
{
	try {
		rs2::frameset frames = pipeline->wait_for_frames();
		rs2::video_frame frameRGB = frames.get_color_frame();

#ifdef DEBUG_LOG
		uint8* dat = (uint8*)frameRGB.get_data();
		UE_LOG(RealSenseLog, Log, TEXT("Received RGB Image: w/h: %d/%d, channels: %d, pixels: %d, %d, %d ..."),
			frameRGB.get_width(), frameRGB.get_height(), frameRGB.get_bytes_per_pixel(), dat[0], dat[1], dat[2]);
#endif
		TextureRGB->UpdateTextureRegions(0, 1, TextureUpdateRegion, static_cast<uint32>(WidthRGB * frameRGB.get_bytes_per_pixel()),
			frameRGB.get_bytes_per_pixel(), (uint8*)frameRGB.get_data(), texCleanUpFP);
	}
	catch (std::exception e) {
		UE_LOG(RealSenseLog, Error, TEXT("Could not receive frame. %s"), *FString(e.what()));
		return false;
	}
	return true;
}

UTexture2D * URealsenseRGBComponent::getTextureRGB()
{
	return TextureRGB;
}

// Called when the game starts
void URealsenseRGBComponent::BeginPlay()
{
	Super::BeginPlay();

	// init realsense
	rs2::config cfg;
	cfg.enable_stream(RS2_STREAM_COLOR, 0, WidthRGB, HeightRGB, RS2_FORMAT_BGRA8);

	if (!cfg.can_resolve(*pipeline)) {
		UE_LOG(RealSenseLog, Error, TEXT("Wrong configuration for RGB Camera!"));
	}

	try {
		selection = pipeline->start(cfg);

		// create textures
		rs2::frameset frames = pipeline->wait_for_frames();
		rs2::video_frame frameRGB = frames.get_color_frame();
		WidthRGB = frameRGB.get_width();
		HeightRGB = frameRGB.get_height();

		TextureRGB = UTexture2D::CreateTransient(WidthRGB, HeightRGB, EPixelFormat::PF_B8G8R8A8);
		TextureRGB->AddToRoot();
		TextureRGB->UpdateResource();
		TextureUpdateRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, WidthRGB, HeightRGB);
	}
	catch (std::exception e) {
		UE_LOG(RealSenseLog, Error, TEXT("Realsense device stream could not be started: %s"), *FString(e.what()));
		return;
	}
}


// Called every frame
void URealsenseRGBComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

