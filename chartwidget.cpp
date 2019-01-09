#ifdef CREATE_CHARTS

#include "chartwidget.h"
#include "core.h"

ChartWidget::ChartWidget(QWidget* parent) : QWidget(parent)
{
	slow_down_chart = new SlowDownChart();
	speed_chart = new SpeedChart();
	slow_down_chart_view.setChart(slow_down_chart);
	tabs.addTab(&slow_down_chart_view, tr("Slow Down Influence"));
	speed_chart_view.setChart(speed_chart);
	tabs.addTab(&speed_chart_view, tr("Speed Distribution"));
	main_layout.addWidget(&tabs);
	this->setLayout(&main_layout);

	set_slow_down_data(0, 0, 0);
	set_car_speed_data(std::vector<std::size_t>());
}

void ChartWidget::set_slow_down_data(std::size_t slow_down_chance, double avg_speed, std::size_t time)
{
	slow_down_chart->add_data(slow_down_chance, avg_speed, time);
	update();
}

void ChartWidget::set_car_speed_data(const std::vector<std::size_t>& amount_of_car_at_speed)
{
	speed_chart->add_data(amount_of_car_at_speed);
	update();
}

SlowDownChart::SlowDownChart()
{
	this->setTheme(QChart::ChartThemeDark);
}

SlowDownChart::~SlowDownChart()
{
	this->removeAllSeries();
}

void SlowDownChart::add_data(std::size_t slow_down_chance, double avg_speed, std::size_t time)
{
	if(!slow_down_series.count(slow_down_chance))
		slow_down_series[slow_down_chance] = new QLineSeries();

	QLineSeries* line = slow_down_series[slow_down_chance];

	if(!this->series().contains(line))
		this->addSeries(line);
	// clear old line
	if(time < line->at(line->count() - 1).x())
	{
		line->clear();
		this->removeSeries(line);
	}

	line->setName(QString::number(slow_down_chance));
	line->append(time, avg_speed);

	this->createDefaultAxes();
	if(time >= static_cast<QValueAxis*>(this->axisX())->max())
		this->axisX()->setRange(0, std::ceil(time / 5.) * 5);
	else if(static_cast<QValueAxis*>(this->axisX())->max() < 5)
		this->axisX()->setRange(0, 5);
	static_cast<QValueAxis*>(this->axisX())->setLabelFormat("%d");
	static_cast<QValueAxis*>(this->axisX())->applyNiceNumbers();
//	static_cast<QValueAxis*>(this->axisX())->setTickCount(6);

	this->axisY()->setRange(0, static_cast<int>(Core::get_max_speed()));
	static_cast<QValueAxis*>(this->axisY())->setTickCount(Core::get_max_speed() + 1);
}

SpeedChart::SpeedChart() : speed_bar_set("")
{
	this->setTheme(QChart::ChartThemeDark);
}

void SpeedChart::add_data(const std::vector<std::size_t>& amount_of_car_at_speed)
{
	speed_bar_set.remove(0, speed_bar_set.count());
/*	if(speed_axis.count() != Core::get_max_speed())
	{
		this->removeAxis(&speed_axis);
		speed_axis.clear();
	}*/

	for(auto it = amount_of_car_at_speed.begin(); it != amount_of_car_at_speed.end(); ++it)
		speed_bar_set.append(*it);

	speed_series.append(&speed_bar_set);

	if(!this->series().contains(&speed_series))
		this->addSeries(&speed_series);

	this->createDefaultAxes();
//	if(speed_axis.count() != Core::get_max_speed())
//		this->setAxisX(&speed_axis, &speed_series);
	this->axisX()->setRange(0, static_cast<int>(Core::get_max_speed()));
	this->axisY()->setRange(0, static_cast<int>(Core::get_car_amount()));

	static_cast<QValueAxis*>(this->axisY())->setLabelFormat("%d");
}

#endif
