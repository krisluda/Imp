// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImpGameplayAbility.h"
#include "ImpDamageAbility.generated.h"

struct FDamageEffectInfo;
/**
 * 
 */
UCLASS()
class IMP_API UImpDamageAbility : public UImpGameplayAbility
{
	GENERATED_BODY()

public:

	void CaptureDamageEffectInfo(AActor* TargetActor, FDamageEffectInfo& OutInfo);

//really?
private: 

	UPROPERTY(EditDefaultsOnly, Category="Custom Values|Damage Effect")
	TSubclassOf<UGameplayEffect> DamageEffect;

	//GAS only variable ScalableFloat. Uhr: "This is an insane variable. I love this."
	UPROPERTY(EditDefaultsOnly, Category="Custom Values|Damage Effect")
	FScalableFloat BaseDamage;
};
