// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PixelizerSceneCaptureComponent2D.generated.h"

/**
 * 
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class PIXELIZERCAMERA_API UPixelizerSceneCaptureComponent2D : public USceneCaptureComponent2D
{
	GENERATED_BODY()

public:
	//UPixelizerSceneCaptureComponent2D();
	UPixelizerSceneCaptureComponent2D(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool ResizeRenderTarget() const;
	
protected:
	//TODO: BlueprintSetter 적용하기.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pixelizer", meta = (ClampMin = "10", ClampMax = "480", UIMin = "10", UIMax = "480"))
	int32 TargetHeight = 240;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pixelizer", meta = (ClampMin = "0.5", ClampMax = "4.0", UIMin = "0.5", UIMax = "4.0"))
	float RenderScale = 2.0f;
	
private:
	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> DownscaleMaterial;
	TSubclassOf<class UUserWidget> SceneWidgetClass;
	TObjectPtr<class UUserWidget> SceneWidget;

	uint8 bResizePending : 1 = false;
};
