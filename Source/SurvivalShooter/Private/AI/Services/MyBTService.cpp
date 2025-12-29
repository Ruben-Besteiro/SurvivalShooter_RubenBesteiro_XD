// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/MyBTService.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "AIController.h"                         // GetAIOwner()
#include "GameFramework/Character.h"              // ACharacter, GetCharacter()
#include "GameFramework/CharacterMovementComponent.h" // GetCharacterMovement()

UMyBTService::UMyBTService()
{
	// Los servicios ocurren en 3 fases, que se pueden habilitar o deshabilitar:
	bNotifyBecomeRelevant = true;	// Inicio
	bNotifyTick = false;	// Actualización
	bNotifyCeaseRelevant = false;	// Fin
}

void UMyBTService::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	OwnerComp.GetAIOwner()->GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

}