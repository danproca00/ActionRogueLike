// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGamePlayInterface.h"
#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	//add object types to query
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	//to fill up start and end
	AActor* MyOwner = GetOwner(); //in this case it s going to be the character
	//FVector Start;


	FVector EyeLocation;
	FRotator EyeRotation;

	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000); //extends to the direction we're looking -> character model eye, not our camera

	//making the hit result
	FHitResult Hit;


	//collision querries - like spawning an actor
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit/*hit result filled wh a bunch of data*/, EyeLocation/*start location*/, End/*end location*/, ObjectQueryParams /*Fcollison object query parameter */); //we want to find anything that is of type world dynamic

	//bool bBlockingHit = GetWorld()->GetActorEyesViewPoint(EyeLocation, EyeRotation); //fills location and rotation - "did i hit something that was blocking me or we didn t trace anything?"
	//TArray<FHitResult> Hits;

	//float Radius = 30.0f;

	//FCollisionShape Shape;
	//Shape.SetSphere(Radius);
	
	

	//bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity /*empty rotation*/, ObjectQueryParams, Shape/*collision shape*/); //takes a sphere and it moves it virtually from start to end, and it figures out where it finds first blocking hit, anywhere on its radius


	//iterate through the whole array
	/*for (FHitResult Hit : Hits)
	{*/
		AActor* HitActor = Hit.GetActor(); //bc we want to call the interact function that we created
		if (HitActor) //hit when it s null
		{
			//check if we implement that interface
			if (HitActor->Implements<USGamePlayInterface>())
			{
				//cast that owner to a pawn
				APawn* MyPawn = Cast<APawn>(MyOwner); //this casting type is safer than the regular c style one

				ISGamePlayInterface::Execute_Interact(HitActor, MyPawn);
				//break;
			}

		}
		
		//DrawDebugSphere(GetWorld(), Hit.ImpactPoint /*centre*/, Radius, 32 /*the ammount of lines that it will draw the sphere*/, LineColor, false /*persistent lines*/,  2.0f /*time*/);
	//}

	
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	//line for debug purpose - to see where the character is looking
	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false /*persistent lines*/, 2.0f /*time*/, 0 /*priority*/, 2.0f/*thickness*/);
	
}