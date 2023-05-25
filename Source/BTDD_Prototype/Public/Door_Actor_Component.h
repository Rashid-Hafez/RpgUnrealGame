// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Door_Actor_Component.generated.h"

class ATriggerBox;
class AActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BTDD_PROTOTYPE_API UDoor_Actor_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoor_Actor_Component();

	UPROPERTY(EditAnywhere)
	float TimeToRotate = 1.0f;

	UFUNCTION(BlueprintCallable)
	bool IsOverlapping();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FRotator DeltaRotator = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere)
	FRotator DesiredRotation = FRotator::ZeroRotator; //must assign zero to variables otherwise garbage may be assigned to the variables.
	FRotator FinalRotation = FRotator::ZeroRotator;

	FRotator StartRotation = FRotator::ZeroRotator;
	
	

	float CurrentRotationTime = 0.0f;

	UPROPERTY(EditAnywhere)
	ATriggerBox* FrontDoorTriggerBox;

	UPROPERTY(EditAnywhere)
	ATriggerBox* BackDoorTriggerBox;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
