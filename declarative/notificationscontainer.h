// -*- c++ -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2014 Mohammed Sameer <msameer@foolab.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef NOTIFICATIONS_CNTAINER_H
#define NOTIFICATIONS_CNTAINER_H

#include <QObject>
#include <QPointer>
#include <QMutex>

class QtCamDevice;
class Sounds;

class NotificationsContainer : public QObject {
  Q_OBJECT

public:
  NotificationsContainer(QObject *parent = 0);
  ~NotificationsContainer();

  void setDevice(QtCamDevice *dev);
  Sounds *sounds() const;
  bool setSounds(Sounds *sounds);

private slots:
  void imageCaptureStarted();
  void imageCaptureEnded();
  void videoRecordingStarted();
  void videoRecordingEnded();
  void autoFocusAcquired();
  void autoFocusFailed();

private:
  QPointer<QtCamDevice> m_dev;
  Sounds *m_sounds;
  QMutex m_mutex;
};

#endif /* NOTIFICATIONS_CNTAINER_H */
