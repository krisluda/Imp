#include "ImpPlayerController.h"
#include "ImpCharacter.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY(LogTemplateController);

AImpPlayerController::AImpPlayerController() {
    InputMapping = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Imp/Core/Player/Input/IMC_Imp.IMC_Imp"));
    MoveAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Imp/Core/Player/Input/IA_Move.IA_Move"));
    LookAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Imp/Core/Player/Input/IA_Look.IA_Look"));
    JumpAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Imp/Core/Player/Input/IA_Jump.IA_Jump"));
}

void AImpPlayerController::BeginPlay() {
    Super::BeginPlay();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
        Subsystem->AddMappingContext(InputMapping, 0);
    }
}

void AImpPlayerController::SetupInputComponent() {
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AImpPlayerController::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AImpPlayerController::Look);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AImpPlayerController::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AImpPlayerController::StopJumping);
    }
}

void AImpPlayerController::Move(const FInputActionValue& Value) {
    UE_LOG(LogTemplateController, Log, TEXT("Move function called on controller!"));
    
    AImpCharacter* ImpCharacter = Cast<AImpCharacter>(GetPawn());
    if (ImpCharacter) {
        ImpCharacter->Move(Value.Get<FVector2D>());
    } else {
        UE_LOG(LogTemplateController, Error, TEXT("Move function: No character found"));
    }
}

void AImpPlayerController::Look(const FInputActionValue& Value) {
    UE_LOG(LogTemplateController, Log, TEXT("Look function called on controller!"));
    
    AImpCharacter* ImpCharacter = Cast<AImpCharacter>(GetPawn());
    if (ImpCharacter) {
        ImpCharacter->Look(Value.Get<FVector2D>());
    } else {
        UE_LOG(LogTemplateController, Error, TEXT("Look function: No character found"));
    }
    
}

void AImpPlayerController::Jump() {
    UE_LOG(LogTemplateController, Log, TEXT("Jump function called on controller!"));
    
    AImpCharacter* ImpCharacter = Cast<AImpCharacter>(GetPawn());
    if (ImpCharacter) {
        ImpCharacter->Jump();
    } else {
        UE_LOG(LogTemplateController, Error, TEXT("Jump function: No character found"));
    }
}

void AImpPlayerController::StopJumping() {
    UE_LOG(LogTemplateController, Log, TEXT("StopJumping function called on controller!"));
    
    AImpCharacter* ImpCharacter = Cast<AImpCharacter>(GetPawn());
    if (ImpCharacter) {
        ImpCharacter->StopJumping();
    } else {
        UE_LOG(LogTemplateController, Error, TEXT("StopJumping function: No character found"));
    }
}