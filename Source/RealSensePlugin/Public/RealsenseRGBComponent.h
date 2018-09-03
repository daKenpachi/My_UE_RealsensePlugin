// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/Texture2D.h"
#include "rs.hpp"
#include "RealsenseRGBComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class REALSENSEPLUGIN_API URealsenseRGBComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URealsenseRGBComponent();

	UFUNCTION(BlueprintCallable, Category = "Realsense")
		bool ReceiveFrame();

	UFUNCTION(BlueprintCallable, Category = "Realsense")
		UTexture2D* getTextureRGB();

	// from https://www.mouser.com/pdfdocs/Intel_D400_Series_Datasheet.pdf for D435
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Realsense|Intrinsics")
		float VerticalFOV = 69.4;		// in degree
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Realsense|Intrinsics")
		float HorizontalFOV = 42.5;		// in degree
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Realsense|Intrinsics")
		int FocalLength = 0.193;		// in cm



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Realsense|Video")
		int WidthRGB = 1920;
	UPROPERTY(BlueprintReadOnly, Category = "Realsense|Video")
		int HeightRGB = 1080;

	UPROPERTY(BlueprintReadOnly, Category = "Realsense")
		UTexture2D* TextureRGB = nullptr;
	FUpdateTextureRegion2D*  TextureUpdateRegion = nullptr;

	rs2::pipeline_profile selection;
	rs2::pipeline* pipeline = nullptr;

	void TextureRegionCleanUp(uint8* rawData, const FUpdateTextureRegion2D* region) {};
	TFunction<void(uint8*, const FUpdateTextureRegion2D*)> texCleanUpFP = [this](uint8* rawData, const FUpdateTextureRegion2D* region) { TextureRegionCleanUp(rawData, region); };


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



};
