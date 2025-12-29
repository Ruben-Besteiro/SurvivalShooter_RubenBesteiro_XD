// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/Attack.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "AIController.h"
#include "AI/BaseEnemy.h"

EBTNodeResult::Type UAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	
	MyComp = &OwnerComp;
	
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (auto ShooterEnemy = Cast<ABaseEnemy>(Pawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("eJECUTADO"));
		ShooterEnemy->PlayAttackMontage();
		
		//Estoy al tanto de cuando se termina el montage establecido en la linea anterior.
		ShooterEnemy->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UAttack::OnAttackEnded);

		//Marco la tarea como en InProgress, porque me obligan a devolver algo
	}
	
	return EBTNodeResult::InProgress;
	
	
}

void UAttack::OnAttackEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Warning, TEXT("hOLAAAAAAA"))
	FinishLatentTask(*MyComp, EBTNodeResult::Succeeded);
}
