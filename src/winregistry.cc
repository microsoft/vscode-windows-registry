/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Licensed under the MIT License. See License.txt in the project root for
 *license information.
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

napi_value EnumRegKeyKeys(napi_env env, napi_callback_info info) {
  napi_value argv[2];
  size_t argc = 2;

  napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

  // Check that we have 2 arguments - Hive, Path, Name
  if (argc < 2) {
    napi_throw_error(env, "EINVAL", "Wrong number of arguments");
    return nullptr;
  }

  // Retrieve the 2 arguments
  for (int i = 0; i < 2; i++) {
    napi_valuetype value_type;
    napi_typeof(env, argv[i], &value_type);
    if (value_type != napi_string) {
      napi_throw_error(env, "EINVAL", "Expected string");
      return nullptr;
    }
  }

  size_t str_len = 0;
  const int MAX_LEN = 16383;

  napi_get_value_string_utf8(env, argv[0], nullptr, 0, &str_len);
  if (str_len + 1 > MAX_LEN) {
    napi_throw_error(env, "EINVAL", "Arguments too long");
  }
  std::string hive_arg;
  hive_arg.reserve(str_len + 1);
  hive_arg.resize(str_len);
  napi_get_value_string_utf8(env, argv[0], &hive_arg[0], hive_arg.capacity(), nullptr);
  HKEY hive = GetHive(hive_arg);

  napi_get_value_string_utf8(env, argv[1], nullptr, 0, &str_len);
  if (str_len + 1 > MAX_LEN) {
    napi_throw_error(env, "EINVAL", "Arguments too long");
  }
  std::string path;
  path.reserve(str_len + 1);
  path.resize(str_len);
  napi_get_value_string_utf8(env, argv[1], &path[0], path.capacity(), nullptr);

  if (hive == NULL) {
    napi_throw_error(env, nullptr, "Unable to open registry hive");
    return nullptr;
  }

  std::string result;
  HKEY hKey;
  if (path.length()) {
    if (ERROR_SUCCESS != RegOpenKeyEx(hive, path.c_str(), 0, KEY_READ, &hKey)) {
      napi_throw_error(env, nullptr, "Unable to open registry key");
      return nullptr;
    }
  } else {
	hKey = hive;
  }

  int dwIndex = 0;
  char szBuffer[512];
  DWORD dwBufferSize = sizeof(szBuffer);

  napi_value napi_result;
  if (napi_ok != napi_create_array(env, &napi_result)) {
	napi_throw_error(env, nullptr, "Unable to create result array");
	if (path.length()) {
	  RegCloseKey(hKey);
	}
    return nullptr;
  }

  while (ERROR_SUCCESS == RegEnumKeyEx(hKey, dwIndex, szBuffer, &dwBufferSize, NULL, NULL, NULL, NULL)) {
	napi_value value;
	result = szBuffer;
	if (napi_ok != napi_create_string_utf8(env, result.c_str(), result.length(), &value)) {
		break;
	};
	if (napi_ok != napi_set_element(env, napi_result, dwIndex, value)) {
		break;
	}
	dwBufferSize = sizeof(szBuffer);
	dwIndex += 1;
  }

  if (path.length()) {
    RegCloseKey(hKey);
  }

  return napi_result;
}

napi_value EnumRegKeyValues(napi_env env, napi_callback_info info) {
  napi_value argv[2];
  size_t argc = 2;

  napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

  // Check that we have 2 arguments - Hive, Path, Name
  if (argc < 2) {
    napi_throw_error(env, "EINVAL", "Wrong number of arguments");
    return nullptr;
  }

  // Retrieve the 2 arguments
  for (int i = 0; i < 2; i++) {
    napi_valuetype value_type;
    napi_typeof(env, argv[i], &value_type);
    if (value_type != napi_string) {
      napi_throw_error(env, "EINVAL", "Expected string");
      return nullptr;
    }
  }

  size_t str_len = 0;
  const int MAX_LEN = 16383;

  napi_get_value_string_utf8(env, argv[0], nullptr, 0, &str_len);
  if (str_len + 1 > MAX_LEN) {
    napi_throw_error(env, "EINVAL", "Arguments too long");
  }
  std::string hive_arg;
  hive_arg.reserve(str_len + 1);
  hive_arg.resize(str_len);
  napi_get_value_string_utf8(env, argv[0], &hive_arg[0], hive_arg.capacity(), nullptr);
  HKEY hive = GetHive(hive_arg);

  napi_get_value_string_utf8(env, argv[1], nullptr, 0, &str_len);
  if (str_len + 1 > MAX_LEN) {
    napi_throw_error(env, "EINVAL", "Arguments too long");
  }
  std::string path;
  path.reserve(str_len + 1);
  path.resize(str_len);
  napi_get_value_string_utf8(env, argv[1], &path[0], path.capacity(), nullptr);

  if (hive == NULL) {
    napi_throw_error(env, nullptr, "Unable to open registry hive");
    return nullptr;
  }

  std::string result;
  HKEY hKey;
  if (ERROR_SUCCESS != RegOpenKeyEx(hive, path.c_str(), 0, KEY_READ, &hKey)) {
    napi_throw_error(env, nullptr, "Unable to open registry key");
    return nullptr;
  }

  // TODO: enum here
  int dwIndex = 0;
  char szBuffer[512];
  DWORD dwBufferSize = sizeof(szBuffer);


  napi_value napi_result;
  if (napi_ok != napi_create_array(env, &napi_result)) {
	napi_throw_error(env, nullptr, "Unable to create result array");
	RegCloseKey(hKey);
    return nullptr;
  }

  while (ERROR_SUCCESS == RegEnumValue(hKey, dwIndex, szBuffer, &dwBufferSize, NULL, NULL, NULL, NULL)) {
	napi_value value;
	result = szBuffer;
	if (napi_ok != napi_create_string_utf8(env, result.c_str(), result.length(), &value)) {
		break;
	};
	if (napi_ok != napi_set_element(env, napi_result, dwIndex, value)) {
		break;
	}
	dwBufferSize = sizeof(szBuffer);
	dwIndex += 1;
  }

  RegCloseKey(hKey);

  return napi_result;
}

napi_value GetStringRegKey(napi_env env, napi_callback_info info) {
  napi_value argv[3];
  size_t argc = 3;

  napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

  // Check that we have 3 arguments - Hive, Path, Name
  if (argc < 3) {
    napi_throw_error(env, "EINVAL", "Wrong number of arguments");
    return nullptr;
  }

  // Retrieve the 3 arguments
  for (int i = 0; i < 3; i++) {
    napi_valuetype value_type;
    napi_typeof(env, argv[i], &value_type);
    if (value_type != napi_string) {
      napi_throw_error(env, "EINVAL", "Expected string");
      return nullptr;
    }
  }

  size_t str_len = 0;
  const int MAX_LEN = 16383;

  napi_get_value_string_utf8(env, argv[0], nullptr, 0, &str_len);
  if (str_len + 1 > MAX_LEN) {
    napi_throw_error(env, "EINVAL", "Arguments too long");
  }
  std::string hive_arg;
  hive_arg.reserve(str_len + 1);
  hive_arg.resize(str_len);
  napi_get_value_string_utf8(env, argv[0], &hive_arg[0], hive_arg.capacity(), nullptr);
  HKEY hive = GetHive(hive_arg);

  napi_get_value_string_utf8(env, argv[1], nullptr, 0, &str_len);
  if (str_len + 1 > MAX_LEN) {
    napi_throw_error(env, "EINVAL", "Arguments too long");
  }
  std::string path;
  path.reserve(str_len + 1);
  path.resize(str_len);
  napi_get_value_string_utf8(env, argv[1], &path[0], path.capacity(), nullptr);

  napi_get_value_string_utf8(env, argv[2], nullptr, 0, &str_len);
  if (str_len + 1 > MAX_LEN) {
    napi_throw_error(env, "EINVAL", "Arguments too long");
  }
  std::string name;
  name.reserve(str_len + 1);
  name.resize(str_len);
  napi_get_value_string_utf8(env, argv[2], &name[0], name.capacity(), nullptr);
  
  if (hive == NULL) {
    napi_throw_error(env, nullptr, "Unable to open registry hive");
    return nullptr;
  }

  std::string result;
  HKEY hKey;
  if (ERROR_SUCCESS != RegOpenKeyEx(hive, path.c_str(), 0, KEY_READ, &hKey)) {
    napi_throw_error(env, nullptr, "Unable to open registry key");
    return nullptr;
  }

  char szBuffer[512];
  DWORD dwBufferSize = sizeof(szBuffer);

  if (ERROR_SUCCESS == RegQueryValueEx(hKey, name.c_str(), 0, NULL,
                                       (LPBYTE)szBuffer, &dwBufferSize)) {
    result = szBuffer;
  }

  RegCloseKey(hKey);

  napi_value napi_result;
  napi_create_string_utf8(env, result.c_str(), result.length(), &napi_result);

  return napi_result;
}

napi_value SetStringRegKey(napi_env env, napi_callback_info info) {
  napi_value argv[4];
  size_t argc = 4;

  napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

  // Check that we have 3 arguments - Hive, Path, Name
  if (argc < 4) {
    napi_throw_error(env, "EINVAL", "Wrong number of arguments");
    return nullptr;
  }

  // Retrieve the 3 arguments
  for (int i = 0; i < 4; i++) {
    napi_valuetype value_type;
    napi_typeof(env, argv[i], &value_type);
    if (value_type != napi_string) {
      napi_throw_error(env, "EINVAL", "Expected string");
      return nullptr;
    }
  }

  size_t str_len = 0;
  const int MAX_LEN = 16383;

  napi_get_value_string_utf8(env, argv[0], nullptr, 0, &str_len);
  if (str_len + 1 > MAX_LEN) {
    napi_throw_error(env, "EINVAL", "Arguments too long");
  }
  std::string hive_arg;
  hive_arg.reserve(str_len + 1);
  hive_arg.resize(str_len);
  napi_get_value_string_utf8(env, argv[0], &hive_arg[0], hive_arg.capacity(), nullptr);
  HKEY hive = GetHive(hive_arg);

  napi_get_value_string_utf8(env, argv[1], nullptr, 0, &str_len);
  if (str_len + 1 > MAX_LEN) {
    napi_throw_error(env, "EINVAL", "Arguments too long");
  }
  std::string path;
  path.reserve(str_len + 1);
  path.resize(str_len);
  napi_get_value_string_utf8(env, argv[1], &path[0], path.capacity(), nullptr);

  napi_get_value_string_utf8(env, argv[2], nullptr, 0, &str_len);
  if (str_len + 1 > MAX_LEN) {
    napi_throw_error(env, "EINVAL", "Arguments too long");
  }
  std::string name;
  name.reserve(str_len + 1);
  name.resize(str_len);
  napi_get_value_string_utf8(env, argv[2], &name[0], name.capacity(), nullptr);

  napi_get_value_string_utf8(env, argv[3], nullptr, 0, &str_len);
  if (str_len + 1 > MAX_LEN) {
    napi_throw_error(env, "EINVAL", "Arguments too long");
  }
  std::string value;
  value.reserve(str_len + 1);
  value.resize(str_len);
  napi_get_value_string_utf8(env, argv[3], &value[0], value.capacity(), nullptr);

  if (hive == NULL) {
    napi_throw_error(env, nullptr, "Unable to open registry hive");
    return nullptr;
  }

  std::string result;
  HKEY hKey;
  if (ERROR_SUCCESS != RegCreateKeyEx(hive, path.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL)) {
    napi_throw_error(env, nullptr, "Unable to open registry key");
    return nullptr;
  }

  if (ERROR_SUCCESS != RegSetValueEx(hKey, name.c_str(), 0, REG_SZ, (BYTE*)value.c_str(), value.length() + 1)) {
    napi_throw_error(env, nullptr, "Unable to write registry key");
    return nullptr;
  }

  RegCloseKey(hKey);

  napi_value napi_result;
  napi_get_undefined(env, &napi_result);
  return napi_result;
}

napi_value DeleteRegKeyKey(napi_env env, napi_callback_info info) {
  napi_value argv[3];
  size_t argc = 3;

  napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

  // Check that we have 3 arguments - Hive, Path, Name
  if (argc < 3) {
    napi_throw_error(env, "EINVAL", "Wrong number of arguments");
    return nullptr;
  }

  // Retrieve the 3 arguments
  for (int i = 0; i < 3; i++) {
    napi_valuetype value_type;
    napi_typeof(env, argv[i], &value_type);
    if (value_type != napi_string) {
      napi_throw_error(env, "EINVAL", "Expected string");
      return nullptr;
    }
  }

  size_t str_len = 0;
  const int MAX_LEN = 16383;

  napi_get_value_string_utf8(env, argv[0], nullptr, 0, &str_len);
  if (str_len + 1 > MAX_LEN) {
    napi_throw_error(env, "EINVAL", "Arguments too long");
  }
  std::string hive_arg;
  hive_arg.reserve(str_len + 1);
  hive_arg.resize(str_len);
  napi_get_value_string_utf8(env, argv[0], &hive_arg[0], hive_arg.capacity(), nullptr);
  HKEY hive = GetHive(hive_arg);

  napi_get_value_string_utf8(env, argv[1], nullptr, 0, &str_len);
  if (str_len + 1 > MAX_LEN) {
    napi_throw_error(env, "EINVAL", "Arguments too long");
  }
  std::string path;
  path.reserve(str_len + 1);
  path.resize(str_len);
  napi_get_value_string_utf8(env, argv[1], &path[0], path.capacity(), nullptr);

  napi_get_value_string_utf8(env, argv[2], nullptr, 0, &str_len);
  if (str_len + 1 > MAX_LEN) {
    napi_throw_error(env, "EINVAL", "Arguments too long");
  }
  std::string name;
  name.reserve(str_len + 1);
  name.resize(str_len);
  napi_get_value_string_utf8(env, argv[2], &name[0], name.capacity(), nullptr);

  if (hive == NULL) {
    napi_throw_error(env, nullptr, "Unable to open registry hive");
    return nullptr;
  }

  std::string result;
  HKEY hKey;
  if (path.length() > 0) {
	if (ERROR_SUCCESS != RegCreateKeyEx(hive, path.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL)) {
      napi_throw_error(env, nullptr, "Unable to open registry key");
      return nullptr;
    }
  } else {
	hKey = hive;
  }

  if (ERROR_SUCCESS != RegDeleteKey(hKey, name.c_str())) {
    napi_throw_error(env, nullptr, "Unable to delete registry key");
    return nullptr;
  }

  if (path.length()) {
    RegCloseKey(hKey);
  }
  

  napi_value napi_result;
  napi_get_undefined(env, &napi_result);
  return napi_result;
}

napi_value DeleteRegKeyValue(napi_env env, napi_callback_info info) {
  napi_value argv[3];
  size_t argc = 3;

  napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

  // Check that we have 3 arguments - Hive, Path, Name
  if (argc < 3) {
    napi_throw_error(env, "EINVAL", "Wrong number of arguments");
    return nullptr;
  }

  // Retrieve the 3 arguments
  for (int i = 0; i < 3; i++) {
    napi_valuetype value_type;
    napi_typeof(env, argv[i], &value_type);
    if (value_type != napi_string) {
      napi_throw_error(env, "EINVAL", "Expected string");
      return nullptr;
    }
  }

  size_t str_len = 0;
  const int MAX_LEN = 16383;

  napi_get_value_string_utf8(env, argv[0], nullptr, 0, &str_len);
  if (str_len + 1 > MAX_LEN) {
    napi_throw_error(env, "EINVAL", "Arguments too long");
  }
  std::string hive_arg;
  hive_arg.reserve(str_len + 1);
  hive_arg.resize(str_len);
  napi_get_value_string_utf8(env, argv[0], &hive_arg[0], hive_arg.capacity(), nullptr);
  HKEY hive = GetHive(hive_arg);

  napi_get_value_string_utf8(env, argv[1], nullptr, 0, &str_len);
  if (str_len + 1 > MAX_LEN) {
    napi_throw_error(env, "EINVAL", "Arguments too long");
  }
  std::string path;
  path.reserve(str_len + 1);
  path.resize(str_len);
  napi_get_value_string_utf8(env, argv[1], &path[0], path.capacity(), nullptr);

  napi_get_value_string_utf8(env, argv[2], nullptr, 0, &str_len);
  if (str_len + 1 > MAX_LEN) {
    napi_throw_error(env, "EINVAL", "Arguments too long");
  }
  std::string name;
  name.reserve(str_len + 1);
  name.resize(str_len);
  napi_get_value_string_utf8(env, argv[2], &name[0], name.capacity(), nullptr);

  if (hive == NULL) {
    napi_throw_error(env, nullptr, "Unable to open registry hive");
    return nullptr;
  }

  std::string result;
  HKEY hKey;
  if (ERROR_SUCCESS != RegCreateKeyEx(hive, path.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL)) {
    napi_throw_error(env, nullptr, "Unable to open registry key");
    return nullptr;
  }

  if (ERROR_SUCCESS != RegDeleteValue(hKey, name.c_str())) {
    napi_throw_error(env, nullptr, "Unable to delete registry value");
    return nullptr;
  }

  RegCloseKey(hKey);

  napi_value napi_result;
  napi_get_undefined(env, &napi_result);
  return napi_result;
}

napi_value Init(napi_env env, napi_value exports) {
  napi_value enumRegKeyKeys;
  napi_create_function(env, "EnumRegKeyKeys", NAPI_AUTO_LENGTH,
					   EnumRegKeyKeys, NULL, &enumRegKeyKeys);
  napi_set_named_property(env, exports, "EnumRegKeyKeys", enumRegKeyKeys);

  napi_value enumRegKeyValues;
  napi_create_function(env, "EnumRegKeyValues", NAPI_AUTO_LENGTH,
					   EnumRegKeyValues, NULL, &enumRegKeyValues);
  napi_set_named_property(env, exports, "EnumRegKeyValues", enumRegKeyValues);

  napi_value getStringRegKey;
  napi_create_function(env, "GetStringRegKey", NAPI_AUTO_LENGTH,
                       GetStringRegKey, NULL, &getStringRegKey);
  napi_set_named_property(env, exports, "GetStringRegKey", getStringRegKey);

  napi_value setStringRegKey;
  napi_create_function(env, "SetStringRegKey", NAPI_AUTO_LENGTH,
					   SetStringRegKey, NULL, &setStringRegKey);
  napi_set_named_property(env, exports, "SetStringRegKey", setStringRegKey);

  napi_value deleteRegKeyKey;
  napi_create_function(env, "DeleteRegKeyKey", NAPI_AUTO_LENGTH,
					   DeleteRegKeyKey, NULL, &deleteRegKeyKey);
  napi_set_named_property(env, exports, "DeleteRegKeyKey", deleteRegKeyKey);

  napi_value deleteRegKeyValue;
  napi_create_function(env, "DeleteRegKeyValue", NAPI_AUTO_LENGTH,
					   DeleteRegKeyValue, NULL, &deleteRegKeyValue);
  napi_set_named_property(env, exports, "DeleteRegKeyValue", deleteRegKeyValue);

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init);
} // namespace