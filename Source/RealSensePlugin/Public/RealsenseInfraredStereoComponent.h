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
		bool ReceiveFrame();

	UFUNCTION(BlueprintCallable, Category = "Realsense")
		UTexture2D* getTextureLeft();

	UFUNCTION(BlueprintCallable, Category = "Realsense")
		UTexture2D* getTextureRight();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Realsense|Video")
	int WidthLeft = 1280;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Realsense|Video")
	int HeightLeft = 800;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Realsense|Video")
	int WidthRight = 1280;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Realsense|Video")
	int HeightRight = 800;

	// from https://www.mouser.com/pdfdocs/Intel_D400_Series_Datasheet.pdf fpr D435
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Realsense|Intrinsics")
		float VerticalFOV = 91.2;		// in degree
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Realsense|Intrinsics")
		float HorizontalFOV = 65.5;		// in degree
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Realsense|Intrinsics")
		float StereoBaseline = 5;		// in cm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Realsense|Intrinsics")
		int FocalLength = 0.193;		// in cm

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay();




	UPROPERTY(BlueprintReadOnly, Category = "Realsense")
	UTexture2D* TextureLeft = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Realsense")
	UTexture2D* TextureRight = nullptr;
	FUpdateTextureRegion2D*  TextureUpdateRegion = nullptr;

	rs2::pipeline_profile selection;
	rs2::pipeline* pipeline = nullptr;
	
	void TextureRegionCleanUp(uint8* rawData, const FUpdateTextureRegion2D* region) {};
	TFunction<void(uint8*, const FUpdateTextureRegion2D*)> texCleanUpFP = [this](uint8* rawData, const FUpdateTextureRegion2D* region) { TextureRegionCleanUp(rawData, region); };

public:	
	
private:

};


