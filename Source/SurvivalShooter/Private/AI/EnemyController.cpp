// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyController.h"

#include "BrainComponent.h"
#include "AI/BaseEnemy.h"
#include "AI/EnemyStates.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"

AEnemyController::AEnemyController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>("AI Perception");
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
	// Al principio del juego cambio a estado Patrol aunque sea el de menor prioridad
	ChangeState(static_cast<uint8>(ESecondEnemyStates::Patrol));

	
	//Se ejecuta cuando mi percepción es actualizada.
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnSensed);
}

void AEnemyController::OnSensed(AActor* Actor, FAIStimulus Stimulus)
{
	auto SenseType = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);
	
	ESecondEnemyStates CurrentState = static_cast<ESecondEnemyStates>(GetBlackboardComponent()->GetValueAsEnum("CurrentState"));
	
	//Se ha actualizado el sentido de la vista¿?
	if (SenseType == UAISense_Sight::StaticClass())
	{
		if (Stimulus.WasSuccessfullySensed()) //Positivo: He visto algo.
		{
			GetBlackboardComponent()->SetValueAsObject("Target", Actor);
			
			ChangeState(static_cast<uint8>(ESecondEnemyStates::Chase));
		}
		else //Negativo: He dejado de ver algo.
		{
			GetBlackboardComponent()->SetValueAsVector("PointOfInterest", Stimulus.StimulusLocation);
			ChangeState(static_cast<uint8>(ESecondEnemyStates::Investigate));
		}
	}
	//Si escucho algo y no estoy persiguiendo.
	else if (SenseType == UAISense_Hearing::StaticClass() && CurrentState != ESecondEnemyStates::Chase)
	{
		// if (Stimulus.WasSuccessfullySensed())
		// {
		// 	GetBlackboardComponent()->SetValueAsVector("PointOfInterest", Stimulus.StimulusLocation);
		// 	ChangeState(static_cast<uint8>(EEnemyStates::Investigate));
		// }
	}
	else if (SenseType == UAISense_Damage::StaticClass() && CurrentState != ESecondEnemyStates::Chase)
	{
		UE_LOG(LogTemp, Warning, TEXT("Me han dañado!!!"));
		if (Stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Warning, TEXT("Me han dañado!!!"));
			FVector DamageDirection = Stimulus.StimulusLocation - GetPawn()->GetActorLocation();
			DamageDirection.Z = 0;		// No intento volar
			DamageDirection.Normalize();
			
			FVector InterestLocation = GetPawn()->GetActorLocation() + DamageDirection * 800;

			GetBlackboardComponent()->SetValueAsVector("PointOfInterest", InterestLocation);
			GetBlackboardComponent()->ClearValue("CurrentState");		// Para reiniciar el estado actual (?)
			ChangeState(static_cast<uint8>(ESecondEnemyStates::Investigate));
		}
	}
}

void AEnemyController::ChangeState(uint8 State)
{
	GetBlackboardComponent()->SetValueAsEnum("CurrentState", State);
}

void AEnemyController::GG()
{
	UE_LOG(LogTemp, Warning, TEXT("Jugador muerto"));
	ChangeState(static_cast<uint8>(ESecondEnemyStates::Investigate));
}
