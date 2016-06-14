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

// ID
#define ID 0x01


nRF24L01p mqttdata(7, 8); //CSN,CE
void publish_Data(byte Topic,int MessageID,byte ChType,byte ChID ,float Data ,byte DataUnit);

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
  byte DataUnit;
} data_t;

data_t data_tx_t;

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

int i=0;
void loop() 
{
    publish_Data(data,i,ID,ADC,ID0 ,3.3 ,V);
    i++;
    delay(1000);
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
void publish_Data(byte Topic,int MessageID,byte ClientID ,byte ChType,byte ChID ,float Data ,byte DataUnit)
{
    memset(&data_tx_t, 0, sizeof(data_tx_t));
    data_tx_t.Publish = 0x30;
    data_tx_t.LenTopic = 0x01;
    data_tx_t.Len=0x01;
    data_tx_t.LenClientID = 0x01;
    data_tx_t.Len++;
    data_tx_t.ClientID=ClientID;
    data_tx_t.Len++;
    data_tx_t.Topic = Topic;
    data_tx_t.Len++;
    data_tx_t.MessageID=MessageID;;
    data_tx_t.Len=data_tx_t.Len+2;
    data_tx_t.ChID = ChID;
    data_tx_t.Len++;
    data_tx_t.ChType = ChType;
    data_tx_t.Len++;
    data_tx_t.Data = Data;
    data_tx_t.Len=data_tx_t.Len+4;
    data_tx_t.DataUnit = DataUnit;
    data_tx_t.Len++;
    
    
    mqttdata.txPL((byte*)&data_tx_t, sizeof(data_tx_t));
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
   
         mqttdata.read();
         mqttdata.rxPL((byte*)&data_tx_t,sizeof(data_tx_t));
         
          memset(Data ,0, 16);
         memcpy(Data,(byte*)&data_tx_t,sizeof(data_tx_t));
    
        //**Print received arrays**
          Serial.print("Payload : \t");
          for(int it=0;it<data_tx_t.Len+2;it++)
          {
                Serial.print(Data[it],HEX);
              if(it!=data_tx_t.Len+1)
              {
                Serial.print('\t');
              }
              else{
                Serial.println();
              }
          }
          
          if(data_tx_t.Publish==0x30)
          {
              memset(DataAck ,0, 4);
              DataAck[0]=0x40;
              DataAck[1]=0x02;
              DataAck[2]=0x00;
              DataAck[3]=0x00;
              mqttdata.txPL(DataAck, sizeof(DataAck));
              mqttdata.send(SLOW);
              
          }
}


