# Contributing to ShieldUE

Thank you for helping improve ShieldUE.

## Before opening an issue

- Check existing issues and discussions.
- Confirm the issue on a supported Unreal Engine version.
- Include a minimal reproduction whenever possible.
- Do not publish sensitive security details publicly.

## Pull requests

Pull requests should:

- Explain the problem and the proposed solution.
- Keep the change focused.
- Update documentation when behavior or public APIs change.
- Include tests or a reproducible validation path where possible.
- Avoid describing obfuscation as cryptographic protection unless the implementation has been independently reviewed.

## Code guidelines

- Follow Unreal Engine naming and formatting conventions.
- Keep Blueprint nodes clearly named and grouped by category.
- Preserve backward compatibility for public APIs when practical.
- Keep development-only corruption helpers visibly separated from shipping APIs.
- Do not add a security claim without documenting its threat model and limitations.

## Local validation

At minimum, run:

```text
git diff --check
```

When an Unreal Engine environment is available, compile the plugin in both Development and Shipping configurations and run the relevant automation tests.
