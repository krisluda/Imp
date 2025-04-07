// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "ImpAbilityTypes.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemGlobals.h"
#include "ImpAbilitySystemLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileBase::AProjectileBase() {
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	//The mesh is the root? And it has collision and then just ignores everything? 
	//And then the actual collision sphere is a child of that? STUPIDO. FIX.
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	SetRootComponent(ProjectileMesh);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileMesh->SetCollisionObjectType(ECC_WorldDynamic);
	ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ProjectileMesh->SetIsReplicated(true);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");

	//I want to be able to manually set the size of this
	OverlapSphere = CreateDefaultSubobject<USphereComponent>("OverlapSphere");
	OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapSphere->SetupAttachment(GetRootComponent());
}

void AProjectileBase::BeginPlay() {
	Super::BeginPlay();

	if (HasAuthority()) {
		//This binds a dynamic callback to the primitive component's overlap-function, but ONLY on the server. We need to define our own function for overlapping, and match the required signature found in the engine code base. Go to "OnComponentBeginOverlap" to access PrimitiveComponent, and then click the struct name (FComponentBeginOverlapSignature) to be taken to the delegate declaration.
		OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlap);
	}
}

void AProjectileBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor == GetOwner()) return;

	//GAS-methods of applying a gameplayeffect and destroying the projectile.
	//This is where the projectile hitting something finally captures and sets the TargetASC on the FDamageEffectInfo.
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor)) {
		DamageEffectInfo.TargetASC = TargetASC;
		UImpAbilitySystemLibrary::ApplyDamageEffect(DamageEffectInfo);

		Destroy();
	}
}

void AProjectileBase::SetProjectileParams(const FProjectileParams &Params) {
	if (IsValid(ProjectileMesh)) {
		ProjectileMesh->SetStaticMesh(Params.ProjectileMesh);

	}

	if (IsValid(ProjectileMovementComponent)) {
		ProjectileMovementComponent->InitialSpeed = Params.InitialSpeed;
		ProjectileMovementComponent->ProjectileGravityScale = Params.GravityScale;
		ProjectileMovementComponent->bShouldBounce = Params.bShouldBounce;
		ProjectileMovementComponent->Bounciness = Params.Bounciness;
	}
}

