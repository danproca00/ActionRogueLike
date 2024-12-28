// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "SProjectileBase.h"
#include "SActionComponent.h"
#include "SMagicProjectile.generated.h"


class USphereComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;
class USActionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MoveComp;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//the collision component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) // to have full acces on blueprint editor
	USphereComponent* SphereComp;
	
	//component that will give a velocity - go into a straight line
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) // to have full acces on blueprint editor
	UParticleSystemComponent* EffectComp;
	
	//something to show in the game world
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) // to have full acces on blueprint editor
	UProjectileMovementComponent* MovementComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
