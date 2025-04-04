#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ImpAbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "CharacterBase.h"
#include "ImpCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UImpAbilitySystemComponent;
class UImpAttributeSet;

UCLASS()
class IMP_API AImpCharacter : public ACharacterBase, public IAbilitySystemInterface, public IImpAbilitySystemInterface {
    GENERATED_BODY()
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

public:
    AImpCharacter();

/*Implement ImpAbilitySystemInterface*/
    virtual USceneComponent* GetDynamicSpawnPoint_Implementation();
    
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

    //Just using a scene component for now; can be changed to socket(s) later
    UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
    TObjectPtr<USceneComponent> DynamicProjectileSpawnPoint;

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