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
	QValueAxis* axis_x = static_cast<QValueAxis*>(this->axes(Qt::Horizontal).back());
	if(time >= axis_x->max())
		axis_x->setRange(0, std::ceil(time / 5.) * 5);
	else if(static_cast<QValueAxis*>(axis_x)->max() < 5)
		axis_x->setRange(0, 5);
	axis_x->setLabelFormat("%d");
	axis_x->applyNiceNumbers();
//	static_cast<QValueAxis*>(this->axisX())->setTickCount(6);

	QValueAxis* axis_y = static_cast<QValueAxis*>(this->axes(Qt::Vertical).back());
	axis_y->setRange(0, static_cast<int>(Core::get_max_speed()));
	axis_y->setTickCount(static_cast<int>(Core::get_max_speed() + 1));
}

SpeedChart::SpeedChart() : speed_bar_set("")
{
	this->setTheme(QChart::ChartThemeDark);
}

void SpeedChart::add_data(const std::vector<std::size_t>& amount_of_car_at_speed)
{
	speed_bar_set.remove(0, speed_bar_set.count());
	speed_bar_set.setLabelColor(QColor(Qt::transparent));

/*	if(speed_axis.count() != Core::get_max_speed())
	{
		this->removeAxis(&speed_axis);
		speed_axis.clear();
	}*/

	for(std::size_t i = 0; i < amount_of_car_at_speed.size(); ++i)
		speed_bar_set.insert(static_cast<int>(i), amount_of_car_at_speed[i]);
		//speed_bar_set.append();

	speed_series.append(&speed_bar_set);

	if(!this->series().contains(&speed_series))
		this->addSeries(&speed_series);

//	if(speed_axis.count() != Core::get_max_speed())
//		this->setAxisX(&speed_axis, &speed_series);

	this->createDefaultAxes();
	QAbstractAxis* axis_x = this->axes(Qt::Horizontal).back();
	QValueAxis* axis_y = static_cast<QValueAxis*>(this->axes(Qt::Vertical).back());
	axis_y->setLabelFormat("%d");
	axis_x->setRange(0, static_cast<int>(Core::get_max_speed() + 1));
	axis_x->setRange(0, static_cast<int>(Core::get_car_amount()));
}

#endif
