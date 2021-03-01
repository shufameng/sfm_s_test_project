#include "mapsettingsdialog.h"
#include "ui_mapsettingsdialog.h"

MapSettingsDialog::MapSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MapSettingsDialog)
{
    ui->setupUi(this);
}

MapSettingsDialog::~MapSettingsDialog()
{
    delete ui;
}

void MapSettingsDialog::setMapData(int mapW, int mapH, int cellW, int cellH, qreal unpassablePercent)
{
    ui->spinBox_mapWidth->setValue(mapW);
    ui->spinBox_mapHeight->setValue(mapH);
    ui->spinBox_cellWidth->setValue(cellW);
    ui->spinBox_cellHeight->setValue(cellH);
    ui->doubleSpinBox_unpassablePercent->setValue(unpassablePercent);
}

void MapSettingsDialog::getMapData(int &mapW, int &mapH, int &cellW, int &cellH, qreal &unpassablePercent)
{
    mapW = ui->spinBox_mapWidth->value();
    mapH = ui->spinBox_mapHeight->value();
    cellW = ui->spinBox_cellWidth->value();
    cellH = ui->spinBox_cellHeight->value();
    unpassablePercent = ui->doubleSpinBox_unpassablePercent->value();
}

void MapSettingsDialog::setUnpassablePercentSettingsHidden(bool hidden)
{
    if (hidden) {
        ui->doubleSpinBox_unpassablePercent->hide();
        ui->label_unpassablePercentTitle->hide();
    } else {
        ui->doubleSpinBox_unpassablePercent->show();
        ui->label_unpassablePercentTitle->show();
    }
}

void MapSettingsDialog::on_pushButton_ok_clicked()
{
    accept();
}
