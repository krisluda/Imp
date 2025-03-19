// Fill out your copyright notice in the Description page of Project Settings.

#include "Log.h"
#include "ImpInventoryComponent.h"
#include "ImpPlayerController.h"
#include "ImpPlayerState.h"
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

UImpInventoryComponent::UImpInventoryComponent() {
	PrimaryComponentTick.bCanEverTick = false;

}

void UImpInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UImpInventoryComponent, CachedInventory);
}

void UImpInventoryComponent::AddItem(const FGameplayTag &ItemTag, int32 NumItems) {
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

	IMP_DEBUGMSG(Red, "UImpInventoryComponent::AddItem: Server Item Added to %s's Inventory %s, qty: %d", *Cast<APlayerController>(GetOwner())->PlayerState->GetPlayerName(), *ItemTag.ToString(), NumItems);
	IMP_LOG("UImpInventoryComponent::AddItem: Server Item Added to %s's Inventory %s, qty: %d", *Cast<APlayerController>(GetOwner())->PlayerState->GetPlayerName(), *ItemTag.ToString(), NumItems);
	
	PackageInventory(CachedInventory);
}


void UImpInventoryComponent::ServerAddItem_Implementation(const FGameplayTag &ItemTag, int32 NumItems) {
	AddItem(ItemTag, NumItems);
}

void UImpInventoryComponent::PackageInventory(FPackagedInventory& OutInventory) {
	OutInventory.ItemTags.Empty();
	OutInventory.ItemQuantities.Empty();
	
	for (const auto& Pair : InventoryTagMap) {
		if (Pair.Value > 0) {
			OutInventory.ItemTags.Add(Pair.Key);
			OutInventory.ItemQuantities.Add(Pair.Value);
		}
	}
	
}

void UImpInventoryComponent::ReconstructInventoryMap(const FPackagedInventory &Inventory) {
	InventoryTagMap.Empty();
	
	for (int32 i = 0; i < Inventory.ItemTags.Num(); ++i) {
		InventoryTagMap.Emplace(Inventory.ItemTags[i], Inventory.ItemQuantities[i]);
		
		IMP_DEBUGMSG(Blue, "UImpInventoryComponent::ReconstructInventoryMap: In Player %s's Inventory, Tag Added: %s // Quantity Added: %d", 
			*Cast<APlayerController>(GetOwner())->PlayerState->GetPlayerName(), 
			*Inventory.ItemTags[i].ToString(), 
			Inventory.ItemQuantities[i]
		);

		IMP_LOG("UImpInventoryComponent::ReconstructInventoryMap: In Player %s's Inventory, Tag Added: %s // Quantity Added: %d", 
			*Cast<APlayerController>(GetOwner())->PlayerState->GetPlayerName(), 
			*Inventory.ItemTags[i].ToString(), 
			Inventory.ItemQuantities[i]
		);
	}
}

void UImpInventoryComponent::OnRep_CachedInventory() {
	ReconstructInventoryMap(CachedInventory);
}

