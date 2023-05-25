// Fill out your copyright notice in the Description page of Project Settings.

/*
	Character walks into trigger box
	Trigger box sends signal to character allowing to open door
	character presses button
	door rotates (call the tick function)
*/


#include "Door_Actor_Component.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerBox.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

// Sets default values for this component's propertiesss
UDoor_Actor_Component::UDoor_Actor_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}



// Called when the game starts
void UDoor_Actor_Component::BeginPlay()
{
	Super::BeginPlay();

	PrimaryComponentTick.bCanEverTick = false;

	CurrentRotationTime = 0.0f;

	StartRotation = GetOwner()->GetActorRotation();
	DesiredRotation = FRotator(0, 90, 0);

//	FrontDoorTriggerBox->OnActorBeginOverlap.AddDynamic(this,&UDoor_Actor_Component::OnFrontDoorOverlap);
	//on actor end overlap
}




bool UDoor_Actor_Component::IsOverlapping() {



	if (GetWorld() && GetWorld()->GetFirstPlayerController()->GetPawn() && FrontDoorTriggerBox && BackDoorTriggerBox) {
		APawn* MyPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();
		if (MyPlayer && FrontDoorTriggerBox->IsOverlappingActor(MyPlayer))
		{
			return true;
		}
	}

	return false;
}

// Called every frame
void UDoor_Actor_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentRotationTime < TimeToRotate) //while we havent finished our rotation time (set at 1 second) do as follows:
	{ 

		if (GetWorld() && GetWorld()->GetFirstPlayerController()->GetPawn() && FrontDoorTriggerBox && BackDoorTriggerBox)
		{
			APawn* MyPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();

			if (MyPlayer && FrontDoorTriggerBox->IsOverlappingActor(MyPlayer))
			{
				//Set the rotation to the tick time, then divide the current tick time by the time to rotate this will give us the time left to rotate the door.
				CurrentRotationTime += DeltaTime;
				/* RATIO OF ROTATION e.g. if our length of animation is 1 second, and our CPU clock speed is half a second. It will get the Half ratio
				and Set the rotation to be half way, then in the next clock cycle it will be set the full way */ 
				const float RotationAlpha = FMath::Clamp(CurrentRotationTime / TimeToRotate, 0.0f, 1.0f);
				const FRotator CurrentRotation = FMath::Lerp(StartRotation, DesiredRotation, RotationAlpha);
				GetOwner()->SetActorRotation(CurrentRotation);
			}
		}
		
	}

	// ...
}

