// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ItemTypesToTables.h"
#include "ImpAbilitySystemLibrary.h"
#include "ImpPlayerState.h"
#include "Log.h"
#include "Net/UnrealNetwork.h"



void FImpInventoryList::AddItem(const FGameplayTag &ItemTag, int32 NumItems) {
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt) {
		FImpInventoryEntry& Entry = *EntryIt;

		if (Entry.ItemTag.MatchesTagExact(ItemTag)) {
			Entry.Quantity += NumItems;
			MarkItemDirty(Entry);

			if (OwnerComponent->GetOwner()->HasAuthority()) {
				DirtyItemDelegate.Broadcast(Entry); 
			}
			
			return;
		}
	}
	
	FImpInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.ItemTag = ItemTag;
	NewEntry.Quantity = NumItems;
	
	MarkItemDirty(NewEntry); //Could or should this be above the broadcast both for new and non-new items?

	if (OwnerComponent->GetOwner()->HasAuthority()) {
		DirtyItemDelegate.Broadcast(NewEntry); 
	}	
}

void FImpInventoryList::RemoveItem(const FGameplayTag &ItemTag, int32 NumItems) {
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt) {
		FImpInventoryEntry& Entry = *EntryIt;
		
		if (Entry.ItemTag.MatchesTagExact(ItemTag)) {
			Entry.Quantity -= NumItems;
			
			MarkItemDirty(Entry);
			
			if (OwnerComponent->GetOwner()->HasAuthority()) {
				DirtyItemDelegate.Broadcast(Entry); 
			}
		}
	}
}

bool FImpInventoryList::HasEnough(const FGameplayTag &ItemTag, int32 NumItems) {
    for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt) {
		FImpInventoryEntry& Entry = *EntryIt;

		if (Entry.ItemTag.MatchesTagExact(ItemTag)) {
			if (Entry.Quantity >= NumItems) {
				return true;
			}
		}
	}

	return false;
}

void FImpInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize) {
	// Uhr doesn't know what this is reliably good for.
}

void FImpInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize) {
	for (const int32 Index : AddedIndices) {
		FImpInventoryEntry& Entry = Entries[Index];

		DirtyItemDelegate.Broadcast(Entry);
	}
}

void FImpInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize) {
	for (const int32 Index : ChangedIndices) {
		FImpInventoryEntry& Entry = Entries[Index];

		DirtyItemDelegate.Broadcast(Entry);
	}
}

UInventoryComponent::UInventoryComponent() :
	InventoryList(this) //This is called an initializer list. It initializes the InventoryList with this InventoryComponent before the constructor runs. Efficient and maybe necessary.
{


	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, InventoryList);
}

void UInventoryComponent::AddItem(const FGameplayTag &ItemTag, int32 NumItems) {
	AActor* Owner = GetOwner();
	if (!IsValid(Owner)) return;

	if (!Owner->HasAuthority()) {
		ServerAddItem(ItemTag, NumItems);
		return;
	}

	InventoryList.AddItem(ItemTag, NumItems);
}

void UInventoryComponent::ServerAddItem_Implementation(const FGameplayTag &ItemTag, int32 NumItems) {
	AddItem(ItemTag, NumItems);
}

void UInventoryComponent::UseItem(const FGameplayTag &ItemTag, int32 NumItems) {
	AActor* Owner = GetOwner();
	if (!IsValid(Owner)) return;

	if (!Owner->HasAuthority()) {
		ServerUseItem(ItemTag, NumItems);
		return;
	}

	
	if (InventoryList.HasEnough(ItemTag, NumItems)) {
		const FMasterItemDefinition Item = GetItemDefinitionByTag(ItemTag);

		if (UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner)) {
			if (IsValid(Item.ConsumableProps.ItemEffectClass)) {
				const FGameplayEffectContextHandle ContextHandle = OwnerASC->MakeEffectContext();
				const FGameplayEffectSpecHandle SpecHandle = OwnerASC->MakeOutgoingSpec(Item.ConsumableProps.ItemEffectClass, Item.ConsumableProps.ItemEffectLevel, ContextHandle);
				OwnerASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				
				InventoryList.RemoveItem(ItemTag);

				IMP_DEBUGMSG(FColor::Magenta, "UInventoryComponent::UseItem: Player %s Server Item Used: %s,", *Cast<APlayerController>(GetOwner())->PlayerState->GetPlayerName(), *Item.ItemTag.ToString());
				//IMP_LOG("UInventoryComponent::UseItem: Player %s Server Item Used: %s", *Cast<APlayerController>(GetOwner())->PlayerState->GetPlayerName(), *Item.ItemTag.ToString());
			}
		}
	}
}

void UInventoryComponent::ServerUseItem_Implementation(const FGameplayTag &ItemTag, int32 NumItems) {
	if (InventoryList.HasEnough(ItemTag, NumItems)) {
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

TArray<FImpInventoryEntry> UInventoryComponent::GetInventoryEntries() {
	return InventoryList.Entries;
}


