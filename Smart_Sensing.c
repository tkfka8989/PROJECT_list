#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include <HCSR04.h>
#include <NewPing.h> //NewPing ���̺귯��

#define TRIGGER_CH1_PIN  8 //ù��° Trigger pin
#define ECHO_CH1_PIN     9 //ù��° Echo pin
#define MAX_DISTANCE 500   //�ִ� ���� �Ÿ� ����

//ù��° NewPing ���̺귯�� ���� (��(TRIG, ECHO)�� �ִ� �Ÿ� ����)
NewPing sonar_ch1(TRIGGER_CH1_PIN, ECHO_CH1_PIN, MAX_DISTANCE); 

SoftwareSerial MP3Module(10,11);  //10�� rX, 11�� tX
SoftwareSerial BTSerial(6,7);     // ����Ʈ���� �ø��� ��ü�� 6(TX),7��(RX) ���� ����
SoftwareSerial mySerial(2,3);    // Serial ��������� D2������ tx��, D3������ Rx�� ����


int Radar_Signal_Read_Pin = 5;                    // Radar ��� ��ȣ��(3����)�� �Ƶ��̳뿡 ����� �ɹ�ȣ
int val = 0;                                      // Read�� Radar ��� ��ȣ���� ���� ������ ����


char buffer[20];               //����� �Ҷ� buffer�迭�� ���۹��� ������ �Է�
char bufferIndex = 0; 
int n=0;
int m = 0;
int cmd = 0;
int voice_recogn=0;
int b = 0;

void setup()
{
  pinMode(Radar_Signal_Read_Pin, INPUT); // ������ 5������ GPIO INPUT���� ����
  Serial.begin(9600); // ��� �ӵ� 9600bps�� PC�� �ø��� ��� ����
  mySerial.begin(9600); // ��� �ӵ� 9600bps�� ���� �ø��� ��� ����
  BTSerial.begin(9600);              // ����Ʈ����ø��� ��� ����, �ӵ��� 9600
  MP3Module.begin(9600);
  mp3_set_serial (MP3Module);  //set softwareSerial for DFPlayer-mini mp3 module 
  delay(1);  //wait 1ms for mp3 module to set volume
  mp3_set_volume (30);  
  Serial.println("wait settings are in progress");
  delay(1000);
  mySerial.write(0xAA); // compact mode ���
  mySerial.write(0x37);
  delay(1000);
  mySerial.write(0xAA); // �׷�1 ���� ��ɾ� imported
  mySerial.write(0x21);
  Serial.println("The settings are complete");
  
}

void loop(){

    while(1){
      
      Serial.println("��ֹ�");
      while(Serial.available()) {
        buffer[bufferIndex] = Serial.read();   //�ø��� ������� ���۹迭�� ������ ����
        bufferIndex++;     //������ ���� �� ���� �ε��� 1 ����
      }         
      int pos = atoi(buffer);                   //atoi()�Լ��� char���� int������ ��ȯ
      if(pos != 0) {
        Serial.print("Input data 1 : ");
        Serial.println(pos);                    //int������ ��ȯ�� ������ ���
        if (0<pos && pos <60){
          n++;
        }
        if (59<pos && pos<160){
          m++;
        }
      }
          
      delay(1000);
      for(int a=0; a<21; a++) {
        buffer[a] = NULL;
      }
      bufferIndex = 0;
   
      while(Serial.available()) {
        buffer[bufferIndex] = Serial.read();   //�ø��� ������� ���۹迭�� ������ ����
        bufferIndex++;     //������ ���� �� ���� �ε��� 1 ����
      }         
      pos = atoi(buffer);                   //atoi()�Լ��� char���� int������ ��ȯ
      if(pos != 0) {
        Serial.print("Input data 2 : ");
        Serial.println(pos);                    //int������ ��ȯ�� ������ ���
        if(0< pos && pos<60){
          n++;
        }
        if (59<pos && pos<160){
          m++;
        }
      }
      for(int a=0; a<21; a++) {
        buffer[a] = NULL;
      }
      bufferIndex = 0;
          
      delay(1000);
      if (n>1){    
        mp3_play (2);
        delay(6000);
        n=0;
        
      }

      delay(1000);
      if (m>1){    
        mp3_play (1);
        delay(7000);
        m=0;
         
      }
      
      Serial.print("back distance : ");
      Serial.println(sonar_ch1.ping_cm());
      val = digitalRead(Radar_Signal_Read_Pin);
      Serial.println(val);
      if (0<sonar_ch1.ping_cm() && sonar_ch1.ping_cm()<50){
        b++;
      }
      if(val > 0){
        if (b>0){
          mp3_play(5);
          b = 0;
        }
      }

      Serial.println("voice input");
      delay(1000);
      mySerial.listen();    
      voice_recogn=mySerial.read();
      delay(1000);
      
      if(voice_recogn == 0x13){
       Serial.println("�������");
       BTSerial.listen();
       Serial.println("������");
       delay(3000);
       if (BTSerial.available()){
        cmd =(char)BTSerial.read(); //cmd�� �ø����� ���� ��(����)dlrh��
        if(cmd == '1'){                 //cmd�� 1�̸�(���ڴϱ�'1'�� ǥ��)
          mp3_play (3);
          delay(17000);  
        } else if (cmd =='0'){      //0�̸�
          mp3_play (4);
          delay(23000);
        }
       } 

     }
    }   
 }