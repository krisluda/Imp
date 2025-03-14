// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ImpAbilitySystemLibrary.generated.h"

class UImpCharacterClassInfo;

UCLASS()
class IMP_API UImpAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintPure)
	static UImpCharacterClassInfo* GetImpCharacterClassDefaultInfo(const UObject* WorldContextObject);

};
