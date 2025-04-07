// Fill out your copyright notice in the Description page of Project Settings.

#include "ExecCalc_Damage.h"
#include "ImpGameplayTags.h"
#include "ImpAttributeSet.h"

struct ImpDamageStatics {
    
    // Source Captures

    // Target Captures
    DECLARE_ATTRIBUTE_CAPTUREDEF(IncomingHealthDamage);
    DECLARE_ATTRIBUTE_CAPTUREDEF(IncomingShieldDamage);
    DECLARE_ATTRIBUTE_CAPTUREDEF(DamageReduction);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Shield);

    ImpDamageStatics() {
        // Source Defines

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

    // Target Captures
    RelevantAttributesToCapture.Add(DamageStatics().IncomingHealthDamageDef);
    RelevantAttributesToCapture.Add(DamageStatics().IncomingShieldDamageDef);
    RelevantAttributesToCapture.Add(DamageStatics().DamageReductionDef);
    RelevantAttributesToCapture.Add(DamageStatics().ShieldDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const {
    const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

    const FGameplayTagContainer* SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvalParams;
    EvalParams.TargetTags = TargetTags;
    EvalParams.SourceTags = SourceTags;

    // Get raw damage value
    float Damage = EffectSpec.GetSetByCallerMagnitude(ImpGameplayTags::Combat::Data_Damage);
    Damage = FMath::Max<float>(Damage, 0.f);

    //Target captures
    float Shield = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ShieldDef, EvalParams, Shield);
    Shield = FMath::Max<float>(Shield, 0.f);
    
    float DamageReduction = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageReductionDef, EvalParams, DamageReduction);
    DamageReduction = FMath::Max<float>(DamageReduction, 0.f);

    float OutShield = 0.f;

    if (Damage > 0.f && Shield > 0.f) {
        //Doing the following, means damage reduction only applies IF we have a shield amount. This has to change later to separate their roles.
        Damage *= (100 - DamageReduction) / 100;
        OutShield = Shield - Damage;

        OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().IncomingShieldDamageProperty, EGameplayModOp::Additive, Damage));
    }

    if (OutShield <= 0.f) {
        const float RemainderDamage = fabs(Shield - Damage);
        OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().IncomingHealthDamageProperty, EGameplayModOp::Additive, RemainderDamage));
    }
}