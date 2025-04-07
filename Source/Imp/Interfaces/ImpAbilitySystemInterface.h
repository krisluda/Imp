// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
//#include "GameplayTagContainer.h"
#include "ImpAbilitySystemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UImpAbilitySystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IMP_API IImpAbilitySystemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// This is used on the character as a placeholder for spawning projectiles. Might need to change later to sockets or smth.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USceneComponent* GetDynamicSpawnPoint();

	// Uhr said this need the include, but it compiles without, so I'm testing without.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetDynamicProjectile(const FGameplayTag& ProjectileTag);
};
