#pragma once

#include <QObject>
#include<QMap>

#include"calnode.h"

class NodeManager  : public QObject
{
	Q_OBJECT

public:
	NodeManager(QObject *parent);
	~NodeManager();
	// 创建节点
	CalNode* createNode(const QString& id);
	// 删除节点
	bool deleteNode(const QString& id);
	//初始化所有节点
	bool initializeAllNodes();
	// 获取节点
	CalNode* getNode(const QString& id) const;
	// 获取所有节点
	QMap<QString, CalNode*> getAllNodes() const;
	// 启动所有节点
	void startAllNodes();
	// 停止所有节点
	void stopAllNodes();
	// 设置节点间连接
	bool connectNodes(const QString& sourceId, const QString& targetId);
	// 断开节点间连接
	bool disconnectNodes(const QString& sourceId, const QString& targetId);
private:
	QMap<QString, CalNode*> mNodes;
};

