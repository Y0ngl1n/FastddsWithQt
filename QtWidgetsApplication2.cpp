#include "QtWidgetsApplication2.h"

QtWidgetsApplication2::QtWidgetsApplication2(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.listWidget->setStyleSheet("background-color: lightblue;");
    initComponent();
    initNodes();
    initConnect();
}

QtWidgetsApplication2::~QtWidgetsApplication2()
{}

void QtWidgetsApplication2::initComponent()
{
    subTimer = new QTimer(this);
    subTimer->start(1000);
}

void QtWidgetsApplication2::initNodes()
{
    //节点管理器
    nodeManager = new NodeManager(this);
    //创建2个节点
    node1 = nodeManager->createNode("目标特性生成节点");
    node2 = nodeManager->createNode("背景特性生成节点");
    // 初始化所有节点
    nodeManager->initializeAllNodes();
    //创建主题
    node1->createTopic("TopicOfMyData");
    node2->createTopic("TopicOfMyData");

    //创建Reader和Writer
    node1->createDataReader("TopicOfMyData");
    node1->createDataWriter("TopicOfMyData");

    node2->createDataReader("TopicOfMyData");
    node2->createDataWriter("TopicOfMyData");
    connect(subTimer, &QTimer::timeout, node2, &CalNode::receiveDataWithTake);
    connect(node2, &CalNode::signalReceivedData, this, &QtWidgetsApplication2::onReceivedData);

}

void QtWidgetsApplication2::initConnect()
{
    connect(ui.pushButton, &QPushButton::clicked, this, &QtWidgetsApplication2::onPushButtonClicked);
}

void QtWidgetsApplication2::onPushButtonClicked()
{
    //把界面上的参数传给publish方法，这里先省略，直接写死
    node1->publish();
}

void QtWidgetsApplication2::onReceivedData(std::array<int32_t, 12> data)
{
    ui.spinBox_13->setValue(data[0]);
    ui.spinBox_14->setValue(data[1]);
    ui.spinBox_15->setValue(data[2]);
    ui.spinBox_16->setValue(data[3]);
    ui.spinBox_17->setValue(data[4]);
    ui.spinBox_18->setValue(data[5]);
    ui.spinBox_19->setValue(data[6]);
    ui.spinBox_20->setValue(data[7]);
    ui.spinBox_21->setValue(data[8]);
    ui.spinBox_22->setValue(data[9]);
    ui.spinBox_23->setValue(data[10]);
    ui.spinBox_24->setValue(data[11]);
}
