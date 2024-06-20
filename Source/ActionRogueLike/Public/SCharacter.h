// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"


//to declare the classes bc the compiler will actually not know where to look
class UCameraComponent;
class USpringArmComponent;


UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	//let s us assign a class
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:

	//expose the components
	UPROPERTY(VisibleAnywhere)
	//third person component
	USpringArmComponent* SpringArmComp;

	//expose the components
	UPROPERTY(VisibleAnywhere)
	//Add the components to the camera for the character
	UCameraComponent* CameraComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//declare the MoveForward function
	void MoveForward(float Value /*0 to 1 and 0 to -1*/);

	//declare the MoveRight function
	void MoveRight(float Value /*0 to 1 and 0 to -1*/);

	//bc it is an action, it doesn t need any parameters
	void PrimaryAttack();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
