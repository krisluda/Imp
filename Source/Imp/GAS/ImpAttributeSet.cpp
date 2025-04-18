// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "ImpAbilityTypes.h"
#include "Log.h"
#include "Net/UnrealNetwork.h"

void UImpAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UImpAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UImpAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UImpAttributeSet, Mana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UImpAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UImpAttributeSet, Shield, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UImpAttributeSet, MaxShield, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UImpAttributeSet, DamageReduction, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UImpAttributeSet, CritChance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UImpAttributeSet, CritDamage, COND_None, REPNOTIFY_Always);
}

void UImpAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetHealthAttribute()) {
        SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
    }
    
    if (Data.EvaluatedData.Attribute == GetManaAttribute()) {
        SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
    }

    if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute()) {
        HandleIncomingDamage(Data);
    }
}

void UImpAttributeSet::HandleIncomingDamage(const FGameplayEffectModCallbackData& Data) {
    const float LocalDamage = GetIncomingDamage();
    SetIncomingDamage(0.f);
    
    const float LocalShield = GetShield();
    float OutShield = 0.f;

    if (LocalDamage > 0.f && LocalShield > 0.f) {
        OutShield = LocalShield - LocalDamage;
        SetShield(FMath::Clamp(OutShield, 0.f, GetMaxShield()));
    }

    if (LocalDamage > 0.f && OutShield <= 0.f) {
        const float RemainderDamage = fabs(LocalShield - LocalDamage);
        SetHealth(FMath::Clamp(GetHealth() - RemainderDamage, 0.f, GetMaxHealth()));
    }
}

void UImpAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UImpAttributeSet, Health, OldHealth);
}

void UImpAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UImpAttributeSet, MaxHealth, OldMaxHealth);
}

void UImpAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UImpAttributeSet, Mana, OldMana);
}

void UImpAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UImpAttributeSet, MaxMana, OldMaxMana);
}

void UImpAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldShield) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UImpAttributeSet, Shield, OldShield);
}

void UImpAttributeSet::OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UImpAttributeSet, MaxShield, OldMaxShield);
}

void UImpAttributeSet::OnRep_DamageReduction(const FGameplayAttributeData& OldDamageReduction) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UImpAttributeSet, DamageReduction, OldDamageReduction);
}

void UImpAttributeSet::OnRep_CritChance(const FGameplayAttributeData& OldCritChance) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UImpAttributeSet, CritChance, OldCritChance);
}

void UImpAttributeSet::OnRep_CritDamage(const FGameplayAttributeData& OldCritDamage) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UImpAttributeSet, CritDamage, OldCritDamage);
}