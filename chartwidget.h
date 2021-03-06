#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#ifdef CREATE_CHARTS
#include <QWidget>
#include <QtCharts>
#include <map>

class SlowDownChart : public QChart
{
	std::map<std::size_t, QLineSeries*> slow_down_series;

public:
	SlowDownChart();
	virtual ~SlowDownChart() override;

	void add_data(std::size_t slow_down_chance, double avg_speed, std::size_t time);
};

class SpeedChart : public QChart
{
	QBarSeries speed_series;
	QBarSet speed_bar_set;
	QBarCategoryAxis speed_axis;

public:
	SpeedChart();

	void add_data(const std::vector<std::size_t>& amount_of_car_at_speed);
};

class ChartWidget : public QWidget
{
	QGridLayout main_layout;
	QTabWidget tabs;
	QChartView slow_down_chart_view;
	QChartView speed_chart_view;
	SlowDownChart* slow_down_chart;
	SpeedChart* speed_chart;

public:
	ChartWidget(QWidget* parent = nullptr);

	void set_slow_down_data(std::size_t slow_down_chance, double avg_speed, std::size_t time);
	void set_car_speed_data(const std::vector<std::size_t>& amount_of_car_at_speed);
};
#endif

#endif // CHARTWIDGET_H
