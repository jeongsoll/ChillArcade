// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnableShield.h"


// Sets default values
ASpawnableShield::ASpawnableShield()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMeshObject
		(TEXT("/Game/Player/Item/Model/sm_Shield.sm_Shield"));
	if (BaseMeshObject.Succeeded()) {
		Mesh->SetStaticMesh(BaseMeshObject.Object);
	}
}

// Called when the game starts or when spawned
void ASpawnableShield::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnableShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

