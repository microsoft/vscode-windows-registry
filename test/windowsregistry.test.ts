/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Licensed under the MIT License. See License.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
import { randomUUID } from 'crypto';
import { EnumRegKeyKeys, EnumRegKeyValues, GetStringRegKey, SetStringRegKey, DeleteRegKeyValue, DeleteRegKeyKey } from '../dist/index';
import * as assert from 'assert';

describe('Windows Registry Tests', () => {
	describe('@EnumRegKeyKeys', ()=>{
    if (process.platform === 'win32') {
		  it('List contains the CurrentVersion', ()=>{
			  const keys = EnumRegKeyKeys('HKEY_LOCAL_MACHINE', 'SOFTWARE\\Microsoft\\Windows');
			  assert(keys.includes('CurrentVersion'));
		  });

		  it('Validates argument count', () => {
			  assert.throws(() => (<any>EnumRegKeyKeys)());
			  assert.throws(() => ((<any>EnumRegKeyKeys)('HKEY_LOCAL_MACHINE')));
		  });

		  it('Validates argument length', () => {
			  let reallyLongString = 'areallystring';
			  while (reallyLongString.length < 17000) {
				  reallyLongString += reallyLongString;
			  }

			  assert.throws(() => ((<any>EnumRegKeyKeys)(reallyLongString, 'SOFTWARE\\Microsoft\\Windows')));
			  assert.throws(() => ((<any>EnumRegKeyKeys)('HKEY_LOCAL_MACHINE', reallyLongString, )));
		  });
    } else {
			it('Throws an error when not on Windows', () => {
				assert.throws(() => EnumRegKeyKeys('HKEY_LOCAL_MACHINE', 'SOFTWARE\\Microsoft\\Windows'));
			});
    }
	});

	describe('@EnumRegKeyValues', ()=>{
    if (process.platform === 'win32') {
		  it('List contains the ProgramFilesPath', ()=>{
			  const values = EnumRegKeyValues('HKEY_LOCAL_MACHINE', 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion');
			  assert(values.includes('ProgramFilesPath'));
		  });

  		it('Validates argument count', () => {
	  		assert.throws(() => (<any>EnumRegKeyValues)());
		  	assert.throws(() => ((<any>EnumRegKeyValues)('HKEY_LOCAL_MACHINE')));
		  });

		  it('Validates argument length', () => {
			  let reallyLongString = 'areallystring';
			  while (reallyLongString.length < 17000) {
				  reallyLongString += reallyLongString;
			  }

			  assert.throws(() => ((<any>EnumRegKeyValues)(reallyLongString, 'SOFTWARE\\Microsoft\\Windows')));
			  assert.throws(() => ((<any>EnumRegKeyValues)('HKEY_LOCAL_MACHINE', reallyLongString, )));
		  });
    } else {
      it('Throws an error when not on Windows', () => {
				assert.throws(() => EnumRegKeyValues('HKEY_LOCAL_MACHINE', 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion'));
			});
    }
	});

	describe('@GetStringRegKey', () => {
	  if (process.platform === 'win32') {
		  it('Retrieves the ProgramFilesPath registry value', () => {
			  const prgmFilesPath = GetStringRegKey('HKEY_LOCAL_MACHINE', 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion', 'ProgramFilesPath');
			  assert(prgmFilesPath === '%ProgramFiles%');
		  });

			it('Validates argument count', () => {
				assert.throws(() => (<any>GetStringRegKey)());
				assert.throws(() => ((<any>GetStringRegKey)('HKEY_LOCAL_MACHINE')));
				assert.throws(() => ((<any>GetStringRegKey)('HKEY_LOCAL_MACHINE', 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion')));
			});

			it('Validates argument length', () => {
				let reallyLongString = 'areallystring';
				while (reallyLongString.length < 17000) {
					reallyLongString += reallyLongString;
				}

				assert.throws(() => ((<any>GetStringRegKey)(
					reallyLongString,
					'SOFTWARE\\Microsoft\\Windows\\CurrentVersion',
					'ProgramFilesPath')));

				assert.throws(() => ((<any>GetStringRegKey)(
					'HKEY_LOCAL_MACHINE',
					reallyLongString,
					'ProgramFilesPath')));

				assert.throws(() => ((<any>GetStringRegKey)(
					'HKEY_LOCAL_MACHINE',
					'SOFTWARE\\Microsoft\\Windows\\CurrentVersion',
					reallyLongString)));
			});
	  } else {
			it('Throws an error when not on Windows', () => {
				assert.throws(() => GetStringRegKey('HKEY_LOCAL_MACHINE', 'SOFTWARE\\Microsoft\\Windows\\CurrentVersion', 'ProgramFilesPath'));
			});
    }
	});

	describe('Registry Writing', ()=>{
    if (process.platform === 'win32') {
		  it('@SetStringRegKey', ()=>{
			  const uuid = randomUUID();
			  SetStringRegKey('HKEY_CURRENT_USER', 'NativeModuleTests\\vscode\\registry', 'value', uuid);
			  assert(uuid === GetStringRegKey('HKEY_CURRENT_USER', 'NativeModuleTests\\vscode\\registry', 'value'));
		  });
		  it('@DeleteRegKeyValue', ()=>{
  			const before = EnumRegKeyValues('HKEY_CURRENT_USER', 'NativeModuleTests\\vscode\\registry');
	  		assert(before.includes('value'));
  			DeleteRegKeyValue('HKEY_CURRENT_USER', 'NativeModuleTests\\vscode\\registry', 'value');
  			const after = EnumRegKeyValues('HKEY_CURRENT_USER', 'NativeModuleTests\\vscode\\registry');
  			assert(!after.includes('value'));
  		});
  		it('@DeleteRegKeyKey(inner)', ()=>{
  			const before = EnumRegKeyKeys('HKEY_CURRENT_USER', 'NativeModuleTests\\vscode');
  			assert(before.includes('registry'));
	  		DeleteRegKeyKey('HKEY_CURRENT_USER', 'NativeModuleTests\\vscode', 'registry');
  			const after = EnumRegKeyKeys('HKEY_CURRENT_USER', 'NativeModuleTests\\vscode');
  			assert(!after.includes('registry'));
  		});
  		it('@DeleteRegKeyKey(middle)', ()=>{
	  		const before = EnumRegKeyKeys('HKEY_CURRENT_USER', 'NativeModuleTests');
  			assert(before.includes('vscode'));
  			DeleteRegKeyKey('HKEY_CURRENT_USER', 'NativeModuleTests', 'vscode');
  			const after = EnumRegKeyKeys('HKEY_CURRENT_USER', 'NativeModuleTests');
  			assert(!after.includes('vscode'));
  		});
  		it('@DeleteRegKeyKey(outer)', ()=>{
  			const before = EnumRegKeyKeys('HKEY_CURRENT_USER', '');
  			assert(before.includes('NativeModuleTests'));
  			DeleteRegKeyKey('HKEY_CURRENT_USER', '', 'NativeModuleTests');
  			const after = EnumRegKeyKeys('HKEY_CURRENT_USER', '');
  			assert(!after.includes('NativeModuleTests'));
  		});
		} else {
      it('Throws an error when not on Windows', () => {
				assert.throws(() => SetStringRegKey('HKEY_CURRENT_USER', 'NativeModuleTests\\vscode\\registry', 'value', 'a value'));
			});
    }
  });
});
