
#include "Player/Aura_PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAura_PlayerController::AAura_PlayerController()
{
	bReplicates = true;
}

void AAura_PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//调用光标追踪
	CursorTrace();
	
}

void AAura_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	//检查上下文是否存在，再获取本地玩家子系统，将上文变量挂载到映射上下文里。
	check(AuraContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	//显示鼠标光标以及设置光标样式。
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);//用来设置鼠标锁定（mouse lock）的行为。告诉 PlayerController 在该输入模式下是否把鼠标“锁定”到游戏视口（即鼠标不能移出窗口或被限制到窗口内）。
	InputModeData.SetHideCursorDuringCapture(false);//用来指定当“捕获鼠标”时是否隐藏系统光标。参数是布尔。
	SetInputMode(InputModeData);//SetInputMode 是 APlayerController 提供的一个方法，用来设置当前的输入模式。这里把刚配置好的 InputModeData（GameAndUI 模式）应用到当前控制器。之后引擎将按该模式分配键盘/鼠标/游戏手柄的输入。
	
}

void AAura_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAura_PlayerController::Move);
	
}

void AAura_PlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAura_PlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);//获取鼠标所在位置的命中结果，将结果返回给CrusorHit

	//如果没击中东西就什么也不做，结束函数
	if (!CursorHit.bBlockingHit) return;

	//疑问：为什么接口可以将接收actor类型变量
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/**
	 *  A. 上一个actor与这一个actor都为空
	 *		- 什么都不做
	 *	B. 上一个actor为空但这一个actor有效
	 *		- 高亮这一个actor
	 *	C. 上一个actor有效但这一个actor为空
	 *		- 上一个actor取消高亮
	 *	D. 两个actor都有效，但上一个actor不等于这一个actor
	 *		- 上一个actor取消高亮, 高亮这一个actor
	 *	E. 两个actor都有效，且两帧都是同一个 actor
	 *		- 什么都不做
	 */
	
	if (LastActor == nullptr)//上一个actor为空
	{
		if (ThisActor != nullptr)
		{
			// Case B
			ThisActor->HighlightActor();
		}
		else
		{
			// Case A 
		}
	}
	else // 上一个actor有效
	{
		if (ThisActor == nullptr)
		{
			// Case C
			LastActor->UnHighlightActor();
		}
		else // both actors are valid
		{
			if (LastActor != ThisActor)
			{
				// Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				// Case E - do nothing
			}
		}
	}
}
