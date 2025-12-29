// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CalculateNewDestination.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALSHOOTER_API UCalculateNewDestination : public UBTTaskNode
{
	GENERATED_BODY()

	protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
