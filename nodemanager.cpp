#include "nodemanager.h"

NodeManager::NodeManager(QObject *parent)
	: QObject(parent)
{}

NodeManager::~NodeManager()
{}

CalNode* NodeManager::createNode(const QString & id)
{
	if (mNodes.contains(id)) {
		return mNodes[id];
	}
	CalNode* node = new CalNode(this, id);
	mNodes[id] = node;
	return node;
}

bool NodeManager::initializeAllNodes()
{
	bool success = true;
	for (auto it = mNodes.begin(); it != mNodes.end(); ++it) {
		if (!it.value()->initialize()) {
			success = false;
		}
	}
	return success;
}

