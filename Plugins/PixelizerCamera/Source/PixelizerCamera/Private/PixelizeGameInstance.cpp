// Fill out your copyright notice in the Description page of Project Settings.


#include "PixelizeGameInstance.h"

void UPixelizeGameInstance::Init()
{
	Super::Init();

	static IConsoleVariable* UpscaleQuality = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Upscale.Quality"));
	if (UpscaleQuality)
	{
		UpscaleQuality->Set(0);
	}
	
	ResizeDelegateHandle = GEngine->GameViewport->Viewport->ViewportResizedEvent.AddLambda([&](FViewport* Viewport, uint32)
	{		
		if (!bOnResizing)
		{
			bOnResizing = true;
			GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UPixelizeGameInstance::CheckFinishResize);
		}
	});
}

void UPixelizeGameInstance::Shutdown()
{
	Super::Shutdown();
	GEngine->GameViewport->Viewport->ViewportResizedEvent.Remove(ResizeDelegateHandle);
}

void UPixelizeGameInstance::SetScreenPercentage()
{
	static IConsoleVariable* OnScreenPercentageChanged = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
	if (OnScreenPercentageChanged)
	{
		FIntPoint ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
		float NewPercentage =  StaticCast<float>(TargetHeight) / ViewportSize.Y * 100; 
		OnScreenPercentageChanged->Set(NewPercentage);
	}
}

void UPixelizeGameInstance::CheckFinishResize()
{
	SetScreenPercentage();
	bOnResizing = false;
}

void UPixelizeGameInstance::SetTargetHeight(int32 InTargetHeight)
{
	TargetHeight = InTargetHeight;
	SetScreenPercentage();
}
