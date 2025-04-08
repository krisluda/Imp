// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpAbilityTypes.h"
#include "Log.h"

//We can't cast to a child struct, so this specific function lets us get this (i.e. in the Exec_Calc)
FImpGameplayEffectContext* FImpGameplayEffectContext::GetEffectContext(FGameplayEffectContextHandle Handle) {
    FGameplayEffectContext* EffectContext = Handle.Get();

    if (EffectContext && EffectContext->GetScriptStruct()->IsChildOf(StaticStruct())) {
        return static_cast<FImpGameplayEffectContext*>(EffectContext);
    }

    IMP_LOG("FImpGameplayEffectContext::GetEffectContext: Static Cast of ImpEffectContext failed.");
    return nullptr;
}

bool FImpGameplayEffectContext::NetSerialize(FArchive &Ar, UPackageMap *Map, bool &bOutSuccess)
{
    //return FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

    //When we need to rep more of these, make RepBits a uint16 or 32 or 64 and add more positions for variables
    uint8 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid())
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
        if (bCriticalHit)
        {
            RepBits |= 1 << 7;
        }
	}

    //Remember to increase the length of the bit here to as many as we use.
	Ar.SerializeBits(&RepBits, 8);

	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
    if (RepBits & (1 << 7))
    {
        Ar << bCriticalHit;
    }

	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}	
	
	bOutSuccess = true;
	return true;
}
