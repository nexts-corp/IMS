#include <SPI.h>
#include <nRF24L01p.h>

//Topic 
#define data 0x01
#define confic 0x02

//ChType
#define ADC 0x01
#define Temp 0x01

//ChID
#define ID0 0x00
#define ID1 0x01

//Data Unit
#define V 0x01
#define C 0x02


nRF24L01p mqttdata(7, 8); //CSN,CE
void publish_Data(byte Topic,int MessageID, byte ChType,byte ChID ,float Data ,byte DataUnit);
void Callback();

typedef struct {
  byte Publish;
  byte Len;
  byte LenClientID;
  byte ClientID;
  byte LenTopic;
  byte Topic;
  int MessageID;
  byte ChID;
  byte ChType;
  float Data;
  char stage;
  byte DataUnit;
} data_t;

data_t data_Rx_t;

void setup()
{
    delay(150);
    Serial.begin(9600);
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    mqttdata.channel(90);
    mqttdata.RXaddress("ALL");
    mqttdata.TXaddress("ALL");
    mqttdata.init();
    
    Serial.println("RF Ready...");

}

 
void loop() 
{

     if( mqttdata.available())
     {
        Callback();
     }
     
}


/////////////////Publish Data/////////////////
/**
 * @Function publish_Data
 * @Param         Topic, MessageID, ClientID , ChType, ChID , Data , DataUnit
 * @ParamType     byte ,    int ,     byte      ,byte ,byte  ,float  ,byte 
 * @Return none void
 */

void publish_Data(byte Topic,int MessageID,byte ClientID, byte ChType,byte ChID ,float Data ,byte DataUnit)
{
    memset(&data_Rx_t, 0, sizeof(data_Rx_t));
    data_Rx_t.Publish = 0x30;
    data_Rx_t.LenTopic = 0x01;
    data_Rx_t.Len=0x01;
    data_Rx_t.LenClientID = 0x01;
    data_Rx_t.Len++;
    data_Rx_t.ClientID=ClientID;
    data_Rx_t.Len++;
    data_Rx_t.Topic = Topic;
    data_Rx_t.Len++;
    data_Rx_t.MessageID=MessageID;;
    data_Rx_t.Len=data_Rx_t.Len+2;
    data_Rx_t.ChID = ChID;
    data_Rx_t.Len++;
    data_Rx_t.ChType = ChType;
    data_Rx_t.Len++;
    data_Rx_t.Data = Data;
    data_Rx_t.Len=data_Rx_t.Len+4;
    data_Rx_t.DataUnit = DataUnit;
    data_Rx_t.Len++;
    
    
    mqttdata.txPL((byte*)&data_Rx_t, sizeof(data_Rx_t));
    mqttdata.send(SLOW);
}




/////////////////Callback Data/////////////////
/**
 * @Function Callback
 * @Param         
 * @ParamType 
 * @Return none void
 */
void Callback()
{
        byte Data[16];
        byte DataAck[4];
        byte MID[2];
  
         mqttdata.read();
         mqttdata.rxPL((byte*)&data_Rx_t,sizeof(data_Rx_t));
         
          memset(Data ,0, 16);
         memcpy(Data,(byte*)&data_Rx_t,sizeof(data_Rx_t));
    
        //**Print received arrays**
          Serial.print("Payload : \t");
          for(int it=0;it<data_Rx_t.Len+2;it++)
          {
                Serial.print(Data[it],HEX);
              if(it!=data_Rx_t.Len+1)
              {
                Serial.print('\t');
              }
              else{
                Serial.println();
              }
          }
          
          if(data_Rx_t.Publish==0x30)
          {
              memset(DataAck ,0, 4);
              DataAck[0]=0x40;
              DataAck[1]=0x02;
              
              memcpy(MID,(byte*)&data_Rx_t.MessageID,2);
              DataAck[2]=MID[0];
              DataAck[3]=MID[1];
              mqttdata.txPL(DataAck, sizeof(DataAck));
              mqttdata.send(SLOW);
              
          }

}




