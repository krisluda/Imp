// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetController.h"
#include "InventoryComponent.h"
#include "InventoryInterface.h"

void UInventoryWidgetController::SetOwningActor(AActor * InOwner) {
    OwningActor = InOwner;
}

void UInventoryWidgetController::BindCallbacksToDependencies() {
    OwningInventory = IInventoryInterface::Execute_GetInventoryComponent(OwningActor);

    if (IsValid(OwningInventory)) {
        OwningInventory->InventoryList.DirtyItemDelegate.AddLambda( 
            [this] (const FImpInventoryEntry& DirtyItem) {
                FMasterItemDefinition Item = OwningInventory->GetItemDefinitionByTag(DirtyItem.ItemTag);
                Item.ItemQuantity = DirtyItem.Quantity;

                InventoryItemDelegate.Broadcast(Item);
            }
        );
    }
}

void UInventoryWidgetController::BroadcastInitialValues() {
    if (IsValid(OwningInventory)) {
        for (const FImpInventoryEntry& Entry : OwningInventory->GetInventoryEntries()) {
            FMasterItemDefinition Item = OwningInventory->GetItemDefinitionByTag(Entry.ItemTag);
            Item.ItemQuantity = Entry.Quantity;
            
            InventoryItemDelegate.Broadcast(Item);
        }
    }
}
