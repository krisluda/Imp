// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "ItemTypes.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "InventoryComponent.generated.h"

class UItemTypesToTables;

USTRUCT(BlueprintType)
struct FImpInventoryEntry : public FFastArraySerializerItem {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag ItemTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	int32 Quantity = 0;
};

USTRUCT()
struct FImpInventoryList : public FFastArraySerializer {
	GENERATED_BODY()

	FImpInventoryList() : 
	OwnerComponent(nullptr) 
	{}

	FImpInventoryList(UActorComponent* InComponent) :
	OwnerComponent(InComponent)
	{}

	void AddItem(const FGameplayTag& ItemTag, int32 NumItems = 1);
	void RemoveItem(const FGameplayTag& ItemTag, int32 NumItems = 1);
	bool HasEnough(const FGameplayTag& ItemTag, int32 NumItems = 1);

	// FFastArraySerializer Contract "Three... Two of them are very useful, one I don't know bc can't ever get it to work"
	// Apparently we dont need to do anything else for the FastArraySerializer, we just need to add this specific functionality.
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms) {
		return FastArrayDeltaSerialize<FImpInventoryEntry, FImpInventoryList>(Entries, DeltaParms, *this);
	}


private:

	friend class UInventoryComponent;

	UPROPERTY()
	TArray<FImpInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FImpInventoryList> : TStructOpsTypeTraitsBase2<FImpInventoryList> {
	enum {
		WithNetDeltaSerializer = true
	};
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IMP_API UInventoryComponent : public UActorComponent {
	GENERATED_BODY()

public:	

	UPROPERTY(Replicated)
	FImpInventoryList InventoryList;

	UInventoryComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void AddItem(const FGameplayTag& ItemTag, int32 NumItems = 1);
	
	UFUNCTION(BlueprintCallable)
	void UseItem(const FGameplayTag& ItemTag, int32 NumItems = 1);

	UFUNCTION(BlueprintPure)
	FMasterItemDefinition GetItemDefinitionByTag(const FGameplayTag& ItemTag) const;

//still wondering why private
private:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UItemTypesToTables> InventoryDefinitions;

	UFUNCTION(Server, Reliable)
	void ServerAddItem(const FGameplayTag& ItemTag, int32 NumItems);

	UFUNCTION(Server, Reliable)
	void ServerUseItem(const FGameplayTag& ItemTag, int32 NumItems);	
	
};
