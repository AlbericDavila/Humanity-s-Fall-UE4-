// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanitysFall_Debug.h"
#include "Rotatable.h"


// Sets default values
ARotatable::ARotatable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	rotatableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotatableMesh"));

	animTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("AnimationTrigger"));
	animTrigger->bGenerateOverlapEvents = true;
	animTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARotatable::OnAnimTriggered);
	animTrigger->AttachToComponent(rotatableMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// Called when the game starts or when spawned
void ARotatable::BeginPlay()
{
	Super::BeginPlay();
	
	if (animCurve)
	{
		// Monitor the time it takes for the function to complete.
		FOnTimelineFloat progressFunction;

		progressFunction.BindUFunction(this, FName("HandleProgress"));

		myTimeline.AddInterpFloat(animCurve, progressFunction);
		myTimeline.SetLooping(false);		// If true, it will loop the animation continuously.

		actorInitialRotation = targetRotation = GetActorRotation();

		// Check for with axis we want to rotate.
		// This can be changed in the editor.
		if (RotationAxis == "Yaw")
		{
			targetRotation.Yaw += maxRotation;
		}
		else if (RotationAxis == "Pitch")
		{
			targetRotation.Pitch += maxRotation;
		}
		else if (RotationAxis == "Roll")
		{
			targetRotation.Roll += maxRotation;
		}
	}
}

// Called every frame
void ARotatable::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	myTimeline.TickTimeline(DeltaTime);

}

void ARotatable::HandleProgress(float value)
{
	FRotator newRotation = FMath::Lerp(actorInitialRotation, targetRotation, value);
	SetActorRotation(newRotation);
}

void ARotatable::OnAnimTriggered(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (!myTimeline.IsPlaying())
	{
		myTimeline.PlayFromStart();
	}
}

