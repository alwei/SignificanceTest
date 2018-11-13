// Fill out your copyright notice in the Description page of Project Settings.

#include "SignificanceTestGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "SignificanceManager.h"

#include "SignificanceActor.h"

float MySignificanceFunction(USignificanceManager::FManagedObjectInfo* Obj, const FTransform& InTransform);
void MyPostSignificanceFunction(USignificanceManager::FManagedObjectInfo* Obj, float OldSignificance, float Significance, bool bUnregistered);

ASignificanceTestGameModeBase::ASignificanceTestGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASignificanceTestGameModeBase::BeginPlay()
{
	// レベル上にいるSignificanceActorを配列にセット
	TArray<AActor*> ManageActors;
	UGameplayStatics::GetAllActorsOfClass(this, ASignificanceActor::StaticClass(), ManageActors);

	if (UWorld* World = GetWorld())
	{
		if (USignificanceManager* SignificanceManager = FSignificanceManagerModule::Get(World))
		{
			FName Tag(TEXT("Actor"));
			for (auto Actor : ManageActors)
			{
				// 配列に存在するアクターをSignificanceManagerに登録
				SignificanceManager->RegisterObject(Actor, Tag, MySignificanceFunction, USignificanceManager::EPostSignificanceType::Sequential, MyPostSignificanceFunction);
			}
		}
	}
}

void ASignificanceTestGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UWorld* World = GetWorld())
	{
		if (USignificanceManager* SignificanceManager = FSignificanceManagerModule::Get(World))
		{
			if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0))
			{
				TArray<FTransform> TransformArray;
				TransformArray.Add(PlayerPawn->GetActorTransform());

				// PlayerPawnをViewPointとして設定
				SignificanceManager->Update(TArrayView<FTransform>(TransformArray));
			}
		}
	}
}

void ASignificanceTestGameModeBase::EndPlay(const EEndPlayReason::Type Reason)
{
	if (UWorld* World = GetWorld())
	{
		if (USignificanceManager* SignificanceManager = FSignificanceManagerModule::Get(World))
		{
			// 全ての登録を解除
			SignificanceManager->UnregisterAll(FName(TEXT("Actor")));
		}
	}
}

// 基準となるSignificance距離値
static const float GSignificanceDistance = 2000.f;

float MySignificanceFunction(USignificanceManager::FManagedObjectInfo* Obj, const FTransform& InTransform)
{
	// 関数内はSignificanceManagerからParallelForによって、並列処理で呼ばれるので注意
	if (ASignificanceActor* Actor = Cast<ASignificanceActor>(Obj->GetObject()))
	{
		// プレイヤーと自分の距離からSignificance計算
		FVector Distance = InTransform.GetLocation() - Actor->GetActorLocation();
		if (Distance.Size() < GSignificanceDistance)
		{
			float Significance = 1.f - Distance.Size() / GSignificanceDistance;
			return Significance;
		}
	}

	return 0.f;
}

void MyPostSignificanceFunction(USignificanceManager::FManagedObjectInfo* Obj, float OldSignificance, float Significance, bool bUnregistered)
{
	if (ASignificanceActor* Actor = Cast<ASignificanceActor>(Obj->GetObject()))
	{
		if (Significance > 0.f)
		{
			// Significanceが0よりも上の場合はTickをオンにし、TextColorを赤に変更
			Actor->SetActorTickEnabled(true);
			Actor->SetTextColor(FColor::Red);
		}
		else
		{
			// Significanceが0になった場合はTickをオフにし、TextColorを白に変更
			Actor->SetActorTickEnabled(false);
			Actor->SetTextColor(FColor::White);
		}
	}
}

