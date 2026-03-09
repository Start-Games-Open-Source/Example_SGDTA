# Contributing to Example_SGDTA

Thank you for your interest in contributing to the Example_SGDTA project! This is a reference project demonstrating the [SGDynamicTextAssets](https://github.com/Start-Games-Open-Source/SGDynamicTextAssets) plugin. This guide will help you get set up and walk you through the contribution process.

## Table of Contents

- [Before You Start](#before-you-start)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [Coding Standards](#coding-standards)
- [Commit Convention](#commit-convention)
- [Branching and Pull Requests](#branching-and-pull-requests)
- [Licensing](#licensing)

---

## Before You Start

This is an **example project** that demonstrates how to use the SGDynamicTextAssets plugin, not the plugin itself. Please make sure you are filing issues and PRs in the right place:

| What | Where |
|------|-------|
| Bug in the **plugin** (serializers, subsystem, browser, etc.) | [SGDynamicTextAssets Issues](https://github.com/Start-Games-Open-Source/SGDynamicTextAssets/issues) |
| Bug in the **example project** (Shooter variant, example DTAs, etc.) | [Example_SGDTA Issues](https://github.com/Start-Games-Open-Source/Example_SGDTA/issues) |
| Feature request for the **plugin** | [SGDynamicTextAssets Issues](https://github.com/Start-Games-Open-Source/SGDynamicTextAssets/issues) |
| Request for a **new example or demo** | [Example_SGDTA Issues](https://github.com/Start-Games-Open-Source/Example_SGDTA/issues) |

## Getting Started

1. **Fork** the repository on GitHub: https://github.com/Start-Games-Open-Source/Example_SGDTA
2. **Clone** your fork with submodules (the SGDynamicTextAssets plugin is included as a git submodule):

   ```bash
   git clone --recurse-submodules https://github.com/<your-username>/Example_SGDTA.git
   ```

   If you already cloned without `--recurse-submodules`, run:

   ```bash
   git submodule update --init --recursive
   ```

3. **Add the upstream remote** so you can stay up to date:

   ```bash
   cd Example_SGDTA
   git remote add upstream https://github.com/Start-Games-Open-Source/Example_SGDTA.git
   ```

4. **Create a branch** for your work (see [Branching and Pull Requests](#branching-and-pull-requests)).

## Development Setup

### Requirements

- **Unreal Engine 5.6+** (source or launcher build)
- A C++ IDE supported by Unreal Engine (Visual Studio 2022, Rider, etc.)

### Building

1. Right-click `Example_SGDTA.uproject` and select **Generate Visual Studio project files** (or use UnrealBuildTool).
2. Open the generated solution and build in **Development Editor** configuration.
3. Launch the editor. It should open the Shooter variant map (`Lvl_Shooter`) automatically.

### Project Structure

```
Example_SGDTA/
  Example_SGDTA.uproject
  Source/Example_SGDTA/              # C++ source (game module)
  Content/
    SGDynamicTextAssets/             # DTA data files (.dta.json, .dta.yaml, .dta.xml)
    Variant_Shooter/                 # Shooter variant Blueprints and assets
    Weapons/                         # Weapon meshes and materials
  Plugins/SGDynamicTextAssets/       # Plugin (git submodule, do not modify here)
  Config/                           # Engine, game, editor, and input configuration
```

> **Important:** The `Plugins/SGDynamicTextAssets/` directory is a git submodule. Do not make changes to plugin files in this project. Submit plugin changes to the [plugin repository](https://github.com/Start-Games-Open-Source/SGDynamicTextAssets) instead.

## Coding Standards

Follow the existing conventions in the codebase:

- Use standard [Unreal Engine Coding Standards](https://dev.epicgames.com/documentation/en-us/unreal-engine/epic-cplusplus-coding-standard-for-unreal-engine) (U-prefix for UObjects, F-prefix for structs, etc.).
- Every source file must include the copyright header as the first line:

  ```cpp
  // Copyright Start Games, Inc. All Rights Reserved.
  ```

  All contributions are made under the Start Games copyright. This is intentional and benefits everyone involved:

  - **Shields contributors from legal liability.** If someone violates the Elastic License 2.0 (for example, reselling the plugin), Start Games handles enforcement as the sole copyright holder. Individual contributors are never named in or dragged into legal disputes over their contributed code.
  - **Simplifies license enforcement.** With a single copyright holder, Start Games can act decisively to protect the project. Fragmented copyright across many contributors would require coordinating with every contributor who owns a piece, which is impractical.
  - **Maintains a clean IP chain.** A unified copyright makes it unambiguous that all code falls under the Elastic License 2.0. There is no question of which contributor licensed which file under what terms.
  - **Preserves relicensing flexibility.** If Start Games ever adjusts the license (for example, moving to a more permissive one), a single copyright holder can do so without needing written consent from every past contributor.
  - **Follows established open source practice.** Many major projects (Apache Foundation, Google, Meta) use the same approach where contributions are made under the project's copyright, not the individual's. It is a well-understood model.

- Follow the patterns established by existing classes (e.g., `ShooterWeapon`, `ShooterPickup`, `WeaponInfoDTA`).
- DTA subclasses must use soft references only. No hard asset references are allowed (the plugin's UHT validator enforces this at compile time).

## Commit Convention

This project uses [Conventional Commits](https://www.conventionalcommits.org/). Every commit message should follow this format:

```
<type>: <short description>

[optional body]

[optional footer]
```

### Types

| Type | When to use |
|------|-------------|
| `feat` | A new example, demo scenario, or capability |
| `fix` | A bug fix |
| `docs` | Documentation changes (README, inline comments) |
| `refactor` | Code change that neither fixes a bug nor adds a feature |
| `chore` | Build process, configuration, or other maintenance |

### Examples

```
feat: add Shotgun weapon to the Shooter variant
fix: correct Rifle DTA referencing wrong Blueprint class
docs: add clone instructions with --recurse-submodules to README
refactor: simplify ShooterPickup weapon loading logic
chore: update SGDynamicTextAssets submodule to latest release
```

## Branching and Pull Requests

### Branch Naming

Create a branch from `main` using one of these prefixes:

| Prefix | Use |
|--------|-----|
| `feat/` | New examples or demos (e.g., `feat/shotgun-weapon`) |
| `fix/` | Bug fixes (e.g., `fix/rifle-dta-path`) |
| `docs/` | Documentation changes (e.g., `docs/readme-clone-instructions`) |
| `refactor/` | Refactoring (e.g., `refactor/pickup-loading`) |
| `chore/` | Maintenance (e.g., `chore/update-plugin-submodule`) |

### Pull Request Process

1. **Sync with upstream** before opening your PR:

   ```bash
   git fetch upstream
   git rebase upstream/main
   ```

2. **Push your branch** to your fork:

   ```bash
   git push -u origin feat/my-example
   ```

3. **Open a Pull Request** against `main` on the upstream repository.
4. **Fill out the PR template.** Describe your changes, link the related issue, explain what you tested, and check the licensing box.
5. A maintainer will review your PR. Be prepared for feedback and follow-up commits.

### Code Review Expectations

- All PRs require at least one maintainer review before merging.
- PRs must compile without warnings.
- New examples should be self-contained and demonstrate a clear use case of the SGDynamicTextAssets plugin.
- Keep PRs focused. One example or fix per PR.

## Licensing

Example_SGDTA is licensed under the [Elastic License 2.0](./LICENSE).

By submitting a contribution to this project, you agree that your contribution is submitted under the same Elastic License 2.0 that covers the project, and you have the right to submit it. You also confirm that you are not submitting code that is owned by a third party unless that code is compatible with the Elastic License 2.0.

---

Thank you for contributing to Example_SGDTA!
