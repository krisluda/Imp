// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

void UImpAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UImpAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UImpAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UImpAttributeSet, Mana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UImpAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UImpAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetHealthAttribute()) {
        SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
    }
    
    if (Data.EvaluatedData.Attribute == GetManaAttribute()) {
        SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
    }
}

void UImpAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UImpAttributeSet, Health, OldHealth);
}

void UImpAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData &OldMaxHealth) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UImpAttributeSet, Health, OldMaxHealth);
}

void UImpAttributeSet::OnRep_Mana(const FGameplayAttributeData &OldMana) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UImpAttributeSet, Mana, OldMana);
}

void UImpAttributeSet::OnRep_MaxMana(const FGameplayAttributeData &OldMaxMana) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UImpAttributeSet, MaxMana, OldMaxMana);
}
