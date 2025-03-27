// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ImpGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class IMP_API UImpGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category="Custom Values|Input")
	FGameplayTag InputTag;
	
};
