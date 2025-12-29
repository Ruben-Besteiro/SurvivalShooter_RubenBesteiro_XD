// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

UCLASS()
class SURVIVALSHOOTER_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

	FVector GetNextPatrolPoint();
	void PlayAttackMontage();
	void SetIsStrafing(bool NewStrafingState);

	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* AttackPoint;
	
	UPROPERTY(EditInstanceOnly)
	class APatrolPath* PatrolPath;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float AttackRadius = 150;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float BaseDamage = 20;
	
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Health = 100.0f;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsStrafing;

	bool Muerto = false;

	
	int32 CurrentIndex = -1;
	

	UFUNCTION(BlueprintCallable)
	void OnAttackHit();
	
	void DestroyDelayed();
};
