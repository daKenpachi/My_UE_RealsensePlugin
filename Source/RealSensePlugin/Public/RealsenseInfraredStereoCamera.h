// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/Texture2D.h>
#include "rs.hpp"
#include "RealsenseInfraredStereoCamera.generated.h"


UCLASS(ClassGroup = (Custom))
class REALSENSEPLUGIN_API ARealsenseInfraredStereoCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARealsenseInfraredStereoCamera();

	UFUNCTION(BlueprintCallable, Category = "Realsense")
		bool UpdateTextures();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay();
	UPROPERTY(BlueprintReadOnly, Category = "Realsense|Video")
	int WidthLeft = 1280;
	UPROPERTY(BlueprintReadOnly, Category = "Realsense|Video")
	int HeightLeft = 800;
	UPROPERTY(BlueprintReadOnly, Category = "Realsense|Video")
	int WidthRight = 1280;
	UPROPERTY(BlueprintReadOnly, Category = "Realsense|Video")
	int HeightRight = 800;

	UPROPERTY(BlueprintReadOnly, Category = "Realsense")
	UTexture2D* TextureLeft = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Realsense")
	UTexture2D* TextureRight = nullptr;
	FUpdateTextureRegion2D*  TextureUpdater = nullptr;

	rs2::pipeline_profile selection;
	rs2::pipeline* pipeline = nullptr;
	
	void TextureRegionCleanUp(uint8* rawData, const FUpdateTextureRegion2D* region) {};
	TFunction<void(uint8*, const FUpdateTextureRegion2D*)> texCleanUpFP = [this](uint8* rawData, const FUpdateTextureRegion2D* region) { TextureRegionCleanUp(rawData, region); };

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
private:

};

//UENUM()
enum InfraredSensor {
	LEFT = 1,
	RIGHT = 2
};
