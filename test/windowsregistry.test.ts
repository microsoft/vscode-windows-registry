/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Licensed under the MIT License. See License.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

import { GetStringRegKey } from '../dist/index';
import * as assert from 'assert';

describe('Windows Registry Tests', () => {
	describe('@GetStringRegKey', () => {
		it('Retrieves the ProgramFilesPath registry value', () => {
			const prgmFilesPath = GetStringRegKey('HKEY_LOCAL_MACHINE', 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion', 'ProgramFilesPath');
			assert(prgmFilesPath === '%ProgramFiles%');
		});

		it('Validates argument count', () => {
			assert.throws(() => (<any>GetStringRegKey)());
			assert.throws(() => ((<any>GetStringRegKey)('HKEY_LOCAL_MACHINE')));
			assert.throws(() => ((<any>GetStringRegKey)('HKEY_LOCAL_MACHINE', 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion')));
		});
	});
});