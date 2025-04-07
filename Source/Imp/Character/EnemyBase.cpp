// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "ImpCharacterClassInfo.h"
#include "ImpAttributeSet.h"
#include "ImpAbilitySystemLibrary.h"
#include "ImpAbilitySystemComponent.h"
#include "Log.h"
#include "Net/UnrealNetwork.h"

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

void AEnemyBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AEnemyBase, bInitAttributes);
}

void AEnemyBase::BeginPlay() {
    Super::BeginPlay();

    BindCallbacksToDependencies();
    InitAbilityActorInfo();
}

void AEnemyBase::InitAbilityActorInfo() {
    if (IsValid(ImpAbilitySystemComp) && IsValid(ImpAttributes)) {
        ImpAbilitySystemComp->InitAbilityActorInfo(this, this);

        if (HasAuthority()) {
            InitClassDefaults();
            BroadcastInitialValues();
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

void AEnemyBase::BindCallbacksToDependencies() {
    if (IsValid(ImpAbilitySystemComp) && IsValid(ImpAttributes)) {
        ImpAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(ImpAttributes->GetHealthAttribute()).AddLambda(
            [this] (const FOnAttributeChangeData& Data) {
                OnHealthChanged(Data.NewValue, ImpAttributes->GetMaxHealth());
            }
        );

        ImpAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(ImpAttributes->GetShieldAttribute()).AddLambda(
            [this] (const FOnAttributeChangeData& Data) {
                OnShieldChanged(Data.NewValue, ImpAttributes->GetMaxShield());
            }
        );

        if (HasAuthority()) {
            ImpAbilitySystemComp->OnAttributesGiven.AddLambda(
                [this] {
                    bInitAttributes = true;
                }
            );
        }
    }
}

void AEnemyBase::BroadcastInitialValues() {
    if (IsValid(ImpAttributes)) {
        OnHealthChanged(ImpAttributes->GetHealth(), ImpAttributes->GetMaxHealth());
        OnShieldChanged(ImpAttributes->GetShield(), ImpAttributes->GetMaxShield());
    }
}

void AEnemyBase::OnRep_InitAttributes() {
    BroadcastInitialValues();
}
