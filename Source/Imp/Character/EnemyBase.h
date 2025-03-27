// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CharacterBase.h"
#include "EnemyBase.generated.h"

class UImpAbilitySystemComponent;
class UImpAttributeSet;

/**
 * 
 */
UCLASS()
class IMP_API AEnemyBase : public ACharacterBase, public IAbilitySystemInterface {
	GENERATED_BODY()
	
public:

	AEnemyBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void BindCallbacksToDependencies() override;
	virtual void InitClassDefaults() override;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UImpAbilitySystemComponent> ImpAbilitySystemComp;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UImpAttributeSet> ImpAttributes;

};
