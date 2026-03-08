# Example SGDynamicTextAssets Project
Example project using [SGDynamicTextAssets plugin](https://github.com/Start-Games-Open-Source/SGDynamicTextAssets) for simple usage.

Initially created in `Unreal Engine 5.6`, but should support later versions. Earlier versions are untested.

Current version is `Unreal Engine 5.6`

To provide feedback or see whats in development, visit the [Issues Page](https://github.com/Start-Games-Open-Source/Example_SGDTA/issues) of the repo!

# License

This example project is released under the **[Elastic License 2.0 (ELv2)](./LICENSE)** by **Start Games Open Source**.

The examples in this repository demonstrate usage of the [SGDynamicTextAssets](https://github.com/Start-Games-Open-Source/SGDynamicTextAssets) plugin, which is also covered under the same license.

---

## What You Can and Can't Do

| Use Case | Allowed? |
|---|---|
| Use these examples as reference for your own project | ✅ Yes |
| Modify and adapt the example code for your game | ✅ Yes |
| Keep your project private | ✅ Yes |
| Ship a game that uses code derived from these examples | ✅ Yes |
| Sell these examples or a repackaged version of them | ❌ No |
| Redistribute them as part of a paid product or tool | ❌ No |
| Offer them as a paid hosted or managed service | ❌ No |
| Remove or alter the Start Games credit/copyright notices | ❌ No |

---

## Simple Summary

> **Free to learn from. Free to build on. Just don't sell it.**

These examples exist to help you get up and running with SGDynamicTextAssets quickly. You're free to reference, copy, and adapt any of this code into your own projects. 
The only restriction is that you cannot repackage and sell these examples (or the plugin they demonstrate) as a standalone product or service.

Credit to **Start Games Open Source** must remain intact in any copy or distribution of this content.

*This summary is for convenience only and does not replace the full [LICENSE](./LICENSE) text.*

---

This primarily uses the Shooter Variant of the FPS template, 
and removes the Horror Variant since it serves no real purpose.

The usage from this plugin basically replaces the weapon info data for the 
shooter weapon pickups to use that data instead.

# Description

## How do I have my DTA's cook and package?
The plugin comes with a custom deployment handler(sadly Unreal only allows for 1 per platform), if you want to add more custom deployment handling then you would introduce your own and integrate the plugin's already available CI/CD tools, features, and systems to take advanantage of it.

This is also in the CookPipeline.md documentation but just incase you need it immediately you would add this into your `DeafaultEngine.ini`:
```ini
[/Script/WindowsTargetPlatform.WindowsTargetSettings]
CustomDeployment=SGDynamicTextAssetsCleanup
```
