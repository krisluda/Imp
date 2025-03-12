// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpAttributeSet.h"
#include "Net/UnrealNetwork.h"

UImpAttributeSet::UImpAttributeSet() {
    Health.SetBaseValue(1000.0f);
    Health.SetCurrentValue(1000.0f);
}

void UImpAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UImpAttributeSet, Health, OldValue);
}

void UImpAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UImpAttributeSet, Health, COND_None, REPNOTIFY_Always);
}