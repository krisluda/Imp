#include "ImpCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h" 

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AImpCharacter::AImpCharacter() {
    PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;
    bAlwaysRelevant = true;

    GetCapsuleComponent()->InitCapsuleSize(20.0f, 40.0f);

    //Character does not follow controllers rotation, only camera boom will at default.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 200.0f;
    CameraBoom->bUsePawnControlRotation = true; //CameraBoom rotates with controller

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false; //Camera itself does not rotate with controller relative to boom

    GetCharacterMovement()->bOrientRotationToMovement = true; 
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 1000.0f;
    GetCharacterMovement()->AirControl = 0.5f;
    GetCharacterMovement()->MaxWalkSpeed = 400.0f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

}

void AImpCharacter::BeginPlay() {
    Super::BeginPlay();
}

void AImpCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

void AImpCharacter::S_Move_Implementation(const FVector2D& InputValue) {
    if (Controller != nullptr) {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDir, InputValue.Y);
        AddMovementInput(RightDir, InputValue.X);
    }
}

void AImpCharacter::S_Look_Implementation(const FVector2D& InputValue) {
    if (Controller != nullptr) {
        AddControllerYawInput(InputValue.X);
        AddControllerPitchInput(InputValue.Y);    
    }
}

void AImpCharacter::S_Jump_Implementation() {
    Super::Jump();
}

void AImpCharacter::S_StopJumping_Implementation() {
    Super::StopJumping();
}

void AImpCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AImpCharacter, Health);
}