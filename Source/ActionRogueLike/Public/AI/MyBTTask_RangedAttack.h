#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MyBTTask_RangedAttack.generated.h"

UCLASS()
class ACTIONROGUELIKE_API UMyBTTask_RangedAttack : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UMyBTTask_RangedAttack();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    UPROPERTY(EditAnywhere, Category = "AI")
    float MaxBulletSpread;

    UPROPERTY(EditAnywhere, Category = "AI")
    TSubclassOf<AActor> ProjectileClass;
};
