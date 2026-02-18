/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Licensed under the MIT License. See License.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

const windowRegistry = process.platform === 'win32' ? require('../build/Release/winregistry.node') : null;

export type HKEY = "HKEY_CURRENT_USER" | "HKEY_LOCAL_MACHINE" | "HKEY_CLASSES_ROOT" | "HKEY_USERS" | "HKEY_CURRENT_CONFIG";

export function GetStringRegKey(hive: HKEY, path: string, name: string): string | undefined {
	if (windowRegistry) {
		return windowRegistry.GetStringRegKey(hive, path, name);
	}

	throw new Error('GetStringRegKey is only available on Windows.');
}

export function GetDWORDRegKey(hive: HKEY, path: string, name: string): number | undefined {
	if (windowRegistry) {
		return windowRegistry.GetDWORDRegKey(hive, path, name);
	}

	throw new Error('GetDWORDRegKey is only available on Windows.');
}