#pragma once
/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2021 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 *
 * @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
 */

#include <cstdint>
#include <memory>

#include <rex/ui/surface.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <unknwn.h>

#include <rex/platform.h>

namespace rex {
namespace ui {

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP | WINAPI_PARTITION_GAMES) && !REX_PLATFORM_WINRT
class Win32HwndSurface final : public Surface {
 public:
  explicit Win32HwndSurface(HINSTANCE hinstance, HWND hwnd) : hinstance_(hinstance), hwnd_(hwnd) {}
  TypeIndex GetType() const override { return kTypeIndex_Win32Hwnd; }
  HINSTANCE hinstance() const { return hinstance_; }
  HWND hwnd() const { return hwnd_; }

 protected:
  bool GetSizeImpl(uint32_t& width_out, uint32_t& height_out) const override;

 private:
  HINSTANCE hinstance_;
  HWND hwnd_;
};
#endif

#if REX_PLATFORM_WINRT
// The CoreWindow is owned by the windowing layer and must outlive the surface.
class WinRTCoreWindowSurface final : public Surface {
 public:
  explicit WinRTCoreWindowSurface(::IUnknown* core_window) : core_window_(core_window) {}
  TypeIndex GetType() const override { return kTypeIndex_WinRTCoreWindow; }
  ::IUnknown* core_window() const { return core_window_; }

 protected:
  bool GetSizeImpl(uint32_t& width_out, uint32_t& height_out) const override;

 private:
  ::IUnknown* core_window_;
};
#endif

}  // namespace ui
}  // namespace rex
