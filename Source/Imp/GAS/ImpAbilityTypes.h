// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImpAbilityTypes.generated.h"

class AProjectileBase;
class UGameplayEffect;
class UAbilitySystemComponent;

USTRUCT()
struct FProjectileParams {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectileBase> ProjectileClass;

// Add i.e. a Niagara System to represent or accompany the projectile here
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> ProjectileMesh;

// Here we can add more properties we need to adjust or control per projectile type
	UPROPERTY(EditDefaultsOnly)
	float InitialSpeed = 1000.f;
	
	UPROPERTY(EditDefaultsOnly)
	float GravityScale = 1.f;
	
	UPROPERTY(EditDefaultsOnly)
	bool bShouldBounce = false;
	
	UPROPERTY(EditDefaultsOnly)
	float Bounciness = 0.6f;
};

USTRUCT(BlueprintType)
struct FDamageEffectInfo {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> AvatarActor = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffect = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceASC = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetASC = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

};