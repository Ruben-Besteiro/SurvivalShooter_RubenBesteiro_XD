// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/SetFocusOnTarget.h"
#include "AIController.h"
#include "AI/BaseEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

USetFocusOnTarget::USetFocusOnTarget()
{
	bNotifyBecomeRelevant = true;
	bNotifyTick = false;
	bNotifyCeaseRelevant = true;
}


void USetFocusOnTarget::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	AAIController* Controller = OwnerComp.GetAIOwner();
	ABaseEnemy* Enemy = Cast<ABaseEnemy>(Controller->GetPawn());
	Cast<ABaseEnemy>(Controller->GetCharacter());
	
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	Controller->SetFocus(Cast<AActor>(Blackboard->GetValueAsObject("Target")));
	Enemy->SetIsStrafing(true);
}


void USetFocusOnTarget::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!IsValid(Controller)) return;
	ABaseEnemy* Enemy = Cast<ABaseEnemy>(Controller->GetPawn());
	if (!IsValid(Enemy)) return;
	Controller->ClearFocus(EAIFocusPriority::Gameplay);
	Enemy->SetIsStrafing(false);
}