
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

# InventoryComponent

## FInventoryEntry
A FastArraySerializerItem struct for each inventory item

Properties:
- Tags
- Quantity

## FInventoryList
A FastArraySerializer struct for the whole inventory. Holds an array of items and an owner (which is the InventoryComponent that creates it, and all this is done in the inventory component.)

Functions:
- Add
- Remove
- Has Enough (to use)

FAS-boilerplate:
- PreRepRemove (Uhr doesnt understand how it works)
- PostRepAdd
- PostRepChange

NetDeltaSerialize:
- Serializes, but just the change from t1 to t2.
- Uses the same template-shenanigans for netserialization, but we dont need any overrides or adds.

## InventoryComponent itself
Has master functions for adding (removing -1), using, getting item by tag, getting all the entries (in the array in the FAS-struct), and server-versions of add and use.

# 