// Fill out your copyright notice in the Description page of Project Settings.

#include "ImpDamageAbility.h"

#include "AbilitySystemGlobals.h"
#include "ImpAbilityTypes.h"

void UImpDamageAbility::CaptureDamageEffectInfo(AActor* TargetActor, FDamageEffectInfo& OutInfo) {
    if (AActor* AvatarActorFromInfo = GetAvatarActorFromActorInfo()) {
        OutInfo.AbilityLevel = GetAbilityLevel();
        OutInfo.AvatarActor = AvatarActorFromInfo;
        OutInfo.BaseDamage = BaseDamage.GetValueAtLevel(GetAbilityLevel());
        OutInfo.DamageEffect = DamageEffect;
        OutInfo.SourceASC = GetAbilitySystemComponentFromActorInfo();

        if (IsValid(TargetActor)) {
            OutInfo.TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
        }
    }
}