// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpAbilitySystemLibrary.h"
#include "ImpGameMode.h"
#include "GameplayEffectTypes.h"
#include "ImpAbilityTypes.h"
#include "ImpGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"


UImpCharacterClassInfo* UImpAbilitySystemLibrary::GetImpCharacterClassDefaultInfo(const UObject* WorldContextObject) {
    if (const AImpGameMode* ImpGameMode = Cast<AImpGameMode>(UGameplayStatics::GetGameMode(WorldContextObject))) {
        return ImpGameMode->GetImpCharacterClassDefaultInfo();
    }

    return nullptr;
}

UProjectileInfo* UImpAbilitySystemLibrary::GetProjectileInfo(const UObject* WorldContextObject) {
    if (const AImpGameMode* ImpGameMode = Cast<AImpGameMode>(UGameplayStatics::GetGameMode(WorldContextObject))) {
        return ImpGameMode->GetProjectileInfo();
    }

    return nullptr;
}

void UImpAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectInfo& DamageEffectInfo) {
    FGameplayEffectContextHandle ContextHandle = DamageEffectInfo.SourceASC->MakeEffectContext();
    ContextHandle.AddSourceObject(DamageEffectInfo.AvatarActor);

    const FGameplayEffectSpecHandle SpecHandle = DamageEffectInfo.SourceASC->MakeOutgoingSpec(DamageEffectInfo.DamageEffect, DamageEffectInfo.AbilityLevel, ContextHandle);

    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, ImpGameplayTags::Combat::Data_Damage, DamageEffectInfo.BaseDamage);

    if (IsValid(DamageEffectInfo.TargetASC)) {
        DamageEffectInfo.TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }

}
