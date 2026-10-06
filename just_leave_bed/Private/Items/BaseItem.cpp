// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/BaseItem.h"
#include "Components/BoxComponent.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = true;

	// ItemMesh をルートにし、Box をその子に付け替える
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);
	Box->SetupAttachment(ItemMesh);
}

float ABaseItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	// オプションで上下に浮遊するようにアイテムを移動させる
	if (bEnableFloating)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
}
