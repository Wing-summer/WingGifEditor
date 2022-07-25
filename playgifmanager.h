#ifndef PLAYGIFMANAGER_H
#define PLAYGIFMANAGER_H

#include <QMutex>
#include <QObject>
#include <QTimer>

class PlayGifManager : public QObject {
  Q_OBJECT
public:
  explicit PlayGifManager(QObject *parent = nullptr);
  void setTickIntervals(QList<int> intervals);
  void play(int index);
  void stop();

signals:
  void tick(int index);

private:
  void tick_p();

private:
  int curpos = 0;
  QMutex mutex;
  bool isstop = true;
  QList<int> m_intervals;
};

#endif // PLAYGIFMANAGER_H
