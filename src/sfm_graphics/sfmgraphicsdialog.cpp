#include "sfmgraphicsdialog.h"
#include "ui_sfmgraphicsdialog.h"
#include "sfmgraphicsscene.h"
#include "sfmgraphicsview.h"
#include "cncharitem.h"
#include "sfmrectitem.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

SFMGraphicsScene *g_scene = Q_NULLPTR;
SFMGraphicsView *g_view = Q_NULLPTR;

SFMGraphicsDialog::SFMGraphicsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SFMGraphicsDialog)
{
    ui->setupUi(this);

    QHBoxLayout *lay = new QHBoxLayout(ui->widget_view);
    lay->addWidget(g_view = new SFMGraphicsView(this));

    g_view->setScene(g_scene = new SFMGraphicsScene(g_view));

    CNCharItem *item = new CNCharItem;
    item->setCNChar(QString::fromLocal8Bit("ÎÒ"));
    item->setSpell("wo");
    //g_scene->addItem(item);

    SFMRectItem *rct_item = new SFMRectItem(QRectF(0, 0, 300, 300));
    rct_item->setFlag(QGraphicsItem::ItemIsMovable, true);
    g_scene->addItem(rct_item);
    rct_item->setFlag(QGraphicsItem::ItemIsSelectable, true);

    QPen p(Qt::SolidLine);
    p.setColor(QColor(255, 0, 0));
    p.setWidthF(8);
    rct_item->setPen(p);

    QBrush b(Qt::SolidPattern);
    b.setColor(QColor(100, 255, 100));
    rct_item->setBrush(b);

    connect(ui->toolButton_rect, &QToolButton::clicked, [this]() {
        g_view->setCurrentTool(SFMGraphicsView::Tool_ResizeableRect);
    });

    connect(ui->toolButton_round, &QToolButton::clicked, [this]() {

    });

    connect(ui->toolButton_ellipse, &QToolButton::clicked, [this]() {
        g_view->setCurrentTool(SFMGraphicsView::Tool_ResizeableEllipse);
    });

    connect(ui->toolButton_square, &QToolButton::clicked, [this]() {

    });

    connect(ui->toolButton_select, &QToolButton::clicked, [this]() {
        g_view->setCurrentTool(SFMGraphicsView::Tool_Select);
    });

    connect(ui->toolButton_pixmap, &QToolButton::clicked, [this]() {
        QString path = QFileDialog::getOpenFileName(this);
        if (path.isEmpty()) {
            return;
        }

        QPixmap p(path);
        if (p.isNull()) {
            QMessageBox::information(this, "Error", "Unsupportted image format!");
            return;
        }

        g_view->setPixmapForItem(p);
        g_view->setCurrentTool(SFMGraphicsView::Tool_Pixmap);
    });
}

SFMGraphicsDialog::~SFMGraphicsDialog()
{
    delete ui;
}

