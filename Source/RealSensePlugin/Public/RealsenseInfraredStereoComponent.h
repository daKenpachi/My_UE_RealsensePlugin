// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include <Engine/Texture2D.h>
#include "rs.hpp"
#include "RealsenseEnums.h"
#include "RealsenseInfraredStereoComponent.generated.h"


UCLASS(ClassGroup = (Realsense), meta=(BlueprintSpawnableComponent))
class REALSENSEPLUGIN_API URealsenseInfraredStereoComponent : public USceneComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	URealsenseInfraredStereoComponent();

	UFUNCTION(BlueprintCallable, Category = "Realsense")
		bool UpdateTextures();

	UFUNCTION(BlueprintCallable, Category = "Realsense")
		UTexture2D* getTextureLeft();

	UFUNCTION(BlueprintCallable, Category = "Realsense")
		UTexture2D* getTextureRight();


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
	
private:

};


