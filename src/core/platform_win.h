/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2015 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 *
 * @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
 */

#pragma once

// NOTE: This file is auto-included by platform.h on Windows.
// It contains Windows-specific headers and definitions.

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <windowsx.h>

#include <ObjBase.h>
#include <SDKDDKVer.h>
#include <bcrypt.h>
#if !REX_PLATFORM_WINRT
#include <dwmapi.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <shobjidl.h>
#include <tpcshrd.h>
#endif
#if REX_PLATFORM_WINRT
// Exported to store apps through WindowsApp.lib, but declared only for the
// desktop partition.
extern "C" {
WINBASEAPI PVOID WINAPI AddVectoredExceptionHandler(ULONG, PVECTORED_EXCEPTION_HANDLER);
WINBASEAPI ULONG WINAPI RemoveVectoredExceptionHandler(PVOID);
WINBASEAPI BOOL WINAPI TerminateThread(HANDLE, DWORD);
}

inline HANDLE CreateFileW(LPCWSTR file_name, DWORD desired_access, DWORD share_mode,
                          LPSECURITY_ATTRIBUTES security_attributes, DWORD creation_disposition,
                          DWORD flags_and_attributes, HANDLE template_file) {
  CREATEFILE2_EXTENDED_PARAMETERS params = {sizeof(params)};
  params.dwFileAttributes = flags_and_attributes & 0x0000FFFF;
  params.dwFileFlags = flags_and_attributes & 0xFFF00000;
  // SECURITY_SQOS_PRESENT shares its bit with FILE_FLAG_OPEN_NO_RECALL.
  if (flags_and_attributes & SECURITY_SQOS_PRESENT) {
    params.dwFileFlags &= ~SECURITY_SQOS_PRESENT;
    params.dwSecurityQosFlags = flags_and_attributes & SECURITY_VALID_SQOS_FLAGS;
  }
  params.lpSecurityAttributes = security_attributes;
  params.hTemplateFile = template_file;
  return CreateFile2(file_name, desired_access, share_mode, creation_disposition, &params);
}
#endif
#undef DeleteBitmap
#undef DeleteFile
#undef GetFirstChild
