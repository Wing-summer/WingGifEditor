#include "playgifmanager.h"
#include <QtConcurrent/QtConcurrent>

PlayGifManager::PlayGifManager(QObject *parent) : QObject(parent) {}

void PlayGifManager::setTickIntervals(QList<int> intervals) {
  m_intervals.swap(intervals);
}

void PlayGifManager::play(int index) {
  if (index < 0 || index >= m_intervals.count())
    return;
  curpos = index;
  isstop = false;
  QtConcurrent::run([=] {
    while (true) {
      if (isstop) {
        mutex.unlock();
        break;
      }
      if (mutex.tryLock(10)) {
        QTimer::singleShot(m_intervals[curpos], this, &PlayGifManager::tick_p);
      }
    }
  });
}

void PlayGifManager::stop() { isstop = true; }

void PlayGifManager::tick_p() {
  tick(curpos++);
  if (curpos >= m_intervals.count()) {
    curpos = 0;
  }
  mutex.unlock();
}
