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

#ifndef PANORAMA_STITCHER_H
#define PANORAMA_STITCHER_H

#include <QThread>
#include <QMutex>
#include <vector>
#include "stitcher/stitcher.h"
#include <QTimer>
#include <QSize>
#include <glib.h>
#include "panorama.h"

class PanoramaStitcher : public QThread, private Stitcher {
  Q_OBJECT

public:
  PanoramaStitcher(std::vector<guint8 *>& frames, const QSize& size, const QString& output,
		   bool keepFrames, QObject *parent = 0);
  ~PanoramaStitcher();

  void stop();

  int progress();

protected:
  void run();

signals:
  void progressChanged();
  void done();
  void error(const Panorama::Error& errorCode);

private:
  QString m_output;
  std::vector<guint8 *> m_frames;
  bool m_running;
  bool m_keepFrames;
  QTimer m_timer;
  int m_alignProgress;
  QSize m_size;
  QMutex m_progressLock;
};

#endif /* PANORAMA_STITCHER_H */
