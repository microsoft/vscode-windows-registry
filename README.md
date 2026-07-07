**Changes in the fork:**
- Added `node-gyp-build` and `prebuildify` to include prebuilds in the library
- Specified fixed NAPI version

**How to update:**
1. When `@vscode/windows-registry` has a new version
2. Create a new branch from `main`
   ```sh
   git checkout -b sync-upstream-v1.2.3
   ```
3. Merge the new released tag and resolve conflicts
   ```sh
   git fetch upstream --tags
   git merge v1.2.3
   ```
4. Rebuild TS
   ```sh
   npm run compile
   ```
5. Rebuild binaries on **Windows x64**
   ```sh
   npm run prebuild-package
   ```
6. Create PR, merge
7. Add a new tag with the "prebuild" suffix `v1.2.3-prebuild`

**How to install build tools (if not installed with Node.js):**
1. Install **Python** (adjust version to the latest if needed)
    ```sh
    winget install Python.Python.3.14
    ```
    or via **Python Install Manager** for better Python version management
    ```sh
    winget install Python.PythonInstallManager
    py install 3
    ```
1. Install **Visual Studio Build Tools**
    ```sh
    winget install Microsoft.VisualStudio.2022.BuildTools
    ```
1. Modify **Visual Studio** to add **Visual C++ build tools**:
    ```sh
    & "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vs_installer.exe" modify `
        --productId Microsoft.VisualStudio.Product.BuildTools `
        --channelId VisualStudio.17.Release `
        --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
        --add Microsoft.VisualStudio.Component.VC.Runtimes.x86.x64.Spectre `
        --add Microsoft.VisualStudio.Component.Windows11SDK.26100 `
    ```
    or manually run **Visual Studio Installer** and select **Individual components**:
    - MSVC v143 - VS 2022 C++ x64/x86 build tools (latest)
    - MSVC v143 - VS 2022 C++ x64/x86 Spectre-Mitigated libs (latest)
    - Windows 11 SDK
1. If installation goes wrong try following a simple instruction from [node-gyp](https://github.com/nodejs/node-gyp#on-windows) (installs more than 12 GB)

---

# Native node module to access the Windows Registry
This module only has what is needed to support VS Code and is intended to be a lightweight module.

## Installing

```sh
npm install @vscode/windows-registry
```

## Using

```javascript
var vsWinReg = require('@vscode/windows-registry');
console.log(vsWinReg.GetStringRegKey('HKEY_LOCAL_MACHINE', 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion', 'ProgramFilesPath'));
```

## Development
 * `npm install`
 * `npm run compile`
 * `npm test`

## License
[MIT](https://github.com/Microsoft/vscode-windows-registry/blob/master/License.txt)


# Contributing

This project welcomes contributions and suggestions.  Most contributions require you to agree to a
Contributor License Agreement (CLA) declaring that you have the right to, and actually do, grant us
the rights to use your contribution. For details, visit https://cla.microsoft.com.

When you submit a pull request, a CLA-bot will automatically determine whether you need to provide
a CLA and decorate the PR appropriately (e.g., label, comment). Simply follow the instructions
provided by the bot. You will only need to do this once across all repos using our CLA.

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/).
For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or
contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.

