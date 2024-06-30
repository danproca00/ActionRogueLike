// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGamePlayInterface.h"
#include "SItemChest.generated.h"


class UStaticMeshComponent;

UCLASS()
//to add the interface in the class
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGamePlayInterface
{
	GENERATED_BODY()
	//pure virtual interface implementation - we can do that bc of the BlueprintNativeEvent

public:

	//expose float that we can tweak it
	UPROPERTY(EditAnywhere)
	float TargetPitch;

	void Interact_Implementation(APawn* InstigatorPawn);
	
public:	
	// Sets default values for this actor's properties
	ASItemChest();

protected:
	//static mesh component for the base of the chest
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	//static mesh component for the lid of the chest
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) // to have full acces on blueprint editor
	UStaticMeshComponent* LidMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
