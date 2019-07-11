/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Licensed under the MIT License. See License.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#include <nan.h>

#include <string>
#include <node.h>
#include <windows.h>

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::NewStringType;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Array;
using v8::Value;

namespace {

	HKEY GetHive(std::string hkey) {
		if (hkey == "HKEY_CURRENT_USER") {
			return HKEY_CURRENT_USER;
		}

		if (hkey == "HKEY_LOCAL_MACHINE") {
			return HKEY_LOCAL_MACHINE;
		}

		if (hkey == "HKEY_CLASSES_ROOT") {
			return HKEY_CLASSES_ROOT;
		}

		if (hkey == "HKEY_USERS") {
			return HKEY_USERS;
		}

		if (hkey == "HKEY_CURRENT_CONFIG") {
			return HKEY_CURRENT_CONFIG;
		}

		return NULL;
	}

  void GetStringRegKey(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();

		// Check that we have 2 arguments - Hive, Path, Name
		if (args.Length() != 3) {
			Nan::ThrowError("Wrong number of arguments");
			return;
		}

		// Check that arguments are properly typed
		if (!args[0]->IsString() || !args[1]->IsString() || !args[2]->IsString()) {
			Nan::ThrowTypeError("Wrong argument types");
			return;
		}

		HKEY hive = GetHive(*Nan::Utf8String(args[0]));
		std::string path = *Nan::Utf8String(args[1]);
		std::string name = *Nan::Utf8String(args[2]);
    std::string result = "";

    HKEY hKey;
    if (ERROR_SUCCESS != RegOpenKeyEx(hive, path.c_str(), 0, KEY_READ, &hKey)) {
      Nan::ThrowError("Unable to open registry key");
			return;
    }

    char szBuffer[512];
    DWORD dwBufferSize = sizeof(szBuffer);

    if (ERROR_SUCCESS == RegQueryValueEx(hKey, name.c_str(), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize)) {
      result = szBuffer;
    }

    RegCloseKey(hKey);

		Local<String> res = Nan::New<v8::String>(result.c_str()).ToLocalChecked();
    args.GetReturnValue().Set(res);
  }

	void init(Local<Object> exports) {
		NODE_SET_METHOD(exports, "GetStringRegKey", GetStringRegKey);
	}

	NODE_MODULE(addon, init)
}