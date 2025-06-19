#pragma once

#include <QtWidgets/QMainWindow>
#include<QPushButton>
#include<QTimer>

#include "ui_QtWidgetsApplication2.h"

#include"nodemanager.h"
#include"calnode.h"
#include"fastddsmanager.h"
#include"calmodel.h"

class QtWidgetsApplication2 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication2(QWidget *parent = nullptr);
    ~QtWidgetsApplication2();
    void initComponent();
    void initNodes();
    void initConnect();
    void onPushButtonClicked();
public slots:
    void onReceivedData(std::array<int32_t, 12> data);
private:
    Ui::QtWidgetsApplication2Class ui;
    QListWidgetItem* item;
    NodeManager* nodeManager;
    CalNode* node1;
    CalNode* node2;
    QTimer* subTimer;
};

