#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerController.h"
#include "InventoryInterface.h"
#include "ImpPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UInventoryComponent;
class UInventoryWidgetController;
class UImpWidget;

UCLASS ()
class IMP_API AImpPlayerController : public APlayerController, public IAbilitySystemInterface, public IInventoryInterface {
    GENERATED_BODY()
        
public:
    AImpPlayerController();
    
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual void BeginPlay() override;
    
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

/* INVENTORY SECTION */

    /* Implement Inventory Interface */
    virtual UInventoryComponent* GetInventoryComponent_Implementation() override;
    
    UInventoryWidgetController* GetInventoryWidgetController();
    
    UFUNCTION(BlueprintCallable)
    void CreateInventoryWidget();
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Replicated)
    TObjectPtr<UInventoryComponent> InventoryComponent;
    
    UPROPERTY()
    TObjectPtr<UInventoryWidgetController> InventoryWidgetController;
    
    UPROPERTY(EditDefaultsOnly, Category="Custom Values|Widgets")
    TSubclassOf<UInventoryWidgetController> InventoryWidgetControllerClass;
    
    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    TObjectPtr<UImpWidget> InventoryWidget;
    
    UPROPERTY(EditDefaultsOnly, Category="Custom Values|Widgets")
    TSubclassOf<UImpWidget> InventoryWidgetClass;

/* INVENTORY SECTION END */
    

/* INPUT SECTION */

    virtual void SetupInputComponent() override;
    
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void Jump();
    void StopJumping();
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* InputMapping;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* MoveAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* LookAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* JumpAction;

/* INPUT SECTION END */


};