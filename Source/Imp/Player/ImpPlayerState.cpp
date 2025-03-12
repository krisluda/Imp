#include "ImpPlayerState.h"
#include "Log.h"
#include "ImpAbilitySystemComponent.h"

AImpPlayerState::AImpPlayerState() {
    PrimaryActorTick.bCanEverTick = true;

}

UImpAbilitySystemComponent* AImpPlayerState::GetAbilitySystemComponent() const {
    return ImpAbilitySystemComponent;
}

void AImpPlayerState::BeginPlay() {
    Super::BeginPlay();
    IMP_LOG("AImpPlayerState::BeginPlay")
}