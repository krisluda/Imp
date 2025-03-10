#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ImpGameMode.generated.h"

UCLASS(minimalapi)
class AImpGameMode : public AGameModeBase {
    GENERATED_BODY()

public:
    AImpGameMode();

    virtual void PostLogin(APlayerController* NewPlayer) override;
};