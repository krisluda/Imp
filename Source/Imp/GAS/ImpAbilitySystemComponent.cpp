// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpAbilitySystemComponent.h"
#include "ProjectileAbility.h"
#include "Log.h"
#include "ImpGameplayAbility.h"

void UImpAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>> &AbilitiesToGrant) {
    for (const TSubclassOf<UGameplayAbility>& Ability : AbilitiesToGrant) {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1.f);

        if (UImpGameplayAbility* ImpAbility = Cast<UImpGameplayAbility>(AbilitySpec.Ability)) {
            AbilitySpec.DynamicAbilityTags.AddTag(ImpAbility->InputTag);
            GiveAbility(AbilitySpec);
        }
    }
    
}

void UImpAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>> &PassivesToGrant) {
    for (const TSubclassOf<UGameplayAbility>& Ability : PassivesToGrant) {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1.f);
        GiveAbilityAndActivateOnce(AbilitySpec);
    }
}

void UImpAbilitySystemComponent::InitializeDefaultAttributes(const TSubclassOf<UGameplayEffect>& AttributeEffect) {
    checkf(AttributeEffect, TEXT("No valid default attributes for this character %s"), *GetAvatarActor()->GetName());

    const FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
    const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(AttributeEffect, 1.f, ContextHandle);
    ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    OnAttributesGiven.Broadcast();

}

void UImpAbilitySystemComponent::AbilityInputPressed(const FGameplayTag &InputTag) {
    if (!InputTag.IsValid()) return;

    ABILITYLIST_SCOPE_LOCK();

    /*
    'FGameplayAbilitySpec::DynamicAbilityTags': Use GetDynamicSpecSourceTags() which better represents what this variable does.
    
    'FGameplayAbilitySpec::ActivationInfo': ActivationInfo on the Spec only applies to NonInstanced abilities 
    (which are now deprecated; instanced abilities have their own per-instance CurrentActivationInfo)
    */

    for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities()) {
        if (Spec.DynamicAbilityTags.HasTagExact(InputTag)) {
            if (!Spec.IsActive()) {
                TryActivateAbility(Spec.Handle);
            } else {
                InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
            }
        }
    }
}

void UImpAbilitySystemComponent::AbilityInputReleased(const FGameplayTag & InputTag) {
    if (!InputTag.IsValid()) return;

    ABILITYLIST_SCOPE_LOCK();

     /*
    'FGameplayAbilitySpec::DynamicAbilityTags': Use GetDynamicSpecSourceTags() which better represents what this variable does.
    
    'FGameplayAbilitySpec::ActivationInfo': ActivationInfo on the Spec only applies to NonInstanced abilities 
    (which are now deprecated; instanced abilities have their own per-instance CurrentActivationInfo)
    */

    for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities()) {
        if (Spec.DynamicAbilityTags.HasTagExact(InputTag)) {
            InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
        }
    }
}

void UImpAbilitySystemComponent::SetDynamicProjectile(const FGameplayTag& ProjectileTag, int32 AbilityLevel) {
    if(!ProjectileTag.IsValid()) {
        IMP_LOG("UImpAbilitySystemComponent::SetDynamicProjectile: No valid projectile tag.")
        return;
    }

    if (!GetAvatarActor()->HasAuthority()) {
        ServerSetDynamicProjectile(ProjectileTag, AbilityLevel);
        return;
    }

    if (ActiveProjectileAbility.IsValid()) {
        ClearAbility(ActiveProjectileAbility);
    }

    if (IsValid(DynamicProjectileAbility)) {
        FGameplayAbilitySpec Spec = FGameplayAbilitySpec(DynamicProjectileAbility, AbilityLevel);
        if (UProjectileAbility* ProjectileAbility = Cast<UProjectileAbility>(Spec.Ability)) {
            ProjectileAbility->ProjectileToSpawnTag = ProjectileTag;
            Spec.DynamicAbilityTags.AddTag(ProjectileAbility->InputTag);

            ActiveProjectileAbility = GiveAbility(Spec);
        }
    }


}

void UImpAbilitySystemComponent::ServerSetDynamicProjectile_Implementation(const FGameplayTag& ProjectileTag, int32 AbilityLevel) {
    SetDynamicProjectile(ProjectileTag, AbilityLevel);
}
