// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "ImpAbilitySystemComponent.h"
#include "ImpAbilitySystemLibrary.h"
#include "ItemTypesToTables.h"
#include "ImpPlayerController.h"
#include "ImpPlayerState.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Log.h"
#include "Net/UnrealNetwork.h"

bool FPackagedInventory::NetSerialize(FArchive &Ar, UPackageMap *Map, bool &bOutSuccess) {
	//You have to define and hard code the MAXIMUM number of different _kinds_ of items (not quantities per type).
	//Furtermore, the ItemTags are serializes "_WithNetSerialize" because something with GameplayTags having their own serialization that can be subserialized/further triggered with this.
	SafeNetSerializeTArray_WithNetSerialize<100>(Ar, ItemTags, Map);
	//I do believe the number in the ItemQuantities then is amount of items per type.
	SafeNetSerializeTArray_Default<100>(Ar, ItemQuantities);

	bOutSuccess = true;
	return true;
}

UInventoryComponent::UInventoryComponent() {
	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UInventoryComponent, CachedInventory);
}

void UInventoryComponent::AddItem(const FGameplayTag &ItemTag, int32 NumItems) {
	AActor* Owner = GetOwner();
	if (!IsValid(Owner)) return;

	if (!Owner->HasAuthority()) {
		ServerAddItem(ItemTag, NumItems);
		return;
	}

	if (InventoryTagMap.Contains(ItemTag)) {
		InventoryTagMap[ItemTag] += NumItems;
	} else {
		InventoryTagMap.Emplace(ItemTag, NumItems);
	}

	//IMP_DEBUGMSG(FColor::Red, "UInventoryComponent::AddItem: Server Item Added to %s's Inventory %s, qty: %d", *Cast<APlayerController>(GetOwner())->PlayerState->GetPlayerName(), *ItemTag.ToString(), NumItems);
	//IMP_LOG("UInventoryComponent::AddItem: Server Item Added to %s's Inventory %s, qty: %d", *Cast<APlayerController>(GetOwner())->PlayerState->GetPlayerName(), *ItemTag.ToString(), NumItems);
	
	PackageInventory(CachedInventory);
	InventoryPackagedDelegate.Broadcast(CachedInventory);
}

void UInventoryComponent::ServerAddItem_Implementation(const FGameplayTag &ItemTag, int32 NumItems) {
	AddItem(ItemTag, NumItems);
}

void UInventoryComponent::PackageInventory(FPackagedInventory& OutInventory) {
	OutInventory.ItemTags.Empty();
	OutInventory.ItemQuantities.Empty();
	
	for (const auto& Pair : InventoryTagMap) {
		if (Pair.Value > 0) {
			OutInventory.ItemTags.Add(Pair.Key);
			OutInventory.ItemQuantities.Add(Pair.Value);
		}
	}
	
}

void UInventoryComponent::ReconstructInventoryMap(const FPackagedInventory &Inventory) {
	InventoryTagMap.Empty();
	
	for (int32 i = 0; i < Inventory.ItemTags.Num(); ++i) {
		InventoryTagMap.Emplace(Inventory.ItemTags[i], Inventory.ItemQuantities[i]);
		
		/*
		IMP_DEBUGMSG(FColor::Blue, "UInventoryComponent::ReconstructInventoryMap: In Player %s's Inventory, Tag Added: %s // Quantity Added: %d", 
			*Cast<APlayerController>(GetOwner())->PlayerState->GetPlayerName(), 
			*Inventory.ItemTags[i].ToString(), 
			Inventory.ItemQuantities[i]
		);

		IMP_LOG("UInventoryComponent::ReconstructInventoryMap: In Player %s's Inventory, Tag Added: %s // Quantity Added: %d", 
			*Cast<APlayerController>(GetOwner())->PlayerState->GetPlayerName(), 
			*Inventory.ItemTags[i].ToString(), 
			Inventory.ItemQuantities[i]
		);
		*/
	}
}

void UInventoryComponent::OnRep_CachedInventory() {
	if (bOwnerLocallyControlled) {
		ReconstructInventoryMap(CachedInventory);
		InventoryPackagedDelegate.Broadcast(CachedInventory);
	}
}

void UInventoryComponent::UseItem(const FGameplayTag &ItemTag, int32 NumItems) {
	AActor* Owner = GetOwner();
	if (!IsValid(Owner)) return;

	if (!Owner->HasAuthority()) {
		ServerUseItem(ItemTag, NumItems);
		return;
	}

	const FMasterItemDefinition Item = GetItemDefinitionByTag(ItemTag);

	if (UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner)) {
		if (IsValid(Item.ConsumableProps.ItemEffectClass)) {
			const FGameplayEffectContextHandle ContextHandle = OwnerASC->MakeEffectContext();
			const FGameplayEffectSpecHandle SpecHandle = OwnerASC->MakeOutgoingSpec(Item.ConsumableProps.ItemEffectClass, Item.ConsumableProps.ItemEffectLevel, ContextHandle);
			OwnerASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}

		AddItem(ItemTag, -1);

		//IMP_DEBUGMSG(FColor::Magenta, "UInventoryComponent::UseItem: Player %s Server Item Used: %s,", *Cast<APlayerController>(GetOwner())->PlayerState->GetPlayerName(), *Item.ItemTag.ToString());
		//IMP_LOG("UInventoryComponent::UseItem: Player %s Server Item Used: %s", *Cast<APlayerController>(GetOwner())->PlayerState->GetPlayerName(), *Item.ItemTag.ToString());
	}
}

void UInventoryComponent::ServerUseItem_Implementation(const FGameplayTag &ItemTag, int32 NumItems) {
	if (InventoryTagMap.Contains(ItemTag)) {
		UseItem(ItemTag, NumItems);
	}
}

FMasterItemDefinition UInventoryComponent::GetItemDefinitionByTag(const FGameplayTag &ItemTag) const {
    checkf(InventoryDefinitions, TEXT("UInventoryComponent:GetItemDefinitionByTag: No Definitions Inside Component %s"), *GetNameSafe(this));

	for (const auto& Pair : InventoryDefinitions->TagsToTables) {
		if (ItemTag.MatchesTag(Pair.Key)) {
			return *UImpAbilitySystemLibrary::GetDataTableRowByTag<FMasterItemDefinition>(Pair.Value, ItemTag);
		}
	}

	return FMasterItemDefinition();
}

TMap<FGameplayTag,int32> UInventoryComponent::GetInventoryTagMap() {
	return InventoryTagMap;
}

