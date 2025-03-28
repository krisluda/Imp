// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "ItemTypes.h"
#include "InventoryComponent.generated.h"

class UItemTypesToTables;

USTRUCT()
struct FPackagedInventory {
	GENERATED_BODY()

	virtual ~FPackagedInventory() = default;

	UPROPERTY()
	TArray<FGameplayTag> ItemTags;

	UPROPERTY()
	TArray<int32> ItemQuantities;

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess);
};
template<>
struct TStructOpsTypeTraits<FPackagedInventory> : TStructOpsTypeTraitsBase2<FPackagedInventory> {
	enum { 
		WithNetSerializer = true
	};
};
//The above shenanigans is apparently a very specific method that tells the engine that the struct above
//has a NetSerialize-function or whatever. Uhr describes it as a worthwhile optimization, because the inventory replication
//ends up sending a lot of info over the net to stay updated.

/* This declaration must happen directly after the struct for FPackagedInventory*/
DECLARE_MULTICAST_DELEGATE_OneParam(FInventoryPackagedSignature, const FPackagedInventory& /*InventoryContents*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IMP_API UInventoryComponent : public UActorComponent {
	GENERATED_BODY()

public:	
	UInventoryComponent();
	
	FInventoryPackagedSignature InventoryPackagedDelegate;
	
	bool bOwnerLocallyControlled = false; //to prevent others from getting the inventory of others?
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void AddItem(const FGameplayTag& ItemTag, int32 NumItems = 1);
	
	UFUNCTION(BlueprintCallable)
	void UseItem(const FGameplayTag& ItemTag, int32 NumItems = 1);

	UFUNCTION(BlueprintPure)
	FMasterItemDefinition GetItemDefinitionByTag(const FGameplayTag& ItemTag) const;

	TMap<FGameplayTag, int32> GetInventoryTagMap();

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TMap<FGameplayTag, int32> InventoryTagMap;

	UPROPERTY(ReplicatedUsing=OnRep_CachedInventory)
	FPackagedInventory CachedInventory;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UItemTypesToTables> InventoryDefinitions;

	UFUNCTION(Server, Reliable)
	void ServerAddItem(const FGameplayTag& ItemTag, int32 NumItems);

	UFUNCTION(Server, Reliable)
	void ServerUseItem(const FGameplayTag& ItemTag, int32 NumItems);

	void PackageInventory(FPackagedInventory& OutInventory);
	void ReconstructInventoryMap(const FPackagedInventory& Inventory);

	UFUNCTION()
	void OnRep_CachedInventory();

		
};
