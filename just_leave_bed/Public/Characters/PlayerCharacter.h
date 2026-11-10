// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class UResultReceiver;
class UNiagaraSystem;

UENUM(BlueprintType)
enum class ERotationMode : uint8
{
	Orient UMETA(DisplayName = "Orient"),
	Strafe UMETA(DisplayName = "Strafe"),
};

UCLASS()
class JUST_LEAVE_BED_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	void ToggleCrouch();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetRotationMode(ERotationMode NewRotationMode);

	void ResetSmoothingState();

	// エフェクトに関する関数
	UResultReceiver* GetResultReceiver() const { return ResultReceiver; }
	void PlayVanishEffect(); // ミス時にキャラを消す（エフェクトは設定されているときだけ出す）
	void PlayAppearEffect(); // ミスからの復帰時にキャラを表示に戻す（エフェクトは設定されているときだけ出す）

	void PlayDeathMontage();
	void StopDeathMontage();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MovementAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DodgeAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<USkeletalMeshComponent> VisualMesh;

	// ----------------------------------------------------------------
	// 回転モードの設定（Orient: 移動方向に向く、Strafe: 視点方向に向く）
	// ----------------------------------------------------------------

	UPROPERTY(EditAnywhere, Category = "Movement")
	ERotationMode RotationMode = ERotationMode::Orient;

	ERotationMode PreviousRotationMode = ERotationMode::Orient;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.0"))
	float RotationInterpSpeed = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (ClampMin = "0.1"))
	float DodgeDistanceScale = 2.f;

	// 段差でキャラクターが上下する際のメッシュの補間処理に使用する変数
	UPROPERTY(EditAnywhere, Category = "StepSmooth")
	float MeshStepInterpSpeed = 13.f;
	UPROPERTY(EditAnywhere, Category = "StepSmooth")
	float MaxMeshStepOffset = 50.f;
	UPROPERTY(EditAnywhere, Category = "StepSmooth")
	float MaxSmoothVerticalSpeed = 350.f;

	// ギミックの結果を受け取り、演出の再生と通知を行う部品
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UResultReceiver> ResultReceiver;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UNiagaraSystem> VanishEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UNiagaraSystem> AppearEffect;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Dodge();

	bool CanDodge() const;
	bool IsOccupied() const;
	void OnDodgeMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void FinishDodge();

	// しゃがみ時のカメラのZ座標オフセットを設定する関数
	virtual void OnStartCrouch(float HeightAdjust, float ScaledHeightAdjust) override;
	virtual void OnEndCrouch(float HeightAdjust, float ScaledHeightAdjust) override;

private:
	void UpdateMeshStepSmoothing(float DeltaTime);
	void BindRetrySubsystem();
	void SetCharacterVisible(bool bVisible);

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* ViewCamera;

	// ジャンプ連打防止用
	float LastJumpTime = 0.0f;
	float JumpCoolTime = 0.6f;

	// しゃがみ時のカメラのZ座標オフセット
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CrouchCameraZOffset = 40.f;
	UPROPERTY(VisibleInstanceOnly, Category = "Camera")
	float TargetCameraZ = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CameraInterpSpeed = 3.f;

	// 段差でキャラクターが上下する際のメッシュの補間処理に使用する変数
	float MeshStepZOffset = 0.f;
	float PreviousActorZ = 0.f;
	bool bWasOnGround = false;
	bool bWasCrouchedLastFrame = false;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* DodgeMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* DeathMontage;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;
};
