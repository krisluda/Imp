#pragma once

#include "CoreMinimal.h
#include "GameFramework/PlayerController.h"
#include "ImpPlayerController.generated.h"

UCLASS ()
class IMP_API AImpPlayerController : public APlayerController {
    GENERATED_BODY()

public:
    AImpPlayerController();

protected:
    virtual void BeginPlay() override;

    virtual void SetupInputComponent() override;
    
    void MoveForward(float Value);
    void MoveRight(float Value);

};