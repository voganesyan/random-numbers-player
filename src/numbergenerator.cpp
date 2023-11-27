#include "numbergenerator.h"

#include <QCoreApplication>
#include <QThread>

NumberGenerator::NumberGenerator(
    int max_number, int time_interval_ms, QObject* parent) :
  m_max_number(max_number),
  m_time_interval_ms(time_interval_ms),
  QObject(parent){};

void NumberGenerator::set_state(bool is_generating)
{
    if (m_is_generating == is_generating)
    {
        return;
    }

    m_is_generating = is_generating;
    emit state_changed(is_generating);

    while (m_is_generating)
    {
        int number = std::rand() % m_max_number;
        emit generated(number);
        QCoreApplication::processEvents();
        QThread::msleep(m_time_interval_ms);
    }
}
