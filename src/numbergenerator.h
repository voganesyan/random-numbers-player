#pragma once
#include <QObject>

class NumberGenerator : public QObject {
  Q_OBJECT
public:
  NumberGenerator(int max_number, int time_interval_ms,
                  QObject *parent = nullptr);

public slots:
  void set_state(bool is_generating);

signals:
  void generated(int number);

private:
  bool m_is_generating = false;
  const int m_max_number;
  const int m_time_interval_ms;
};
