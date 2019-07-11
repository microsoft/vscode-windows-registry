import { GetStringRegKey } from '../index';
import * as assert from 'assert';

describe('Windows Registry Tests', () => {
	describe('@GetStringRegKey', () => {
		it('Retrieves the ProgramFilesPath registry value', () => {
			const prgmFilesPath = GetStringRegKey('HKEY_LOCAL_MACHINE', 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion', 'ProgramFilesPath');
			assert(prgmFilesPath === '%ProgramFiles%');
		});
	});
});