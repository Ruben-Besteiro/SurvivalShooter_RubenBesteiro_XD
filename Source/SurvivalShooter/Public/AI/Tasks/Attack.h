// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Attack.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALSHOOTER_API UAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnAttackEnded(UAnimMontage* Montage, bool bInterrupted);
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	UPROPERTY()
	UBehaviorTreeComponent* MyComp;
};
