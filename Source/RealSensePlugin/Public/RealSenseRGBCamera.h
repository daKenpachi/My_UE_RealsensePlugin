// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/Texture2D.h>
#include <rs.hpp>
#include "RealSenseRGBCamera.generated.h"

// #define DEBUG_LOG

UCLASS( ClassGroup=(Custom))
class REALSENSEPLUGIN_API ARealSenseRGBCamera : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ARealSenseRGBCamera();


	UFUNCTION(BlueprintCallable, Category = "RealSense")
		UTexture2D* ReceiveRGBFrame();

	UFUNCTION(BlueprintCallable, Category = "RealSense")
		UTexture2D* ReceiveDepthFrame();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//virtual void EndPlay() override;
	
	/** Initialize textures and Update Regions*/
	void CreateUpdateableTextures();

	/** Reveice a frame from realsense device and update texture*/
	UFUNCTION(BluePrintCallable, Category = "Realsense")
	bool receiveFrames();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Video|Color")
		int Width_c = 1280;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Video|Color")
		int Height_c = 720;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Video|Color")
		int Fps_c = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Video|Depth")
		int Width_d = 1280;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Video|Depth")
		int Height_d = 720;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Video|Depth")
		int Fps_d = 15;
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Video|Depth")
		bool Enable_d = false;

	// texture to update
	UPROPERTY(BlueprintReadOnly, Category = "Realsense")
	UTexture2D* TextureFromVideo = nullptr;
	FUpdateTextureRegion2D* textureVideoRegion = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Realsense")
	UTexture2D* TextureFromDepth = nullptr;
	FUpdateTextureRegion2D* textureDepthRegion = nullptr;

	// Realsense device pipeline
	rs2::pipeline* pipeline = nullptr;
	rs2::colorizer depthColorizer;

	// from SRWORKS plugin
	void TextureRegionCleanUp(uint8* rawData, const FUpdateTextureRegion2D* region) {};
	TFunction<void(uint8*, const FUpdateTextureRegion2D*)> texCleanUpFP = [this](uint8* rawData, const FUpdateTextureRegion2D* region) { TextureRegionCleanUp(rawData, region); };


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool cameraWorks = false;
	
};
