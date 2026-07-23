// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QT_WINRTBASE_P_H
#define QT_WINRTBASE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qglobal.h>

#if QT_CONFIG(cpp_winrt)
#  include <winrt/base.h>

#  if defined(_MSC_VER)
// Windows 7 backport: resolve the WinRT entry points at run time.
//
// RoGetActivationFactory() and RoOriginateLanguageException() live in
// combase.dll, which only exists since Windows 8. C++/WinRT calls them through
// its own WINRT_IMPL_* symbols and binds those to the real functions with an
// /alternatename linker directive, so a build with QT_FEATURE_cpp_winrt enabled
// ends up statically importing api-ms-win-core-winrt-l1-1-0.dll and
// api-ms-win-core-winrt-error-l1-1-1.dll. Neither one is redistributable, and
// on Windows 7 the image loader therefore fails before main() is reached - the
// run-time version checks the rest of this backport relies on never get a
// chance to run.
//
// /alternatename only applies to symbols that are not defined anywhere else, so
// defining the WINRT_IMPL_* entry points here takes precedence over it and the
// static imports disappear. On Windows 8 and later the real functions are found
// through combase.dll and behaviour is unchanged; on Windows 7 the lookup fails
// and the callers fall back to the plain Win32 API paths they already have.

namespace QtWinRTBackport {

// LOAD_LIBRARY_SEARCH_SYSTEM32 keeps a rogue combase.dll sitting next to the
// executable from being loaded in place of the system one.
inline void *resolveCombaseSymbol(const char *symbol) noexcept
{
    static void *const library = WINRT_IMPL_LoadLibraryExW(L"combase.dll", nullptr, 0x00000800);
    if (!library)
        return nullptr;
    return WINRT_IMPL_GetProcAddress(library, symbol);
}

} // namespace QtWinRTBackport

extern "C" {

inline int32_t __stdcall WINRT_IMPL_RoGetActivationFactory(void *classId, winrt::guid const &iid,
                                                           void **factory) noexcept
{
    using FunctionPointer = int32_t(__stdcall *)(void *, winrt::guid const &, void **) noexcept;
    static const auto resolved = reinterpret_cast<FunctionPointer>(
            QtWinRTBackport::resolveCombaseSymbol("RoGetActivationFactory"));
    if (!resolved) {
        if (factory)
            *factory = nullptr;
        // Deliberately not error_not_initialized: that would send C++/WinRT
        // down its CoIncrementMTAUsage retry path for nothing.
        return static_cast<int32_t>(0x80040154); // REGDB_E_CLASSNOTREG
    }
    return resolved(classId, iid, factory);
}

inline int32_t __stdcall WINRT_IMPL_RoOriginateLanguageException(int32_t error, void *message,
                                                                 void *exception) noexcept
{
    using FunctionPointer = int32_t(__stdcall *)(int32_t, void *, void *) noexcept;
    static const auto resolved = reinterpret_cast<FunctionPointer>(
            QtWinRTBackport::resolveCombaseSymbol("RoOriginateLanguageException"));
    // Reporting the error is best effort, but WINRT_VERIFY() must not trip in a
    // debug build, so report success when there is nowhere to report it to.
    if (!resolved)
        return 1;
    return resolved(error, message, exception);
}

} // extern "C"
#  endif // defined(_MSC_VER)

#  include <QtCore/private/qfactorycacheregistration_p.h>
// Workaround for Windows SDK bug.
// See https://github.com/microsoft/Windows.UI.Composition-Win32-Samples/issues/47
namespace winrt::impl
{
    template <typename Async>
    auto wait_for(Async const& async, Windows::Foundation::TimeSpan const& timeout);
}
// See https://learn.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/faq#how-do-i-resolve-ambiguities-with-getcurrenttime-and-or-try-
// for more workarounds.
#endif // QT_CONFIG(cpp/winrt)

#endif // QT_WINRTBASE_P_H
