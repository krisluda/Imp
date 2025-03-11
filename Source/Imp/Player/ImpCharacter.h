#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ImpCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class IMP_API AImpCharacter : public ACharacter {
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
    
    UPROPERTY(Replicated)
    float Health;
    //Health is on character, but what is on player state?
    
    void Move(const FVector2D& InputValue);
    void Look(const FVector2D& InputValue);
    void Jump();
    void StopJumping();
    void DebugDestroyCharacter();
};