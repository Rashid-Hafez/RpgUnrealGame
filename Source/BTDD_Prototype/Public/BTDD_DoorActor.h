// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerBox.h"
#include "BTDD_InteractionInterface.h"
#include "BTDD_DoorActor.generated.h"

UENUM()
enum class EDoorDirection {
	DoorFront = 0 UMETA(DisplayName = "Front Door"),
	DoorBack = 1 UMETA(DisplayName = "Back Door"),
	NoInteraction = 2 UMETA(DisplayName = "Not Next To Door"),
};

UENUM(BlueprintType)
enum class EDoorState:uint8 {
	Locked UMETA(DisplayName="Locked"),
	Unlocked UMETA(DisplayName="Unlocked")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BTDD_PROTOTYPE_API ABTDD_DoorActor : public AActor, public IBTDD_InteractionInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ABTDD_DoorActor();

	virtual void InteractBlueprint_Implementation() override;
	virtual void SetAsActiveInteractable_Implementation(AActor* InteractableActor) override;
	virtual ItemType getInteractableType_Implementation() override;
	virtual uint8 GetCurrentInteractionState_Implementation() override;

	
	UFUNCTION()
	void Front_OnOverlapBegin (AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void Front_OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void Back_OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void Back_OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	bool CanInteract;

	UFUNCTION(BlueprintNativeEvent)
	void OnDoorOpenedEvent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	APawn* MyPlayer;

	ItemType Interactabletype = ItemType::Door;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	EDoorState state;

	UPROPERTY(BlueprintReadOnly)
	EDoorDirection DoorDirectionState;

	UPROPERTY(EditAnywhere)
	ATriggerBox* FrontDoorTriggerBox;

	UPROPERTY(EditAnywhere)
	ATriggerBox* BackDoorTriggerBox;

	//MeshComponents to represent Door assets
	UPROPERTY(EditAnywhere, Category="Mesh")
		UStaticMeshComponent* DoorFrame;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		UStaticMeshComponent* Door;

	UPROPERTY(EditAnywhere, Category = "Front Arrow")
		UArrowComponent* FrontDoorArrowComp;

	UPROPERTY(EditAnywhere, Category = "Back Arrow")
		UArrowComponent* BackDoorArrowComp;

	//Float Track Signature to handle our update track event
	FOnTimelineFloat UpdateFunctionFloat;

	////////////////////////////////TIMELINE DOOR STUFF/////////////////////

	//TimelineComponent to animate Door meshes
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTimelineComponent* FRONT_DoorTimelineComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTimelineComponent* BACK_DoorTimelineComp;

	// Variable to hold the Curve asset
	UPROPERTY(EditAnywhere)
	UCurveFloat* FRONT_DoorTimelineFloatCurve;
	
	UPROPERTY(EditAnywhere)
	UCurveFloat* BACK_DoorTimelineFloatCurve;

	// Update door timeline comp
	UFUNCTION()
	void UpdateTimelineComp(float output);

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
};
