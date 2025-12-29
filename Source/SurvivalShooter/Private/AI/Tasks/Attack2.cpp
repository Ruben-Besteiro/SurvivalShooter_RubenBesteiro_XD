// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/Attack2.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "AIController.h"
#include "AI/SecondEnemy.h"

EBTNodeResult::Type UAttack2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	
	MyComp = &OwnerComp;
	
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (auto ShooterEnemy = Cast<ASecondEnemy>(Pawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("Ejecutándose"));
		ShooterEnemy->PlayAttackMontage();
		
		//Estoy al tanto de cuando se termina el montage establecido en la linea anterior.
		ShooterEnemy->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UAttack2::OnAttackEnded);

		//Marco la tarea como en InProgress, porque me obligan a devolver algo
	}
	
	return EBTNodeResult::InProgress;
	
	
}

void UAttack2::OnAttackEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Warning, TEXT("Muy buenos días"));
	FinishLatentTask(*MyComp, EBTNodeResult::Succeeded);
}