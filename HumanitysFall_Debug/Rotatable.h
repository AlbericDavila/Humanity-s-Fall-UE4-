// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Rotatable.generated.h"

UCLASS()
class HUMANITYSFALL_DEBUG_API ARotatable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotatable();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Axis to rotate mesh on
	UPROPERTY(EditAnywhere)
	FString RotationAxis = "Yaw";

	// The static mesh for the rotatable.
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* rotatableMesh;
	
	// Box trigger for the rotatable.
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* animTrigger;

	// The max spin that the rotatable will reach
	UPROPERTY(EditAnywhere, Category = "Animation")
	float maxRotation;

	// Curve float Reference
	UPROPERTY(EditAnywhere, Category = "Animation")
	UCurveFloat* animCurve;

private:

	// Initial Rotation
	FRotator actorInitialRotation;

	// Target rotation
	FRotator targetRotation;

	// Timeline to make rotatable spin.
	FTimeline myTimeline;

	UFUNCTION()
	void HandleProgress(float value);

	UFUNCTION()
	void OnAnimTriggered(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
