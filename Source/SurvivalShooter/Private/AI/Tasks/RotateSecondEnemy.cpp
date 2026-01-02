// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/RotateSecondEnemy.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "AI/SecondEnemyController.h"
#include "AI/SecondEnemyStates.h"

URotateSecondEnemy::URotateSecondEnemy()
{
    bNotifyTick = true;     // Permite usar TickTask
    RotationSpeed = 180;    // Grados por segundo
}

EBTNodeResult::Type URotateSecondEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    ACharacter* Pawn = Cast<ACharacter>(AICon->GetPawn());
    FRotator CurrentRot = Pawn->GetActorRotation();
    
    float RandomYaw = FMath::FRandRange(90.f, 180.f);
    if (FMath::RandBool()) RandomYaw *= -1;

    TargetRotation = CurrentRot + FRotator(0, RandomYaw, 0);
    TargetRotation.Normalize();
    Cast<ASecondEnemyController>(AICon)->bRotating = true;

    return EBTNodeResult::InProgress;
}

void URotateSecondEnemy::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!Cast<ASecondEnemyController>(AICon)->bRotating) return;

    auto BB = OwnerComp.GetBlackboardComponent();
    uint8 CurrentState = BB->GetValueAsEnum("CurrentState");

    if(CurrentState == static_cast<uint8>(ESecondEnemyStates::Chase))
    {
        // Si cambió a chase, terminamos la tarea
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }
    
    ACharacter* Pawn = Cast<ACharacter>(AICon->GetPawn());
    FRotator CurrentRot = Pawn->GetActorRotation();
    FRotator NewRot = FMath::RInterpConstantTo(CurrentRot, TargetRotation, DeltaSeconds, RotationSpeed);

    Pawn->SetActorRotation(NewRot);

    if (NewRot.Equals(TargetRotation, 1))
    {
        Cast<ASecondEnemyController>(AICon)->bRotating = true;
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}