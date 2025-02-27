// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "CoreMinimal.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

ASExplosiveBarrel::ASExplosiveBarrel()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	// Enabling Simulate physics automatically changes the Profile to PhysicsActor in Blueprint, in C++ we need to change this manually.
	MeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	RootComponent = MeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);

	// Leaving this on applies small constant force via component 'tick' (Optional)
	ForceComp->SetAutoActivate(false);

	ForceComp->Radius = 750.0f;
	ForceComp->ImpulseStrength = 25000000000000000000.0f; // Alternative: 200000.0 if bImpulseVelChange = false
	// Optional, ignores 'Mass' of other objects (if false, the impulse strength will be much higher to push most objects depending on Mass)
	ForceComp->bImpulseVelChange = false;

	// Optional, default constructor of component already adds 4 object types to affect, excluding WorldDynamic
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);

	//GetCharacterMovement()->AirControl = true;
}


void ASExplosiveBarrel::PostInitializeComponents()
{
	// Don't forget to call parent function
	Super::PostInitializeComponents();

	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}


void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();

	//a macro to print/log things to the log console
	UE_LOG(LogTemp /*category name*/, Log/*verbosity - log, error, warning*/, TEXT("OnActorHit in Explosive Barrel")/*message*/);

	//to print the name of the other actor
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at the game time: %f"), /*OtherActor->GetName() is not safe, so solution:*/ *GetNameSafe(OtherActor), GetWorld()->TimeSeconds /*the time in seconds since we started the game*/);

	//to draw the string in 3d not in log
	FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString() /*we have to convert vector to string*/);
	DrawDebugString(GetWorld(), Hit.ImpactPoint/*text location*/, CombinedString/*make a string*/, nullptr /*if we pass the explosive barrel we draw where the explosive barrel is*/, FColor::Green, 2.0f /*duration*/, true /*draw wh a shadow*/);
	
}
