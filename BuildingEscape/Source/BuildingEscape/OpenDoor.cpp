// (c) C J Short

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();


}

void UOpenDoor::OpenDoor()
{
	// Get the owner
	AActor *  Owner = GetOwner();
	FString ObjPos = GetOwner()->GetTransform().GetRotation().ToString();

	// create rotator
	FRotator NewRotation = FRotator(0.0f, 60.0f, 0.0f);

	//set the door rotation
	Owner->SetActorRotation(NewRotation);
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the trigger volume
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		// If the actorThatOpens is in the volume
		OpenDoor();
	}
}

