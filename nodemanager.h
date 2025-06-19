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
	// �����ڵ�
	CalNode* createNode(const QString& id);
	// ɾ���ڵ�
	bool deleteNode(const QString& id);
	//��ʼ�����нڵ�
	bool initializeAllNodes();
	// ��ȡ�ڵ�
	CalNode* getNode(const QString& id) const;
	// ��ȡ���нڵ�
	QMap<QString, CalNode*> getAllNodes() const;
	// �������нڵ�
	void startAllNodes();
	// ֹͣ���нڵ�
	void stopAllNodes();
	// ���ýڵ������
	bool connectNodes(const QString& sourceId, const QString& targetId);
	// �Ͽ��ڵ������
	bool disconnectNodes(const QString& sourceId, const QString& targetId);
private:
	QMap<QString, CalNode*> mNodes;
};

