#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerController.h"
#include "ImpAbilitySystemInterface.h"
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
class IMP_API AImpPlayerController : public APlayerController, public IAbilitySystemInterface, public IInventoryInterface, public IImpAbilitySystemInterface {
    GENERATED_BODY()
        
public:
    
    AImpPlayerController();

    virtual void SetupInputComponent() override;
    
    virtual void BeginPlay() override;

    /* Implement InventoryInterface */
    virtual UInventoryComponent* GetInventoryComponent_Implementation() override;

    /* Implement ImpAbilitySystemInterface */
    virtual void SetDynamicProjectile_Implementation(const FGameplayTag& ProjectileTag, int32 AbilityLevel) override;
    
    //This seems to be mandatory, but might be removed, not sure. Uhr seemd to get rid of it at some point, but later it is actual in the top public section. It actually needs an override to compile.
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    //And this is the specific one that is used by Uhr, in the Ability Input-stuff.
    UImpAbilitySystemComponent* GetImpAbilitySystemComponent();
    
    UInventoryWidgetController* GetInventoryWidgetController();

    UFUNCTION(BlueprintCallable)
    void CreateInventoryWidget();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;



protected:

    void AbilityInputPressed(FGameplayTag InputTag);
    void AbilityInputReleased(FGameplayTag InputTag);

private:

    UPROPERTY()
    TObjectPtr<UImpAbilitySystemComponent> ImpAbilitySystemComp;

    UPROPERTY(EditDefaultsOnly, Category="Custom Values|Input")
    TObjectPtr<UImpInputConfig> ImpInputConfig;

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

//This was public but i think it can be private. Also we might just fuck private.
public:

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
};