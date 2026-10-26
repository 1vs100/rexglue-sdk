/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2022 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 *
 * @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
 */

#include <rex/ui/surface_win.h>

#if REX_PLATFORM_WINRT
#include <cmath>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Graphics.Display.h>
#include <winrt/Windows.UI.Core.h>
#endif

namespace rex {
namespace ui {

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP | WINAPI_PARTITION_GAMES) && !REX_PLATFORM_WINRT
bool Win32HwndSurface::GetSizeImpl(uint32_t& width_out, uint32_t& height_out) const {
  RECT client_rect;
  if (!GetClientRect(hwnd(), &client_rect)) {
    return false;
  }
  // GetClientRect returns a rectangle with 0 origin.
  width_out = uint32_t(client_rect.right);
  height_out = uint32_t(client_rect.bottom);
  return true;
}
#endif

#if REX_PLATFORM_WINRT
bool WinRTCoreWindowSurface::GetSizeImpl(uint32_t& width_out, uint32_t& height_out) const {
  winrt::Windows::UI::Core::CoreWindow window{nullptr};
  winrt::copy_from_abi(window, core_window_);
  // Bounds are in view pixels, the swap chain needs physical pixels.
  winrt::Windows::Foundation::Rect bounds = window.Bounds();
  double scale = winrt::Windows::Graphics::Display::DisplayInformation::GetForCurrentView()
                     .RawPixelsPerViewPixel();
  width_out = uint32_t(std::lround(bounds.Width * scale));
  height_out = uint32_t(std::lround(bounds.Height * scale));
  return true;
}
#endif

}  // namespace ui
}  // namespace rex
