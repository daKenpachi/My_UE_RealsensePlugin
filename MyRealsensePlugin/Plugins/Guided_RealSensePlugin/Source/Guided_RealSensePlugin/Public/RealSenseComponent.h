// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Image.h"
#include "RealSenseComponent.generated.h"

#include <rs.hpp>


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GUIDED_REALSENSEPLUGIN_API URealSenseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URealSenseComponent();

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Guided_RealSensePlugin")
	FString filePath;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Guided_RealSensePlugin")
		UTexture2D* TextureVideo = nullptr;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool receiveFrame();

	FUpdateTextureRegion2D* textureVideoRegion = nullptr;

	rs2::pipeline* pipeline = nullptr;

	void TextureRegionCleanUp(uint8* rawData, const FUpdateTextureRegion2D* region) {};
	TFunction<void(uint8*, const FUpdateTextureRegion2D*)> texCleanUpFP = [this](uint8* rawData, const FUpdateTextureRegion2D* region) { TextureRegionCleanUp(rawData, region); };


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool cameraWorks = false;
	
};
