int tr=10,ec1=9,t=2;
volatile long long int irTime=0;
void isr(){
 irTime=millis();
}
long int ans;
void setup() {
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(tr,OUTPUT);
  pinMode(ec1,INPUT);
  pinMode(t,INPUT_PULLUP);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(t), isr, RISING);
  Serial.println("-------------------------St------------------");
}
float x1,y=0;
unsigned long long int timeChange=0;
unsigned long long milTime;
unsigned long long milTimechangr=0,cou=1;
void loop() {
  digitalWrite(tr,LOW);
 delayMicroseconds(2);
 digitalWrite(tr,HIGH);
 delayMicroseconds(5);
 digitalWrite(tr,LOW);
 x1=pulseIn(ec1,HIGH);
 x1=x1/29/2;
if(((x1-y)<8 || (y-x1)>8)&&cou%2){
 milTime=millis();
 y=x1;
 delay(100);
 
}

if( milTimechangr!=milTime || timeChange!=irTime ){
  cou++;
  ans=irTime-milTime;
  if(ans<0){
    ans=ans*-1;
    cou++;
  }
 
  Serial.println((((float)(ans))/1000));
  timeChange=irTime;
  milTimechangr=milTime;
   if(ans<1000){
    Serial.print("Speeeeeeeed---->");
    for(int xx=0;xx<ans/10;xx++){
         digitalWrite(5,HIGH);
         digitalWrite(5,LOW);
      }
      digitalWrite(6,HIGH);
      digitalWrite(6,LOW);
    }
      
  }
}
