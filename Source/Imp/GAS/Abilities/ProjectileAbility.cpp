// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileAbility.h"
#include "ProjectileInfo.h"
#include "ProjectileBase.h"
#include "ImpAbilitySystemInterface.h"
#include "Log.h"
#include "ImpAbilitySystemLibrary.h"

UProjectileAbility::UProjectileAbility() {
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UProjectileAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) {
    Super::OnGiveAbility(ActorInfo, Spec);

    AvatarActorFromInfo = GetAvatarActorFromActorInfo();

    if (!ProjectileToSpawnTag.IsValid() || !IsValid(AvatarActorFromInfo)) return;

    if (UProjectileInfo* ProjectileInfo = UImpAbilitySystemLibrary::GetProjectileInfo(GetAvatarActorFromActorInfo())) {
        CurrentProjectileParams = *ProjectileInfo->ProjectileInfoMap.Find(ProjectileToSpawnTag);
    }
}

void UProjectileAbility::SpawnProjectile() {

    if (!IsValid(CurrentProjectileParams.ProjectileClass)) return;
    
    if (const USceneComponent* SpawnPointComp = IImpAbilitySystemInterface::Execute_GetDynamicSpawnPoint(AvatarActorFromInfo)) {
        const FVector SpawnPoint = SpawnPointComp->GetComponentLocation();
        const FVector TargetLocation = AvatarActorFromInfo->GetActorForwardVector() * 10000;
        const FRotator TargetRotation = (TargetLocation - SpawnPoint).Rotation();

        FTransform SpawnTransform;
        SpawnTransform.SetLocation(SpawnPoint);
        SpawnTransform.SetRotation(TargetRotation.Quaternion());
        
        if (AProjectileBase* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(CurrentProjectileParams.ProjectileClass, SpawnTransform)) {
            SpawnedProjectile->SetProjectileParams(CurrentProjectileParams);
            SpawnedProjectile->FinishSpawning(SpawnTransform);
        }
    }
}
