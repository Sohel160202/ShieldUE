FSecureFloatRules Rules;
Rules.bUseRange = true;
Rules.MinValue = 0.0f;
Rules.MaxValue = 100.0f;
Rules.DefaultValue = 50.0f;
Rules.bEnableAutoRekey = true;
Rules.RekeyIntervalSeconds = 10.0f;

FSecureFloat Value;
FString Error;

if (!Value.ValidateRules(Error))
{
    UE_LOG(LogTemp, Warning, TEXT("Invalid ShieldUE rules: %s"), *Error);
    return;
}

Value.Initialize(50.0f, Rules);
