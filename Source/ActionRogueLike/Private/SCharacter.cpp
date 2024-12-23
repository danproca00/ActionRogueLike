// Fill out your copyright notice in the Description page of Project Settings.

// This include is added at the top of your SCharacter.cpp file
#include "SCharacter.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "D:\Epic Games\UE_5.4\Engine\Source\Runtime\Core\Public\Math\RotationMatrix.h"
#include "SInteractionComponent.h"
#include "SAttributeComponent.h"
#include "SActionComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//bc those components we declare are just "empty" pointers, we need to instantiate them here
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp"); //it s a special function to instantiate, you can't use it outside of the class

	//we can move up and down - we coould also do that in the blueprint
	SpringArmComp->bUsePawnControlRotation = true;

	//to be sure that the camera is attached to the character
	SpringArmComp->SetupAttachment(RootComponent /*whatever it is the first component in the hierarchy of the components*/);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	//to be sure that the camera is attached to the character
	CameraComp->SetupAttachment(SpringArmComp /*to have the distance between the character and the camera*/);

	//instantiate the component
	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
	
	//instantiate the component
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	//get the character movement to set orient to movement rotation -> rotate the character towards whetever the character is moving 
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//to have more of a rpg movement style => the character can look towards the camera
	bUseControllerRotationYaw = false;

	TimeToHitParamName = "TimeToHit";

	
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//bind the on health changed
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	//AttackAnimDelay = 0.2f;
}

//the implementation of the MoveForwardFunction
void ASCharacter::MoveForward(float Value)
{
	//to not move to the direction of the pawn, but of the camera
	FRotator ControlRot = GetControlRotation();

	//bc we are interested in yaw, and not in pitch & the role we put them as 0 -> not to move them in the sky
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;


	//pass along the information - what direction we want to move
	//AddMovementInput(GetActorForwardVector()/*returns a vector that is just pointing to the direction the actor is moving*/, Value /*scale*/);
	AddMovementInput(ControlRot.Vector()/*expects a vector, but we can move back and forth wh vectors and rotators -> vector rotators*/, Value /*scale*/);
}

//the implementation of the MoveRightFunction
void ASCharacter::MoveRight(float Value)
{
	//to not move to the direction of the pawn, but of the camera
	FRotator ControlRot = GetControlRotation();

	//bc we are interested in yaw, and not in pitch & the role we put them as 0 -> not to move them in the sky
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	//get the right access of the rotator
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	//pass along the information - what direction we want to move
	//AddMovementInput(GetActorRightVector()/*returns a vector that is just pointing to the direction the actor is moving, in this case right*/, Value /*scale*/);
	
	AddMovementInput(RightVector, Value);

	
}

void ASCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

// Called every frame
// This is entirely optional, it draws two arrows to visualize rotations of the player
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//to make the character move forward- 2 options: BindAccess and BindAxis
	PlayerInputComponent->BindAxis("MoveForward" /*name that we need to bind in*/, this /*the character*/, &ASCharacter::MoveForward /*pass the function*/); //we choose axis, bc we want to move forward and backward, where forward would be 1 and backward -1
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight); //help move the character left and right

	//to make the character turn around
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput /*yaw - horizontal rotation, pitch - vertical rotation and roll => let us turn */);

	//helps lookup the character
	PlayerInputComponent->BindAxis("Lookup", this, &APawn::AddControllerPitchInput);

	//we want now to BindAccess
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed /*trigger whenever IE is pressed*/, this /*expects an user object*/, &ASCharacter::PrimaryAttack);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this , &ACharacter::Jump);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this , &ASCharacter::PrimaryInteract);

	{
		//PlayAnimMontage(AttackAnim);

		//GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);

		//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
		//GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, AttackAnimDelay);
	}

}

void ASCharacter::HealSelf(float Amount)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

void ASCharacter::PrimaryAttack()
{
	////an animation when we attack
	//PlayAnimMontage(AttackAnim);

	////now we need a timer to not have an "awkward" attack animation
	//GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack/*holds a handle to the timer*/, this, &ASCharacter::PrimaryAttack_TimeElapsed/*function to trigger when the timer elapses*/, 0.2f/*delay*/);

	////GetWorldTimerManager.ClearTimer(TimerHandle_PrimaryAttack); //use it if a character dies but he s in the middle of doing an attack -> cancel the attack

	ActionComp->StartActionByName(this, "PrimaryAttack");

	
}

//void ASCharacter::PrimaryAttack_TimeElapsed()
//{
//	if (ensure(ProjectileClass))
//		SpawnProjectile(ProjectileClass);
//}


void ASCharacter::BlackHoleAttack()
{
	/*PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_BlackholeAttack, this, &ASCharacter::BlackholeAttack_TimeElapsed, AttackAnimDelay);*/
	ActionComp->StartActionByName(this, "Blackhole");
}


//void ASCharacter::BlackholeAttack_TimeElapsed()
//{
//	SpawnProjectile(BlackHoleProjectileClass);
//}


void ASCharacter::Dash()
{
	/*PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_Dash, this, &ASCharacter::Dash_TimeElapsed, AttackAnimDelay);*/

	ActionComp->StartActionByName(this, "Dash");
}


//void ASCharacter::Dash_TimeElapsed()
//{
//	SpawnProjectile(DashProjectileClass);
//}


void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		//GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
		FHitResult Hit;
		FVector TraceStart = CameraComp->GetComponentLocation();
		// endpoint far into the look-at distance (not too far, still adjust somewhat towards crosshair on a miss)
		FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000);

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		// Ignore Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FRotator ProjRotation;
		// true if we got to a blocking hit (Alternative: SweepSingleByChannel with ECC_WorldDynamic)
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			// Adjust location to end up at crosshair look-at
			ProjRotation = FRotationMatrix::MakeFromX(Hit.ImpactPoint - HandLocation).Rotator();
		}
		else
		{
			// Fall-back since we failed to find any blocking hit
			ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		}

	}
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{

	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
	}

	//check if new health is smaller or = wh 0 and delta < 0 (then we re damaged and if it s higer we re healed)
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		//after we died we re not able to move around anymore
		APlayerController* PC =  Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}



void ASCharacter::PrimaryInteract()
{
	//call the function on the components
	//check for null
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}

	
}

