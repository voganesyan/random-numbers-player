#pragma once
#include <QLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QSplineSeries>
#include <QTableWidget>
#include <QThread>
#include <QValueAxis>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void extend_line(int y);
    void add_table_row();
    void clear_table();

private:
    QLayout* create_plot_widgets();
    QLayout* create_table_widgets();
    void create_number_generator();

    QPushButton* m_start_button = nullptr;
    QSplineSeries* m_line = nullptr;
    QValueAxis* m_axis_x = nullptr;
    QTableWidget* m_table = nullptr;

    QThread m_thread;
};
