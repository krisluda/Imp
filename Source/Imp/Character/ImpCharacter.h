#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "CharacterBase.h"
#include "ImpCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UImpAbilitySystemComponent;
class UImpAttributeSet;

UCLASS()
class IMP_API AImpCharacter : public ACharacterBase, public IAbilitySystemInterface {
    GENERATED_BODY()
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

public:
    AImpCharacter();
    
    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
/* From CharacterBase */
    virtual void InitAbilityActorInfo() override;
    virtual void InitClassDefaults() override;
    virtual void BindCallbacksToDependencies() override;
    virtual void BroadcastInitialValues() override;
/* End From CharacterBase */

    UPROPERTY(BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UImpAbilitySystemComponent> ImpAbilitySystemComp;
    
    UPROPERTY(BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UImpAttributeSet> ImpAttributeSet;

    
    void Move(const FVector2D& InputValue);
    void Look(const FVector2D& InputValue);
    void Jump();
    void StopJumping();
    void DebugDestroyCharacter();
};