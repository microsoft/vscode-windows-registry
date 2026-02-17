/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Licensed under the MIT License. See License.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

import { GetStringRegKey, GetDWORDRegKey } from '../dist/index';
import * as assert from 'assert';

describe('Windows Registry Tests', () => {
	if (process.platform === 'win32') {
		describe('@GetStringRegKey', () => {
			it('Retrieves the ProgramFilesPath registry value', () => {
				const prgmFilesPath = GetStringRegKey('HKEY_LOCAL_MACHINE', 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion', 'ProgramFilesPath');
				assert(prgmFilesPath === '%ProgramFiles%');
			});

			it('Validates argument count', () => {
				assert.throws(() => (GetStringRegKey as any)());
				assert.throws(() => ((GetStringRegKey as any)('HKEY_LOCAL_MACHINE')));
				assert.throws(() => ((GetStringRegKey as any)('HKEY_LOCAL_MACHINE', 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion')));
			});

			it('Validates argument length', () => {
				let reallyLongString = 'areallystring';
				while (reallyLongString.length < 17000) {
					reallyLongString += reallyLongString;
				}

				assert.throws(() => ((GetStringRegKey as any)(
					reallyLongString,
					'SOFTWARE\\Microsoft\\Windows\\CurrentVersion',
					'ProgramFilesPath')));

				assert.throws(() => (GetStringRegKey(
					'HKEY_LOCAL_MACHINE',
					reallyLongString,
					'ProgramFilesPath')));

				assert.throws(() => (GetStringRegKey(
					'HKEY_LOCAL_MACHINE',
					'SOFTWARE\\Microsoft\\Windows\\CurrentVersion',
					reallyLongString)));
			});
		});
		describe('@GetDWORDRegKey', () => {
			it('Retrieves a DWORD registry value', () => {
				const result = GetDWORDRegKey('HKEY_LOCAL_MACHINE', 'SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion', 'InstallDate');
				assert(result === undefined || typeof result === 'number');
			});

			it('Returns undefined for non-existent value', () => {
				const result = GetDWORDRegKey('HKEY_LOCAL_MACHINE', 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion', 'NonExistentDWORDValue12345');
				assert(result === undefined);
			});

			it('Validates argument count', () => {
				assert.throws(() => (GetDWORDRegKey as any)());
				assert.throws(() => ((GetDWORDRegKey as any)('HKEY_LOCAL_MACHINE')));
				assert.throws(() => ((GetDWORDRegKey as any)('HKEY_LOCAL_MACHINE', 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion')));
			});
		});
	} else {
		describe('@GetStringRegKey', () => {
			it('Throws an error when not on Windows', () => {
				assert.throws(() => GetStringRegKey('HKEY_LOCAL_MACHINE', 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion', 'ProgramFilesPath'));
			});
		});

		describe('@GetDWORDRegKey', () => {
			it('Throws an error when not on Windows', () => {
				assert.throws(() => GetDWORDRegKey('HKEY_LOCAL_MACHINE', 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion', 'SomeValue'));
			});
		});
	}
});