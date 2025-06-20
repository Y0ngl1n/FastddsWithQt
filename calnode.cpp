#include "calnode.h"

CalNode::CalNode(QObject *parent,const QString &nodeId)
	: QObject(parent),mNodeId(nodeId)
{}

CalNode::~CalNode()
{
    cleanUp();
}

bool CalNode::initialize()
{
    if (mRunningStatus) {
        return true;
    }
    mType = TypeSupport(new MyDataPubSubType());
    mType2 = TypeSupport(new OpticDataPubSubType());
    mType3 = TypeSupport(new OpticDataPubSubType());
    // 创建参与者
    if (!createParticipant()) {
        return false;
    }

    // 注册数据类型
    mType.register_type(mParticipant);
    mType2.register_type(mParticipant);
    mType3.register_type(mParticipant);
    // 创建发布者
    if (!createPublisher()) {
        return false;
    }

    // 创建订阅者
    if (!createSubscriber()) {
        return false;
    }

    mRunningStatus = true;
    return true;
}

bool CalNode::createParticipant()
{
    //直接创建Qos
    //DomainParticipantQos participantQos;
    //participantQos.name(mNodeId.toStdString());
    //创建participant
    //mParticipant = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);

    //使用xml文件创建participant
    bool rst = DomainParticipantFactory::get_instance()->load_XML_profiles_file("config.xml");
    mParticipant = DomainParticipantFactory::get_instance()->create_participant_with_profile(0, "ParticipantProfile");
    return (mParticipant != nullptr);
}

bool CalNode::createPublisher()
{
    mPublisher = mParticipant->create_publisher(PUBLISHER_QOS_DEFAULT);
    return (mPublisher != nullptr);
}

bool CalNode::createSubscriber()
{
    mSubscriber = mParticipant->create_subscriber(SUBSCRIBER_QOS_DEFAULT);
    return (mParticipant != nullptr);
}

bool CalNode::createTopic(const QString& topicName)
{
    if (!mRunningStatus) {
        return false;
    }
    if (mTopics.contains(topicName)) {
        return true;
    }
    QString typeName;
    static const std::unordered_map<std::string, std::string> topicTypeMap = {
        { "TopicOfMyData", "MyData" },
        { "TopicOfOpticData", "OpticData" },
        { "TopicOfRadarData", "RadarData" }
    };
    auto it = topicTypeMap.find(topicName.toStdString());
    if (it != topicTypeMap.end()) {
        const std::string& typeName = it->second;
        Topic* topic = mParticipant->create_topic(topicName.toStdString(), typeName, TOPIC_QOS_DEFAULT);
        if (topic != nullptr) {
            mTopics[topicName] = topic;
            return true;
        }
    }
    return false;  // 没有匹配的类型或创建失败
}

bool CalNode::createDataWriter(const QString& topicName)
{
    if (mTopics[topicName])
    {
        if (mWriters.contains(topicName)) {
            return true;  // 已经创建过这个 writer
        }
        DataWriter* writer = mPublisher->create_datawriter(mTopics[topicName], DATAWRITER_QOS_DEFAULT);
        if (!writer) {
            return false;
        }
        mWriters[topicName] = writer;
        return true;
    };
}

DataWriter* CalNode::getWriter(const QString& topicName)
{
    return mWriters.value(topicName, nullptr);
}

bool CalNode::createDataReader(const QString& topicName)
{
    if (mTopics[topicName])
    {
        if (mReaders.contains(topicName)) {
            return true;  // 已经创建过这个 reader
        }
        DataReader* reader = mSubscriber->create_datareader(mTopics[topicName], DATAREADER_QOS_DEFAULT);
        if (!reader) {
            return false;
        }
        mReaders[topicName] = reader;
        return true;
    }
    else
    {
        return false;
    }
}

DataReader* CalNode::getReader(const QString& topicName)
{
    return mReaders.value(topicName, nullptr);
}

//bool CalNode::publish()
//{
//    myData.data({ 1, 7, 7, 8, 2, 7, 9, 6, 7, 7, 1, 0 });
//    ReturnCode_t returnCode = mWriter->write(&myData);
//    if (returnCode == 0)
//    {
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}

//void CalNode::receiveDataWithTake() {
//    MyData data;  // 使用 MyData 类型
//    SampleInfo info;
//
//    // take() 方法会移除已读取的数据
//    while (mReader->take_next_sample(&data, &info) == RETCODE_OK) {
//        if (info.valid_data) {
//            std::array<int32_t, 12> data_array = data.data();
//            emit signalReceivedData(data_array);
//        }
//        else {
//            break;
//        }
//    }
//}

//template<typename T>
//void CalNode::receiveDataWithTake(DataReader* reader, std::function<void(const T&)> callback)
//{
//    if (!reader) return;
//
//    T data;
//    SampleInfo info;
//
//    while (reader->take_next_sample(&data, &info) == RETCODE_OK) {
//        if (info.valid_data) {
//            callback(data);
//        }
//        else {
//            break;
//        }
//    }
//}


bool CalNode::publish(DataWriter* writer, void* data)
{
    if (!writer || !data) {
        return false;
    }

    ReturnCode_t returnCode = writer->write(data);
    return returnCode == RETCODE_OK;
}

void CalNode::cleanUp()
{
    // 删除所有 DataReader
    for (DataReader* reader : mReaders) {
        if (reader && mSubscriber) {
            mSubscriber->delete_datareader(reader);
        }
    }
    mReaders.clear();

    // 删除所有 DataWriter
    for (DataWriter* writer : mWriters) {
        if (writer && mPublisher) {
            mPublisher->delete_datawriter(writer);
        }
    }
    mWriters.clear();

    // 删除所有 Topic
    for (Topic* topic : mTopics) {
        if (topic && mParticipant) {
            mParticipant->delete_topic(topic);
        }
    }
    mTopics.clear();

    // 删除 Subscriber
    if (mSubscriber && mParticipant) {
        mParticipant->delete_subscriber(mSubscriber);
        mSubscriber = nullptr;
    }

    // 删除 Publisher
    if (mPublisher && mParticipant) {
        mParticipant->delete_publisher(mPublisher);
        mPublisher = nullptr;
    }

    // 删除 Participant
    if (mParticipant) {
        DomainParticipantFactory::get_instance()->delete_participant(mParticipant);
        mParticipant = nullptr;
    }
}



