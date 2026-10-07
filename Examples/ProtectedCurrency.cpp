// Example usage. Add this to an Unreal Actor, Component, or subsystem in your project.

FSecureInt32 SecureCurrency;

void AExampleActor::BeginPlay()
{
    Super::BeginPlay();

    FSecureInt32Rules Rules;
    Rules.bUseRange = true;
    Rules.MinValue = 0;
    Rules.MaxValue = 1000000;
    Rules.DefaultValue = 100;
    Rules.RecoveryMode = ESecureValueRecoveryMode::RestoreLastValid;
    Rules.bEnableAutoRekey = true;
    Rules.RekeyIntervalSeconds = 5.0f;

    SecureCurrency.Initialize(100, Rules);
}

void AExampleActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    SecureCurrency.UpdateProtection(DeltaSeconds);
}

void AExampleActor::AddCurrency(int32 Amount)
{
    const int32 Current = SecureCurrency.Get();
    SecureCurrency.Set(Current + Amount);
}

bool AExampleActor::CheckCurrencyIntegrity()
{
    if (!SecureCurrency.Validate())
    {
        UE_LOG(LogTemp, Warning, TEXT("Currency validation failed. Tamper count: %u"), SecureCurrency.GetTamperCount());
        return false;
    }

    return true;
}
