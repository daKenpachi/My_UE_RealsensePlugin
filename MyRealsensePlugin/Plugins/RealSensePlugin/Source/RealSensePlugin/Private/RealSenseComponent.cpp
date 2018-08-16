// Fill out your copyright notice in the Description page of Project Settings.

#include "RealSenseComponent.h"
#include "IRealSenseModule.h"
#include "RealSenseModule.h"



// Sets default values for this component's properties
URealSenseComponent::URealSenseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}




void URealSenseComponent::CreateUpdateabelTexture()
{
	TextureFromVideo = UTexture2D::CreateTransient(width, height);
	//TextureFromVideo->AddToRoot();
	TextureFromVideo->UpdateResource();
	textureVideoRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, width/2, height/2);
}

UTexture2D * URealSenseComponent::ReceiveRGBFrame()
{
	if (cameraWorks) {
		TextureFromVideo->UpdateResource();
		if (!receiveFrame()) {
			UE_LOG(RealSenseLog, Error, TEXT("Could not receive a Frame from camera!"));
		}
		else {
			UE_LOG(RealSenseLog, Log, TEXT("Received Frame!"))
		}
	}
	else {
		UE_LOG(RealSenseLog, Log, TEXT("CameraWorks is false"));
	}
	return TextureFromVideo;
}

// Called when the game starts
void URealSenseComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IRealSenseModule::IsAvailable()) {
		int x = IRealSenseModule::Get().CheckRealsenseCamera();
		UE_LOG(RealSenseLog, Log, TEXT("CheckRealsenseCamera returned: %d"), x);
	}
	else {
		UE_LOG(RealSenseLog, Warning, TEXT("RealSenseModule not available"));
	}


	try {

		pipeline = new rs2::pipeline();
		pipeline->start();
		cameraWorks = true;
	}
	catch (std::exception e) {
		UE_LOG(RealSenseLog, Error, TEXT("Realsense initialization error: %s"), e.what());
	}

	try {
		rs2::frameset frames = pipeline->wait_for_frames();
		rs2::video_frame colorFrame = frames.get_color_frame();
		width = colorFrame.get_width();
		height = colorFrame.get_height();
		

	}
	catch (std::exception e) {
		UE_LOG(RealSenseLog, Error, TEXT("Realsense video stream error: %s"), e.what());
		cameraWorks = false;
	}
}

bool URealSenseComponent::receiveFrame()
{
	try {
		rs2::frameset frames = pipeline->wait_for_frames();
		rs2::video_frame colorFrame = frames.get_color_frame();
		uint8* data = (uint8*)(colorFrame.get_data());
		int width = colorFrame.get_width();
		int channels = colorFrame.get_bytes_per_pixel();
		int bits = colorFrame.get_bits_per_pixel();
		auto format = colorFrame.get_profile().format();

		//TextureFromVideo->UpdateTextureRegions(DBL_MAX_10_EXP, 1, textureVideoRegion, static_cast<uint32>(width * bits), 
		//	bits, data, texCleanUpFP);

		UpdateTextureRegions(TextureFromVideo, DBL_MAX_10_EXP, 1, textureVideoRegion, static_cast<uint32>(width * bits), bits, data, false);
	}
	catch (const rs2::error & e) {

		UE_LOG(RealSenseLog, Error, TEXT("%s"), *FString(e.what()));
		cameraWorks = false;
	}
	catch (const std::exception& e) {

		UE_LOG(RealSenseLog, Error, TEXT("%s"), *FString(e.what()));
		cameraWorks = false;
	}

	return cameraWorks;
}


// Called every frame
void URealSenseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	TextureFromVideo->UpdateResource();
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*if (cameraWorks) {
		receiveFrame();
	}

	if (MediaTexture) {
		MediaTexture = Cast<UTexture>(TextureFromVideo);
	}

	if (MaterialInstanceToUpdate) {
		MaterialToUpdateDynamic->SetTextureParameterValue(TEXT("Video"), TextureFromVideo);

	}*/
}

void URealSenseComponent::UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
{
	if (Texture && Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			UpdateTextureRegionsData,
			FUpdateTextureRegionsData*, RegionData, RegionData,
			bool, bFreeData, bFreeData,
			{
				for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
				{
					int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
					if (RegionData->MipIndex >= CurrentFirstMip)
					{
						RHIUpdateTexture2D(
							RegionData->Texture2DResource->GetTexture2DRHI(),
							RegionData->MipIndex - CurrentFirstMip,
							RegionData->Regions[RegionIndex],
							RegionData->SrcPitch,
							RegionData->SrcData
							+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
							+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
						);
					}
				}
		if (bFreeData)
		{
			FMemory::Free(RegionData->Regions);
			FMemory::Free(RegionData->SrcData);
		}
		delete RegionData;
			});
	}
}