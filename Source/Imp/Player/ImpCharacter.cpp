#include "ImpCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "ImpPlayerState.h"
#include "ImpAbilitySystemComponent.h"
#include "ImpCharacterClassInfo.h"
#include "ImpAbilitySystemLibrary.h"
#include "GameplayEffect.h"
#include "Log.h"
#include "Net/UnrealNetwork.h" 

AImpCharacter::AImpCharacter() {
    PrimaryActorTick.bCanEverTick = true;
    
    bReplicates = true;
    bAlwaysRelevant = true;

    ImpAbilitySystemComponent = CreateDefaultSubobject<UImpAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

    GetCharacterMovement()->SetIsReplicated(true);

    GetCapsuleComponent()->InitCapsuleSize(20.0f, 40.0f);

    //Character does not follow controllers rotation, only camera boom will at default.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;
    CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 50.0f);
    CameraBoom->TargetOffset = FVector(0.0f, 0.0f, 50.0f);
    CameraBoom->bUsePawnControlRotation = true; //CameraBoom rotates with controller
    CameraBoom->bEnableCameraLag = true;
    CameraBoom->bEnableCameraRotationLag = true;
    CameraBoom->CameraLagSpeed = 10.0f;
    CameraBoom->CameraRotationLagSpeed = 20.0f;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false; //Camera itself does not rotate with controller relative to boom

    GetCharacterMovement()->bOrientRotationToMovement = true; 
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 300.0f;
    GetCharacterMovement()->AirControl = 0.5f;
    GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
    JumpMaxHoldTime = 0.2f;
}

void AImpCharacter::PossessedBy(AController *NewController) {
    Super::PossessedBy(NewController);

    if (HasAuthority()) {
        InitAbilityActorInfo();
    }
}

void AImpCharacter::OnRep_PlayerState() {
    Super::OnRep_PlayerState();

    InitAbilityActorInfo();
}

void AImpCharacter::BeginPlay() {
    Super::BeginPlay();
}

void AImpCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

void AImpCharacter::InitAbilityActorInfo() {
    if (AImpPlayerState* ImpPlayerState = GetPlayerState<AImpPlayerState>()) {
        ImpAbilitySystemComponent = ImpPlayerState->GetImpAbilitySystemComponent();
        ImpAttributeSet = ImpPlayerState->GetImpAttributeSet();

        if (IsValid(ImpAbilitySystemComponent)) {
            ImpAbilitySystemComponent->InitAbilityActorInfo(ImpPlayerState, this);

            if (HasAuthority()) {
                InitClassDefaults();
            } 
        }
    }
}

void AImpCharacter::InitClassDefaults() {
    if (!CharacterTag.IsValid()) {
        IMP_LOG("AImpCharacter::InitClassDefaults: No Character Tag Selected In This Character %s", *GetNameSafe(this));
    } else if (UImpCharacterClassInfo* ClassInfo = UImpAbilitySystemLibrary::GetImpCharacterClassDefaultInfo(this)) {
        if (const FImpCharacterClassDefaultInfo* SelectedClassInfo = ClassInfo->ClassDefaultInfoMap.Find(CharacterTag)) {
            if (IsValid(ImpAbilitySystemComponent)) {
                ImpAbilitySystemComponent->AddCharacterAbilities(SelectedClassInfo->StartingAbilities);
                ImpAbilitySystemComponent->AddCharacterPassiveAbilities(SelectedClassInfo->StartingPassives);
                ImpAbilitySystemComponent->InitializeDefaultAttributes(SelectedClassInfo->DefaultAttributes);
            }
        }

    }


}

void AImpCharacter::Move(const FVector2D &InputValue)
{
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);
        
    const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        
    AddMovementInput(ForwardDir, InputValue.Y);
    AddMovementInput(RightDir, InputValue.X);
}

void AImpCharacter::Look(const FVector2D& InputValue) {
    AddControllerYawInput(InputValue.X);
    AddControllerPitchInput(InputValue.Y);    
}

void AImpCharacter::Jump() {
    Super::Jump();
}

void AImpCharacter::StopJumping() {
    Super::StopJumping();
}

void AImpCharacter::DebugDestroyCharacter() {
    Destroy();
}