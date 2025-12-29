// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseEnemy.h"

#include "BrainComponent.h"
#include "AI/EnemyController.h"
#include "AI/PatrolPath.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/ShooterController.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttackPoint = CreateDefaultSubobject<USceneComponent>("AttackPoint");
	AttackPoint->SetupAttachment(RootComponent); //Pongo el empty como hijo del comp. raíz.

}

FVector ABaseEnemy::GetNextPatrolPoint()
{
	CurrentIndex = (CurrentIndex + 1) % PatrolPath->GetSplineComponent()->GetNumberOfSplinePoints();
	
	return PatrolPath->GetSplineComponent()->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
}

void ABaseEnemy::PlayAttackMontage()
{
	PlayAnimMontage(AttackMontage);
}

void ABaseEnemy::SetIsStrafing(bool NewStrafingState)
{
	bIsStrafing = NewStrafingState;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABaseEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health -= DamageAmount;
	
	if (Health <= 0)
	{
		if (!Muerto) Muerto = true;
		else return 0;
		GetMesh()->SetSimulatePhysics(true); //Ahora tengo físicas
		GetMesh()->SetCollisionProfileName("Ragdoll"); //Me pongo en modo ragdoll
		if (auto MyController = Cast<AEnemyController>(GetController()))
		{
			MyController->GetBrainComponent()->StopLogic("Death");
			SetLifeSpan(3.0f);
			AShooterController* A = Cast<AShooterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			A->ActualizarKillsDesdeFueraPorqueSiNoNoFunciona();
		}
	}
	//Puedo retornar ncualquier tipo de float para dar información del golpe al causante del daño.
	return Health;
}

//Se ejecuta bajo un AnimNotify.
void ABaseEnemy::OnAttackHit()
{
	TArray<FOverlapResult> Results;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AttackRadius);
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	DrawDebugSphere(GetWorld(), AttackPoint->GetComponentLocation(), AttackRadius, 16, FColor::Red, false, 2.0f);
	GetWorld()->OverlapMultiByChannel(Results, AttackPoint->GetComponentLocation(), FQuat::Identity, ECC_GameTraceChannel1, CollisionShape, CollisionParams);

	for (FOverlapResult Result : Results)
	{
		UGameplayStatics::ApplyDamage(Result.GetActor(), BaseDamage, GetController(), this, UDamageType::StaticClass());
	}
}