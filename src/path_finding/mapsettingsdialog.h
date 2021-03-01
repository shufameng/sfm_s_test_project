#ifndef MAPSETTINGSDIALOG_H
#define MAPSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class MapSettingsDialog;
}

class MapSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MapSettingsDialog(QWidget *parent = nullptr);
    ~MapSettingsDialog();

    void setMapData(int mapW, int mapH, int cellW, int cellH, qreal unpassablePercent);
    void getMapData(int &mapW, int &mapH, int &cellW, int &cellH, qreal &unpassablePercent);

    void setUnpassablePercentSettingsHidden(bool hidden);

private slots:
    void on_pushButton_ok_clicked();

private:
    Ui::MapSettingsDialog *ui;
};

#endif // MAPSETTINGSDIALOG_H
