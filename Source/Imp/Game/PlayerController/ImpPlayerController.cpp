#include "ImpPlayerController.h"
#include "ImpCharacter.h"
#include "ImpPlayerState.h"
#include "ImpAbilitySystemComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "InventoryComponent.h"
#include "InventoryWidgetController.h"
#include "ImpWidget.h"
#include "ImpInputComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Log.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"

AImpPlayerController::AImpPlayerController() {
    bReplicates = true;

    
    InputMapping = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Imp/Core/Player/Input/IMC_Imp.IMC_Imp"));
    MoveAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Imp/Core/Player/Input/IA_Move.IA_Move"));
    LookAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Imp/Core/Player/Input/IA_Look.IA_Look"));
    JumpAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Imp/Core/Player/Input/IA_Jump.IA_Jump"));


    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
    InventoryComponent->SetIsReplicated(true);
}

void AImpPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AImpPlayerController, InventoryComponent);
}

void AImpPlayerController::BeginPlay() {
    Super::BeginPlay();

    if (IsValid(InventoryComponent)) {
        InventoryComponent->bOwnerLocallyControlled = IsLocalController();
    }

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
        Subsystem->AddMappingContext(InputMapping, 0);
    }
}

UAbilitySystemComponent* AImpPlayerController::GetAbilitySystemComponent() const {
    //This is a generic getter for the AbilitySystemComp. We also made a specific getter for the ImpAbilitySystemComp that is used in the Ability Pressed and Released
    //functions. Maybe they could be combined, but I didn't bother yet.

    return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
}

UImpAbilitySystemComponent* AImpPlayerController::GetImpAbilitySystemComponent() {
    if (!IsValid(ImpAbilitySystemComp)) {
        if (const AImpPlayerState* ImpPlayerState = GetPlayerState<AImpPlayerState>()) {
            ImpAbilitySystemComp = ImpPlayerState->GetImpAbilitySystemComponent();
        }
    }

    return ImpAbilitySystemComp;
}


UInventoryComponent* AImpPlayerController::GetInventoryComponent_Implementation() {
    return InventoryComponent;
}

void AImpPlayerController::SetDynamicProjectile_Implementation(const FGameplayTag &ProjectileTag) {
    if (IsValid(ImpAbilitySystemComp)) {
        ImpAbilitySystemComp->SetDynamicProjectile(ProjectileTag);
    }
}

UInventoryWidgetController* AImpPlayerController::GetInventoryWidgetController()
{
    if (!IsValid(InventoryWidgetController)) {
        InventoryWidgetController = NewObject<UInventoryWidgetController>(this, InventoryWidgetControllerClass);
        InventoryWidgetController->SetOwningActor(this);
        InventoryWidgetController->BindCallbacksToDependencies();
    }

    return InventoryWidgetController;
}

void AImpPlayerController::CreateInventoryWidget() {
    if (UUserWidget* Widget = CreateWidget<UImpWidget>(this, InventoryWidgetClass)) {
        InventoryWidget = Cast<UImpWidget>(Widget);
        InventoryWidget->SetWidgetController(GetInventoryWidgetController());
        InventoryWidgetController->BroadcastInitialValues();
        InventoryWidget->AddToViewport();
    }
}


void AImpPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    if (UImpInputComponent* ImpInputComp = Cast<UImpInputComponent>(InputComponent)) {
        ImpInputComp->BindAbilityActions(ImpInputConfig, this, &ThisClass::AbilityInputPressed, &ThisClass::AbilityInputReleased);
    }
    
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AImpPlayerController::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AImpPlayerController::Look);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AImpPlayerController::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AImpPlayerController::StopJumping);
    }
}

void AImpPlayerController::AbilityInputPressed(FGameplayTag InputTag) {
    if (IsValid(GetImpAbilitySystemComponent())) {
        ImpAbilitySystemComp->AbilityInputPressed(InputTag);
    }
}

void AImpPlayerController::AbilityInputReleased(FGameplayTag InputTag) {
    if (IsValid(ImpAbilitySystemComp)) {
        ImpAbilitySystemComp->AbilityInputReleased(InputTag);
    }
}

//Add if-tests to see if characters exist, and add other logic for when the character isnt.
void AImpPlayerController::Move(const FInputActionValue& Value) {  
    if (AImpCharacter* ImpCharacter = Cast<AImpCharacter>(GetPawn())) {
        ImpCharacter->Move(Value.Get<FVector2D>());
    }
}

void AImpPlayerController::Look(const FInputActionValue& Value) {    
    //AImpCharacter* ImpCharacter = Cast<AImpCharacter>(GetPawn());
    //ImpCharacter->Look(Value.Get<FVector2D>());
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