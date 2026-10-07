// Example server-side validation flow.

UShieldUEAntiTamperComponent* Integrity = FindComponentByClass<UShieldUEAntiTamperComponent>();
if (!Integrity || !HasAuthority())
{
    return;
}

const bool bCurrencyIsValid = Integrity->ValidateInt64(
    TEXT("PurchaseCurrency"),
    ClientReportedCurrency,
    ServerAuthoritativeCurrency,
    0);

const bool bRateIsValid = Integrity->ValidateRateLimit(
    TEXT("WeaponFire"),
    CallsInCurrentWindow,
    MaximumCallsPerWindow);

if (!bCurrencyIsValid || !bRateIsValid)
{
    // Reject the request. Do not apply the client-provided gameplay state.
    return;
}

// Apply the transaction using server-owned values, then replicate the result.
