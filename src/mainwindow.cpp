#include "mainwindow.h"
#include "numbergenerator.h"
#include <QChartView>
#include <QLayout>
#include <QTableWidget>

static constexpr int MAX_NUMBER = 100;
static constexpr int TIME_INTERVAL_MS = 500;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  // Create buttons
  m_play_button = new QPushButton("Start/Stop");
  m_play_button->setCheckable(true);
  auto reset_button = new QPushButton("Reset");

  // Create chart
  m_line = new QSplineSeries();
  m_axis_x = new QValueAxis();
  auto axis_y = new QValueAxis();
  axis_y->setMax(MAX_NUMBER);

  auto chart = new QChart();
  chart->addSeries(m_line);
  chart->addAxis(m_axis_x, Qt::AlignBottom);
  chart->addAxis(axis_y, Qt::AlignLeft);
  chart->legend()->hide();

  m_line->attachAxis(m_axis_x);
  m_line->attachAxis(axis_y);

  // Add widgets to the window
  auto buttons_layout = new QHBoxLayout();
  buttons_layout->addWidget(m_play_button);
  buttons_layout->addWidget(reset_button);
  buttons_layout->addStretch();

  auto chart_view = new QChartView(chart, nullptr);
  chart_view->setRenderHint(QPainter::Antialiasing);

  auto left_layout = new QVBoxLayout();
  left_layout->addWidget(chart_view);
  left_layout->addLayout(buttons_layout);

  auto table = new QTableWidget();
  table->setColumnCount(2);
  table->setHorizontalHeaderLabels({"Count", "Summary"});
  auto save_button = new QPushButton("Save");
  auto clear_button = new QPushButton("Clear");
  auto right_buttons_layout = new QHBoxLayout();
  right_buttons_layout->addWidget(save_button);
  right_buttons_layout->addWidget(clear_button);
  right_buttons_layout->addStretch();

  auto right_layout = new QVBoxLayout();
  right_layout->addWidget(table);
  right_layout->addLayout(right_buttons_layout);

  auto main_layout = new QHBoxLayout();
  main_layout->addLayout(left_layout);
  main_layout->addLayout(right_layout);

  auto main_widget = new QWidget();
  main_widget->setLayout(main_layout);
  setCentralWidget(main_widget);

  // Create number generator
  auto generator = new NumberGenerator(MAX_NUMBER, TIME_INTERVAL_MS);
  generator->moveToThread(&m_thread);

  connect(&m_thread, &QThread::finished, generator, &QObject::deleteLater);
  connect(m_play_button, &QPushButton::toggled, generator,
          &NumberGenerator::set_state);
  connect(reset_button, &QPushButton::clicked, m_line, &QLineSeries::clear);
  connect(generator, &NumberGenerator::generated, this,
          &MainWindow::extend_line);
  m_thread.start();
}

MainWindow::~MainWindow() {
  m_play_button->setChecked(false);
  m_thread.quit();
  m_thread.wait();
}

void MainWindow::extend_line(int y) {
  m_line->append(m_line->count(), y);
  m_axis_x->setMax(m_line->count() + 1);
}
