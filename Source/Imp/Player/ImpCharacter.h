#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ImpCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UImpAbilitySystemComponent;

UCLASS()
class IMP_API AImpCharacter : public ACharacter, public IAbilitySystemInterface {
    GENERATED_BODY()

public:
    AImpCharacter();
    
    
    
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    //virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; 
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
    TObjectPtr<UImpAbilitySystemComponent> ImpAbilitySystemComponent;
    
    //IAbilitySystemInterface
    virtual UImpAbilitySystemComponent* GetAbilitySystemComponent() const override;
    
    void Move(const FVector2D& InputValue);
    void Look(const FVector2D& InputValue);
    void Jump();
    void StopJumping();
    void DebugDestroyCharacter();
};