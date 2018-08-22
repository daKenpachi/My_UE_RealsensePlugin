// Fill out your copyright notice in the Description page of Project Settings.

#include "RealSenseComponent.h"
#include "IRealSenseModule.h"
#include "RealSenseModule.h"




// Sets default values for this component's properties
ARealSenseComponent::ARealSenseComponent()
{

}




void ARealSenseComponent::CreateUpdateableTextures()
{
	TextureFromVideo = UTexture2D::CreateTransient(Width_c, Height_c, EPixelFormat::PF_B8G8R8A8);
	TextureFromVideo->AddToRoot();
	TextureFromVideo->UpdateResource();
	textureVideoRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, Width_c, Height_c);

	TextureFromDepth = UTexture2D::CreateTransient(Width_d, Height_d, EPixelFormat::PF_DepthStencil);
	TextureFromDepth->AddToRoot();
	TextureFromDepth->UpdateResource();
	textureDepthRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, Width_c, Height_c);

}

UTexture2D*  ARealSenseComponent::ReceiveRGBFrame()
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
void ARealSenseComponent::BeginPlay()
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
		// start pipeline with config
		pipeline = new rs2::pipeline();
		rs2::config config;
		rs2_stream stream_type = rs2_stream::RS2_STREAM_COLOR;
		rs2_format format = rs2_format::RS2_FORMAT_BGRA8;
		config.enable_stream(stream_type, Width_c, Height_c, format, Fps_c);
		//stream_type = rs2_stream::RS2_STREAM_DEPTH;
		//format = rs2_format::RS2_FORMAT_Z16;
		//config.enable_stream(stream_type, Width_d, Height_d, format, Fps_d);

		// check config
		//if (config.can_resolve(*(pipeline->operator std::shared_ptr<rs2_pipeline>))) {
		if(true){

			pipeline->start(config);
			cameraWorks = true;
		}
		else {
			UE_LOG(RealSenseLog, Error, TEXT("COnfigured Video Settings are wrong!"))
		}
	}
	catch (std::exception e) {
		UE_LOG(RealSenseLog, Error, TEXT("Realsense initialization error: %s"), e.what());
	}

	try {
		rs2::frameset frames = pipeline->wait_for_frames();
		rs2::video_frame colorFrame = frames.get_color_frame();
		CreateUpdateableTextures();

	}
	catch (std::exception e) {
		UE_LOG(RealSenseLog, Error, TEXT("Realsense video stream error: %s"), e.what());
		cameraWorks = false;
	}
}

bool ARealSenseComponent::receiveFrame()
{
	try {
		rs2::frameset frames = pipeline->wait_for_frames();
		rs2::video_frame colorFrame = frames.get_color_frame();
		uint8* data = (uint8*)(colorFrame.get_data());
		int height = colorFrame.get_height();
		int width = colorFrame.get_width();
		int channels = colorFrame.get_bytes_per_pixel();
		int bits = colorFrame.get_bits_per_pixel();
		int bytes = colorFrame.get_bytes_per_pixel();
		rs2_format format = colorFrame.get_profile().format();
		int end = width * height * channels;

		UE_LOG(RealSenseLog, Log, TEXT("Image Received. Resolution: %d/%d, Channels: %d, Format: %s"), width, height, channels, *FString(rs2_format_to_string(format)));
		UE_LOG(RealSenseLog, Log, TEXT("First/Last pixel: (%d/%d/%d), (%d/%d/%d)"), data[0], data[1], data[2], data[end - 3], data[end - 2], data[end - 1]);

		// tried differend updating methods
		TextureFromVideo->UpdateTextureRegions(0, 1, textureVideoRegion, static_cast<uint32>(width * bytes),
			bytes, data, texCleanUpFP);

		//UpdateTextureRegions(TextureFromVideo, 0, 1, textureVideoRegion, static_cast<uint32>(width * bytes), bytes, data, false);
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
void ARealSenseComponent::Tick(float DeltaTime)
{
	//TextureFromVideo->UpdateResource();
	Super::Tick(DeltaTime);

}

void ARealSenseComponent::UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
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