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
	ChangeEnemyState(static_cast<uint8>(EEnemyStates::Patrol));

	
	//Se ejecuta cuando mi percepción es actualizada.
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnSensed);
}

void AEnemyController::OnSensed(AActor* Actor, FAIStimulus Stimulus)
{
	// No queremos que los enemigos se peguen entre sí
	if (!Actor->ActorHasTag("Player")) return;
	
	auto SenseType = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);
	
	EEnemyStates CurrentState = static_cast<EEnemyStates>(GetBlackboardComponent()->GetValueAsEnum("CurrentState"));
	
	//Se ha actualizado el sentido de la vista¿?
	if (SenseType == UAISense_Sight::StaticClass())
	{
		if (Stimulus.WasSuccessfullySensed()) //Positivo: He visto algo.
		{
			GetBlackboardComponent()->SetValueAsObject("Target", Actor);
			
			ChangeEnemyState(static_cast<uint8>(EEnemyStates::Chase));
		}
		else //Negativo: He dejado de ver algo.
		{
			GetBlackboardComponent()->SetValueAsVector("PointOfInterest", Stimulus.StimulusLocation);
			ChangeEnemyState(static_cast<uint8>(EEnemyStates::Investigate));
		}
	}
	//Si escucho algo y no estoy persiguiendo.
	else if (SenseType == UAISense_Hearing::StaticClass() && CurrentState != EEnemyStates::Chase)
	{
		// if (Stimulus.WasSuccessfullySensed())
		// {
		// 	GetBlackboardComponent()->SetValueAsVector("PointOfInterest", Stimulus.StimulusLocation);
		// 	ChangeState(static_cast<uint8>(EEnemyStates::Investigate));
		// }
	}
	else if (SenseType == UAISense_Damage::StaticClass() && CurrentState != EEnemyStates::Chase)
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
			ChangeEnemyState(static_cast<uint8>(EEnemyStates::Investigate));
		}
	}
}

void AEnemyController::ChangeEnemyState(uint8 State)
{
	GetBlackboardComponent()->SetValueAsEnum("CurrentState", State);
}

void AEnemyController::GG()
{
	UE_LOG(LogTemp, Warning, TEXT("Jugador muerto"));
	ChangeEnemyState(static_cast<uint8>(EEnemyStates::Investigate));
}
