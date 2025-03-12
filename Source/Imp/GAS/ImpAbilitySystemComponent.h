// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ImpAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class IMP_API UImpAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	public:
	UImpAbilitySystemComponent();

	virtual void BeginPlay() override;

};
