// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDD_DoorTriggerTest.h"

// Sets default values
ABTDD_DoorTriggerTest::ABTDD_DoorTriggerTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DoorSwitchTriggerBox = CreateDefaultSubobject<UBoxComponent>("Door Switch Trigger");
	DoorSwitchTriggerBox->SetupAttachment(RootComponent);

	//RootComponent = DoorSwitchTriggerBox;

}

// Called when the game starts or when spawned
void ABTDD_DoorTriggerTest::BeginPlay()
{
	Super::BeginPlay();
	isOpen = false;
	
}

// Called every frame
void ABTDD_DoorTriggerTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABTDD_DoorTriggerTest::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	isOpen = !isOpen;
	DoorSwitchDelegate.Broadcast(isOpen);
}