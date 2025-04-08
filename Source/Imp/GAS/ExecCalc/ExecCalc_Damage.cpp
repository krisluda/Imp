// Fill out your copyright notice in the Description page of Project Settings.

#include "ExecCalc_Damage.h"
#include "ImpGameplayTags.h"
#include "ImpAbilityTypes.h"
#include "ImpAttributeSet.h"

struct ImpDamageStatics {
    
    // Source Captures
    DECLARE_ATTRIBUTE_CAPTUREDEF(CritChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CritDamage);

    // Target Captures
    DECLARE_ATTRIBUTE_CAPTUREDEF(IncomingHealthDamage);
    DECLARE_ATTRIBUTE_CAPTUREDEF(IncomingShieldDamage);
    DECLARE_ATTRIBUTE_CAPTUREDEF(DamageReduction);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Shield);

    ImpDamageStatics() {
        // Source Defines
        DEFINE_ATTRIBUTE_CAPTUREDEF(UImpAttributeSet, CritChance, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UImpAttributeSet, CritDamage, Source, false);

        // Target Defines
        DEFINE_ATTRIBUTE_CAPTUREDEF(UImpAttributeSet, IncomingHealthDamage, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UImpAttributeSet, IncomingShieldDamage, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UImpAttributeSet, DamageReduction, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UImpAttributeSet, Shield, Target, false);
    }
};

static const ImpDamageStatics& DamageStatics() {
    static ImpDamageStatics DStatics;
    return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage() {
    // Source Captures
    RelevantAttributesToCapture.Add(DamageStatics().CritChanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().CritDamageDef);

    // Target Captures
    RelevantAttributesToCapture.Add(DamageStatics().IncomingHealthDamageDef);
    RelevantAttributesToCapture.Add(DamageStatics().IncomingShieldDamageDef);
    RelevantAttributesToCapture.Add(DamageStatics().DamageReductionDef);
    RelevantAttributesToCapture.Add(DamageStatics().ShieldDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const {
    const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

    FAggregatorEvaluateParameters EvalParams;
    EvalParams.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();
    EvalParams.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();

    // Cast the EffectContext to an ImpGameplayEffectContext using the static cast function (from Lyra).
    const FGameplayEffectContextHandle EffectContextHandle = EffectSpec.GetContext();
    FImpGameplayEffectContext* ImpContext = FImpGameplayEffectContext::GetEffectContext(EffectContextHandle);


    // Get raw damage value
    float Damage = EffectSpec.GetSetByCallerMagnitude(ImpGameplayTags::Combat::Data_Damage);
    Damage = FMath::Max<float>(Damage, 0.f);

    //Source captures
    float CritChance = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritChanceDef, EvalParams, CritChance);
    CritChance = FMath::Max<float>(CritChance, 0.f);

    float CritDamage = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritDamageDef, EvalParams, CritDamage);
    CritDamage = FMath::Max<float>(CritDamage, 0.f);

    //Target captures
    float Shield = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ShieldDef, EvalParams, Shield);
    Shield = FMath::Max<float>(Shield, 0.f);
    
    float DamageReduction = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageReductionDef, EvalParams, DamageReduction);
    DamageReduction = FMath::Max<float>(DamageReduction, 0.f);

    //Begin calculation

    const bool bCriticalHit = FMath::RandRange(0, 100) < CritChance;
    Damage = bCriticalHit ? Damage += (CritDamage * 0.5f) : Damage; //Rewrite this. It now gives a damage bonus of half of our critdamage. Why?
    ImpContext->SetIsCriticalHit(bCriticalHit);

    float OutShield = 0.f;

    if (Damage > 0.f && Shield > 0.f) {
        //Doing the following means damage reduction only applies IF we have a shield amount. This has to change later to separate their roles.
        Damage *= (100 - DamageReduction) / 100;
        OutShield = Shield - Damage;

        OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().IncomingShieldDamageProperty, EGameplayModOp::Additive, Damage));
    }

    if (OutShield <= 0.f) {
        const float RemainderDamage = fabs(Shield - Damage);
        OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().IncomingHealthDamageProperty, EGameplayModOp::Additive, RemainderDamage));
    }
}