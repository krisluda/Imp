// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "ImpCharacterClassInfo.h"
#include "ImpAttributeSet.h"
#include "ImpAbilitySystemLibrary.h"
#include "Log.h"
#include "ImpAbilitySystemComponent.h"

AEnemyBase::AEnemyBase() {
    bReplicates = true;

    ImpAbilitySystemComp = CreateDefaultSubobject<UImpAbilitySystemComponent>("AbilitySystemComponent");
    ImpAbilitySystemComp->SetIsReplicated(true);
    ImpAbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    ImpAttributes = CreateDefaultSubobject<UImpAttributeSet>("AttributeSet");

}

UAbilitySystemComponent* AEnemyBase::GetAbilitySystemComponent() const {
    return ImpAbilitySystemComp;
}

void AEnemyBase::BeginPlay() {
    Super::BeginPlay();

    InitAbilityActorInfo();
}

void AEnemyBase::InitAbilityActorInfo() {
    if (IsValid(ImpAbilitySystemComp) && IsValid(ImpAttributes)) {
        ImpAbilitySystemComp->InitAbilityActorInfo(this, this);

        if (HasAuthority()) {
            InitClassDefaults();
        }
    }
}

void AEnemyBase::InitClassDefaults() {
    if (!CharacterTag.IsValid()) {
        IMP_LOG("AEnemyBase::InitClassDefaults: No character tag selected in character %s", *GetNameSafe(this));
    } else if (UImpCharacterClassInfo* ClassInfo = UImpAbilitySystemLibrary::GetImpCharacterClassDefaultInfo(this)) {
        if (const FImpCharacterClassDefaultInfo* SelectedClass = ClassInfo->ClassDefaultInfoMap.Find(CharacterTag)) {
            if (IsValid(ImpAbilitySystemComp)) {
                ImpAbilitySystemComp->AddCharacterAbilities(SelectedClass->StartingAbilities);
                ImpAbilitySystemComp->AddCharacterPassiveAbilities(SelectedClass->StartingPassives);
                ImpAbilitySystemComp->InitializeDefaultAttributes(SelectedClass->DefaultAttributes);
            }
        }
    }
}
