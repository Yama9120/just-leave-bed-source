// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);
	ViewCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

	// キャラクターがしゃがむことができるように設定
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 140.f;
	// しゃがんでいるときに段差から落ちることができるように設定
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;

	// 親のメッシュを非表示にして、アニメーションは常に更新するように設定（親のアニメーションを子に引き継がせるため）
	GetMesh()->SetVisibility(false);
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	// 子のメッシュを作成して、親のアニメーションを引き継ぐように設定
	VisualMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VisualMesh"));
	VisualMesh->SetupAttachment(GetMesh());
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetRotationMode(RotationMode);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerContext, 0);
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// すでにアクション中の場合は移動できない
	if (IsOccupied())
	{
		return;
	}

	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void APlayerCharacter::Dodge()
{
	if (!CanDodge())
	{
		return;
	}

	const float MontageLength = PlayAnimMontage(DodgeMontage);
	if (MontageLength <= 0.f)
	{
		return;
	}

	ActionState = EActionState::EAS_Dodge;

	// しゃがんでいる場合は立ち上がる
	if (bIsCrouched)
	{
		UnCrouch();
	}

	// ドッジアニメーションが終了したときに呼ばれるデリゲートを設定
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	FOnMontageEnded EndDelegate;

	// delegateにOnDodgeMontageEnded関数をバインド
	EndDelegate.BindUObject(this, &APlayerCharacter::OnDodgeMontageEnded);
	// ドッジアニメーションが終了したときにOnDodgeMontageEndedが呼ばれるように設定
	AnimInstance->Montage_SetEndDelegate(EndDelegate, DodgeMontage);
}

bool APlayerCharacter::CanDodge() const
{
	// すでにアクション中の場合はドッジできない
	if (IsOccupied())
	{
		return false;
	}

	// ドッジアニメーションが設定されていない場合はドッジできない
	if (!DodgeMontage)
	{
		return false;
	}

	// 空中ではドッジできない
	if (GetCharacterMovement()->IsFalling())
	{
		return false;
	}

	return true;
}

bool APlayerCharacter::IsOccupied() const
{
	return ActionState != EActionState::EAS_Unoccupied;
}

void APlayerCharacter::OnDodgeMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != DodgeMontage)
	{
		return;
	}
	else
	{
		FinishDodge();
	}
}

void APlayerCharacter::FinishDodge()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void APlayerCharacter::Jump()
{
	// すでにアクション中の場合はジャンプできない
	if (IsOccupied())
	{
		return;
	}

	if (bIsCrouched)
	{
		UnCrouch();
	}

	Super::Jump();
}

void APlayerCharacter::ToggleCrouch()
{
	// 空中ではしゃがめない
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	// しゃがみ状態なら立ち上がる、立っている状態ならしゃがむ
	if (bIsCrouched)
	{
		UnCrouch();

		// 立ち上がるときはしゃがむ前の回転モードに戻す
		SetRotationMode(PreviousRotationMode);
	}
	else
	{
		// しゃがむ前の回転モードを保存
		PreviousRotationMode = RotationMode;

		Crouch();

		// しゃがむときはキャラクターの回転モードをOrientに設定する
		SetRotationMode(ERotationMode::Orient);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Strafeモードのみ（移動中：キャラを回転させる、停止中：キャラは回転させない）
	const FVector HorizontalVelocity = FVector(GetVelocity().X, GetVelocity().Y, 0.f);
	if (RotationMode == ERotationMode::Strafe && Controller && !HorizontalVelocity.IsNearlyZero()) // 移動中のみ処理を行う
	{
		// キャラクターの回転をコントローラーの向きに合わせる
		const FRotator Target(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FRotator Smoothed = FMath::RInterpTo(GetActorRotation(), Target, DeltaTime, RotationInterpSpeed); // 回転を滑らかにするために補間
		SetActorRotation(Smoothed);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::ToggleCrouch);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &APlayerCharacter::Dodge);
	}
}

void APlayerCharacter::SetRotationMode(ERotationMode NewRotationMode)
{
	RotationMode = NewRotationMode;
	// キャラクターの回転モードを設定する（OrientならTrueに）
	GetCharacterMovement()->bOrientRotationToMovement = (RotationMode == ERotationMode::Orient);
}
