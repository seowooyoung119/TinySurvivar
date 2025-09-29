#include "Player/TSPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilityManager/TSAbilityManagerSubSystem.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Player/DataAsset/PlayerInputDataAsset.h"
#include "Player/TSPlayerState.h"

ATSPlayer::ATSPlayer()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicatingMovement(true);
	
	bWantCreateInitAbilityHere = false;
	bWantSetInitAbilityOwnerActor = true;
}

void ATSPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =  ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerInputDataAsset->PlayerInputMappingContext, 0); // 우선순위 0
		}
	}
}

void ATSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(PlayerInputDataAsset->MoveAction, ETriggerEvent::Triggered, this, &ThisClass::OnMove);
		EIC->BindAction(PlayerInputDataAsset->LookAction, ETriggerEvent::Triggered, this, &ThisClass::OnLook);
		EIC->BindAction(PlayerInputDataAsset->JumpAction, ETriggerEvent::Triggered, this, &ThisClass::OnJump);
		
		EIC->BindAction(PlayerInputDataAsset->SprintAction, ETriggerEvent::Started, this, &ThisClass::OnSprint);
		EIC->BindAction(PlayerInputDataAsset->CrouchAction, ETriggerEvent::Started, this, &ThisClass::OnCrouch);
		EIC->BindAction(PlayerInputDataAsset->ProneAction, ETriggerEvent::Started, this, &ThisClass::OnProne);

		EIC->BindAction(PlayerInputDataAsset->ActiveItemAction, ETriggerEvent::Started, this, &ThisClass::OnActiveItem);
		EIC->BindAction(PlayerInputDataAsset->PickUpItemAction, ETriggerEvent::Started, this, &ThisClass::OnPickUpItem);
		
	}
}

void ATSPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ATSPlayerState* TSPlayerState = Cast<ATSPlayerState>(GetPlayerState());
	if (!TSPlayerState) return;
	
	SetAbilitySystemComp_And_AttributeSet(TSPlayerState->TSAbilitySystemComponent, TSPlayerState->TSAttributeSet);
	SetInitAbilityActorInfo(NewController);
	initializeAbilities();
}

void ATSPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	ATSPlayerState* TSPlayerState = Cast<ATSPlayerState>(GetPlayerState());
	if (!TSPlayerState) return;
	
	SetAbilitySystemComp_And_AttributeSet(TSPlayerState->TSAbilitySystemComponent, TSPlayerState->TSAttributeSet);
	SetInitAbilityActorInfo(GetController());
}

void ATSPlayer::initializeAbilities()
{
	const FGameplayTag Jump = FGameplayTag::RequestGameplayTag(FName("Character.Move.Jump"));
	UTSAbilityManagerSubSystem::GetAbilityManager(this)->GiveAbilityByTag(TSAbilitySystemComponent, Jump);

	const FGameplayTag Sprint = FGameplayTag::RequestGameplayTag(FName("Character.Move.Sprint"));
	UTSAbilityManagerSubSystem::GetAbilityManager(this)->GiveAbilityByTag(TSAbilitySystemComponent, Sprint);

	const FGameplayTag Prone = FGameplayTag::RequestGameplayTag(FName("Character.Move.Prone"));
	UTSAbilityManagerSubSystem::GetAbilityManager(this)->GiveAbilityByTag(TSAbilitySystemComponent, Prone);

	const FGameplayTag Crouch = FGameplayTag::RequestGameplayTag(FName("Character.Move.Crouch"));
	UTSAbilityManagerSubSystem::GetAbilityManager(this)->GiveAbilityByTag(TSAbilitySystemComponent, Crouch);

	const FGameplayTag ActiveItem = FGameplayTag::RequestGameplayTag(FName("Character.Interact.ActiveItem"));
	UTSAbilityManagerSubSystem::GetAbilityManager(this)->GiveAbilityByTag(TSAbilitySystemComponent, ActiveItem);

	const FGameplayTag PickUpItem = FGameplayTag::RequestGameplayTag(FName("Character.Interact.PickUpItem"));
	UTSAbilityManagerSubSystem::GetAbilityManager(this)->GiveAbilityByTag(TSAbilitySystemComponent, PickUpItem);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ATSPlayer::OnMove(const FInputActionValue& Value)
{
	const FVector2D InputAxisVector = Value.Get<FVector2D>();

	const FRotator ControllerRotation = GetControlRotation();
	const FRotator YwaRotation(0.f, ControllerRotation.Yaw, 0.f);

	const FVector ForwardVector = FRotationMatrix(YwaRotation).GetUnitAxis(EAxis::X);
	const FVector ForRightVector = FRotationMatrix(YwaRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardVector, InputAxisVector.X);
	AddMovementInput(ForRightVector, InputAxisVector.Y);
}

void ATSPlayer::OnLook(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ATSPlayer::OnJump(const FInputActionValue& Value)
{
	const FGameplayTag Jump = FGameplayTag::RequestGameplayTag(FName("Character.Move.Jump"));
	UTSAbilityManagerSubSystem::GetAbilityManager(this)->ActiveAbilityByTag(TSAbilitySystemComponent, Jump);

	// TODO : 이거는 걍 이렇게 할까 고민해보장.
	// TSAbilitySystemComponent->AbilityLocalInputPressed(0);
}

void ATSPlayer::OnSprint(const FInputActionValue& Value)
{
	const FGameplayTag Sprint = FGameplayTag::RequestGameplayTag(FName("Character.Move.Sprint"));
	UTSAbilityManagerSubSystem::GetAbilityManager(this)->ActiveAbilityByTag(TSAbilitySystemComponent, Sprint);
}

void ATSPlayer::OnProne(const FInputActionValue& Value)
{
	const FGameplayTag Prone = FGameplayTag::RequestGameplayTag(FName("Character.Move.Prone"));
	UTSAbilityManagerSubSystem::GetAbilityManager(this)->ActiveAbilityByTag(TSAbilitySystemComponent, Prone);
}

void ATSPlayer::OnCrouch(const FInputActionValue& Value)
{
	const FGameplayTag Crouch = FGameplayTag::RequestGameplayTag(FName("Character.Move.Crouch"));
	UTSAbilityManagerSubSystem::GetAbilityManager(this)->ActiveAbilityByTag(TSAbilitySystemComponent, Crouch);
}

void ATSPlayer::OnActiveItem(const FInputActionValue& Value)
{
	const FGameplayTag ActiveItem = FGameplayTag::RequestGameplayTag(FName("Character.Interact.ActiveItem"));
	UTSAbilityManagerSubSystem::GetAbilityManager(this)->ActiveAbilityByTag(TSAbilitySystemComponent, ActiveItem);
}

void ATSPlayer::OnPickUpItem(const FInputActionValue& Value)
{	
	const FGameplayTag PickUpItem = FGameplayTag::RequestGameplayTag(FName("Character.Interact.PickUpItem"));
	UTSAbilityManagerSubSystem::GetAbilityManager(this)->ActiveAbilityByTag(TSAbilitySystemComponent, PickUpItem);
}

