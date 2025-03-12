// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpAbilitySystemComponent.h"

UImpAbilitySystemComponent::UImpAbilitySystemComponent() {
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicated(true);
}

void UImpAbilitySystemComponent::BeginPlay() {
    Super::BeginPlay();
}

