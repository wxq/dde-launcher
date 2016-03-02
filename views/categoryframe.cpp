/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "categoryframe.h"

#include "categorytablewidget.h"
#include "navigationbar.h"
#include "app/global.h"
#include <QHBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QShowEvent>

const int BottomMargin = 70;

CategoryFrame::CategoryFrame(QWidget *parent) :
    QFrame(parent)
{

}

void CategoryFrame::initUI(int leftMargin, int rightMargin, int column, int itemWidth, int gridWidth, int gridHeight){
    setGeometry(static_cast<QWidget*>(parent())->geometry());
    m_navigationBar = new NavigationBar(this);
    m_navigationBar->initUI(leftMargin);

    m_categoryTableWidget = new CategoryTableWidget(this);
    m_categoryTableWidget->setGridParameter(column, gridWidth, gridHeight, itemWidth);

    QVBoxLayout* tableLayout = new QVBoxLayout;
    tableLayout->addWidget(m_categoryTableWidget);
    tableLayout->setContentsMargins(0, 20, 0, BottomMargin);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(m_navigationBar);
    layout->addLayout(tableLayout);
    layout->addStretch();
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, rightMargin, 0);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    m_categoryTableWidget->setFocusPolicy(Qt::NoFocus);

    initConnect();

}

void CategoryFrame::initConnect() {
    connect(m_categoryTableWidget, SIGNAL(verticalPositionChanged(int)),
            this, SIGNAL(contentScrolled(int)));
    connect(signalManager, SIGNAL(categoryFlag(bool)), this, SLOT(setCategoryFlag(bool)));
}

NavigationBar* CategoryFrame::getNavigationBar(){
    return m_navigationBar;
}

CategoryTableWidget* CategoryFrame::getCategoryTabelWidget(){
    return m_categoryTableWidget;
}

CategoryFrame::~CategoryFrame()
{

}

void CategoryFrame::showEvent(QShowEvent * event)
{
    emit showed();
    QFrame::showEvent(event);
}

QRect CategoryFrame::topGradientRect() const
{
    QPoint topLeft = m_categoryTableWidget->mapTo(parentWidget(),
                                                  QPoint(0, 0));
    QSize size(m_categoryTableWidget->width(), TopBottomGradientHeight);

    return QRect(topLeft, size);
}

QRect CategoryFrame::bottomGradientRect() const
{
    QPoint bottomLeft = m_categoryTableWidget->mapTo(parentWidget(),
                                                     m_categoryTableWidget->rect().bottomLeft());
    QSize size(m_categoryTableWidget->width(), TopBottomGradientHeight);
    // FIXME: workaround here, to fix the bug that the bottom gradient
    // if one pixel above the bottom of m_appTableWidget.
    QPoint topLeft(bottomLeft.x(), bottomLeft.y() + 1 - size.height());

    return QRect(topLeft, size);
}

void  CategoryFrame::setCategoryFlag(bool xflag) {
    if (xflag) {
       changeCategoryTableViewModeFlag = false;
    } else {
        changeCategoryTableViewModeFlag = true;
        emit signalManager->hightlightButtonByIndex(0);
    }
}