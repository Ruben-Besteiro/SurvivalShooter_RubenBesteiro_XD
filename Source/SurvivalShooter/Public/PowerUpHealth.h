// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUp.h"
#include "PowerUpHealth.generated.h"

UCLASS()
class SURVIVALSHOOTER_API APowerUpHealth : public APowerUp
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerUpHealth();
	
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
	int HPAmount;
};