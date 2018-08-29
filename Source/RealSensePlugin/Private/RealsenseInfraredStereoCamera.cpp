// Fill out your copyright notice in the Description page of Project Settings.

#include "RealsenseInfraredStereoCamera.h"


// Sets default values
ARealsenseInfraredStereoCamera::ARealsenseInfraredStereoCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	pipeline = new rs2::pipeline();
}

bool ARealsenseInfraredStereoCamera::UpdateTextures()
{
	try {
		rs2::frameset frames = pipline->wait_for_frames();

		rs2::video_frame frameLeft = frames.get_infrared_frame(InfraredSensor::LEFT);
		rs2::video_frame frameRight = frames.get_infrared_frame(InfraredSensor::RIGHT);
		
		textureUpdater->UpdateTextureRegions(0, 1, textureLeft, static_cast<uint32>(WidthLeft * frameLeft.get_bytes_per_pixel()),
			frameLeft.get_bytes_per_pixel(), frameLeft.get_data(), texCleanUpFP);

	}
	catch (std::exception e) {
		UE_LOG(RealSenseLog, Error, TEXT("Could not receive frame. %s"), *FString(e.what()));
		return false;
	}
	return true;
}

// Called when the game starts or when spawned
void ARealsenseInfraredStereoCamera::BeginPlay()
{
	Super::BeginPlay();
	
	// init realsense
	rs2::config cfg;
	cfg.enable_stream(RS2_STREAM_INFRARED, InfraredSensor::LEFT, WidthLeft, HeightLeft, rs2_format::RS2_FORMAT_Y8);
	cfg.enable_stream(RS2_STREAM_INFRARED, InfraredSensor::RIGHT, WidthRight, HeightRight, rs2_format::RS2_FORMAT_Y8);

	if (!cfg.can_resolve(*pipline)) {
		UE_LOG(RealSenseLog, Error, TEXT("Wrong configuration for Infrared Stereo Camera!"));
		
	}
	// start realsense
	try {
		selection = pipeline->start(cfg);
		selection.get_device().first<rs2::depth_sensor>().set_option(RS2_OPTION_LASER_POWER, 0);	// disable laser

		// create textures with right sizes
		rs2::frameset frames = pipline->wait_for_frames();
		rs2::video_frame frameLeft = frames.get_infrared_frame(InfraredSensor::LEFT);
		rs2::video_frame frameRight = frames.get_infrared_frame(InfraredSensor::RIGHT);
		WidthLeft = frameLeft.get_width();
		HeightLeft = frameLeft.get_height();
		WidthRight = frameRight.get_width();
		HeightRight = frameRight.get_height();

		textureLeft = UTexture2D::CreateTransient(WidthLeft, HeightLeft, EPixelFormat::PF_A8);
		textureLeft->AddToRoot();
		textureLeft->UpdateResource();
		textureRight = UTexture2D::CreateTransient(WidthRight, HeightRight, EPixelFormat::PF_A8);
		textureRight->AddToRoot();
		textureRight->UpdateResource();
		textureUpdater = new FUpdateTextureRegion2D(0, 0, 0, 0, WidthLeft, WidthRight);
	}
	catch (std::exception e) {
		UE_LOG(RealSenseLog, Error, TEXT("Realsense device stream could not be started: %s"), *FString(e.what()));
		return;
	}
}

void ARealsenseInfraredStereoCamera::EndPlay()
{
	if (pipeline) {
		pipeline->stop();
	}
}

// Called every frame
void ARealsenseInfraredStereoCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

