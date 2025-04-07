// Fill out your copyright notice in the Description page of Project Settings.

#include "ExecCalc_Damage.h"
#include "ImpGameplayTags.h"
#include "ImpAttributeSet.h"

struct ImpDamageStatics {
    
    // Source Captures

    // Target Captures
    DECLARE_ATTRIBUTE_CAPTUREDEF(IncomingHealthDamage);

    ImpDamageStatics() {
        // Source Defines

        // Target Defines
        DEFINE_ATTRIBUTE_CAPTUREDEF(UImpAttributeSet, IncomingHealthDamage, Target, false);
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
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const {
    const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

    // Get raw damage value
    float Damage = EffectSpec.GetSetByCallerMagnitude(ImpGameplayTags::Combat::Data_Damage);
    Damage = FMath::Max<float>(Damage, 0.f);

    if (Damage > 0.f) {
        OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().IncomingHealthDamageProperty, EGameplayModOp::Additive, Damage));
    }


}