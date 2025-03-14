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
    

    UPROPERTY(BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UImpAbilitySystemComponent> ImpAbilitySystemComponent;
    
    UPROPERTY(BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UImpAttributeSet> ImpAttributeSet;

    UPROPERTY(EditAnywhere, Category = "Custom Values|Character Info")
    FGameplayTag CharacterTag;
    
    void InitAbilityActorInfo();
    void InitClassDefaults();

    
    void Move(const FVector2D& InputValue);
    void Look(const FVector2D& InputValue);
    void Jump();
    void StopJumping();
    void DebugDestroyCharacter();
};