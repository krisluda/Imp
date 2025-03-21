#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerController.h"
#include "Logging/LogMacros.h"
#include "ImpPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UImpInventoryComponent;

UCLASS ()
class IMP_API AImpPlayerController : public APlayerController, public IAbilitySystemInterface {
    GENERATED_BODY()
        
public:
    AImpPlayerController();

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    virtual void BeginPlay() override;
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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Replicated)
    TObjectPtr<UImpInventoryComponent> ImpInventoryComponent;
    
};