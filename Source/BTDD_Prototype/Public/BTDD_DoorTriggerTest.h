// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "BTDD_DoorTriggerTest.generated.h"


//Dynamic means callable by blueprints AND C++
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(F_DoorSwitchOpen, bool, isOpen);

//class BoxComponent;

UCLASS()
class BTDD_PROTOTYPE_API ABTDD_DoorTriggerTest : public AActor
{
	GENERATED_BODY() //YOU NEED THIS???
	
public:	
	// Sets default values for this actor's properties
	ABTDD_DoorTriggerTest();

	UPROPERTY(BlueprintCallable)
	F_DoorSwitchOpen DoorSwitchDelegate;

	UPROPERTY(EditAnywhere)
	UBoxComponent* DoorSwitchTriggerBox;

	UFUNCTION(BlueprintCallable)
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere)
	bool isOpen;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
