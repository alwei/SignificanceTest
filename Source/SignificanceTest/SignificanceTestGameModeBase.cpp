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
	// ���x����ɂ���SignificanceActor��z��ɃZ�b�g
	TArray<AActor*> ManageActors;
	UGameplayStatics::GetAllActorsOfClass(this, ASignificanceActor::StaticClass(), ManageActors);

	if (UWorld* World = GetWorld())
	{
		if (USignificanceManager* SignificanceManager = FSignificanceManagerModule::Get(World))
		{
			FName Tag(TEXT("Actor"));
			for (auto Actor : ManageActors)
			{
				// �z��ɑ��݂���A�N�^�[��SignificanceManager�ɓo�^
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

				// PlayerPawn��ViewPoint�Ƃ��Đݒ�
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
			// �S�Ă̓o�^������
			SignificanceManager->UnregisterAll(FName(TEXT("Actor")));
		}
	}
}

// ��ƂȂ�Significance�����l
static const float GSignificanceDistance = 2000.f;

float MySignificanceFunction(USignificanceManager::FManagedObjectInfo* Obj, const FTransform& InTransform)
{
	// �֐�����SignificanceManager����ParallelFor�ɂ���āA���񏈗��ŌĂ΂��̂Œ���
	if (ASignificanceActor* Actor = Cast<ASignificanceActor>(Obj->GetObject()))
	{
		// �v���C���[�Ǝ����̋�������Significance�v�Z
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
			// Significance��0������̏ꍇ��Tick���I���ɂ��ATextColor��ԂɕύX
			Actor->SetActorTickEnabled(true);
			Actor->SetTextColor(FColor::Red);
		}
		else
		{
			// Significance��0�ɂȂ����ꍇ��Tick���I�t�ɂ��ATextColor�𔒂ɕύX
			Actor->SetActorTickEnabled(false);
			Actor->SetTextColor(FColor::White);
		}
	}
}

