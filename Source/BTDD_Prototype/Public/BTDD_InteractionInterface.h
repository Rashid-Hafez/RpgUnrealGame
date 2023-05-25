// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ENUM_Item_Type.h"
#include "BTDD_InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBTDD_InteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BTDD_PROTOTYPE_API IBTDD_InteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	//virtual void OnInteract() =0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction Implementation")
	void InteractBlueprint();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetAsActiveInteractable(AActor* ActiveInteractable);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction Type")
	ItemType getInteractableType();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction State")
	uint8 GetCurrentInteractionState();
}; 

