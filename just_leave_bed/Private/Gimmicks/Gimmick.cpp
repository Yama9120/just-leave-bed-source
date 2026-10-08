// Fill out your copyright notice in the Description page of Project Settings.

#include "Gimmicks/Gimmick.h"

void AGimmick::Start_Implementation()
{
}

void AGimmick::OnPlayerEnter(APlayerCharacter* Player)
{
	if (bRunning)
	{
		return;
	}
	else
	{
		bRunning = true;
		Start();
	}
}
