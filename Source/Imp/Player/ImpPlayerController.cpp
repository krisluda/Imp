#include "ImpPlayerController.h"
#include "ImpCharacter.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "Log.h"
#include "GameFramework/Character.h"

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

//Add if-tests to see if characters exist, and add other logic for when the character isnt.
void AImpPlayerController::Move(const FInputActionValue& Value) {  
    AImpCharacter* ImpCharacter = Cast<AImpCharacter>(GetPawn());
    ImpCharacter->Move(Value.Get<FVector2D>());
}

void AImpPlayerController::Look(const FInputActionValue& Value) {    
    AImpCharacter* ImpCharacter = Cast<AImpCharacter>(GetPawn());
    ImpCharacter->Look(Value.Get<FVector2D>());
    AddYawInput(Value.Get<FVector2D>().X);
    AddPitchInput(Value.Get<FVector2D>().Y);
}

void AImpPlayerController::Jump() {
    AImpCharacter* ImpCharacter = Cast<AImpCharacter>(GetPawn());
    ImpCharacter->Jump();
}

void AImpPlayerController::StopJumping() {  
    AImpCharacter* ImpCharacter = Cast<AImpCharacter>(GetPawn());
    ImpCharacter->StopJumping();
}