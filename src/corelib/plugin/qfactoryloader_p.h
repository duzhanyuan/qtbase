/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QFACTORYLOADER_P_H
#define QFACTORYLOADER_P_H

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

#include "QtCore/qglobal.h"
#ifndef QT_NO_QOBJECT

#include "QtCore/qobject.h"
#include "QtCore/qstringlist.h"
#include "QtCore/qjsonobject.h"
#include "QtCore/qmap.h"
#include "private/qlibrary_p.h"

QT_BEGIN_NAMESPACE

class QFactoryLoaderPrivate;
class Q_CORE_EXPORT QFactoryLoader : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QFactoryLoader)

public:
    explicit QFactoryLoader(const char *iid,
                   const QString &suffix = QString(),
                   Qt::CaseSensitivity = Qt::CaseSensitive);

#ifndef QT_NO_LIBRARY
    ~QFactoryLoader();

    void update();
    static void refreshAll();

#if defined(Q_OS_UNIX) && !defined (Q_OS_MAC)
    QLibraryPrivate *library(const QString &key) const;
#endif // Q_OS_UNIX && !Q_OS_MAC
#endif // !QT_NO_LIBRARY

    QMultiMap<int, QString> keyMap() const;
    int indexOf(const QString &needle) const;

    QList<QJsonObject> metaData() const;
    QObject *instance(int index) const;
};

#ifdef Q_COMPILER_VARIADIC_TEMPLATES

template <class PluginInterface, class FactoryInterface, typename ...Args>
PluginInterface *qLoadPlugin(const QFactoryLoader *loader, const QString &key, Args &&...args)
{
    const int index = loader->indexOf(key);
    if (index != -1) {
        QObject *factoryObject = loader->instance(index);
        if (FactoryInterface *factory = qobject_cast<FactoryInterface *>(factoryObject))
            if (PluginInterface *result = factory->create(key, std::forward<Args>(args)...))
                return result;
    }
    return nullptr;
}

#else

template <class PluginInterface, class FactoryInterface>
    PluginInterface *qLoadPlugin(const QFactoryLoader *loader, const QString &key)
{
    const int index = loader->indexOf(key);
    if (index != -1) {
        QObject *factoryObject = loader->instance(index);
        if (FactoryInterface *factory = qobject_cast<FactoryInterface *>(factoryObject))
            if (PluginInterface *result = factory->create(key))
                return result;
    }
    return 0;
}

template <class PluginInterface, class FactoryInterface, class P1>
PluginInterface *qLoadPlugin(const QFactoryLoader *loader,
                              const QString &key,
                              P1 &&p1)
{
    const int index = loader->indexOf(key);
    if (index != -1) {
        QObject *factoryObject = loader->instance(index);
        if (FactoryInterface *factory = qobject_cast<FactoryInterface *>(factoryObject))
            if (PluginInterface *result = factory->create(key, std::forward<P1>(p1)))
                return result;
    }
    return 0;
}

template <class PluginInterface, class FactoryInterface, class P1, class P2>
PluginInterface *qLoadPlugin(const QFactoryLoader *loader,
                              const QString &key,
                              P1 &&p1, P2 &&p2)
{
    const int index = loader->indexOf(key);
    if (index != -1) {
        QObject *factoryObject = loader->instance(index);
        if (FactoryInterface *factory = qobject_cast<FactoryInterface *>(factoryObject))
            if (PluginInterface *result = factory->create(key, std::forward<P1>(p1), std::forward<P2>(p2)))
                return result;
    }
    return 0;
}

template <class PluginInterface, class FactoryInterface, class P1, class P2, class P3>
PluginInterface *qLoadPlugin(const QFactoryLoader *loader,
                              const QString &key,
                              P1 &&p1, P2 &&p2, P3 &&p3)
{
    const int index = loader->indexOf(key);
    if (index != -1) {
        QObject *factoryObject = loader->instance(index);
        if (FactoryInterface *factory = qobject_cast<FactoryInterface *>(factoryObject))
            if (PluginInterface *result = factory->create(key, std::forward<P1>(p1), std::forward<P2>(p2), std::forward<P3>(p3)))
                return result;
    }
    return 0;
}

#endif

template <class PluginInterface, class FactoryInterface, typename Arg>
Q_DECL_DEPRECATED PluginInterface *qLoadPlugin1(const QFactoryLoader *loader, const QString &key, Arg &&arg)
{ return qLoadPlugin(loader, key, std::forward<Arg>(arg)); }

QT_END_NAMESPACE

#endif // QT_NO_QOBJECT

#endif // QFACTORYLOADER_P_H
