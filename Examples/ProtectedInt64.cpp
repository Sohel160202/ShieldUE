FSecureInt64Rules Rules;
Rules.bUseRange = true;
Rules.MinValue = 0;
Rules.MaxValue = 9223372036854775807LL;
Rules.DefaultValue = 0;
Rules.bEnableAutoRekey = true;
Rules.RekeyIntervalSeconds = 5.0f;

FSecureInt64 Experience;
Experience.Initialize(0, Rules);
Experience.Set(Experience.Get() + 1000);
