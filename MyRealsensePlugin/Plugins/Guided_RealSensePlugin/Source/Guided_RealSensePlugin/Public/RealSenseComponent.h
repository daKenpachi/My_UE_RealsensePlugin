// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Image.h"
#include <Engine/Texture.h>
#include <Engine/Texture2D.h>
#include <Engine/Classes/Materials/MaterialInstanceDynamic.h>
#include <Engine/Classes/Materials/MaterialInterface.h>
#include <rs.hpp>
#include "RealSenseComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class REALSENSEPLUGIN_API URealSenseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URealSenseComponent();

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Guided_RealSensePlugin")
	FString filePath;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Guided_RealSensePlugin")
		UTexture* MediaTexture = nullptr;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Guided_RealSensePlugin")
		UMaterialInterface* MaterialInstanceToUpdate = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Guided_RealSensePlugin")
		UMaterialInstanceDynamic* MaterialToUpdateDynamic = nullptr;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool receiveFrame();

	UTexture2D* TextureFromVideo = nullptr;
	FUpdateTextureRegion2D* textureVideoRegion = nullptr;

	rs2::pipeline* pipeline = nullptr;

	void TextureRegionCleanUp(uint8* rawData, const FUpdateTextureRegion2D* region) {};
	TFunction<void(uint8*, const FUpdateTextureRegion2D*)> texCleanUpFP = [this](uint8* rawData, const FUpdateTextureRegion2D* region) { TextureRegionCleanUp(rawData, region); };


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool cameraWorks = false;
	
};
