
#pragma once

#include <vector>
#include <cstdio>
#include <string>
#include <fstream>
#include <algorithm>

#define DEFPLOTFILE "plot.html"

using namespace std;

class Plotter
{

public:

    Plotter(const char* title = "", int width = 600, int height = 300);

    void SetTitle(const char* title);
    void SetSize(int width, int height);

    void AddRow(double x, double y);
    void AddRow(double x, double y1, double y2);
    void AddRow(double x, double y1, double y2, double y3);
    void AddRow(double x, double y1, double y2, double y3, double y4);
    void AddRow(double x, double y1, double y2, double y3, double y4, double y5);
    void AddRow(double x, double y1, double y2, double y3, double y4, double y5, double y6);
    void AddRow(double x, double y1, double y2, double y3, double y4, double y5, double y6, double y7);
    void AddRow(double x, double y1, double y2, double y3, double y4, double y5, double y6, double y7, double y8);
    void AddRow(double x, double y1, double y2, double y3, double y4, double y5, double y6, double y7, double y8, double y9);
    void AddRow(double x, double y1, double y2, double y3, double y4, double y5, double y6, double y7, double y8, double y9, double y10);

    void SetLabels(const char* label1);
    void SetLabels(const char* label1, const char* label2);
    void SetLabels(const char* label1, const char* label2, const char* label3);
    void SetLabels(const char* label1, const char* label2, const char* label3, const char* label4);
    void SetLabels(const char* label1, const char* label2, const char* label3, const char* label4, const char* label5);
    void SetLabels(const char* label1, const char* label2, const char* label3, const char* label4, const char* label5, const char* label6);
    void SetLabels(const char* label1, const char* label2, const char* label3, const char* label4, const char* label5, const char* label6, const char* label7);
    void SetLabels(const char* label1, const char* label2, const char* label3, const char* label4, const char* label5, const char* label6, const char* label7, const char* label8);
    void SetLabels(const char* label1, const char* label2, const char* label3, const char* label4, const char* label5, const char* label6, const char* label7, const char* label8, const char* label9);
    void SetLabels(const char* label1, const char* label2, const char* label3, const char* label4, const char* label5, const char* label6, const char* label7, const char* label8, const char* label9, const char* label10);

    void Plot();
    void Title(const char* title);
    void Xlabel(const char* label);
    void Ylabel(const char* label);

private:

    ofstream chartfile;

    string title;

    int width;
    int height;

    string xlabel;
    string ylabel;
    vector<double> xdata;
    vector<vector<double> > ydata;
    vector<string> labels;

    unsigned int curveCnt;

    void PrintFrontMatter();
    void PrintPlotData();
    void PrintEndMatter();

};

Plotter::Plotter(const char* title, int width, int height)
{
    this->title = title;
    this->width = width;
    this->height = height;
    this->xlabel = "t (s)";
    this->ylabel = "";
    this->xdata = std::vector<double>(0);
    this->ydata = std::vector<std::vector<double> >(10);
    this->labels = std::vector<std::string>(10);
    this->curveCnt = 0;
}

void Plotter::SetTitle(const char* title)
{
    this->title = title;
}

void Plotter::SetSize(int width, int height)
{
    this->width = width;
    this->height = height;
}

void Plotter::AddRow(double x, double y)
{
    this->xdata.push_back(x);
    this->ydata[0].push_back(y);
    this->curveCnt = 1;
}

void Plotter::AddRow(double x, double y1, double y2)
{
    Plotter::AddRow(x, y1);
    this->ydata[1].push_back(y2);
    this->curveCnt = 2;
}

void Plotter::AddRow(double x, double y1, double y2, double y3)
{
    Plotter::AddRow(x, y1, y2);
    this->ydata[2].push_back(y3);
    this->curveCnt = 3;
}

void Plotter::AddRow(double x, double y1, double y2, double y3, double y4)
{
    Plotter::AddRow(x, y1, y2, y3);
    this->ydata[3].push_back(y4);
    this->curveCnt = 4;
}

void Plotter::AddRow(double x, double y1, double y2, double y3, double y4, double y5)
{
    Plotter::AddRow(x, y1, y2, y3, y4);
    this->ydata[4].push_back(y5);
    this->curveCnt = 5;
}

void Plotter::AddRow(double x, double y1, double y2, double y3, double y4, double y5, double y6)
{
    Plotter::AddRow(x, y1, y2, y3, y4, y5);
    this->ydata[5].push_back(y6);
    this->curveCnt = 6;
}

void Plotter::AddRow(double x, double y1, double y2, double y3, double y4, double y5, double y6, double y7)
{
    Plotter::AddRow(x, y1, y2, y3, y4, y5, y6);
    this->ydata[6].push_back(y7);
    this->curveCnt = 7;
}

void Plotter::AddRow(double x, double y1, double y2, double y3, double y4, double y5, double y6, double y7, double y8)
{
    Plotter::AddRow(x, y1, y2, y3, y4, y5, y6, y7);
    this->ydata[7].push_back(y8);
    this->curveCnt = 8;
}

void Plotter::AddRow(double x, double y1, double y2, double y3, double y4, double y5, double y6, double y7, double y8, double y9)
{
    Plotter::AddRow(x, y1, y2, y3, y4, y5, y6, y7, y8);
    this->ydata[8].push_back(y9);
    this->curveCnt = 9;
}

void Plotter::AddRow(double x, double y1, double y2, double y3, double y4, double y5, double y6, double y7, double y8, double y9, double y10)
{
    Plotter::AddRow(x, y1, y2, y3, y4, y5, y6, y7, y8, y9);
    this->ydata[9].push_back(y10);
    this->curveCnt = 10;
}

void Plotter::SetLabels(const char* label1)
{
    this->labels[0] = label1;
}

void Plotter::SetLabels(const char* label1, const char* label2)
{
    Plotter::SetLabels(label1);
    this->labels[1] = label2;
}

void Plotter::SetLabels(const char* label1, const char* label2, const char* label3)
{
    Plotter::SetLabels(label1, label2);
    this->labels[2] = label3;
}

void Plotter::SetLabels(const char* label1, const char* label2, const char* label3, const char* label4)
{
    Plotter::SetLabels(label1, label2, label3);
    this->labels[3] = label4;
}

void Plotter::SetLabels(const char* label1, const char* label2, const char* label3, const char* label4, const char* label5)
{
    Plotter::SetLabels(label1, label2, label3, label4);
    this->labels[4] = label5;
}

void Plotter::SetLabels(const char* label1, const char* label2, const char* label3, const char* label4, const char* label5, const char* label6)
{
    Plotter::SetLabels(label1, label2, label3, label4, label5);
    this->labels[5] = label6;
}

void Plotter::SetLabels(const char* label1, const char* label2, const char* label3, const char* label4, const char* label5, const char* label6, const char* label7)
{
    Plotter::SetLabels(label1, label2, label3, label4, label5, label6);
    this->labels[6] = label7;
}

void Plotter::SetLabels(const char* label1, const char* label2, const char* label3, const char* label4, const char* label5, const char* label6, const char* label7, const char* label8)
{
    Plotter::SetLabels(label1, label2, label3, label4, label5, label6, label7);
    this->labels[7] = label8;
}

void Plotter::SetLabels(const char* label1, const char* label2, const char* label3, const char* label4, const char* label5, const char* label6, const char* label7, const char* label8, const char* label9)
{
    Plotter::SetLabels(label1, label2, label3, label4, label5, label6, label7, label8);
    this->labels[8] = label9;
}

void Plotter::SetLabels(const char* label1, const char* label2, const char* label3, const char* label4, const char* label5, const char* label6, const char* label7, const char* label8, const char* label9, const char* label10)
{
    Plotter::SetLabels(label1, label2, label3, label4, label5, label6, label7, label8, label9);
    this->labels[9] = label10;
}

void Plotter::PrintFrontMatter()
{
    chartfile << "<html>" << endl;
    chartfile << "  <head>" << endl;
    chartfile << "    <script type='text/javascript' src='https://www.gstatic.com/charts/loader.js'></script>" << endl;
    chartfile << "    <script type='text/javascript'>" << endl;
    chartfile << "      google.charts.load('current', {'packages':['corechart']});" << endl;
    chartfile << "      google.charts.setOnLoadCallback(drawChart);" << endl;
    chartfile << "      function drawChart() {" << endl;
    chartfile << "        var data = google.visualization.arrayToDataTable([" << endl;
}

void Plotter::PrintPlotData()
{
    chartfile << "          " << "['" << xlabel.c_str() << "'";

    for (unsigned int i = 0; i < curveCnt; i++)
    {
        chartfile << ",'" << labels[i].c_str() << "'";
    }

    chartfile << "]";

    for (unsigned int k = 0; k < xdata.size(); k++)
    {
        chartfile << "," << endl << "          " << "[" << xdata[k];

        for (unsigned int i = 0; i < curveCnt; i++)
        {
            chartfile << ", " << ydata[i][k];
        }

        chartfile << "]";
    }

    chartfile << endl;
}

void Plotter::PrintEndMatter()
{
    chartfile << "        ]);" << endl;
    chartfile << "        var options = {" << endl;
    chartfile << "          title: '" << title << "'," << endl;
    chartfile << "          width: " << width << "," << endl;
    chartfile << "          height: " << height << "," << endl;

    // add first column label as x-axis label:
    chartfile << "          hAxis: {title: data.getColumnLabel(0)}," << endl;

    // Allow multiple simultaneous selections:
    chartfile << "          selectionMode: 'multiple'," << endl;

    // Trigger tooltips on selections:
    chartfile << "          tooltip: {trigger: 'selection'}," << endl;

    // Group selections by x-value:
    chartfile << "          aggregationTarget: 'category'," << endl;

    chartfile << "          curveType: 'none'" << endl;

    chartfile << "        };" << endl;

    chartfile << "        var chart_div = document.getElementById('elct350output');" << endl;
    chartfile << "        var chart = new google.visualization.LineChart(document.getElementById('elct350output'));" << endl;

    // for extracting png image:
    chartfile << "        google.visualization.events.addListener(chart, 'ready', function () {" << endl;
    chartfile << "          chart_div.innerHTML = '<img src=\"' + chart.getImageURI() + '\">';" << endl;
    chartfile << "        });" << endl;

    // draw chart and close the blocks
    chartfile << "        chart.draw(data, options);" << endl;
    chartfile << "      }" << endl;
    chartfile << "    </script>" << endl;
    chartfile << "  </head>" << endl;
    chartfile << "  <body>" << endl;
    chartfile << "    <div id='elct350output' style='width: 900px; height: 500px'></div>" << endl;
    chartfile << "  </body>" << endl;
    chartfile << "</html>" << endl;

}

void Plotter::Plot()
{
    string plotfile = DEFPLOTFILE;

    if (!title.empty())
    {
        plotfile = title;
        replace(plotfile.begin(), plotfile.end(), ' ', '_');
        plotfile += ".html";
    }

    chartfile.open(plotfile.c_str());

    PrintFrontMatter();

    PrintPlotData();

    PrintEndMatter();

    chartfile.close();
}