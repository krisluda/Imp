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

    if (Data.EvaluatedData.Attribute == GetIncomingHealthDamageAttribute()) {
        HandleIncomingHealthDamage(Data);
    }

    if (Data.EvaluatedData.Attribute == GetIncomingShieldDamageAttribute()) {
        HandleIncomingShieldDamage(Data);
    }
}

void UImpAttributeSet::HandleIncomingHealthDamage(const FGameplayEffectModCallbackData &Data) {
    const float LocalDamage = GetIncomingHealthDamage();
    SetIncomingHealthDamage(0.f);

    FGameplayEffectContextHandle ContextHandle = Data.EffectSpec.GetContext();
    FImpGameplayEffectContext* ImpContext = FImpGameplayEffectContext::GetEffectContext(ContextHandle);
    FColor DebugColor = ImpContext->IsCriticalHit() ? FColor::Red : FColor::Green;

    IMP_DEBUGMSG(DebugColor, "UImpAttributeSet::HandleIncomingHealthDamage: Damage dealt %d", FMath::TruncToInt(LocalDamage));;
    
    SetHealth(FMath::Clamp(GetHealth() - LocalDamage, 0.f, GetMaxHealth()));
}

void UImpAttributeSet::HandleIncomingShieldDamage(const FGameplayEffectModCallbackData & Data) {
    const float LocalShieldDamage = GetIncomingShieldDamage();
    SetIncomingShieldDamage(0.f);

    FGameplayEffectContextHandle ContextHandle = Data.EffectSpec.GetContext();
    FImpGameplayEffectContext* ImpContext = FImpGameplayEffectContext::GetEffectContext(ContextHandle);
    FColor DebugColor = ImpContext->IsCriticalHit() ? FColor::Red : FColor::Green;

    IMP_DEBUGMSG(DebugColor, "UImpAttributeSet::HandleIncomingShieldDamage: Damage dealt %d", FMath::TruncToInt(LocalShieldDamage));;

    SetShield(FMath::Clamp(GetShield() - LocalShieldDamage, 0.f, GetMaxShield()));
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