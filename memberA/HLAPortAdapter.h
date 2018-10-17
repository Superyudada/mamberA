
/*
* Adapter��
* @date : 2018/09/24
* @author : jihang
*/

#include <iostream>

#include "HLAPortInterface.h"

#define READY_TO_RUN "ReadyToRun"

using namespace std;

class HLAPortAdapter : public CHLAPortInterface {

public:
	//��Ա��Ϣ
	string fedFile;
	string federationName;
	string federateName;
	long federateId;

public:
	//1.�����ඨ��

	//Ԥ��ģ�������ƣ�Ŀǰ������
	ObjectNames objectNamesPause;
	ObjectHandles *objectHandlesPause;

	ObjectNames objectNamesContinue;
	ObjectHandles *objectHandlesContinue;

	ObjectNames objectNamesEnd;
	ObjectHandles *objectHandlesEnd;

	//���еģ��û���ӣ�����һ��
	ObjectNames objectNamesA;
	ObjectHandles *objectHandlesA;
	ObjectNames objectNamesB;
	ObjectHandles *objectHandlesB;

	//�Լ��ģ��û����,������
	RTI::ObjectHandle handleA;


public:
	//2.���캯��
	HLAPortAdapter() {
		//Ԥ��ģ��������
		objectNamesPause.className = "ObjectRoot.PAUSE";
		objectNamesPause.attributeNumber = 1;
		objectNamesPause.attributeList.push_back("pause");

		objectNamesContinue.className = "ObjectRoot.CONTINUE";
		objectNamesContinue.attributeNumber = 1;
		objectNamesContinue.attributeList.push_back("continue");

		objectNamesEnd.className = "ObjectRoot.End";
		objectNamesEnd.attributeNumber = 1;
		objectNamesEnd.attributeList.push_back("end");

		//Ԥ��ģ���Ա����
		fedFile = "testfom.fed";
		federationName = "ExampleFederation";
		federateName = "fedA";
		isPause = false;
		isEnd = false;

		//�Լ��ģ��û����
		objectNamesA.className = "ObjectRoot.A";
		objectNamesA.attributeNumber = 3;
		objectNamesA.attributeList.push_back("aa");
		objectNamesA.attributeList.push_back("ab");
		objectNamesA.attributeList.push_back("ac");

		objectNamesB.className = "ObjectRoot.B";
		objectNamesB.attributeNumber = 3;
		objectNamesB.attributeList.push_back("ba");
		objectNamesB.attributeList.push_back("bb");
		objectNamesB.attributeList.push_back("bc");
	};

	virtual ~HLAPortAdapter() {};

public:
	//3.�û�����
	void initial() {
		//Ԥ��ģ��������
		objectHandlesPause = initOneHandle(objectNamesPause);
		objectHandlesContinue = initOneHandle(objectNamesContinue);
		objectHandlesEnd = initOneHandle(objectNamesEnd);

		//�Լ��ģ��û����
		objectHandlesA = initOneHandle(objectNamesA);
		
		
	}

	void pubAndSub() {
		//Ԥ��ģ�������ƣ�ȫΪ����
		subscribeOne(objectHandlesPause);
		subscribeOne(objectHandlesContinue);
		subscribeOne(objectHandlesEnd);

		//�Լ��ģ��û����
		publishOne(objectHandlesA);
	
	}

	void rigister() {
		//�Լ��ģ��û����
		handleA = registerObject(objectNamesA.className);
		cout << "Registered Object, handle = " << handleA << endl;
	}
	
	void processData(RTI::ObjectHandle theObject,
		const RTI::AttributeHandleValuePairSet& theAttributes, const char* theTag)
		throw(RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateOwnsAttributes,
			RTI::FederateInternalError) {

		//Ԥ��ģ��յ����������Ϣ
		//���ɶ�������ȡ��������
		int classHandle = getObjectClass(theObject);
		//���ݶ��������ж�
		if (classHandle == objectHandlesPause->classHandle) {
			cout << "i should pause" << endl;
			isPause = true;
		}
		if (classHandle == objectHandlesContinue->classHandle) {
			cout << "i should continue" << endl;
			isPause = false;
		}
		if (classHandle == objectHandlesEnd->classHandle) {
			cout << "i should end" << endl;
			isEnd = true;
		}
		//�Լ��ģ��û���ӣ��յ�������Ϣ
	}

	void simulate() {
		while (true) {
			if (isEnd) {
				del();
				resignFederation();
				//�����й���������
				//destroyFederation(federationName);
				break;
			}
			send(handleA);
			advanceTime(1.0);
		}
	}

	void send(RTI::ObjectHandle handle) {
		//�Լ��ģ��û���ӣ�����������Ϣ
		RTI::AttributeHandleValuePairSet *attributes = RTI::AttributeSetFactory::create(3);
		char baValue[16], bbValue[16], bcValue[16];
		sprintf_s(baValue, "haha");
		sprintf_s(bbValue, "hehe");
		sprintf_s(bcValue, "hihi");
		attributes->add(objectHandlesA->handleList[0], baValue, (RTI::ULong)strlen(baValue) + 1);
		attributes->add(objectHandlesA->handleList[1], bbValue, (RTI::ULong)strlen(bbValue) + 1);
		attributes->add(objectHandlesA->handleList[2], bcValue, (RTI::ULong)strlen(bcValue) + 1);
		sendObject(handle, attributes);
	}

	void del() {
		deleteOne(handleA);
		
		
	}
};