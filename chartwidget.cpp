#ifdef CREATE_CHARTS

#include "chartwidget.h"
#include "core.h"

ChartWidget::ChartWidget(QWidget* parent) : QWidget(parent), speed_bar_set("")
{
	slow_down_chart_view.setChart(&slow_down_chart);
	tabs.addTab(&slow_down_chart_view, tr("Slow Down Influence"));
	speed_chart_view.setChart(&speed_chart);
	tabs.addTab(&speed_chart_view, tr("Speed Distribution"));
	main_layout.addWidget(&tabs);
	this->setLayout(&main_layout);

	set_slow_down_data(0, 0, 0);
	set_car_speed_data(std::vector< std::pair<std::size_t, std::size_t> >());
}

void ChartWidget::set_slow_down_data(const std::size_t& slow_down_chance, const double& avg_speed, const std::size_t& time)
{
	if(slow_down_series.find(slow_down_chance) == slow_down_series.end())
		slow_down_series[slow_down_chance] = new QLineSeries();

	QLineSeries* line = slow_down_series[slow_down_chance];
	if(!slow_down_chart.series().contains(line))
		slow_down_chart.addSeries(line);
	else if(time < line->at(line->count() - 1).x())
	{
		line->clear();
		slow_down_chart.removeSeries(line);
	}

	line->setName(QString::number(slow_down_chance));
	line->append(time, avg_speed);

	slow_down_chart.createDefaultAxes();
	if(time > static_cast<QValueAxis*>(slow_down_chart.axisX())->max())
		slow_down_chart.axisX()->setRange(0, static_cast<int>(time));
	static_cast<QValueAxis*>(slow_down_chart.axisX())->setLabelFormat("%d");
	slow_down_chart.axisY()->setRange(0, static_cast<int>(Core::get_instance()->get_max_speed()));

	update();
}

void ChartWidget::set_car_speed_data(const std::vector< std::pair<std::size_t, std::size_t> >& amount_of_car_at_speed)
{
	speed_bar_set.remove(0, speed_bar_set.count());
	speed_axis.clear();
	speed_chart.removeAxis(&speed_axis);
	for(std::size_t i = 0; i <= Core::get_instance()->get_max_speed(); ++i)
	{
		bool appended = false;
		for(const auto& a : amount_of_car_at_speed)
		{
			if(a.first == i)
			{
				speed_bar_set.append(a.second);
				appended = true;
				break;
			}
		}

		if(!appended)
			speed_bar_set.append(0);

		speed_axis.append(QString::number(i));
	}

	speed_series.append(&speed_bar_set);

	if(!speed_chart.series().contains(&speed_series))
		speed_chart.addSeries(&speed_series);

	speed_chart.createDefaultAxes();
	speed_chart.setAxisX(&speed_axis, &speed_series);
	speed_chart.axisX()->setRange(0, static_cast<int>(Core::get_instance()->get_max_speed()));
	speed_chart.axisY()->setRange(0, static_cast<int>(Core::get_instance()->get_car_amount()));
	static_cast<QValueAxis*>(slow_down_chart.axisY())->setLabelFormat("%d");

	update();
}

#endif
