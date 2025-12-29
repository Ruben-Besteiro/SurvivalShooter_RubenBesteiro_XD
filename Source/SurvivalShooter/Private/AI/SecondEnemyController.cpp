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
    
    AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ASecondEnemyController::OnSensed);
    ChangeEnemyState(static_cast<uint8>(ESecondEnemyStates::Wait));
}


void ASecondEnemyController::OnSensed(AActor* Actor, FAIStimulus Stimulus)
{
    auto SenseType = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);
    
    ESecondEnemyStates CurrentState = static_cast<ESecondEnemyStates>(GetBlackboardComponent()->GetValueAsEnum("CurrentState"));
    UE_LOG(LogTemp, Warning, TEXT("El estado actual es %d"), static_cast<int>(CurrentState));
    
    if (SenseType == UAISense_Sight::StaticClass())
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            GetBlackboardComponent()->SetValueAsObject("Target", Actor);
            ChangeEnemyState(static_cast<uint8>(ESecondEnemyStates::Chase));
        }
        else
        {
            /*GetBlackboardComponent()->SetValueAsVector("PointOfInterest", Stimulus.StimulusLocation);
            ChangeEnemyState(static_cast<uint8>(ESecondEnemyStates::Wait));*/
        }
    }
    else if (SenseType == UAISense_Hearing::StaticClass())
    {
        // Comentado...
    }
    else if (SenseType == UAISense_Damage::StaticClass())
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            GetBlackboardComponent()->SetValueAsObject("Target", Actor);
            ChangeEnemyState(static_cast<uint8>(ESecondEnemyStates::Chase));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Sentido desconocido"));
    }
}

void ASecondEnemyController::ChangeEnemyState(uint8 State)
{
    GetBlackboardComponent()->SetValueAsEnum("CurrentState", State);
    UE_LOG(LogTemp, Warning, TEXT("Estado cambiado a %d"), State);
}