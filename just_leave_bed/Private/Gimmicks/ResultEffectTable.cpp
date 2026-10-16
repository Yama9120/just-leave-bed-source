// Fill out your copyright notice in the Description page of Project Settings.

#include "Gimmicks/ResultEffectTable.h"
#include "NiagaraSystem.h"

UNiagaraSystem* UResultEffectTable::Find(FGameplayTag Result) const
{
	return Effects.FindRef(Result);
}
