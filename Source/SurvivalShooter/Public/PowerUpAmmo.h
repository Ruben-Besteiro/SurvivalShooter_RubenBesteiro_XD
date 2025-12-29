// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUp.h"
#include "PowerUpAmmo.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALSHOOTER_API APowerUpAmmo : public APowerUp
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerUpAmmo();
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere);
	UStaticMeshComponent* MyMesh;

	UPROPERTY(EditAnywhere)
	int AmmoAmount;
};
