#include "ImpPlayerState.h"
#include "ImpAbilitySystemComponent.h"
#include "ImpAttributeSet.h"
#include "Log.h"

AImpPlayerState::AImpPlayerState() {
    PrimaryActorTick.bCanEverTick = true;

    //If quick attribute changes in multiplayer is required
    SetNetUpdateFrequency(100.f);
    SetMinNetUpdateFrequency(66.f);

    ImpAbilitySystemComp = CreateDefaultSubobject<UImpAbilitySystemComponent>("AbilitySystemComponent");
    ImpAbilitySystemComp->SetIsReplicated(true);
    ImpAbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    ImpAttributeSet = CreateDefaultSubobject<UImpAttributeSet>("AttributeSet");
    
}

UAbilitySystemComponent* AImpPlayerState::GetAbilitySystemComponent() const {
    return ImpAbilitySystemComp;
}

UImpAbilitySystemComponent *AImpPlayerState::GetImpAbilitySystemComponent() const {
    return ImpAbilitySystemComp;
}

UImpAttributeSet *AImpPlayerState::GetImpAttributeSet() const {
    return ImpAttributeSet;
}

void AImpPlayerState::BeginPlay() {
    Super::BeginPlay();
    IMP_LOG("AImpPlayerState::BeginPlay")
}