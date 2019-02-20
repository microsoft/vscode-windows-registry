# Native node module to access the Windows Registry
This module only has what is needed to support VS Code and is intended to be a lightweight module.

## Installing

```sh
npm install vscode-windows-registry
```

## Using

```javascript
var vsWinReg = require('vscode-windows-registry');
console.log(vsWinReg.GetStringRegKey('HKEY_LOCAL_MACHINE', 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion', 'ProgramFilesPath');
```

## Development
 * `yarn`
 * `yarn node-gyp configure`
 * `yarn node-gyp build`
 * `yarn tsc`
 * `yarn test`

## License
[MIT](https://github.com/Microsoft/vscode-windows-registry/blob/master/License.txt)
