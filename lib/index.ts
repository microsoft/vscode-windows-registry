/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Licensed under the MIT License. See License.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

const windowregistry = process.platform === 'win32' ? require('../build/Release/winregistry.node') : null;

export type HKEY = "HKEY_CURRENT_USER" | "HKEY_LOCAL_MACHINE" | "HKEY_CLASSES_ROOT" | "HKEY_USERS" | "HKEY_CURRENT_CONFIG";

export function GetStringRegKey(hive: HKEY, path: string, name: string): string | undefined {
	if (process.platform !== 'win32') {
		throw new Error('GetStringRegKey is only available on Windows.');
	}

	if (windowregistry) {
		return windowregistry.GetStringRegKey(hive, path, name);
	}

	console.error('Could not initialize Windows Registry native node module.');
	return undefined;
}