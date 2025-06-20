#pragma once

#include <QObject>
#include <QString>
#include <QMap>

#include "../types/MyDataPubSubTypes.hpp"
#include "../types/OpticDataPubSubTypes.hpp"
#include "../types/RadarData.hpp"

#include <chrono>
#include <thread>

#include<unordered_map>

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
	//��ʼ��
	bool initialize();
	//����Participant
	bool createParticipant();
	//����Publisher
	bool createPublisher();
	//����Subscriber
	bool createSubscriber();
	//����Topic
	bool createTopic(const QString& topicName);
	//����DataWriter
	bool createDataWriter(const QString& topicName);
	DataWriter* getWriter(const QString& topicName);
	//����DataReader
	bool createDataReader(const QString& topicName);
	DataReader* getReader(const QString& topicName);
	//��������
	//bool publish();
	bool publish(DataWriter* writer, void* data);
	//��������
	void receiveDataWithTake();
	//����
	void cleanUp();

	template<typename T>
	void receiveDataWithTake(DataReader* reader, std::function<void(const T&)> callback)
	{
		if (!reader) return;

		T data;
		SampleInfo info;

		while (reader->take_next_sample(&data, &info) == RETCODE_OK) {
			if (info.valid_data) {
				callback(data);
			}
			else {
				break;
			}
		}
	}
private:
	//�ڵ�id
	QString mNodeId;
	//�ڵ�״̬
	bool mRunningStatus;
	//DDSʵ��
	DomainParticipant* mParticipant;
	//Publisher
	Publisher* mPublisher;
	//Subscriber
	Subscriber* mSubscriber;
	//Topics
	QMap<QString, Topic*> mTopics;
	//Writer
	//DataWriter* mWriter;
	QMap<QString, DataWriter*> mWriters;
	//Reader
	//DataReader* mReader;
	QMap<QString, DataReader*> mReaders;
	TypeSupport mType;
	TypeSupport mType2;
	TypeSupport mType3;

signals:
	void signalReceivedData(std::array<int32_t, 12> data);
};

