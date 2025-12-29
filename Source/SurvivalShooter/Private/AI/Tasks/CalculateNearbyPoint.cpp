// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/CalculateNearbyPoint.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UCalculateNearbyPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	FNavLocation ResultLocation;
	
	FVector PointOfInterest = OwnerComp.GetBlackboardComponent()->GetValueAsVector("PointOfInterest");

	//Obtener la información del NavMesh del mundo actual.
	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	//Genero un punto aleatorio que sea alcanzable.
	NavigationSystem->GetRandomReachablePointInRadius(PointOfInterest, SearchRadius, ResultLocation);

	//Establezco como destino actual, dicho punto aleatorio.
	OwnerComp.GetBlackboardComponent()->SetValueAsVector("CurrentDestination", ResultLocation.Location);

	return EBTNodeResult::Succeeded;
}
