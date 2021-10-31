
#include "ActorComponents/BaseGameplayEffectExecutionCalc.h"
#include "AbilitySystemComponent.h"
#include "ActorComponents/BaseAttributeSet.h"


struct DamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);
	DamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, MaxHealth, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CurHealth, Target, false);
	}
};

UBaseGameplayEffectExecutionCalc::UBaseGameplayEffectExecutionCalc()
{
	RelevantAttributesToCapture.Add(DamageStatics().CurHealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().MaxHealthDef);
}

void UBaseGameplayEffectExecutionCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;


	float Life = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CurHealthDef, EvaluationParameters, Life);

	float MaxLife = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MaxHealthDef, EvaluationParameters, MaxLife);


	// 计算公式
	float DamageDone = 0.5 * Life;
	if (DamageDone > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().CurHealthProperty, EGameplayModOp::Additive, -DamageDone));
	}


}
