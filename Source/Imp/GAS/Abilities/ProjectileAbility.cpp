// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileAbility.h"
#include "ProjectileInfo.h"
#include "ProjectileBase.h"
#include "Log.h"
#include "ImpAbilitySystemLibrary.h"

UProjectileAbility::UProjectileAbility() {
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UProjectileAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) {
    Super::OnGiveAbility(ActorInfo, Spec);

    AvatarActorFromInfo = GetAvatarActorFromActorInfo();

    if (!ProjectileToSpawnTag.IsValid() || IsValid(AvatarActorFromInfo)) return;

    if (UProjectileInfo* ProjectileInfo = UImpAbilitySystemLibrary::GetProjectileInfo(AvatarActorFromInfo) {
        CurrentProjectileParams = *ProjectileInfo->ProjectileInfoMap.Find(ProjectileToSpawnTag);
    }
}

void UProjectileAbility::SpawnProjectile() {
    
    if (!IsValid(CurrentProjectileParams.ProjectileClass)) return;

    IMP_DEBUGMSG(Cyan, "UProjectileAbility::SpawnProjectile: Spawning projectile %s on server.", *CurrentProjectileParams.ProjectileClass->GetName());
}
