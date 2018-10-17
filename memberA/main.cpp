
/*
* 主程序流程，勿改！
* @date : 2018/09/25
* @author : jihang
*/

#include <iostream>
#include <string>

#include "HLAPortAdapter.h"

using namespace std;

void main() {
	cout << "here to start everything." << endl;

	HLAPortAdapter *ha = new HLAPortAdapter();
	ha->createFederation(ha->fedFile, ha->federationName);
	ha->federateId = ha->joinFederation(ha->federateName, ha->federationName);
	ha->initial();
	ha->registerSynchronization(READY_TO_RUN);

	cout << " >>>>>>>>>> Please start all other federates. <<<<<<<<<<" << endl;
	string line;
	getline(cin, line);

	ha->synchronizationAchieved(READY_TO_RUN);
	ha->enableTimePolicy(0.01);
	ha->pubAndSub();
	ha->rigister();
	ha->simulate();
}