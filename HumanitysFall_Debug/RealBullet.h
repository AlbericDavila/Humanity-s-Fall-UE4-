// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PhysicsEngine/DestructibleActor.h"
#include "RealBullet.generated.h"

UCLASS()
class HUMANITYSFALL_DEBUG_API ARealBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARealBullet();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	USceneComponent* rootComp;

	UPROPERTY(EditAnywhere)
	FVector velocity = FVector(100.f);
	
	// Bullet ios deleted after some time of not hitting anything.
	float bulletExpiry = 0;
	
};
