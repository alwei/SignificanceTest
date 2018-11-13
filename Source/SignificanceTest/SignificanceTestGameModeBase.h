// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "SignificanceTestGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SIGNIFICANCETEST_API ASignificanceTestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:	
	ASignificanceTestGameModeBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
};
