# smart-debug
Smart-Debug is a Qt-based module that brings facility of debugging c++ codes through plots, scatters, messengers, etc. almost in realtime. If you want plot the values of a variable by time, compare it to some other variables, or you need displaying a set of 2D points on a professional coordinate system this small tool will help you.

For example the plots shown in the following shot is created by this simple code:
```
    for(int i=0; i<1000; i++)
    {
        sdbug::dbuger()->getPlotter("Ploter1")->addValue(sin(i/10.), i, "Sin(x)");
        sdbug::dbuger()->getPlotter("Ploter1")->addValue((rand()%100)/100., i, "Rand(x)");
    }

    QVector<sdbug::PointD> pnts;
    int N = 50 , V = 300;
    for(int i=0; i<N; i++)
    {
        double x = (rand()%120)/10.;
        double y = (rand()%120)/10.;
        pnts.push_back(sdbug::PointD(x, y));
        sdbug::dbuger()->getScatter("Scatter1")->addData(x, y, "Node");
    }

    for(int i=0; i<V; i++)
    {
        sdbug::dbuger()->getScatter("Scatter1")->addLineSegment(pnts[rand()%N], pnts[rand()%N], "Vertex");
    }

```
![Plot](https://github.com/amiryanj/smart-debug/blob/master/resources/shot1.png "Plot")

This tool itself uses QCustomPlot library which is a strong tool for plot purposes. However we tried to provide a very simple API to faciliate test scenarios.
