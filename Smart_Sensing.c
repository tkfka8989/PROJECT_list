#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include <HCSR04.h>
#include <NewPing.h> //NewPing 라이브러리

#define TRIGGER_CH1_PIN  8 //첫번째 Trigger pin
#define ECHO_CH1_PIN     9 //첫번째 Echo pin
#define MAX_DISTANCE 500   //최대 측정 거리 설정

//첫번째 NewPing 라이브러리 생성 (핀(TRIG, ECHO)과 최대 거리 설정)
NewPing sonar_ch1(TRIGGER_CH1_PIN, ECHO_CH1_PIN, MAX_DISTANCE); 

SoftwareSerial MP3Module(10,11);  //10번 rX, 11번 tX
SoftwareSerial BTSerial(6,7);     // 소프트웨어 시리얼 객체를 6(TX),7번(RX) 으로 생성
SoftwareSerial mySerial(2,3);    // Serial 통신핀으로 D2번핀을 tx로, D3번핀을 Rx로 선언


int Radar_Signal_Read_Pin = 5;                    // Radar 모듈 신호선(3번핀)과 아두이노에 연결된 핀번호
int val = 0;                                      // Read한 Radar 모듈 신호선의 값을 저장할 변수


char buffer[20];               //통신을 할때 buffer배열에 전송받은 데이터 입력
char bufferIndex = 0; 
int n=0;
int m = 0;
int cmd = 0;
int voice_recogn=0;
int b = 0;

void setup()
{
  pinMode(Radar_Signal_Read_Pin, INPUT); // 디지털 5번핀을 GPIO INPUT으로 설정
  Serial.begin(9600); // 통신 속도 9600bps로 PC와 시리얼 통신 시작
  mySerial.begin(9600); // 통신 속도 9600bps로 모듈과 시리얼 통신 시작
  BTSerial.begin(9600);              // 소프트웨어시리얼 통신 개시, 속도는 9600
  MP3Module.begin(9600);
  mp3_set_serial (MP3Module);  //set softwareSerial for DFPlayer-mini mp3 module 
  delay(1);  //wait 1ms for mp3 module to set volume
  mp3_set_volume (30);  
  Serial.println("wait settings are in progress");
  delay(1000);
  mySerial.write(0xAA); // compact mode 사용
  mySerial.write(0x37);
  delay(1000);
  mySerial.write(0xAA); // 그룹1 음성 명령어 imported
  mySerial.write(0x21);
  Serial.println("The settings are complete");
  
}

void loop(){

    while(1){
      
      Serial.println("장애물");
      while(Serial.available()) {
        buffer[bufferIndex] = Serial.read();   //시리얼 통신으로 버퍼배열에 데이터 수신
        bufferIndex++;     //데이터 수신 후 버퍼 인덱스 1 증가
      }         
      int pos = atoi(buffer);                   //atoi()함수로 char값을 int값으로 변환
      if(pos != 0) {
        Serial.print("Input data 1 : ");
        Serial.println(pos);                    //int값으로 변환된 데이터 출력
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
        buffer[bufferIndex] = Serial.read();   //시리얼 통신으로 버퍼배열에 데이터 수신
        bufferIndex++;     //데이터 수신 후 버퍼 인덱스 1 증가
      }         
      pos = atoi(buffer);                   //atoi()함수로 char값을 int값으로 변환
      if(pos != 0) {
        Serial.print("Input data 2 : ");
        Serial.println(pos);                    //int값으로 변환된 데이터 출력
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
       Serial.println("블루투스");
       BTSerial.listen();
       Serial.println("수집중");
       delay(3000);
       if (BTSerial.available()){
        cmd =(char)BTSerial.read(); //cmd는 시리얼을 읽은 값(문자)dlrh고
        if(cmd == '1'){                 //cmd가 1이면(문자니까'1'로 표현)
          mp3_play (3);
          delay(17000);  
        } else if (cmd =='0'){      //0이면
          mp3_play (4);
          delay(23000);
        }
       } 

     }
    }   
 }
