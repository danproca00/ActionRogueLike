// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"


void ASAIController::BeginPlay()
{
	//call super to not lose any of the logic
	Super::BeginPlay();

	//lets us have an assert check
	if (ensureMsgf(BehaviourTree, TEXT("Behaviour tree is null pointer! Pls assign BehaviourTree in ur AIController uwu!")))
	{
		RunBehaviorTree(BehaviourTree);
	}

	

	//APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	//if (MyPawn)
	//{
	//	//get blackboard component and set that move to location
	//	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());

	//	//move towards the character
	//	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	//}
}


