// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanitysFall_Debug.h"
#include "RealBullet.h"
#include "PhysicsEngine/DestructibleActor.h"


// Sets default values
ARealBullet::ARealBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize synch component to spawn bullets.
	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = rootComp;
}

// Called when the game starts or when spawned
void ARealBullet::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARealBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult hitResult;

	FVector startTrace = this->GetActorLocation();
	FVector endTrace = (velocity * DeltaTime) + startTrace;
	endTrace.Z += this->GetActorRotation().Pitch;		// If we're looking up then the bullet will go upwards.

	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);				// Line trace must ignore the player.

	if (GetWorld()->LineTraceSingleByChannel(hitResult, startTrace, endTrace, ECC_Destructible, collisionParams))
	{
		if (hitResult.GetActor())
		{
			// Draw a blue box on whatever the bullet hits.
			DrawDebugSolidBox(GetWorld(), hitResult.ImpactPoint, FVector(10.f), FColor::Blue, true);
			ADestructibleActor* mesh = Cast<ADestructibleActor>(hitResult.GetActor());

			if (mesh)
			{
				mesh->GetDestructibleComponent()->ApplyRadiusDamage(10.f, hitResult.ImpactPoint, 32.f, 10.f, false);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Could not get the mesh. Type is: %s"), *hitResult.GetActor()->StaticClass()->GetFName().ToString()));
			}
		}

		// Destroy bullets after not hitting anything (improves performance).
		Destroy();
	}
	else
	{
		bulletExpiry += DeltaTime;

		// Make line trace color darker the longer it goes.
		DrawDebugLine(GetWorld(), startTrace, endTrace, FColor(0.f, -bulletExpiry * 80.f, 100.f), true);

		SetActorLocation(endTrace);

		// Increment velocity on Z-axis.
		velocity += FVector(0.f, 0.f, -200.f) * DeltaTime;
	}

	if (bulletExpiry > 3)
	{
		// After the bullet flies for 3 seconds, destroy it.
		Destroy();
	}


}

