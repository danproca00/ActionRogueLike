// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGamePlayInterface.h"
#include "DrawDebugHelpers.h"
#include "SWorldUserWidget.h"


static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for  Interact Component."), ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TraceRadius = 30.0f;
	TraceDistance = 500.0f;
	CollisionChannel = ECC_WorldDynamic;
}

// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = Cast<APawn>(GetOwner());

	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
	
}

void USInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	FCollisionObjectQueryParams ObjectQueryParams;
	//add object types to query
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	//to fill up start and end
	AActor* MyOwner = GetOwner(); //in this case it s going to be the character
	//FVector Start;



	FVector EyeLocation;
	FRotator EyeRotation;

	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance); //extends to the direction we're looking -> character model eye, not our camera

	//making the hit result
	FHitResult Hit;


	//collision querries - like spawning an actor
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit/*hit result filled wh a bunch of data*/, EyeLocation/*start location*/, End/*end location*/, ObjectQueryParams /*Fcollison object query parameter */); //we want to find anything that is of type world dynamic

	//bool bBlockingHit = GetWorld()->GetActorEyesViewPoint(EyeLocation, EyeRotation); //fills location and rotation - "did i hit something that was blocking me or we didn t trace anything?"
	TArray<FHitResult> Hits;

	//float Radius = 30.0f;

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);



	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity /*empty rotation*/, ObjectQueryParams, Shape/*collision shape*/); //takes a sphere and it moves it virtually from start to end, and it figures out where it finds first blocking hit, anywhere on its radius

	FocusedActor = nullptr;

	//iterate through the whole array
	/*for (FHitResult Hit : Hits)
	{*/
		AActor* HitActor = Hit.GetActor(); //bc we want to call the interact function that we created
		if (HitActor) //hit when it s null
		{
			//check if we implement that interface
			if (HitActor->Implements<USGamePlayInterface>())
			{
				FocusedActor = HitActor;
				//break;
			}

		}

		//DrawDebugSphere(GetWorld(), Hit.ImpactPoint /*centre*/, TraceRadius, 32 /*the ammount of lines that it will draw the sphere*/, LineColor, false /*persistent lines*/,  2.0f /*time*/);
	//}


		FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;


		if (FocusedActor)
		{
			if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
			{
				DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
			}

			if (DefaultWidgetInstance)
			{
				DefaultWidgetInstance->AttachedActor = FocusedActor;

				if (!DefaultWidgetInstance->IsInViewport())
				{
					DefaultWidgetInstance->AddToViewport();
				}

			}

		}

		else
		{
			if (DefaultWidgetInstance)
			{
				DefaultWidgetInstance->RemoveFromParent();
			}
		}

		if (bDebugDraw)
		{
			//line for debug purpose - to see where the character is looking
			DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false /*persistent lines*/, 2.0f /*time*/, 0 /*priority*/, 2.0f/*thickness*/);
		}


	}
//}


void USInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No focus actor to interact uwu ^.^");
		return;
	}

	//cast that owner to a pawn
	APawn* MyPawn = Cast<APawn>(GetOwner()); //this casting type is safer than the regular c style one

	ISGamePlayInterface::Execute_Interact(InFocus, MyPawn);
}