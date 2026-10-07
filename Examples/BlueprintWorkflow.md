# Blueprint workflow

## Initialization

1. Create a `Secure Int` variable named `Currency`.
2. Create `Secure Int Rules` and enable the range.
3. Set `Min Value` to `0`, `Max Value` to the desired limit, and `Default Value` to the starting balance.
4. Enable automatic re-keying and choose an interval.
5. Call `Secure Int Initialize` on Begin Play.

## Runtime use

- Use `Secure Int Get` whenever the game needs the balance.
- Use `Secure Int Set` when the balance changes.
- Call `Secure Int Update Protection` from Tick when automatic re-keying is enabled.
- Call `Secure Int Validate` at important checkpoints.
- Use `Secure Int Get Tamper Count` and `Secure Int Get Last Tamper Reason` for diagnostics.

## Development test

In a development-only test path, call `Secure Int Corrupt For Testing`, then call `Secure Int Validate` or `Secure Int Get`. Confirm that the configured recovery mode runs and that the tamper count increases.
