#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ImpCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class IMP_API AImpCharacter : public ACharacter {
    GENERATED_BODY()

public:
    AImpCharacter();
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    virtual void BeginPlay() override;

public: 
    virtual void Tick(float DeltaTime) override;
    //virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; 

private: 
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

    
protected:
    UPROPERTY(Replicated)
    float Health;
    
public:
    float GetHealth() const { return Health; }
    
 public:
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
    
    UFUNCTION(Server, Reliable)
    void S_Move(const FVector2D& InputValue);
    
    UFUNCTION(Server, Reliable)
    void S_Look(const FVector2D& InputValue);
    
    UFUNCTION(Server, Reliable)
    void S_Jump();
    
    UFUNCTION(Server, Reliable)
    void S_StopJumping();
    
    friend class AImpPlayerController;
};