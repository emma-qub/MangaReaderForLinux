#ifndef NIGHTCHARTS_H
#define NIGHTCHARTS_H

#define PerConst = 3.6;

#include <QObject>
#include <QPainter>
#include <QVector>
#include <QDebug>
#include <QLinearGradient>
#include <math.h>

class pieceNC {

public:
    explicit pieceNC() {}

    inline void addName(const QString& name) { pname = name; }
    inline void setColor(Qt::GlobalColor color) { rgbColor = color; }
    inline void setColor(const QColor& color) { rgbColor = color; }
    inline void setPerc(float Percentage) { pPerc = Percentage; }

    QString pname;
    QColor rgbColor;
    float pPerc;

private:

};

class Nightcharts {

public:
    enum type { Histogramm , Pie, Dpie };
    enum legend_type { Vertical, Round };

    explicit Nightcharts();
    ~Nightcharts();

    void addPiece(QString name,Qt::GlobalColor,float Percentage);
    void addPiece(QString name,QColor, float Percentage);

    inline void setCords(double x, double y, double w, double h) { cX = x; cY = y; cW = w; cH = h; lX = cX+cW+20; lY = cY; }
    inline void setLegendCords(double x, double y) { lX = x; lY = y; }
    inline void setType(Nightcharts::type t) { ctype = t; }
    inline void setLegendType(Nightcharts::legend_type t) { cltype = t; }
    inline void setShadows(bool ok = true) { shadows = ok; }
    inline void setFont(QFont font) { _font = font; }

    void draw(QPainter *painter);
    void drawLegend(QPainter *painter);

    double palpha;

private:
    QFont _font;
    type ctype;
    legend_type cltype;
    double cX, cY, cW, cH, pW, lX, lY;
    bool shadows;
    int nPiece;

private:
    QVector<pieceNC> pieces;
    QPointF GetPoint(double angle, double R1 = 0, double R2 = 0);
    int GetQuater(double angle);
    double Angle360(double angle);

signals:

public slots:

};

#endif // NIGHTCHARTS_H
