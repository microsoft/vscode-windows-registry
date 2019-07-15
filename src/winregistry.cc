/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Licensed under the MIT License. See License.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#include <string>
#include <windows.h>
#include <node_api.h>

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

	napi_value GetStringRegKey(napi_env env, napi_callback_info info) {
		napi_value argv[3];
		size_t argc = 3;

		napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

		// Check that we have 3 arguments - Hive, Path, Name
		if (argc < 3) {
			napi_throw_error(env, "EINVAL", "Wrong number of arguments");
			return NULL;
		}

		// Retrieve the 3 arguments
		size_t str_len;
		char arg1[1024], arg2[1024], arg3[1024];
		for (int i = 0; i < 3; i++) {
			if (napi_get_value_string_utf8(env, argv[i], i == 0 ? (char *)&arg1 : i == 1 ? (char *)&arg2 : (char *)arg3, 1024, &str_len) != napi_ok) {
				napi_throw_error(env, "EINVAL", "Expected string");
				return NULL;
			}
		}

		HKEY hive = GetHive(std::string(arg1));
		std::string path = std::string(arg2);
		std::string name = std::string(arg3);
    	std::string result = "";

		HKEY hKey;
		if (ERROR_SUCCESS != RegOpenKeyEx(hive, path.c_str(), 0, KEY_READ, &hKey)) {
			napi_throw_error(env, "EINVAL", "Unable to open registry key");
			return NULL;
		}

		char szBuffer[512];
		DWORD dwBufferSize = sizeof(szBuffer);

		if (ERROR_SUCCESS == RegQueryValueEx(hKey, name.c_str(), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize)) {
			result = szBuffer;
		}

    	RegCloseKey(hKey);

		napi_value napi_result;
		napi_create_string_utf8(env, result.c_str(), NAPI_AUTO_LENGTH, &napi_result);

		return napi_result;
	}

	napi_value Init(napi_env env, napi_value exports) {
		napi_value getStringRegKey;
		napi_create_function(env, "GetStringRegKey", NAPI_AUTO_LENGTH, GetStringRegKey, NULL, &getStringRegKey);
		napi_set_named_property(env, exports, "GetStringRegKey", getStringRegKey);

		return exports;
	}

	NAPI_MODULE(NODE_GYP_MODULE_NAME, Init);
}