// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImpDamageAbility.h"
#include "ImpAbilityTypes.h"
#include "ProjectileAbility.generated.h"

/**
 * 
 */
UCLASS()
class IMP_API UProjectileAbility : public UImpDamageAbility {

	GENERATED_BODY()

public:
	
	UProjectileAbility();

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UPROPERTY(EditDefaultsOnly, Category="Custom Values|Projectiles")
	FGameplayTag ProjectileToSpawnTag;

	UPROPERTY()
	TObjectPtr<AActor> AvatarActorFromInfo;

	FProjectileParams CurrentProjectileParams;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile();
	
};
