// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableObject.h"
#include "../Public/HorrorCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void ACollectableObject::BeginPlay()
{
	Super::BeginPlay();

	OriginTransform = GetActorTransform();
	PC = GetWorld()->GetFirstPlayerController();
	Player = Cast<AHorrorCharacter>(PC->GetCharacter());
}

void ACollectableObject::Interact()
{
	Inspect();
}

void ACollectableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PC->WasInputKeyJustPressed(EKeys::E))
		DropDown();
	else
		RotateMesh();
}

void ACollectableObject::Inspect()
{
	if (Player && Player->IsValidLowLevelFast())
	{
		Focus(false);
		WidgetComponent->SetVisibility(false);

		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
		AttachToComponent(Player->InspectingLocation, Rules);

		RelativeScale = GetActorRelativeScale3D();

		StaticMeshComponent->SetSimulatePhysics(false);
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Player->DisableInput(PC);
		this->EnableInput(PC);
		FTimerHandle WaitTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(WaitTimerHandle, FTimerDelegate::CreateLambda([&]() {
			SetActorTickEnabled(true);
			}), 0.5f, false);
	}
}

void ACollectableObject::DropDown()
{
	SetActorTickEnabled(false);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	this->DisableInput(PC);
	Player->EnableInput(PC);
	FDetachmentTransformRules Rules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
	DetachFromActor(Rules);
	WidgetComponent->SetVisibility(true);
	SetActorTransform(OriginTransform);
	Focus(true);
}

void ACollectableObject::RotateMesh()
{
	if (PC->IsInputKeyDown(EKeys::LeftMouseButton))
	{
		float MouseX, MouseY;
		PC->GetInputMouseDelta(MouseX, MouseY);

		SetActorRotation(UKismetMathLibrary::ComposeRotators(GetActorRotation(), FRotator(0, MouseX * -2.0f, 0)));
		SetActorRotation(UKismetMathLibrary::ComposeRotators(GetActorRotation(), FRotator(0, 0, MouseY * 2.0f)));
	}

	if (PC->WasInputKeyJustPressed(EKeys::MouseScrollDown))
	{
		if (ZoomLevel > 0)
		{
			ZoomLevel -= 1;
			SetActorScale3D(RelativeScale + (ZoomLevel * ZoomSize));
		}
	}

	if (PC->WasInputKeyJustPressed(EKeys::MouseScrollUp))
	{
		if (ZoomLevel < 10)
		{
			ZoomLevel += 1;
			SetActorScale3D(RelativeScale + (ZoomLevel * ZoomSize));
		}
	}
}
