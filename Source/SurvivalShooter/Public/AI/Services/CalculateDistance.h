// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CalculateDistance.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALSHOOTER_API UCalculateDistance : public UBTService
{
	GENERATED_BODY()

public:
	UCalculateDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere)
	float MinDistanceForStrafing = 500;		// Ha de coincidir con el radio del EQS
};
