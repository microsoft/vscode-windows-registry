import { GetStringRegKey } from '../dist/index';
import * as assert from 'assert';

describe('Windows Registry Tests', () => {
	describe('@GetStringRegKey', () => {
		const prgmFilesPath = GetStringRegKey('HKEY_LOCAL_MACHINE', 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion', 'ProgramFilesPath');
		assert(prgmFilesPath === '%ProgramFiles%');
	});
});