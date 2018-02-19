#include "MyVeinsApp.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include<bits/stdc++.h>
using namespace std;
Define_Module(MyVeinsApp);

ofstream myfile;

static int data;
struct neighbour{
    int id;
    int sid;
    Coord pos;
    Coord spd;
    double dist;
    simtime_t t;
};
int s_id;
int f=0;
int MyId;
bool flag = false;
Coord rsu_pos=Coord(2500.0,5000.0,3.0);

vector<struct neighbour> v;

void MyVeinsApp::initialize(int stage) {

    BaseWaveApplLayer::initialize(stage);
    MyId=49;
    if (stage == 0) {
        //Initializing members and pointers of your application goes here
        EV << "Initializing " << par("appName").stringValue() << std::endl;
       // MyId =49;

    }
    else if (stage == 1) {
        //Initializing members that require initialized other modules goes here

    }
}

void MyVeinsApp::finish() {

//for(int i=0;i< BaseWaveApplLayer::rec.size();i++)
  //      cout<<BaseWaveApplLayer::rec[i]<<endl;

    BaseWaveApplLayer::finish();
 //   cout<<"finish finish";
    //statistics recording goes here

}

void MyVeinsApp::onBSM(BasicSafetyMessage* bsm) {
    myfile.open("routing.csv",ios::app);
        bool flag =true;

        double d=traci->getDistance(bsm->getSenderPos(),rsu_pos, 0);
       bool taken =true;
        for(int i=0;i!=v.size();i++){
            if(v[i].sid == bsm->getSenderAddress() && myId == v[i].id)
            {
                myfile<<"id \t"<<v[i].id<<" \t"<<"sender\t"<<v[i].sid<<"distance from rsu\t"<<d<<endl;
                taken =false;
                v[i].id=myId;

                v[i].sid = bsm->getSenderAddress();
                v[i].pos = bsm->getSenderSpeed();
                v[i].dist=d;
                v[i].t = simTime();

            }
        }

        if(taken){
            myfile<<"id \t"<<myId<<" \t"<<"sender\t"<<bsm->getSenderAddress()<<"distance from rsu\t"<<d<<endl;
        neighbour n;
        n.id = myId;
        n.sid = bsm->getSenderAddress();
        n.pos = bsm->getSenderPos();
        n.spd = bsm->getSenderSpeed();
        n.dist=d;
        n.t= simTime();
        v.push_back(n);

        }

        simtime_t l = simTime();
        for(int i=0;i<v.size();i++){
            simtime_t a = fabs(v[i].t - l);
            if(a>3){
                v.erase(v.begin()+i);
                cout<<myId<<" erased "<<a<<" is time "<<v[i].sid<<" sender id"<<endl;
            }

        }

myfile.close();

}

void MyVeinsApp::onWSM(WaveShortMessage* wsm) {
    //Your application has received a data message from another car or RSU
    //code for handling the message goes here, see TraciDemo11p.cc for examples
    myfile.open("receiving_file.csv", ios::app);
    myfile<<"sender\t";
    myfile<<wsm->getSenderAddress()<<"\t";
    myfile<<"msg is \t"<<wsm->getWsmData()<<"\t";
  //  myfile<<"\n";
    myfile<<"Reciever\t";
    myfile<<wsm->getRecipientAddress()<<"\n";
    myfile.close();
 //   if(wsm->getRecipientAddress() ==7)
    //        cout<<"RSU reached\n";
}

void MyVeinsApp::onWSA(WaveServiceAdvertisment* wsa) {
    //Your application has received a service advertisement from another car or RSU
    //code for handling the message goes here, see TraciDemo11p.cc for examples

}

void MyVeinsApp::handleSelfMsg(cMessage* msg) {
    BaseWaveApplLayer::handleSelfMsg(msg);


}

void MyVeinsApp::handlePositionUpdate(cObject* obj) {
    BaseWaveApplLayer::handlePositionUpdate(obj);
    //the vehicle has moved. Code that reacts to new positions goes here.
    //member variables such as currentPosition and currentSpeed are updated in the parent class
    myfile.open("sending.csv", ios :: app);
    WaveShortMessage* wsm = new WaveShortMessage();
    if(myId == MyId && !flag){

   // myfile<<"hello\n";
    populateWSM(wsm);
    string wdata = "hello_" + to_string(data);
    const char* d = wdata.c_str();
    wsm->setWsmData(d);
    data++;
    flag = true;
    f=0;
    }
   // myfile<< myId <<"\n";
    int min=INT_MAX;

    for(int i=0;i<v.size();i++){
        if(v[i].id==myId){
            if(min>v[i].dist){
                min=v[i].dist;
                s_id=v[i].sid;

            }
        }
    }
  // int ds = traci->getDistance(curPosition, rsu_pos, 0);

    if(!f){
        if(myId==MyId){
        //    if(ds < min)
          //             s_id=7;
            myfile<< myId <<"\t";
            myfile<<"sender\t";
            myfile<<"\t msg \t"<<wsm->getWsmData()<<"\t";
            myfile<< s_id<<"\t";
            myfile<<"Recipient\n";
            wsm->setSenderAddress(myId);
            wsm->setRecipientAddress(s_id);
            if(s_id == 7){
                  //  cout<<"reached";
                  // cout<<"msg "<<wsm->getWsmData()<<endl;
            }
            sendDown(wsm);
            f=1;
            cout<<"i am "<<MyId<<"and"<<s_id<<endl;
            flag = false;
            MyId = s_id;
       }
   }
 /*   else{
        myfile << myId <<"\t";
        myfile<<"sender\t";
        myfile<<"\t msg \t"<<wsm->getWsmData()<<"\t";
        myfile << s_id <<"\t";
        if(s_id == 7){
               // cout<<"RSU";
               // cout<<"msg "<<wsm->getWsmData()<<endl;
        }
        myfile << "Recipient"<<"\n";
        wsm->setSenderAddress(myId);
        wsm->setRecipientAddress(s_id);
        sendDown(wsm);
    }*/
    myfile.close();
}
