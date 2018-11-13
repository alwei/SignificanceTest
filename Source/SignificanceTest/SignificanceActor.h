// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SignificanceActor.generated.h"

class UTextRenderComponent;

UCLASS()
class SIGNIFICANCETEST_API ASignificanceActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASignificanceActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SetTextColor(FColor Color);

	UPROPERTY()
	UTextRenderComponent* SignificanceText;
};
