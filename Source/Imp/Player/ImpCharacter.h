#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "ImpCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UImpAbilitySystemComponent;
class UImpAttributeSet;

UCLASS()
class IMP_API AImpCharacter : public ACharacter {
    GENERATED_BODY()
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

public:
    AImpCharacter();
    
    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    //**Uhr says these will be refactored later. I wonder if they should be on PlayerState at some point**//
    UFUNCTION(BlueprintImplementableEvent)
    void OnHealthChanged(float CurrentHealth, float MaxHealth);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnManaChanged(float CurrentMana, float MaxMana);
    //** End **//

    UPROPERTY(BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UImpAbilitySystemComponent> ImpAbilitySystemComponent;
    
    UPROPERTY(BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UImpAttributeSet> ImpAttributeSet;

    UPROPERTY(EditAnywhere, Category = "Custom Values|Character Info")
    FGameplayTag CharacterTag;
    
    void InitAbilityActorInfo();
    void InitClassDefaults();
    void BindCallbacksToDependencies();

    UFUNCTION(BlueprintCallable)
    void BroadcastInitialValues();
    
    void Move(const FVector2D& InputValue);
    void Look(const FVector2D& InputValue);
    void Jump();
    void StopJumping();
    void DebugDestroyCharacter();
};