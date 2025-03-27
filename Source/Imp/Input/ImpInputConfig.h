// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ImpInputConfig.generated.h"

class UInputAction;

USTRUCT() 
struct FImpInputAction {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

UCLASS()
class IMP_API UImpInputConfig : public UDataAsset {

	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
	TArray<FImpInputAction> ImpInputActions;
};
