/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012 Mohammed Sameer <msameer@foolab.org>
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

#include "camera.h"
#include "qtcamera.h"
#include "qtcamdevice.h"
#include "qtcammode.h"
#include "qtcamimagemode.h"
#include "qtcamvideomode.h"
#include "qtcamgraphicsviewfinder.h"
#include "qtcamconfig.h"

// TODO: a viewfinder class that inherits QDeclarativeItem

Camera::Camera(QDeclarativeItem *parent) :
  QDeclarativeItem(parent),
  m_cam(new QtCamera(this)),
  m_dev(0),
  m_vf(new QtCamGraphicsViewfinder(m_cam->config(), this)),
  m_mode(Camera::ImageMode) {

  // TODO:
}

Camera::~Camera() {
  // TODO:
}

void Camera::componentComplete() {
  QDeclarativeItem::componentComplete();

  if (m_id.isValid()) {
    QVariant oldId = m_id;
    m_id = QVariant();
    setDeviceId(oldId);
  }

  emit deviceCountChanged();
}

int Camera::deviceCount() const {
  return m_cam ? m_cam->devices().size() : 0;
}

QString Camera::deviceName(int index) const {
  return m_cam->devices().at(index).first;
}

QVariant Camera::deviceId(int index) const {
  return m_cam->devices().at(index).second;
}

void Camera::setDeviceId(const QVariant& id) {
  if (id == m_id) {
    return;
  }

  if (!isComponentComplete()) {
    m_id = id;
    emit deviceIdChanged();
    return;
  }

  if (m_dev && m_dev->stop()) {
    delete m_dev;
  }
  else if (m_dev) {
    qWarning() << "Failed to stop device";
    return;
  }

  m_dev = m_cam->device(id, this);

  m_id = id;

  m_vf->setDevice(m_dev);

  QObject::connect(m_dev, SIGNAL(runningStateChanged(bool)),
		      this, SIGNAL(runningStateChanged()));
  QObject::connect(m_dev, SIGNAL(idleStateChanged(bool)), this, SIGNAL(idleStateChanged()));
  QObject::connect(m_dev, SIGNAL(error(const QString&, int, const QString&)),
		   this, SIGNAL(error(const QString&, int, const QString&)));

  emit deviceIdChanged();
  emit deviceChanged();
}

QVariant Camera::deviceId() const {
  return m_id;
}

void Camera::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {
  QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);

  m_vf->setGeometry(newGeometry);
}

QtCamDevice *Camera::device() const {
  return m_dev;
}

void Camera::setMode(const Camera::CameraMode& mode) {
  if (m_mode == mode) {
    return;
  }

  m_mode = mode;

  if (!m_dev) {
    return;
  }

  if (m_dev->isRunning()) {
    applyMode();
  }

  emit modeChanged();
}

Camera::CameraMode Camera::mode() {
  return m_mode;
}

bool Camera::start() {
  if (!m_dev) {
    return false;
  }

  applyMode();

  return m_dev->start();
}

bool Camera::stop() {
  if (m_dev) {
    return m_dev->stop();
  }

  return true;
}

bool Camera::isIdle() {
  return m_dev ? m_dev->isIdle() : true;
}

bool Camera::isRunning() {
  return m_dev ? m_dev->isRunning() : false;
}

void Camera::applyMode() {
  if (m_mode == Camera::VideoMode && m_dev->activeMode() != m_dev->videoMode()) {
    m_dev->videoMode()->activate();
  }
  else if (m_mode == Camera::ImageMode && m_dev->activeMode() != m_dev->imageMode()) {
    m_dev->imageMode()->activate();
  }
}

QString Camera::imageSuffix() const {
  return m_cam->config()->imageSuffix();
}

QString Camera::videoSuffix() const {
  return m_cam->config()->videoSuffix();
}
