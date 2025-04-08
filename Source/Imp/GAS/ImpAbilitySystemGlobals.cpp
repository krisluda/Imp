// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpAbilitySystemGlobals.h"
#include "ImpAbilityTypes.h"

FGameplayEffectContext* UImpAbilitySystemGlobals::AllocGameplayEffectContext() const {
    return new FImpGameplayEffectContext();
}