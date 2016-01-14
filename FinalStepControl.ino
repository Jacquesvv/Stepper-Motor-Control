//Jacques Janse Van Vuuren
//12092369
//Stepper Motor Control


//********************    Pin Definitions    ******************
#define Step 7
#define Dir 6
#define Red 11
#define Green 10
//*************************************************************


//****************    Variable Definitions    *****************
float dist =15000;           //Velocity calculation variables
float acc = 0.005;
float vel = 1400;   
float actual;

long x=0;                    //Distance count variables
long k=0;

int dir =1;                  //Toggle variables
boolean OnOff = false;
boolean LinkEst =false;
boolean Onceoff =false;
boolean toggle =false;

unsigned long timeElapsed;    //Time variables
unsigned long stepTime;
unsigned long startTimer;
unsigned long endTimer;

char floatbuffDist[32];      //Buffer Arrays
char floatbuffAcc[32];
char floatbuffVel[32];
char floatbuffDir[32];
//*************************************************************


//********************    Void Setup    ***********************
void setup() {
        pinMode(13, OUTPUT);
        pinMode(Green, OUTPUT);
        pinMode(Red, OUTPUT);
        pinMode(Step, OUTPUT);
        pinMode(Dir, OUTPUT);
        
        Serial.begin(9600);}
//*************************************************************


//********************    Void Loop    ************************
void loop() {
  timeElapsed = micros(); 
  digitalWrite(Dir,dir);
  
  if (LinkEst ==false){                        //Initial link confirmation
     Serial.println("Link Established!"); 
  LinkEst=true;}


//*************************************************************
  byte rByte = Serial.read();                  //Turn stepper ON
  if (rByte == '1'){
    digitalWrite(Green,HIGH);
    digitalWrite(Red, LOW);
    digitalWrite(13,HIGH);
    Serial.print("ON ");
    OnOff =true;}
   
   
//*************************************************************        
  if (rByte == '0' || Onceoff ==true){        //Turn stepper OFF
    digitalWrite(Green,LOW);
    digitalWrite(Red, HIGH);
        
    digitalWrite(13,LOW);
    Serial.print("OFF ");
    Onceoff = false;
    OnOff=false;
        
    x=0;    //Reset distance variables
    k=0;}  


//*************************************************************
 if (rByte =='2'){                          //Recieve new velocity and distance instructions
  while(Serial.available()==0){
  }
  
  String Disrec = Serial.readStringUntil(',');                  //Read variables from serial
  String Velrec = Serial.readStringUntil(',');
  String Accrec = Serial.readStringUntil(',');
  String Dirrec = Serial.readStringUntil(';');
  
  Disrec.toCharArray(floatbuffDist, sizeof(floatbuffDist));    //Convert variables to temporary array
  Velrec.toCharArray(floatbuffVel, sizeof(floatbuffVel));
  Accrec.toCharArray(floatbuffAcc, sizeof(floatbuffAcc));
  Dirrec.toCharArray(floatbuffDir, sizeof(floatbuffDir));
  
  dist = atof(floatbuffDist)*2;    //Turn arrays into floats / ints. Distance *2 for PWM
  vel = atof(floatbuffVel);
  acc = atof(floatbuffAcc);
  dir = atoi(floatbuffDir);
  
  Serial.print("Upload Complete!");}                   


//*************************************************************
  if (OnOff == true){            //On recieved
    //startTimer =micros();
    
  if (x==dist){
      Onceoff =true;}
    
  if (timeElapsed >= stepTime){  //Controls loop speed
      k = dist -x;
    if(x<dist){
         if(toggle == false){    //PWM
           toggle = true;
           digitalWrite( Step, LOW );}
         else if(toggle==true){
           toggle = false;
           digitalWrite( Step, HIGH );}
         
         x++;
         if(toggle == false){
         if (x > (dist/2)){
           actual= vel *(-(pow(2,-(k*acc)))+1);}    //Calculate velocity first half
         else{
           actual= vel *(-(pow(2,-(x*acc)))+1);}}    //Calculate velocity second half

    stepTime=timeElapsed + (1/(2*actual))*1000000;}
          //  Serial.print(x);                        //Use for troubleshooting
          //  Serial.print(", ");
            //endTimer =micros();
            //Serial.println(endTimer-startTimer);}
}
}

}
  
  
//*************************************************************
//               Max speed loop = 1350Hz
//               Min loop speed = 1Hz

