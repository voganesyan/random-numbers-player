#pragma once
#include <QLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QSplineSeries>
#include <QThread>
#include <QValueAxis>

class SummaryModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void extend_line(int y);
    void update_table();
    void clear_table();

private:
    QLayout* create_plot_widgets();
    QLayout* create_table_widgets();

    QPushButton* m_play_button = nullptr;
    QSplineSeries* m_line = nullptr;
    QValueAxis* m_axis_x = nullptr;
    SummaryModel* m_summary_model = nullptr;

    QThread m_thread;
};
