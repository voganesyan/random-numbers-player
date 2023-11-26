#include "mainwindow.h"
#include "numbergenerator.h"

#include <QChartView>
#include <QHeaderView>
#include <bit>

static constexpr int MAX_NUMBER = 100;
static constexpr int TIME_INTERVAL_MS = 500;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    auto chart_layout = init_chart_widgets();
    auto table_layout = init_table_widgets();
    auto main_layout = new QHBoxLayout();
    main_layout->addLayout(chart_layout, 1);
    main_layout->addLayout(table_layout, 0);

    auto main_widget = new QWidget();
    main_widget->setLayout(main_layout);
    setCentralWidget(main_widget);

    init_number_generator();
}

MainWindow::~MainWindow()
{
    m_start_button->setChecked(false);
    m_number_generator_thread.quit();
    m_number_generator_thread.wait();
}

QLayout* MainWindow::init_chart_widgets()
{
    // Create chart & line
    m_line = new QSplineSeries();
    m_axis_x = new QValueAxis();
    auto axis_y = new QValueAxis();
    axis_y->setMax(MAX_NUMBER);

    m_axis_x->setTitleText("Count");
    axis_y->setTitleText("Value");

    auto chart = new QChart();
    chart->addSeries(m_line);
    chart->addAxis(m_axis_x, Qt::AlignBottom);
    chart->addAxis(axis_y, Qt::AlignLeft);
    chart->legend()->hide();

    m_line->attachAxis(m_axis_x);
    m_line->attachAxis(axis_y);

    auto chart_view = new QChartView(chart, nullptr);
    chart_view->setRenderHint(QPainter::Antialiasing);

    // Create buttons
    m_start_button = new QPushButton("Start/Stop");
    m_start_button->setCheckable(true);
    auto reset_button = new QPushButton("Reset");
    connect(reset_button, &QPushButton::clicked, m_line, &QLineSeries::clear);

    // Add all widgets together
    auto buttons_layout = new QHBoxLayout();
    buttons_layout->addWidget(m_start_button);
    buttons_layout->addWidget(reset_button);
    buttons_layout->addStretch();

    auto main_layout = new QVBoxLayout();
    main_layout->addWidget(chart_view);
    main_layout->addLayout(buttons_layout);

    return main_layout;
}

QLayout* MainWindow::init_table_widgets()
{
    // Create table
    m_table = new QTableWidget();
    m_table->setColumnCount(2);
    m_table->setHorizontalHeaderLabels({"Count", "Summary"});
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Create buttons
    auto save_button = new QPushButton("Save");
    auto clear_button = new QPushButton("Clear");
    connect(
        save_button, &QPushButton::clicked, this, &MainWindow::add_table_row);
    connect(clear_button, &QPushButton::clicked, this, &MainWindow::clear_table);

    // Add all widgets together
    auto buttons_layout = new QHBoxLayout();
    buttons_layout->addWidget(save_button);
    buttons_layout->addWidget(clear_button);
    buttons_layout->addStretch();
    auto main_layout = new QVBoxLayout();
    main_layout->addWidget(m_table);
    main_layout->addLayout(buttons_layout);
    return main_layout;
}

void MainWindow::init_number_generator()
{
    auto generator = new NumberGenerator(MAX_NUMBER, TIME_INTERVAL_MS);
    generator->moveToThread(&m_number_generator_thread);

    connect(
        &m_number_generator_thread,
        &QThread::finished,
        generator,
        &QObject::deleteLater);
    connect(
        m_start_button,
        &QPushButton::toggled,
        generator,
        &NumberGenerator::set_state);
    connect(
        generator, &NumberGenerator::generated, this, &MainWindow::extend_line);
    m_number_generator_thread.start();
}

void MainWindow::extend_line(int y)
{
    m_line->append(m_line->count(), y);
    m_axis_x->setMax(std::bit_ceil(static_cast<unsigned int>(m_line->count())));
}

void MainWindow::add_table_row()
{
    // Summarize
    const int count = m_line->count();
    int sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += m_line->at(i).y();
    }

    // Append row
    const int row_pos = m_table->rowCount();
    m_table->insertRow(row_pos);
    m_table->setItem(row_pos, 0, new QTableWidgetItem(QString::number(count)));
    m_table->setItem(row_pos, 1, new QTableWidgetItem(QString::number(sum)));
}

void MainWindow::clear_table()
{
    m_table->setRowCount(0);
}
