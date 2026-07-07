// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/BaseItem.h"
#include "Components/BoxComponent.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(GetRootComponent());
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnBoxOverlap);
}

float ABaseItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

void ABaseItem::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FString OtherActorName = OtherActor->GetName();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Overlapped with: %s"), *OtherActorName));
	}
}

void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
}
