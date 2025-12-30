// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseEnemy.h"

#include "BrainComponent.h"
#include "AI/EnemyController.h"
#include "AI/PatrolPath.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Player/ShooterController.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttackPoint = CreateDefaultSubobject<USceneComponent>("AttackPoint");
	AttackPoint->SetupAttachment(RootComponent); //Pongo el empty como hijo del comp. raíz.
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Los enemigos al spawnear pillan el PatrolPath más cercano
	if (!PatrolPath)
	{
		TArray<AActor*> Paths;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APatrolPath::StaticClass(), Paths);

		float MinDistanceSq = TNumericLimits<float>::Max();
		APatrolPath* ClosestPath = nullptr;

		for (AActor* Actor : Paths)
		{
			APatrolPath* Path = Cast<APatrolPath>(Actor);
			if (!Path) continue;

			float DistSq = FVector::DistSquared(GetActorLocation(), Path->GetActorLocation());
			if (DistSq < MinDistanceSq)
			{
				MinDistanceSq = DistSq;
				ClosestPath = Path;
			}
		}

		PatrolPath = ClosestPath;
	}
}

FVector ABaseEnemy::GetNextPatrolPoint()
{
	if (!PatrolPath)
	{
		UE_LOG(LogTemp, Error, TEXT("PatrolPath es NULL en %s"), *GetName());
		return GetActorLocation();
	}

	USplineComponent* Spline = PatrolPath->GetSplineComponent();
	if (!Spline || Spline->GetNumberOfSplinePoints() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Spline inválido o vacío"));
		return GetActorLocation();
	}

	CurrentIndex = (CurrentIndex + 1) % Spline->GetNumberOfSplinePoints();
	return Spline->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
}

void ABaseEnemy::PlayAttackMontage()
{
	PlayAnimMontage(AttackMontage);
}

void ABaseEnemy::SetIsStrafing(bool NewStrafingState)
{
	bIsStrafing = NewStrafingState;
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
			A->ActualizarKillsDesdeAquiPorqueSiNoNoFunciona();
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
		// Si el enemigo pega a otro enemigo, no ocurre nada
		if (Result.GetActor()->ActorHasTag("Player")) UGameplayStatics::ApplyDamage(Result.GetActor(), BaseDamage, GetController(), this, UDamageType::StaticClass());
	}
}