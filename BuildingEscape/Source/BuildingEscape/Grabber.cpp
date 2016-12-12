// (c) C J Short

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsComponent();
	SetupInputComponent();
}

/// Look for physics handle
void UGrabber::FindPhysicsComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		// Physics handle not found
		UE_LOG(LogTemp, Error, TEXT("%s is missing physics handle component."),
			*GetOwner()->GetName());
	}
}

/// Look for attached input component - only found at runtime.
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		// InputComponent  not found
		UE_LOG(LogTemp, Error, TEXT("%s is missing Input Component."),
			*GetOwner()->GetName());
	}
}

void UGrabber::Grab() {
	/// LINE TRACE and see if we reach any actors with physics bodies collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();	// get the mesh
	auto ActorHit = HitResult.GetActor();

	/// If we hit something then attach a phtsic handle
	if (ActorHit) {
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,				// no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true					// allow rotation
		);
	}
}

void UGrabber::Release() {
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
	// If the physics handle attached
	if (PhysicsHandle->GrabbedComponent)
	{
		// nove the object we are holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Raycast to reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return HitResult;
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation,
		OUT PlayerViewpointRotation
	);

	return PlayerViewpointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation,
		OUT PlayerViewpointRotation
	);

	return PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;
}

