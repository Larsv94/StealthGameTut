// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"


// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HoleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HoleMesh"));
	HoleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = HoleMesh;

	EffectRadius = CreateDefaultSubobject<USphereComponent>(TEXT("EffectRadius"));
	EffectRadius->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EffectRadius->SetupAttachment(HoleMesh);

	DestroyRadius = CreateDefaultSubobject<USphereComponent>(TEXT("DestroyRadius"));
	DestroyRadius->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DestroyRadius->SetupAttachment(HoleMesh);

	DestroyRadius->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverlapDestroyRadius);
}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();

}

void AFPSBlackHole::OverlapDestroyRadius(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TArray<UPrimitiveComponent*> OverlappingComponents;
	EffectRadius->GetOverlappingComponents(OverlappingComponents);
	for (int32 i = 0; i < OverlappingComponents.Num(); i++)
	{
		UPrimitiveComponent* OverlapComponent = OverlappingComponents[i];
		if (OverlapComponent && OverlapComponent->IsSimulatingPhysics())
		{
			OverlapComponent->AddRadialForce(GetActorLocation(), EffectRadius->GetScaledSphereRadius(), AttractionForce*-1, ERadialImpulseFalloff::RIF_Constant,true);
		}
	}

}

