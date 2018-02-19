
#include "veins/modules/application/traci/TraCIDemoRSU11p.h"
#include<bits/stdc++.h>
using namespace std;
Define_Module(TraCIDemoRSU11p);
ofstream myf;

void TraCIDemoRSU11p::onWSA(WaveServiceAdvertisment* wsa) {
    //if this RSU receives a WSA for service 42, it will tune to the chan
    if (wsa->getPsid() == 42) {
        mac->changeServiceChannel(wsa->getTargetChannel());
    }
}

void TraCIDemoRSU11p::onWSM(WaveShortMessage* wsm) {
    //this rsu repeats the received traffic update in 2 seconds plus some random delay
  //  wsm->setSenderAddress(myId);
   // sendDelayedDown(wsm->dup(), 2 + uniform(0.01,0.2));
    myf.open("rsu_file.csv",ios::app);


        BaseWaveApplLayer::rec.push_back(wsm->getSenderAddress());
        if(wsm->getRecipientAddress() == 7){
            std:: cout<<"my id"<<myId<<" reci add"<<wsm->getRecipientAddress()<<endl;
                cout<<"msg "<<wsm->getWsmData() ;
                std::cout<<" sender id "<<wsm->getSenderAddress()<<endl;
                myf<<"sender "<<wsm->getSenderAddress()<<endl;
                    myf<<"msg  "<<wsm->getWsmData()<<endl;
        }
                       std::cout<<"sender id "<<wsm->getSenderAddress()<<endl;
                         std::cout<<"rsu "<<wsm->getWsmData()<<endl;
                   myf.close();

}
