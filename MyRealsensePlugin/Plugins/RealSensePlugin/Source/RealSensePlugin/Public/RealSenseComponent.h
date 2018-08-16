// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/Texture.h>
#include <Engine/Texture2D.h>
#include <Engine/Classes/Materials/MaterialInstanceDynamic.h>
#include <Engine/Classes/Materials/MaterialInterface.h>
#include <rs.hpp>
#include "RealSenseComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class REALSENSEPLUGIN_API ARealSenseComponent : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ARealSenseComponent();

	//UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "RealSense")
	//FString filePath;

	//UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "RealSense")
	//	UTexture* MediaTexture = nullptr;

	//UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "RealSense")
	//	UMaterialInterface* MaterialInstanceToUpdate = nullptr;

	//UPROPERTY(BlueprintReadOnly, Category = "RealSense")
	//	UMaterialInstanceDynamic* MaterialToUpdateDynamic = nullptr;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "VideoSettings")
		int  Width = 640;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "VideoSettings")
		int Height = 480;

	UFUNCTION(BlueprintCallable, Category = "RealSense")
		void CreateUpdateableTexture(int img_width, int img_height);

	UFUNCTION(BlueprintCallable, Category = "RealSense")
		UTexture2D* ReceiveRGBFrame();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool receiveFrame();

	UTexture2D* TextureFromVideo = nullptr;
	FUpdateTextureRegion2D* textureVideoRegion = nullptr;

	rs2::pipeline* pipeline = nullptr;

	/** Update texture region from https://wiki.unrealengine.com/Dynamic_Textures */
	void UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData);


	void TextureRegionCleanUp(uint8* rawData, const FUpdateTextureRegion2D* region) {};
	TFunction<void(uint8*, const FUpdateTextureRegion2D*)> texCleanUpFP = [this](uint8* rawData, const FUpdateTextureRegion2D* region) { TextureRegionCleanUp(rawData, region); };


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool cameraWorks = false;
	
};
