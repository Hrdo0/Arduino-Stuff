
int micPin = A0;                             // analog pin to sample level from amp

int LED1 = 5;                               // indicates 1st clap
int LED2 = 6;                               // indicates 2nd clap
int LED3 = 7;                               // indicates 3rd clap



int threshold;
int level;
unsigned long prevmillis, currentmillis;    // used to call AveBGSoundLevel() once per minute

int clapcount;                              // a var to keep count of claps in ...
int dctimeout = 2;                          // ... a given timeframe. 1 second

/****************************************/

void setup() 
{
   
  pinMode(micPin, INPUT); 

  Serial.begin(115200);                     // open serial port at a fast rate so to make minimal interfere with timing
  
  //Serial.println("Running... ");
  delay(500);                               // let things settle down

  pinMode(LED1, OUTPUT);                    // We use 3 LEDs to show 1 clap, 2 claps, 3 claps detected
  pinMode(LED2, OUTPUT); 
  pinMode(LED3, OUTPUT); 


  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  
  
  FindAveBGLevel();

  prevmillis = millis();                    // get a time ref 
        
}  // End of setup()

/****************************************/
 
void loop() 
{
  int i;
  
  level = analogRead(micPin);
  Serial.println(level);
  
  if (level > threshold)                    
    {
    clapcount = 1;      
    digitalWrite(LED1, LOW);
   
    
    delay(100);                              // delay to let echo's go away and set min time between double/triple claps

    for (i = 0; i < 1000 * dctimeout; i++)  // make a number of attempts to detect another clap
      {
      level = analogRead(micPin);

      if (level > threshold)                // we have a sample above the threshold
        {
        clapcount += 1;
        delay(40);
          
        if (clapcount == 2)
          {
          digitalWrite(LED2, LOW);
          delay(40);
          }
          
        if (clapcount == 3)
          {
          Serial.println("DAJE TUTTA ZI");
          digitalWrite(LED3, LOW);
          delay(40);
          }
      }    
      delay(2);                             // set to X millisecs between samples
    }

    clapcount = 0; 
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    
    }  
  else  
    {
    delay(1);                               // short delay between samples, unless a loud sample is detected above
    }

  // determine if has been 1 minute since last recalibration of background noise
  currentmillis = millis();
  if (currentmillis > prevmillis + 60000)
    {
    prevmillis = currentmillis;            // keep prev time
    FindAveBGLevel();                      // recalibrate sound level  
    }
  
} 


void FindAveBGLevel()
{
  int i;
  unsigned long sum = 0;

  Serial.print("...calculating background sound level...  ");

  for (i = 0; i < 1000; i++)               // take 1000 samples in 1 sec
    {
    sum += analogRead(micPin);             // an analogRead() takes 100us but timing is not critical at this point
    delay(1);                                 
    }

  threshold = int((sum/1000) * 1.35);      // determine average of the samples then add a fudge factor

  //mySPL("threshold set at ", threshold);
  
} // End of FindAveBGLevel()

/****************************************/



void mySPL(String p1, unsigned long p2)
{
  
  Serial.print(p1);
  Serial.println(p2);
  
} 

/****************************************/
