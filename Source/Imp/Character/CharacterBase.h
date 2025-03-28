// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "CharacterBase.generated.h"

UCLASS()
class IMP_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

	//**Uhr says these will be refactored later. I wonder if they should be on PlayerState at some point**//
    UFUNCTION(BlueprintImplementableEvent)
    void OnHealthChanged(float CurrentHealth, float MaxHealth);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnManaChanged(float CurrentMana, float MaxMana);
	//** End **//

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();
	virtual void InitClassDefaults();
	virtual void BindCallbacksToDependencies();

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();

	UPROPERTY(EditAnywhere, Category = "Custom Values|Character Info")
    FGameplayTag CharacterTag;

};
