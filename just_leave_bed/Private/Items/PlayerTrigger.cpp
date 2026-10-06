// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/PlayerTrigger.h"
#include "Components/BoxComponent.h"
#include "Characters/PlayerCharacter.h"

APlayerTrigger::APlayerTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
}

void APlayerTrigger::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &APlayerTrigger::OnBoxBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &APlayerTrigger::OnBoxEndOverlap);
}

void APlayerTrigger::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 重なったアクターがプレイヤーキャラクターかどうかを確認
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		OnPlayerEnter(Player);
	}
}

void APlayerTrigger::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 離れたアクターがプレイヤーキャラクターかどうかを確認
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		OnPlayerExit(Player);
	}
}

void APlayerTrigger::OnPlayerEnter(APlayerCharacter* Player)
{
}

void APlayerTrigger::OnPlayerExit(APlayerCharacter* Player)
{
}
