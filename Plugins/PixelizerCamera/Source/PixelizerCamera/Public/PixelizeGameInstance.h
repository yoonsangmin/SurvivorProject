// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PixelizeGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PIXELIZERCAMERA_API UPixelizeGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

private:
	void SetScreenPercentage();
	void CheckFinishResize();

	UFUNCTION(BlueprintCallable)
	void SetTargetHeight(int32 InTargetHeight); 
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BlueprintSetter = "SetTargetHeight"))
	int32 TargetHeight = 240;

private:
	FDelegateHandle ResizeDelegateHandle;
	uint8 bOnResizing : 1 = false;
};
