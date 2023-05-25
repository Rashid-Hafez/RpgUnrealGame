/*/
	Character walks into trigger box
	Trigger box sends signal to character allowing to open door
	character presses button
	door rotates (call the tick function)
/*/


//cast otheractor to interface
//character inherits interface (has to be C++)
// char Bluprint calls this class and enables tick to true, so now we can rotate door.
//pass through a reference of THIS door through the interface function to the character blueprint


#include "BTDD_DoorActor.h"
#include "Engine/World.h"

const FTransform;

// Sets default values
ABTDD_DoorActor::ABTDD_DoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CanInteract = false;

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>("DoorFrame");
	Door = CreateDefaultSubobject<UStaticMeshComponent>("DoorMain");
	DoorFrame->SetupAttachment(RootComponent);
	Door->SetupAttachment(DoorFrame);
	FrontDoorArrowComp = CreateDefaultSubobject<UArrowComponent>("Front Door Arrow");
	FrontDoorArrowComp->SetupAttachment(Door);
	BackDoorArrowComp = CreateDefaultSubobject<UArrowComponent>("Back Door Arrow");
	BackDoorArrowComp->SetupAttachment(Door);

	FRONT_DoorTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("Front DoorTimelineComp"));
	BACK_DoorTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("Back DoorTimelineComp"));
	
	FrontDoorTriggerBox = nullptr;
	BackDoorTriggerBox = nullptr;

	state = EDoorState::Locked;

	UpdateFunctionFloat.BindDynamic(this,&ABTDD_DoorActor::UpdateTimelineComp);
}

// Called when the game starts or when spawned
void ABTDD_DoorActor::BeginPlay()
{
	Super::BeginPlay();

	
	//When trigger box is overlapped, send out delegate to call function
	if(IsValid(FrontDoorTriggerBox) && IsValid(BackDoorTriggerBox))
	{
		FrontDoorTriggerBox->OnActorBeginOverlap.AddDynamic(this, &ABTDD_DoorActor::Front_OnOverlapBegin);
		FrontDoorTriggerBox->OnActorEndOverlap.AddDynamic(this, &ABTDD_DoorActor::Front_OnOverlapEnd);

		BackDoorTriggerBox->OnActorBeginOverlap.AddDynamic(this, &ABTDD_DoorActor::Back_OnOverlapBegin);
		BackDoorTriggerBox->OnActorEndOverlap.AddDynamic(this, &ABTDD_DoorActor::Back_OnOverlapEnd);
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("No Triggerbox assigned to door")));
	}


	if (FRONT_DoorTimelineFloatCurve && BACK_DoorTimelineComp) 
	{
		FRONT_DoorTimelineComp->AddInterpFloat(FRONT_DoorTimelineFloatCurve, UpdateFunctionFloat);
		BACK_DoorTimelineComp->AddInterpFloat(BACK_DoorTimelineFloatCurve, UpdateFunctionFloat);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("No Float Curve assigned to door")));
	}

	if (FrontDoorArrowComp && BackDoorArrowComp) {

	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("MUST assign arrow component")));
	}
	
}

//Must put this in every interactive actor, and also put inside player. this function passes THIS class to the character
//So the character can know which specific actor it will interact with.
void ABTDD_DoorActor::SetAsActiveInteractable_Implementation(AActor* InteractableActor){}


void ABTDD_DoorActor::Front_OnOverlapBegin( AActor* OverlappedActor, AActor* OtherActor) 
{
	//if overlapping character is the main player
	MyPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (MyPlayer->Implements<UBTDD_InteractionInterface>())
	{

		if (FrontDoorTriggerBox->IsOverlappingActor(MyPlayer))
		{
			CanInteract = true;
			DoorDirectionState = EDoorDirection::DoorFront; //set current state of the door interaction process
			//execute function inside an object which inherits the interface, by passing the object in memory (I.E. MyPlayer)
			Execute_SetAsActiveInteractable(MyPlayer, this);
		}
	}

	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::Printf(TEXT("You Can't Interact With This Object")));
	}
	
}

void ABTDD_DoorActor::Front_OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	CanInteract = false;
	DoorDirectionState = EDoorDirection::NoInteraction;
	Execute_SetAsActiveInteractable(MyPlayer, nullptr);
}

//Behind the door
void ABTDD_DoorActor::Back_OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor) 
{
	MyPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (MyPlayer->Implements<UBTDD_InteractionInterface>())
	{
		if (BackDoorTriggerBox->IsOverlappingActor(MyPlayer)) 
		{
			CanInteract = true;
			DoorDirectionState = EDoorDirection::DoorBack;
			Execute_SetAsActiveInteractable(MyPlayer, this);
		}
	}

	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::Printf(TEXT("You Can't Interact With This Object")));
	}
	
}

void ABTDD_DoorActor::Back_OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor) 
{
	CanInteract = false;
	DoorDirectionState = EDoorDirection::NoInteraction;
	Execute_SetAsActiveInteractable(MyPlayer, nullptr);
}

 void ABTDD_DoorActor::InteractBlueprint_Implementation()
 {
	 if (CanInteract)
	 {
		 if (state == EDoorState::Locked)
		 {
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::Printf(TEXT("Door Is Locked")));
		 }

		 else  
		 {
			 if (DoorDirectionState == EDoorDirection::DoorFront)
			 {
				 if (FRONT_DoorTimelineComp->IsPlaying())
				 {
					 GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::Printf(TEXT("Already Interacting")));
				 }
				 else
				 {
					 //Snap player to the front of the door handle for nice animation
					 MyPlayer->SetActorRelativeTransform(FrontDoorArrowComp->GetComponentTransform());
					 FRONT_DoorTimelineComp->PlayFromStart();
					 OnDoorOpenedEvent();
				 }
			 }
			 else if (DoorDirectionState == EDoorDirection::DoorBack)
			 {
				 if (BACK_DoorTimelineComp->IsPlaying())
				 {
					 GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::Printf(TEXT("Already Interacting")));
				 }
				 else
				 {
					 //Snap player to the back of the door handle for nice animation
					 MyPlayer->SetActorRelativeTransform(BackDoorArrowComp->GetComponentTransform());
					 BACK_DoorTimelineComp->PlayFromStart();
					 OnDoorOpenedEvent();
				 }
			 }
		 }

	 }	
 }

 void ABTDD_DoorActor::UpdateTimelineComp(float output) 
 {
	 // Create and set our Door's new relative location based on the output from our Timeline Curve
	 FRotator DoorNewRotation = FRotator(0.0f, output, 0.0f);
	 Door->SetRelativeRotation(DoorNewRotation);
 }

 ItemType ABTDD_DoorActor::getInteractableType_Implementation()
 {
	 return Interactabletype;
 }

 uint8 ABTDD_DoorActor::GetCurrentInteractionState_Implementation() 
 {
	 return (uint8)DoorDirectionState;
 }

 void ABTDD_DoorActor::OnDoorOpenedEvent_Implementation(){}

// Called every frame ///// DEPRECATED
//void ABTDD_DoorActor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	if (CanInteract) {
//		if (CurrentRotationTime < TimeToRotate) //while we havent finished our rotation time (set at 1 second) do as follows:
//		{
//
//			//Set the rotation to the tick time, then divide the current tick time by the time to rotate this will give us the time left to rotate the door.
//			CurrentRotationTime += DeltaTime;
//			/* RATIO OF ROTATION e.g. if our length of animation is 1 second, and our CPU clock speed is half a second. It will get the Half ratio
//			and Set the rotation to be half way, then in the next clock cycle it will be set the full way */
//			const float RotationAlpha = FMath::Clamp(CurrentRotationTime / TimeToRotate, 0.0f, 1.0f);
//			const FRotator CurrentRotation = FMath::Lerp(StartRotation, DesiredRotation, RotationAlpha);
//			Door->SetRelativeRotation(CurrentRotation);
//
//		}
//	}
//}

