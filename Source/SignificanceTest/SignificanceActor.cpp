// Fill out your copyright notice in the Description page of Project Settings.

#include "SignificanceActor.h"

#include "Engine/Classes/Components/TextRenderComponent.h"
#include "SignificanceManager.h"


ASignificanceActor::ASignificanceActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SignificanceText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("SignificanceText"));
	SignificanceText->SetText(FText::AsNumber(0.f));
	SignificanceText->SetWorldScale3D(FVector(5.f));
}

void ASignificanceActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASignificanceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UWorld* World = GetWorld())
	{
		if (USignificanceManager* SignificanceManager = FSignificanceManagerModule::Get(World))
		{
			// Significance‚ðŽæ“¾‚µATextRender‚É•\Ž¦
			float Significance = SignificanceManager->GetSignificance(this);
			SignificanceText->SetText(FText::AsNumber(Significance));
		}
	}
}

void ASignificanceActor::SetTextColor(FColor Color)
{
	SignificanceText->SetTextRenderColor(Color);
}
