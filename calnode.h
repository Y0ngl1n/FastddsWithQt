#pragma once

#include <QObject>
#include <QString>
#include <QMap>

#include "../types/MyDataPubSubTypes.hpp"
#include "../types/OpticDataPubSubTypes.hpp"

#include <chrono>
#include <thread>

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>

#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>

#include <fastdds/dds/topic/TypeSupport.hpp>


#pragma comment(lib,"libcrypto.lib")
#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"ShLwApi.Lib")
#pragma comment(lib,"iphlpapi.lib")

#ifdef _DEBUG
#pragma comment(lib,"fastcdrd-2.3.lib")
#pragma comment(lib,"fastddsd-3.2.lib")
#pragma comment(lib,"foonathan_memory-0.7.3-dbg.lib")
#else
#pragma comment(lib,"fastcdr-2.3.lib")
#pragma comment(lib,"fastdds-3.2.lib")
#pragma comment(lib,"foonathan_memory-0.7.3.lib")
#endif

using namespace eprosima::fastdds::dds;

class CalNode  : public QObject
{
	Q_OBJECT

public:
	CalNode(QObject *parent,const QString &nodeId);
	~CalNode();
	//初始化
	bool initialize();
	//创建Participant
	bool createParticipant();
	//创建Publisher
	bool createPublisher();
	//创建Subscriber
	bool createSubscriber();
	//创建Topic
	bool createTopic(const QString& topicName);
	//创建DataWriter
	bool createDataWriter(const QString& topicName);
	//创建DataReader
	bool createDataReader(const QString& topicName);
	//发送数据
	bool publish();
	//接收数据
	void receiveDataWithTake();
	//清理
	void cleanUp();
private:
	//节点id
	QString mNodeId;
	//节点状态
	bool mRunningStatus;
	//DDS实体
	DomainParticipant* mParticipant;
	//Publisher
	Publisher* mPublisher;
	//Subscriber
	Subscriber* mSubscriber;
	//Topics
	QMap<QString, Topic*> mTopics;
	//Writer
	DataWriter* mWriter;
	//Reader
	DataReader* mReader;

	TypeSupport mType;
	TypeSupport mType2;

	MyData myData;
signals:
	void signalReceivedData(std::array<int32_t, 12> data);
};

