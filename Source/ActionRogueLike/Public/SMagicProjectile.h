// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"


class USphereComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();

protected:

	//the collision component
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;
	
	//component that will give a velocity - go into a straight line
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* EffectComp;
	
	//something to show in the game world
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* MovementComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
