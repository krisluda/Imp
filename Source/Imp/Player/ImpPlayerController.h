#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerController.h"
#include "InventoryInterface.h"
#include "GameplayTagContainer.h"
#include "ImpPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UImpAbilitySystemComponent;
class UInventoryComponent;
class UInventoryWidgetController;
class UImpWidget;
class UImpInputConfig;

UCLASS ()
class IMP_API AImpPlayerController : public APlayerController, public IAbilitySystemInterface, public IInventoryInterface {
    GENERATED_BODY()
        
public:
    AImpPlayerController();
    
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual void BeginPlay() override;
    
    //This might be removed, not sure. Uhr gets rid of it at some point, but i believe it is asked for i BP. It actually needs an override to compile
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    UImpAbilitySystemComponent* GetImpAbilitySystemComponent();

    //This
    UPROPERTY()
    TObjectPtr<UImpAbilitySystemComponent> ImpAbilitySystemComp;


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

    UPROPERTY(EditDefaultsOnly, Category="Custom Values|Input")
    TObjectPtr<UImpInputConfig> ImpInputConfig;

    void AbilityInputPressed(FGameplayTag InputTag);
    void AbilityInputReleased(FGameplayTag InputTag);

//Old movement shit below, lets see if it stays
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
//Old movement shit above

/* INPUT SECTION END */


};