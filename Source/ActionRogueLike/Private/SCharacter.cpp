// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//bc those components we declare are just "empty" pointers, we need to instantiate them here
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp"); //it s a special function to instantiate, you can't use it outside of the class

	//to be sure that the camera is attached to the character
	SpringArmComp->SetupAttachment(RootComponent /*whatever it is the first component in the hierarchy of the components*/);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	//to be sure that the camera is attached to the character
	CameraComp->SetupAttachment(SpringArmComp /*to have the distance between the character and the camera*/);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

//the implementation of the MoveForwardFunction
void ASCharacter::MoveForward(float Value)
{
	//pass along the information - what direction we want to move
	AddMovementInput(GetActorForwardVector()/*returns a vector that is just pointing to the direction the actor is moving*/, Value /*scale*/);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//to make the character move forward- 2 options: BindAccess and BindAxis
	PlayerInputComponent->BindAxis("MoveForward" /*name that we need to bind in*/, this /*the character*/, &ASCharacter::MoveForward /*pass the function*/); //we choose axis, bc we want to move forward and backward, where forward would be 1 and backward -1

	//to make the character turn around
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput /*yaw - horizontal rotation, pitch - vertical rotation and roll => let us turn */);

}

