/**************************************************************************
** This file is part of LiteIDE
**
** Copyright (c) 2011-2016 LiteIDE Team. All rights reserved.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** In addition, as a special exception,  that plugins developed for LiteIDE,
** are allowed to remain closed sourced and can be distributed under any license .
** These rights are included in the file LGPL_EXCEPTION.txt in this package.
**
**************************************************************************/
// Module: processex.cpp
// Creator: visualfc <visualfc@gmail.com>

#include "processex.h"
#include <QMap>
#ifdef Q_OS_WIN
#include <windows.h>
//lite_memory_check_begin
#if defined(WIN32) && defined(_MSC_VER) &&  defined(_DEBUG)
     #define _CRTDBG_MAP_ALLOC
     #include <stdlib.h>
     #include <crtdbg.h>
     #define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
     #define new DEBUG_NEW
#endif
//lite_memory_check_end
#endif



QString ProcessEx::exitStatusText(int code, QProcess::ExitStatus status)
{
    static QString text;
    switch (status) {
    case QProcess::NormalExit:
        text = tr("process exited with code %1").arg(code);
        break;
    case QProcess::CrashExit:
        text = tr("process crashed or was terminated");
        break;
    default:
        text = tr("process exited with an unknown status");
    }
    return text;
}

QString ProcessEx::processErrorText(QProcess::ProcessError code)
{
    static QString text;
    switch (code) {
    case QProcess::FailedToStart:
        text = tr("process failed to start");
        break;
    case QProcess::Crashed:
        text = tr("process crashed or was terminated while running");
        break;
    case QProcess::Timedout:
        text = tr("timed out waiting for process");
        break;
    case QProcess::ReadError:
        text = tr("couldn't read from the process");
        break;
    case QProcess::WriteError:
        text = tr("couldn't write to the process");
        break;
    case QProcess::UnknownError:
    default:
        text = tr("an unknown error occurred");
    }
    return text;
}

ProcessEx::ProcessEx(QObject *parent)
    : Process(parent), m_suppressFinish(false)
{
    connect(this,SIGNAL(stateChanged(QProcess::ProcessState)),this,SLOT(slotStateChanged(QProcess::ProcessState)));
    connect(this,SIGNAL(readyReadStandardOutput()),this,SLOT(slotReadOutput()));
    connect(this,SIGNAL(readyReadStandardError()),this,SLOT(slotReadError()));
    connect(this,SIGNAL(error(QProcess::ProcessError)),this,SLOT(slotError(QProcess::ProcessError)));
    connect(this,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slotFinished(int,QProcess::ExitStatus)));
}

void ProcessEx::slotStateChanged(QProcess::ProcessState newState)
{
    if (newState == QProcess::Starting) {
        m_suppressFinish = false;
    }
}

void ProcessEx::slotError(QProcess::ProcessError error)
{
    switch (error) {

    // Suppress only if the process has stopped
    default:
    case QProcess::UnknownError:
        if (this->isRunning()) break;
        // Fall through

    // Always suppress
    case QProcess::FailedToStart:
    case QProcess::Crashed:
        m_suppressFinish = true;
        emit extFinish(true,-1,this->processErrorText(error));
        break;

    // Never suppress
    case QProcess::Timedout:
    case QProcess::WriteError:
    case QProcess::ReadError:
        break;
    }
}

void ProcessEx::slotFinished(int code,QProcess::ExitStatus status)
{
    if (!m_suppressFinish) {
        emit extFinish(false,code,this->exitStatusText(code,status));
    }
}

void ProcessEx::slotReadOutput()
{
    emit extOutput(this->readAllStandardOutput(),false);
}

void ProcessEx::slotReadError()
{
    emit extOutput(this->readAllStandardError(),true);
}

Process::Process(QObject *parent) : QProcess(parent)
{

}

Process::~Process()
{
    stop(1);
}

bool Process::isRunning() const
{
    return this->state() == QProcess::Running;
}

bool Process::isStop() const
{
    return this->state() == QProcess::NotRunning;
}

void Process::stop(int ms)
{
    if (!isStop()) {
        terminate();
        closeReadChannel(QProcess::StandardOutput);
        closeReadChannel(QProcess::StandardError);
        waitForFinished(ms);
        if (!isStop()) {
            kill();
        }
    }
}

void Process::startEx(const QString &cmd, const QString &args)
{
#ifdef Q_OS_WIN
    this->setNativeArguments(args);
    if (cmd.indexOf(" ")) {
        this->start("\""+cmd+"\"");
    } else {
        this->start(cmd);
    }
#else
    this->start(cmd+" "+args);
#endif
}

bool Process::startDetachedEx(const QString &cmd, const QStringList &args)
{
#ifdef Q_OS_WIN
    return (intptr_t)ShellExecuteW(NULL, NULL, (LPCWSTR)cmd.toStdWString().data(), (LPCWSTR)args.join(" ").toStdWString().data(), NULL, SW_HIDE) > 32;
#else
    return QProcess::startDetached(cmd, args);
#endif
}

void Process::setUserData(int id, const QVariant &data)
{
    m_idVarMap.insert(id,data);
}

QVariant Process::userData(int id) const
{
    return m_idVarMap.value(id);
}
