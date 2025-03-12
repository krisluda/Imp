// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "UObject/ObjectMacros.h"
#include "ImpAbilitySystemComponent.h"
#include "ImpAttributeSet.generated.h"

/**
 * 
 */

UCLASS()
class IMP_API UImpAttributeSet : public UAttributeSet {
	
	GENERATED_BODY()
	
	public:
	UImpAttributeSet();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);
};
