#include "pathfindingdialog.h"
#include "ui_pathfindingdialog.h"
#include <QHBoxLayout>
#include "mapsettingsdialog.h"
#include <QButtonGroup>
#include "mapview.h"

PathFindingDialog::PathFindingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PathFindingDialog)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::Window | Qt::WindowMinMaxButtonsHint);
    setWindowTitle("Path Finding Test Program");

    QButtonGroup *grp = new QButtonGroup(this);
    grp->addButton(ui->toolButton_setPassable);
    grp->addButton(ui->toolButton_setUnpassable);
    grp->addButton(ui->toolButton_setStart);
    grp->addButton(ui->toolButton_setDest);
    grp->addButton(ui->toolButton_noneInteract);
    ui->toolButton_noneInteract->setChecked(true);
    connect(grp, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(onButtonGroupButtonClicked(QAbstractButton*)));

    QHBoxLayout *lay = new QHBoxLayout(ui->widget_view);
    m_view = new MapView(ui->widget_view);
    lay->addWidget(m_view);

    m_view->generateRandomMap();
    m_view->update();

    //m_view->setInteractMode(MapViewA::IM_None);
}

PathFindingDialog::~PathFindingDialog()
{
    delete ui;
}

void PathFindingDialog::on_pushButton_genRandomMap_clicked()
{
#if 1
    MapSettingsDialog d(this);
    d.setWindowTitle("Map Attribute");
    d.setMapData(
                m_view->getMapSize().width(),
                m_view->getMapSize().height(),
                m_view->getCellSize().width(),
                m_view->getCellSize().height(),
                m_view->getUnwalkablePercent()
                );

    int ret = d.exec();

    if (QDialog::Accepted == ret) {
        int mapw, maph, cellw, cellh;
        qreal unpassablePercent;

        d.getMapData(mapw, maph, cellw, cellh, unpassablePercent);
        m_view->setMapSize(mapw, maph);
        m_view->setCellSize(cellw, cellh);
        m_view->setUnwalkablePercent(unpassablePercent);

        m_view->generateRandomMap();
        m_view->update();

        resize(minimumSize());
    }
#endif
}

void PathFindingDialog::on_pushButton_genEmptyMap_clicked()
{
#if 1
    MapSettingsDialog d(this);
    d.setUnpassablePercentSettingsHidden(true);
    d.setWindowTitle("Map Attribute");
    d.setMapData(
                m_view->getMapSize().width(),
                m_view->getMapSize().height(),
                m_view->getCellSize().width(),
                m_view->getCellSize().height(),
                m_view->getUnwalkablePercent()
                );

    int ret = d.exec();

    if (QDialog::Accepted == ret) {
        int mapw, maph, cellw, cellh;
        qreal unpassablePercent;

        d.getMapData(mapw, maph, cellw, cellh, unpassablePercent);
        m_view->setMapSize(mapw, maph);
        m_view->setCellSize(cellw, cellh);
        m_view->cleanMap();
        m_view->update();

        resize(minimumSize());
    }
#endif
}

void PathFindingDialog::onButtonGroupButtonClicked(QAbstractButton *button)
{
#if 1
    if (button == ui->toolButton_setPassable) {
        m_view->setInteractMode(MapView::IM_SetWalkable);
    } else if (button == ui->toolButton_setUnpassable) {
        m_view->setInteractMode(MapView::IM_SetUnwalkable);
    } else if (button == ui->toolButton_setStart) {
        m_view->setInteractMode(MapView::IM_SetStart);
    } else if (button == ui->toolButton_setDest) {
        m_view->setInteractMode(MapView::IM_SetDest);
    } else if (button == ui->toolButton_noneInteract) {
        m_view->setInteractMode(MapView::IM_None);
    }
#endif
}

void PathFindingDialog::on_pushButton_findPath_clicked()
{
    m_view->cleanPath();
    m_view->findPath();
}


