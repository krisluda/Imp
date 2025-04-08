// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "ImpAbilityTypes.generated.h"

class AProjectileBase;
class UGameplayEffect;
class UAbilitySystemComponent;

USTRUCT()
struct FImpGameplayEffectContext : public FGameplayEffectContext {
//This child of FGameplayEffectContext is passed around at the right spots to be able to use it at the right times. Pack extra data into one of these, but required a bit of effort.
	GENERATED_BODY();

	bool IsCriticalHit() const { return bCriticalHit; }	

	void SetIsCriticalHit(const bool InCriticalHit) { bCriticalHit = InCriticalHit; }

	//We can't cast to a child struct, so this specific function lets us get this (i.e. in the Exec_Calc)
	static IMP_API
	FImpGameplayEffectContext* GetEffectContext(FGameplayEffectContextHandle Handle);

	virtual UScriptStruct* GetScriptStruct() const override {
		return StaticStruct();
	}
	
	virtual FImpGameplayEffectContext* Duplicate() const override {
		FImpGameplayEffectContext* NewContext = new FImpGameplayEffectContext();
		*NewContext = *this;

		if (GetHitResult()) {
			NewContext->AddHitResult(*GetHitResult(), true);
		}

		return NewContext;
	}

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

//ok
private:

	UPROPERTY()
	bool bCriticalHit = false;

};

template<>
struct TStructOpsTypeTraits<FImpGameplayEffectContext> : TStructOpsTypeTraitsBase2<FImpGameplayEffectContext> {
	enum {
		WithNetSerializer = true,
		WithCopy = true,
	};
};

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