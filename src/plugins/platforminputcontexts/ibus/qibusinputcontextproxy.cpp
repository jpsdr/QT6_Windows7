/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -N -p qibusinputcontextproxy -c QIBusInputContextProxy interfaces/org.freedesktop.IBus.InputContext.xml
 *
 * qdbusxml2cpp is Copyright (C) 2015 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "qibusinputcontextproxy.h"

/*
 * Implementation of interface class QIBusInputContextProxy
 */

QIBusInputContextProxy::QIBusInputContextProxy(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

QIBusInputContextProxy::~QIBusInputContextProxy()
{
}

#include "moc_qibusinputcontextproxy.cpp"
