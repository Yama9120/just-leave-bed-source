// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
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

	// カメララグを有効にして、カメラがなめらかに追従されるように設定
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 15.f;

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

	PreviousActorZ = GetActorLocation().Z;
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// すでにアクション中の場合、またはコントローラーが存在しない場合は移動処理を行わない
	if (IsOccupied() || !Controller)
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

	// ドッジの方向にキャラクターを向ける
	const FVector LastInput = GetCharacterMovement()->GetLastInputVector();
	if (!LastInput.IsNearlyZero())
	{
		const FRotator Target(0.f, LastInput.Rotation().Yaw, 0.f);
		SetActorRotation(Target);
	}

	const float MontageLength = PlayAnimMontage(DodgeMontage);
	if (MontageLength <= 0.f)
	{
		return;
	}

	ActionState = EActionState::EAS_Dodge;

	// ドッジ中はキャラクターの回転モードをOrientに設定する
	// ※ TODO：現在の実装方法では、しゃがみ状態からドッジするときに、Strafeが消滅する。Strafeを使わない or ターゲット処理を実装
	PreviousRotationMode = RotationMode;
	SetRotationMode(ERotationMode::Orient);

	// ドッジ中はルートモーションの移動量にDodgeDistanceScaleを適用
	SetAnimRootMotionTranslationScale(DodgeDistanceScale);

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
	AnimInstance->Montage_SetBlendingOutDelegate(EndDelegate, DodgeMontage);
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

	FinishDodge();
}

void APlayerCharacter::FinishDodge()
{
	ActionState = EActionState::EAS_Unoccupied;
	SetRotationMode(PreviousRotationMode);

	// ドッジ中のルートモーションの移動量（DodgeDistanceScale）を等倍に戻す
	SetAnimRootMotionTranslationScale(1.f);
}

void APlayerCharacter::OnStartCrouch(float HeightAdjust, float ScaledHeightAdjust)
{
	Super::OnStartCrouch(HeightAdjust, ScaledHeightAdjust);

	// しゃがみ初めに急にカメラが下がるのを防ぐため、HeightAdjust分だけカメラのZ座標を上げる
	SpringArm->TargetOffset.Z += HeightAdjust;

	// オフセット分だけカメラのZ座標を下げる
	TargetCameraZ = -CrouchCameraZOffset;

	// カプセルサイズ変更によるZ移動を段差スムージングの対象から除外
	PreviousActorZ = GetActorLocation().Z;
	// Super側でメッシュ相対Zが絶対値で再設定されるため、オフセットもリセットして整合を取る
	MeshStepZOffset = 0.f;
}

void APlayerCharacter::OnEndCrouch(float HeightAdjust, float ScaledHeightAdjust)
{
	Super::OnEndCrouch(HeightAdjust, ScaledHeightAdjust);

	// しゃがみ終了時に急にカメラが上がるのを防ぐため、HeightAdjust分だけカメラのZ座標を下げる
	SpringArm->TargetOffset.Z -= HeightAdjust;

	// カメラのZ座標を元に戻す
	TargetCameraZ = 0.f;

	// カプセルサイズ変更によるZ移動を段差スムージングの対象から除外
	PreviousActorZ = GetActorLocation().Z;
	// Super側でメッシュ相対Zが絶対値で再設定されるため、オフセットもリセットして整合を取る
	MeshStepZOffset = 0.f;
}

void APlayerCharacter::UpdateMeshStepSmoothing(float DeltaTime)
{
	// 前フレームからのカプセルのZ座標の変化量を計算する
	const float CurrentActorZ = GetActorLocation().Z;
	const float DeltaZ = CurrentActorZ - PreviousActorZ;
	PreviousActorZ = CurrentActorZ;

	// 「通常の移動」として説明できる1フレームあたりのZ変化量の上限
	const float MaxExplainableDelta = MaxSmoothVerticalSpeed * DeltaTime;

	// 上限を超えた分（例：段差乗り越えによる瞬間移動の成分）だけを取り出す
	const float SnapDelta = DeltaZ - FMath::Clamp(DeltaZ, -MaxExplainableDelta, MaxExplainableDelta);

	const float PreviousOffset = MeshStepZOffset;

	// 段差の影響でメッシュが上下する量を滑らかに補間する
	MeshStepZOffset = FMath::FInterpTo(MeshStepZOffset - SnapDelta, 0.f, DeltaTime, MeshStepInterpSpeed);

	// メッシュのZ座標を補間後の段差の影響量に応じて上下させる
	GetMesh()->AddRelativeLocation(FVector(0.f, 0.f, MeshStepZOffset - PreviousOffset));
}

void APlayerCharacter::Jump()
{
	// すでにアクション中の場合はジャンプできない
	if (IsOccupied())
	{
		return;
	}

	float CurrentTime = GetWorld()->GetTimeSeconds();
	// 以前のジャンプからクールタイム
	if (CurrentTime - LastJumpTime < JumpCoolTime)
	{
		return;
	}

	if (bIsCrouched)
	{
		UnCrouch();
	}

	Super::Jump();

	// ジャンプ時の時間を保持
	LastJumpTime = CurrentTime;
}

void APlayerCharacter::ToggleCrouch()
{
	// 空中ではしゃがめない or すでにアクション中の場合はしゃがめない
	if (GetCharacterMovement()->IsFalling() || IsOccupied())
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

	// しゃがみ時のカメラのZ座標オフセットを滑らかに補間する
	const FVector CurrentCameraLocation = SpringArm->TargetOffset;
	const FVector TargetCameraLocation = FVector(CurrentCameraLocation.X, CurrentCameraLocation.Y, TargetCameraZ);
	const FVector SmoothedCameraLocation = FMath::VInterpTo(CurrentCameraLocation, TargetCameraLocation, DeltaTime, CameraInterpSpeed);
	SpringArm->TargetOffset = SmoothedCameraLocation;

	UpdateMeshStepSmoothing(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
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
