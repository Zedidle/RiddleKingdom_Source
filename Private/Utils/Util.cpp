// 工具集

#include "Utils/Util.h"
#include "Settings/BaseGameInstance.h"
#include "Pawns/BaseCreature.h"  // 用到一些枚举类型



FString Util::GetWeaponTypeString(EWeaponType WeaponType, FString Prefix)
{
	TMap<EWeaponType, FString> W;
	W.Add(EWeaponType::E_None, "");
	W.Add(EWeaponType::E_GreatSword, "GreatSword");
	W.Add(EWeaponType::E_Sword, "Sword");
	W.Add(EWeaponType::E_Bow, "Bow");
	W.Add(EWeaponType::E_MagicWand, "MagicWand");

	FString R = W[WeaponType];
	if (R != "")
	{
		R = Prefix + R;
	}
	return R;
}


FString Util::GetDeputyTypeString(EDeputyType DeputyType, FString Prefix)
{
	TMap<EDeputyType, FString> D;
	D.Add(EDeputyType::E_None, "");
	D.Add(EDeputyType::E_Shild, "Shield");
	D.Add(EDeputyType::E_Dagger, "Dagger");

	FString R = D[DeputyType];
	if (R != "")
	{
		R = Prefix + R;
	}

	return R;
}

bool Util::AbilityCanUseWeaponType(ECharacterAbility Ability, EWeaponType WeaponType)
{
	TMap<ECharacterAbility, TArray<EWeaponType>> Map_AbilityWeapons;

	TArray<EWeaponType> BowWeapons;
	BowWeapons.Add(EWeaponType::E_Bow);
	Map_AbilityWeapons.Add(ECharacterAbility::E_BOW, BowWeapons);


	TArray<EWeaponType> SwordWeapons;
	SwordWeapons.Add(EWeaponType::E_GreatSword);
	SwordWeapons.Add(EWeaponType::E_Sword);
	Map_AbilityWeapons.Add(ECharacterAbility::E_SWORD, SwordWeapons);


	TArray<EWeaponType> MagicWeapons;
	MagicWeapons.Add(EWeaponType::E_MagicWand);
	Map_AbilityWeapons.Add(ECharacterAbility::E_MAGIC, MagicWeapons);

	if (Map_AbilityWeapons.Contains(Ability))
	{
		return Map_AbilityWeapons[Ability].Contains(WeaponType);
	}
	else
	{
		return false;
	}
}





bool Util::AbilityCanUseDeputyType(ECharacterAbility Ability, EDeputyType DeputyType)
{
	TMap<ECharacterAbility, TArray<EDeputyType>> Map_AbilityDeputys;

	TArray<EDeputyType> BowDeputys;
	BowDeputys.Add(EDeputyType::E_Dagger);
	Map_AbilityDeputys.Add(ECharacterAbility::E_BOW, BowDeputys);

	TArray<EDeputyType> SwordWeapons;
	SwordWeapons.Add(EDeputyType::E_Shild);
	Map_AbilityDeputys.Add(ECharacterAbility::E_SWORD, SwordWeapons);


	if (Map_AbilityDeputys.Contains(Ability))
	{
		return Map_AbilityDeputys[Ability].Contains(DeputyType);
	}
	else
	{
		return false;
	}
}

float Util::CalDamageByDifficulty(float Damage, bool bPlayerControlled)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GWorld);
	UBaseGameInstance* GI = Cast<UBaseGameInstance>(GameInstance);

	if (GI)
	{
		switch (GI->Setting.Difficulty)
		{
		case EDifficulty::E_Simple:
		{
			if (bPlayerControlled)
			{
				Damage *= 0.5;
			}
			else
			{
				Damage *= 2;
			}
			break;
		}
		case EDifficulty::E_Normal:
		{
			break;
		}
		case EDifficulty::E_Hard:
		{
			if (bPlayerControlled)
			{
				Damage *= 1.5;
			}
			else
			{
				Damage *= 0.75;
			}
			break;
		}
		case EDifficulty::E_Hell:
		{
			if (bPlayerControlled)
			{
				Damage *= 2;
			}
			else
			{
				Damage *= 0.5;
			}
			break;
		}
		}
	}
	return Damage;
}

void Util::TestCmd()
{
	UE_LOG(LogTemp, Warning, TEXT("Util::TestCmd"));

}
