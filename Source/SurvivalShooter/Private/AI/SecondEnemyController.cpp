#include "AI/SecondEnemyController.h"

#include "AI/SecondEnemyStates.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"

ASecondEnemyController::ASecondEnemyController()
{
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>("AI Perception");
}

void ASecondEnemyController::BeginPlay()
{
    Super::BeginPlay();
    RunBehaviorTree(BehaviorTree);
    
    // ⬇️ RENOMBRADO ⬇️
    ChangeEnemyState(static_cast<uint8>(ESecondEnemyStates::Wait));
    
    // ⬇️ CORREGIDO: ASecondEnemyController en vez de AEnemyController ⬇️
    AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ASecondEnemyController::OnSensed);
}

void ASecondEnemyController::OnSensed(AActor* Actor, FAIStimulus Stimulus)
{
    UE_LOG(LogTemp, Warning, TEXT("El segundo enemigo sintió algo!!!"));
    auto SenseType = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);
    
    ESecondEnemyStates CurrentState = static_cast<ESecondEnemyStates>(GetBlackboardComponent()->GetValueAsEnum("CurrentState"));
    UE_LOG(LogTemp, Warning, TEXT("El estado actual es %d"), static_cast<int>(CurrentState));
    
    if (SenseType == UAISense_Sight::StaticClass())
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            GetBlackboardComponent()->SetValueAsObject("Target", Actor);
            ChangeEnemyState(static_cast<uint8>(ESecondEnemyStates::Chase));  // ⬅️ Renombrado
        }
        else
        {
            GetBlackboardComponent()->SetValueAsVector("PointOfInterest", Stimulus.StimulusLocation);
            ChangeEnemyState(static_cast<uint8>(ESecondEnemyStates::Wait));  // ⬅️ Renombrado
        }
    }
    else if (SenseType == UAISense_Hearing::StaticClass() && CurrentState != ESecondEnemyStates::Chase)
    {
        // Comentado...
    }
    else if (SenseType == UAISense_Damage::StaticClass() && CurrentState != ESecondEnemyStates::Chase)
    {
        UE_LOG(LogTemp, Warning, TEXT("Enemigo 2: Me han dañado!!!"));
        if (Stimulus.WasSuccessfullySensed())
        {
            UE_LOG(LogTemp, Warning, TEXT("Enemigo 2: Me han dañado!!!"));
            FVector DamageDirection = Stimulus.StimulusLocation - GetPawn()->GetActorLocation();
            DamageDirection.Z = 0;
            DamageDirection.Normalize();
            
            FVector InterestLocation = GetPawn()->GetActorLocation() + DamageDirection * 800;

            GetBlackboardComponent()->SetValueAsVector("PointOfInterest", InterestLocation);
            GetBlackboardComponent()->ClearValue("CurrentState");
            ChangeEnemyState(static_cast<uint8>(ESecondEnemyStates::Wait));  // ⬅️ Renombrado
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Sentido desconocido"));
    }
}

// ⬇️ RENOMBRADO ⬇️
void ASecondEnemyController::ChangeEnemyState(uint8 State)
{
    GetBlackboardComponent()->SetValueAsEnum("CurrentState", State);
}