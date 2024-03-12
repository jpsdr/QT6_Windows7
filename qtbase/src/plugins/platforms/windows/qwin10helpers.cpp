// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwin10helpers.h"

QT_BEGIN_NAMESPACE

// Return tablet mode, note: Does not work for GetDesktopWindow().
bool qt_windowsIsTabletMode(HWND hwnd)
{
        return false;
}

QT_END_NAMESPACE
