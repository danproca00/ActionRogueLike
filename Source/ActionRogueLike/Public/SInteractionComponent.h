// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SGamePlayInterface.h"
#include "SWorldUserWidget.h"
#include "SInteractionComponent.generated.h"


class USGameplayInterface;
class USWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

//public bc we're going to call the function from character into this component
public:

	void PrimaryInteract();

	/**
 * The location in world space where the moving shape would end up against the impacted object, if there is a hit. Equal to the point of impact for line tests.
 * Example: for a sphere trace test, this is the point where the center of the sphere would be located when it touched the other object.
 * For swept movement (but not queries) this may not equal the final location of the shape since hits are pulled back slightly to prevent precision issues from overlapping another surface.
 */
	UPROPERTY()
	FVector_NetQuantize Location;

	/**
	 * Location in world space of the actual contact of the trace shape (box, sphere, ray, etc) with the impacted object.
	 * Example: for a sphere trace test, this is the point where the surface of the sphere touches the other object.
	 * @note: In the case of initial overlap (bStartPenetrating=true), ImpactPoint will be the same as Location because there is no meaningful single impact point to report.
	 */
	UPROPERTY()
	FVector_NetQuantize ImpactPoint;


public:	
	// Sets default values for this component's properties
	USInteractionComponent();

protected:

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);

	void FindBestInteractable();

	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;
	
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel>CollisionChannel;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass;
	
	UPROPERTY()
	USWorldUserWidget* DefaultWidgetInstance;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	
};
