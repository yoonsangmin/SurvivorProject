// Fill out your copyright notice in the Description page of Project Settings.


#include "PixelizerSceneCaptureComponent2D.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetMaterialLibrary.h"

UPixelizerSceneCaptureComponent2D::UPixelizerSceneCaptureComponent2D(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> SceneWidgetClassRef(
		TEXT("/PixelizerCamera/WBP_Scene.WBP_Scene_C"));

	if (SceneWidgetClassRef.Succeeded())
	{
		SceneWidgetClass = SceneWidgetClassRef.Class;
	}


	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> TextureTargetRef(
		TEXT("/PixelizerCamera/RT_Scene.RT_Scene"));

	if (TextureTargetRef.Succeeded())
	{
		if (StaticClass()->ClassDefaultObject && StaticClass()->ClassDefaultObject)
		TextureTarget = TextureTargetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialTargetRef(
		TEXT("/PixelizerCamera/PP_Downsale.PP_Downsale"));

	if (MaterialTargetRef.Succeeded())
	{
		DownscaleMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, MaterialTargetRef.Object);
		if (DownscaleMaterial)
		{
			UE_LOG(LogTemp, Display, TEXT("Created DownscaleMaterial"));
		}
		PostProcessSettings.WeightedBlendables.Array.Emplace(1.0f, DownscaleMaterial);
	}
}

void UPixelizerSceneCaptureComponent2D::BeginPlay()
{
	Super::BeginPlay();

	// 뷰포트 생성.
	SceneWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), SceneWidgetClass);
	SceneWidget->AddToViewport(0);

	// 화면 사이즈 변경 이벤트 콜백 등록.
	bResizePending = true;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->Viewport->ViewportResizedEvent.AddLambda([&](FViewport* Viewport, uint32)
		{
			bResizePending = true;
			ResizeRenderTarget();
		});
	}
}

void UPixelizerSceneCaptureComponent2D::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bResizePending)
	{
		bResizePending = !ResizeRenderTarget();
	}

	// 화면 사이즈에 따라 다운스케일 포스트 프로세스 변수 설정.
	if (DownscaleMaterial)
	{
		UE_LOG(LogTemp, Display, TEXT("DownscaleMaterial"));
		DownscaleMaterial->SetScalarParameterValue(FName("TargetHeight"), TargetHeight);
	}
	StaticClass();
}

bool UPixelizerSceneCaptureComponent2D::ResizeRenderTarget() const
{
	if (TextureTarget == nullptr || TextureTarget->GameThread_GetRenderTargetResource()->GetRenderTargetTexture() == nullptr)
	{
		return false;
	}
	
	FVector2d NewSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
	int32 NewHeight = TargetHeight * RenderScale;
	int32 NewWidth = (NewSize.X / NewSize.Y) * NewHeight;

	if ((NewWidth > 0) && (NewHeight > 0))
	{
		TextureTarget->ResizeTarget(NewWidth, NewHeight);
	}
	
	return true;
}
