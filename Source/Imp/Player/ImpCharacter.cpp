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

    GetCharacterMovement()->SetIsReplicated(true);

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
    GetCharacterMovement()->JumpZVelocity = 300.0f;
    GetCharacterMovement()->AirControl = 0.5f;
    GetCharacterMovement()->MaxWalkSpeed = 400.0f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
    JumpMaxHoldTime = 0.1f;
}

void AImpCharacter::BeginPlay() {
    Super::BeginPlay();
}

void AImpCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

void AImpCharacter::Move(const FVector2D& InputValue) {
    if (!HasAuthority()) {
        UE_LOG(LogTemp, Warning, TEXT("Client: Attempting to move character"));
    }
    
    if (Controller == nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Client: Character has no controller"));
        return;
    }



    if (HasAuthority()) {
        if (Controller != nullptr) {
            const FRotator Rotation = Controller->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);
        
            const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
            const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        
            AddMovementInput(ForwardDir, InputValue.Y);
            AddMovementInput(RightDir, InputValue.X);
        }
        
    } else {
        ServerMoveChar(InputValue);
    }
}

void AImpCharacter::ServerMoveChar_Implementation(const FVector2D& InputValue) {
    UE_LOG(LogTemp, Warning, TEXT("Server: Executing movement for client"));
    if (Controller != nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Server: Controller is not null"));
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
    
        const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    
        AddMovementInput(ForwardDir, InputValue.Y);
        AddMovementInput(RightDir, InputValue.X);
    }
}

bool AImpCharacter::ServerMoveChar_Validate(const FVector2D& InputValue) {
    return true;
}

void AImpCharacter::Look(const FVector2D& InputValue) {
    if (HasAuthority()) {
        if (Controller != nullptr) {
        AddControllerYawInput(InputValue.X);
            AddControllerPitchInput(InputValue.Y);    
        } 
    } else {
        ServerLook(InputValue);
    }

}

void AImpCharacter::ServerLook_Implementation(const FVector2D& InputValue) {
    Look(InputValue);
}

void AImpCharacter::Jump() {
    if (HasAuthority()) {
        Super::Jump();
    } else {
        ServerJump();
    }
}

void AImpCharacter::ServerJump_Implementation() {
    Super::Jump();
}

void AImpCharacter::StopJump() {
    if (HasAuthority()) {
        StopJumping();
    } else {
        ServerStopJump();
    }
}

void AImpCharacter::ServerStopJump_Implementation() {
    StopJumping();
}

void AImpCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AImpCharacter, Health);
}