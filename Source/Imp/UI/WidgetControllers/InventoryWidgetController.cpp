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
 
    }
}

void UInventoryWidgetController::BroadcastInitialValues() {
    if (IsValid(OwningInventory)) {

    }
}
