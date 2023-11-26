#include "mainwindow.h"
#include "numbergenerator.h"
#include <QChartView>
#include <QTableWidget>

static constexpr int MAX_NUMBER = 100;
static constexpr int TIME_INTERVAL_MS = 500;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  auto plot_layout = create_plot_widgets();
  auto table_layout = create_table_widgets();
  auto main_layout = new QHBoxLayout();
  main_layout->addLayout(plot_layout);
  main_layout->addLayout(table_layout);

  auto main_widget = new QWidget();
  main_widget->setLayout(main_layout);
  setCentralWidget(main_widget);
}

MainWindow::~MainWindow() {
  m_play_button->setChecked(false);
  m_thread.quit();
  m_thread.wait();
}

QLayout *MainWindow::create_plot_widgets() {
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

  auto main_layout = new QVBoxLayout();
  main_layout->addWidget(chart_view);
  main_layout->addLayout(buttons_layout);

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
  return main_layout;
}

QLayout *MainWindow::create_table_widgets() {
  auto table = new QTableWidget();
  table->setColumnCount(2);
  table->setHorizontalHeaderLabels({"Count", "Summary"});
  auto save_button = new QPushButton("Save");
  auto clear_button = new QPushButton("Clear");
  auto buttons_layout = new QHBoxLayout();
  buttons_layout->addWidget(save_button);
  buttons_layout->addWidget(clear_button);
  buttons_layout->addStretch();
  auto main_layout = new QVBoxLayout();
  main_layout->addWidget(table);
  main_layout->addLayout(buttons_layout);
  return main_layout;
}

void MainWindow::extend_line(int y) {
  m_line->append(m_line->count(), y);
  m_axis_x->setMax(m_line->count() + 1);
}
