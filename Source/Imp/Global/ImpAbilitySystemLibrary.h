// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ImpAbilitySystemLibrary.generated.h"

class UImpCharacterClassInfo;
class UProjectileInfo;

UCLASS()
class IMP_API UImpAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	//This getter is some abstract getters from Uhr, he says don't wanna use it unless you're on the server (bc of WorldContextObject?)
	UFUNCTION(BlueprintPure)
	static UImpCharacterClassInfo* GetImpCharacterClassDefaultInfo(const UObject* WorldContextObject);
	
	//This getter is some abstract getters from Uhr, he says don't wanna use it unless you're on the server (bc of WorldContextObject?)
	UFUNCTION(BlueprintPure)
	static UProjectileInfo* GetProjectileInfo(const UObject* WorldContextObject);

	template<typename T> 
	static T* GetDataTableRowByTag(UDataTable* DataTable, FGameplayTag Tag);

};

template <typename T>
T* UImpAbilitySystemLibrary::GetDataTableRowByTag(UDataTable* DataTable, FGameplayTag Tag) {
    //Uhr: a very flexible getter (episode 6) https://www.youtube.com/watch?v=GApyM5gRFRI
	return DataTable->FindRow<T>(Tag.GetTagName(), FString(""));
}
