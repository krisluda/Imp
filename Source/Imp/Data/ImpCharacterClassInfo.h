// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ImpCharacterClassInfo.generated.h"

class UGameplayEffect;
class UGameplayAbility;

USTRUCT() 
struct FImpCharacterClassDefaultInfo {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> StartingAbilities;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> StartingPassives;
};

UCLASS()
class IMP_API UImpCharacterClassInfo : public UDataAsset {
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FImpCharacterClassDefaultInfo> ClassDefaultInfoMap;
	
};
