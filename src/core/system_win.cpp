/**
 * @file        core/system_win.cpp
 * @brief       Windows implementations of rex/system.h platform helpers.
 *
 * @copyright   Copyright (c) 2026 Tom Clay
 * @license     BSD 3-Clause License
 */

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <rex/logging.h>
#include <rex/string.h>
#include <rex/system.h>

namespace rex {

void ShowSimpleMessageBox(SimpleMessageBoxType type, std::string_view message) {
  UINT flags = MB_OK | MB_TOPMOST | MB_SETFOREGROUND;
  const wchar_t* title = L"ReXGlue";
  switch (type) {
    case SimpleMessageBoxType::Help:
      flags |= MB_ICONINFORMATION;
      break;
    case SimpleMessageBoxType::Warning:
      flags |= MB_ICONWARNING;
      break;
    case SimpleMessageBoxType::Error:
      flags |= MB_ICONERROR;
      break;
  }
#if REX_PLATFORM_WINRT
  // UWP has no MessageBox.
  (void)flags;
  (void)title;
  if (type == SimpleMessageBoxType::Error) {
    REXLOG_ERROR("{}", message);
  } else if (type == SimpleMessageBoxType::Warning) {
    REXLOG_WARN("{}", message);
  } else {
    REXLOG_INFO("{}", message);
  }
#else
  auto wide = rex::string::to_utf16(message);
  ::MessageBoxW(nullptr, reinterpret_cast<const wchar_t*>(wide.c_str()), title, flags);
#endif
}

}  // namespace rex
