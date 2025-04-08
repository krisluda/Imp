
# Exec_Calc

## CritDamage calculation:
- Rewrite the math, it chooses between half the CritDamage or regular damage.

## DamageReduction calculation:
- Rewrite functionality. It now only applies if we have shield, but I'm thinking more separating the two.

# ImpAbilityTypes

## FImpGameplayEffectContext
Holds a child struct of FGameplayEffectContext
Add transient data to follow a hit around

### FImpGameplayEffectContext::NetSerialize
Mirrors the NetSerialize of the parent struct using bitwise operators. It is a uint8, but we can make it a uint16/32/64 to make room for more variables.
NB! Remember to increase the length of the bit here to as many as we use:
	Ar.SerializeBits(&RepBits, int NumberOfUsedBits);

## FProjectileParams
A struct containing the parameters of our dynamic projectiles (which will probably be ALL combat abilites later)

## FDamageEffectInfo
A struct containing all the necessary info for a damage effect to work when and as intended (can be expanded).

# ImpAbilitySystemGlobals
Subclasses and overrides FGameplayEffectContext* AllocGameplayEffectContext() to return new FImpGameplayEffectContext(). 
Coupled with setting the AbilitySystemGlobalsClassName="/Script/Imp.ImpAbilitySystemGlobals" in DefaultGame.ini, this makes our globals-class the globals class, and then allocates our ImpGameplayEffectContext as the GameplayEffectContext.