#include "AI/MyBTTask_RangedAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SAttributeComponent.h"

UMyBTTask_RangedAttack::UMyBTTask_RangedAttack()
{
    MaxBulletSpread = 2.0f;
}

// Spawn a projectile in the direction of the actor (target actor)
EBTNodeResult::Type UMyBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* MyController = OwnerComp.GetAIOwner();
    if (ensure(MyController))
    {
        ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
        if (MyPawn == nullptr)
        {
            return EBTNodeResult::Failed;
        }

        FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");
        AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
        if (TargetActor == nullptr || !USAttributeComponent::IsActorAlive(TargetActor))
        {
            return EBTNodeResult::Failed;
        }

        FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
        FRotator MuzzleRotation = Direction.Rotation();
        MuzzleRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread);
        MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        Params.Instigator = MyPawn;

        AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);
        return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
    }

    return EBTNodeResult::Failed;
}
