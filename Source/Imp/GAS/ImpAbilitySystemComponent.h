// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ImpAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttributesGiven);

UCLASS()
class IMP_API UImpAbilitySystemComponent : public UAbilitySystemComponent {
	GENERATED_BODY()

public:

	FOnAttributesGiven OnAttributesGiven;

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToGrant);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassivesToGrant);
	void InitializeDefaultAttributes(const TSubclassOf<UGameplayEffect>& AttributeEffect);

	void AbilityInputPressed(const FGameplayTag& InputTag);
	void AbilityInputReleased(const FGameplayTag& InputTag);
};
