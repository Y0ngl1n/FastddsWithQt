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
    // 创建参与者
    if (!createParticipant()) {
        return false;
    }

    // 注册数据类型
    mType.register_type(mParticipant);
    mType2.register_type(mParticipant);
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
    //创建Qos
    //DomainParticipantQos participantQos;
    //participantQos.name(mNodeId.toStdString());
    //创建participant
    //mParticipant = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);
    //bool rst = mParticipant != nullptr;
    //使用xml文件创建participant
    //mParticipant = DomainParticipantFactory::get_instance()->create_participant_with_default_profile();

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

    // 创建主题
    Topic* topic = mParticipant->create_topic(topicName.toStdString(),"MyData",TOPIC_QOS_DEFAULT);

    if (topic != nullptr) {
        mTopics[topicName] = topic;
        return true;
    }
    else {
        return false;
    }
}

bool CalNode::createDataWriter(const QString& topicName)
{
    if (mTopics[topicName])
    {
        mWriter = mPublisher->create_datawriter(mTopics[topicName], DATAWRITER_QOS_DEFAULT);
    }

    if (mWriter == nullptr)
    {
        return false;
    }
    return true;
}

bool CalNode::createDataReader(const QString& topicName)
{
    if (mTopics[topicName])
    {
        mReader = mSubscriber->create_datareader(mTopics[topicName], DATAREADER_QOS_DEFAULT);
    }

    if (mReader == nullptr)
    {
        return false;
    }
    return true;
}

bool CalNode::publish()
{
    myData.data({ 1, 7, 7, 8, 2, 7, 9, 6, 7, 7, 1, 0 });
    ReturnCode_t returnCode = mWriter->write(&myData);
    if (returnCode == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CalNode::receiveDataWithTake() {
    MyData data;  // 使用 MyData 类型
    SampleInfo info;

    // take() 方法会移除已读取的数据
    while (mReader->take_next_sample(&data, &info) == RETCODE_OK) {
        if (info.valid_data) {
            std::array<int32_t, 12> data_array = data.data();
            emit signalReceivedData(data_array);
        }
        else {
            break;
        }
    }
}

void CalNode::cleanUp()
{
    //删除Reader
    if (mReader != nullptr)
    {
        mSubscriber->delete_datareader(mReader);
    }
    //删除Writer
    if (mWriter != nullptr)
    {
        mPublisher->delete_datawriter(mWriter);
    }
    //删除Topics
    while (!mTopics.isEmpty()) {
        QString key = mTopics.keys().first();
        Topic* data = mTopics.take(key);
        mParticipant->delete_topic(data);
        //delete data;
    }
    //删除Subscriber
    if (mSubscriber != nullptr)
    {
        mParticipant->delete_subscriber(mSubscriber);
    }
    //删除mPublisher
    if (mPublisher != nullptr)
    {
        mParticipant->delete_publisher(mPublisher);
    }
    //删除Participant
    DomainParticipantFactory::get_instance()->delete_participant(mParticipant);
}

