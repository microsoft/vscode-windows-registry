/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Licensed under the MIT License. See License.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

const windowregistry = process.platform === 'win32' ? require('./winregistry.node') : null;

export type HKEY = "HKEY_CURRENT_USER" | "HKEY_LOCAL_MACHINE" | "HKEY_CLASSES_ROOT" | "HKEY_USERS" | "HKEY_CURRENT_CONFIG";

export function EnumRegKeyKeys(hive: HKEY, path: string) : string[] {
	if (windowregistry) {
		return windowregistry.EnumRegKeyKeys(hive, path);
	} 
	console.error('Could not initialize Windows Registry native node module.');
	return [];
}

export function EnumRegKeyValues(hive: HKEY, path: string) : string[] {
	if (windowregistry) {
		return windowregistry.EnumRegKeyValues(hive, path);
	} 
	console.error('Could not initialize Windows Registry native node module.');
	return [];
}

export function GetStringRegKey(hive: HKEY, path: string, name: string): string | undefined {
	if (windowregistry) {
		return windowregistry.GetStringRegKey(hive, path, name);
	}
	console.error('Could not initialize Windows Registry native node module.');
	return undefined;
}

export function SetStringRegKey(hive: HKEY, path: string, name: string, value: string) : void {
	if (windowregistry) {
		windowregistry.SetStringRegKey(hive, path, name, value);
	} else {
		console.error('Could not initialize Windows Registry native node module.');
	}
}

export function DeleteRegKeyKey(hive: HKEY, path: string, name: string) : void {
	if (windowregistry) {
		windowregistry.DeleteRegKeyKey(hive, path, name);
	} else {
		console.error('Could not initialize Windows Registry native node module.');
	}
}

export function DeleteRegKeyValue(hive: HKEY, path: string, name: string) : void {
	if (windowregistry) {
		windowregistry.DeleteRegKeyValue(hive, path, name);
	} else {
		console.error('Could not initialize Windows Registry native node module.');
	}
}
