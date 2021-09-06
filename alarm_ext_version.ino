/*
Manual do alarmu by Marcin Pierchala 37361:
po uruchomieniu symulatora:
swieci czerwony led
system nieaktywny

uzbrajanie:
keypad:
nacisnac start button (LEWY)
jest 20 sekund na wprowadzenie kodu aktywujacego

konsola:
wprowadzic kod aktywujacy
zatwierdzic ENTER

w konsoli bedzie informacja o stanie aktualnym alarmu

aby dezaktywowac:
keypad:
nacisnac stop button (PRAWY)
20 sekund na wprowadzenie kodu dezaktywujacego

konsola:

wprowadzic kod dezaktywujacy
potwierdzic ENTER
w konsoli bedzie informacja o stanie alarmu

zmiana hasla do uzbrajania / dezaktywacji z konsoli

komenda w konsoli do zmiany hasla uzbrajania: zmUzb
potwierdzic ENTER

komenda w konsoli do zmiany hasla dezaktywacji: zmDez
potwierdzic ENTER

pojawi sie info o statym kodzie
prosba o nowy
wprodwadzic nowe haslo
zatwierdzic ENTERnowe haslo wyswietli sie w konsoli

po zmianie hasla stan alarmu == NIEAKTYWNY

nalezy posluzyc sie nowymi kodami



*/







#include <Keypad.h>                                       //import biblioteki klawiatury matrycowej
int buzPin = 19;                                          //przypisanie pinow do modulow zewnetrznych
#define redL  11
#define greL  10
#define PIRP  A4
int stop=14;                                             //button START
int start=12;                                            //button STOP
int PIRV = LOW;                                          //zmienne i przypisanie wartosci stanow 
int status=10;
int zone=0;
int g=0;
int buzS=LOW;
int redS=LOW;
int greS=LOW;
unsigned long u=0;
unsigned long now=0;
unsigned long after=0;
String passUzb = "1234";                                // default kod Uzbrajania
String passRest = "9876";                               //default kod DEZAKTYWACJI
String readCode = "";
int pinPos = 1;
char pinC1 = '1';                                      //przypisanie znakow do poszczegolnych pozycji hasel
char pinC2 = '2';
char pinC3 = '3';
char pinC4 = '4';
char pinQ1 = '9';
char pinQ2 = '8';
char pinQ3 = '7';
char pinQ4 = '6';
String serChng ="";





const byte ilosc_wierszy= 4;                          //okreslenie budowy klawiatury matrycowej
const byte ilosc_kolumn= 4;

 char Mapa[ilosc_wierszy][ilosc_wierszy]=             //przypisanie znakow do buttonow
{
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};

byte wyprowadzenia_wierszy[ilosc_wierszy] = {9,8,7,6};
byte wyprowadzenia_kolumn[ilosc_kolumn]= {5,4,3,2};

Keypad keypd= Keypad(makeKeymap(Mapa), wyprowadzenia_wierszy, wyprowadzenia_kolumn,
ilosc_wierszy, ilosc_kolumn);                        //input z klawiatury
char but;

  
  void setup()
{
  Serial.begin(9600); //otwarcie portu transmisji szeregowej
  pinMode(start,INPUT_PULLUP);                       //ustawienie typu pinow
  pinMode(stop,INPUT_PULLUP);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(buzPin, OUTPUT);
  pinMode(redL, OUTPUT);
  pinMode(greL, OUTPUT);
  pinMode(PIRP, INPUT);
  digitalWrite(redL,HIGH);
}




void loop()
{
  u=millis();                                     //przypisanie zmiennej do czasu dzialania urzadzenia
  PIRV = digitalRead(PIRP);                       //inicjacja sygnalu z czujnika PIR
  if(digitalRead(start)==LOW)                     //aktywacja nasluchu buttonow start/stop
  {
    status=1;                                     // status pracy alarmu
  }
  else if(digitalRead(stop)==LOW)
  {
    status=2;
  }
  if(status==1)                                   //po nacisnieciu start
  {
    if(u-now>=100UL)
    {
      now=u;
      greS = !greS;
      digitalWrite(greL, greS);
    }
   but=keypd.getKey();                             //pobranie znakow z klawiatury 4x4
   if(but)                                         //uzbrajanie alarmu
   {
     if(pinPos==1&&but==pinC1)
     {
       pinPos++;
     }
     else if(pinPos==2&&but==pinC2)
     {
       pinPos++;
     }
     else if(pinPos==3&&but==pinC3)
     {
       pinPos++;
     }
     else if(pinPos==4&&but==pinC4)                              //jezeli dobry kod
     {
       Serial.println("PODANO WLASCIWY KOD");
       pinPos=1;
       status=3;
       Serial.println("SYSTEM UZBROJONY");
       digitalWrite(greL,HIGH);
       digitalWrite(redL,LOW);
       noTone(buzPin);
     }
     else                                                        //jezeli zly kod
     {
       Serial.println("PODANO ZLY KOD");
       pinPos=1;
       status=4;
       now=0;
       after=u;
     }
   }
  }
  else if(status==2)                                           //po nacisnieciu stop
  {
    noTone(buzPin);
    digitalWrite(greL,HIGH);
    if(u-now>=100UL)
    {
      now=u;
      redS = !redS;
      digitalWrite(redL, redS);
    }
   but=keypd.getKey();                                        //dezaktywacja alarmu
   if(but)
   {
     if(pinPos==1&&but==pinQ1)
     {
       pinPos++;
     }
     else if(pinPos==2&&but==pinQ2)
     {
       pinPos++;
     }
     else if(pinPos==3&&but==pinQ3)
     {
       pinPos++;
     }
     else if(pinPos==4&&but==pinQ4)
     {
       Serial.println("PODANO WLASCIWY KOD");
       pinPos=1;
       status=5;
       Serial.println("SYSTEM NIEAKTYWNY");
     }
     else
     {
       Serial.println("PODANO ZLY KOD");
       pinPos=1;
       status=4;
       now=0;
       after=u;
     }
   }
  }
    /*noTone(buzPin);
    digitalWrite(greL,LOW);
    digitalWrite(redL,HIGH);*/
  
  else if(status==3)                                          //jezeli czujnik PIR wykryje ruch w stanie uzbrojonym
  {
    if(PIRV == HIGH)
      {
      Serial.println("!!!CZUJKA!!!");
      status=4;
      delay(2000);
      now=0;
      after=u;
    }
  }
  else if(status==4)                                             //sygnalizacja dzwiekowo optyczna
  {
    if(u-now>=300UL)
    {
      now=u;
      buzS= !buzS;
      redS = !redS;
      //digitalWrite(buzPin,buzS);                 
      digitalWrite(redL, redS);
       if(redS==HIGH)
       {
         noTone(buzPin);
       }
       else
       {
        tone(buzPin, 1200);
       }
     }
    if(u-after>=10000UL)                                          //zakonczenie sygnalizacjio defaultowym czasie
     {
      noTone(buzPin);
      digitalWrite(greL,HIGH);
      digitalWrite(redL,LOW);
      Serial.println("stan czerwony po alarmie");
      status=2;
      }
    }
  else if(status==5)                                               //stan nieaktywny
  {
    noTone(buzPin);
    digitalWrite(greL,LOW);
    digitalWrite(redL,HIGH);
  }
  if(Serial.available()>0)                                         //pobranie danych z konsoli
    {
    readCode = Serial.readString();
    if(readCode==passUzb)                                          //jezeli kod uzbrojenia
      {
      Serial.println("PODANO WLASCIWY KOD");
      status=3;
      Serial.println("SYSTEM UZBROJONY");
      digitalWrite(greL,HIGH);
      digitalWrite(redL,LOW);
      noTone(buzPin);
      }
    else if(readCode==passRest)                                     //jezeli kod dezaktywacji
      {
      Serial.println("PODANO WLASCIWY KOD");
      status=5;
      Serial.println("SYSTEM NIEAKTYWNY");
      noTone(buzPin);
      digitalWrite(greL,LOW);
      digitalWrite(redL,HIGH);
      }
    else if(readCode=="zmUzb")                                    //jezeli polecenie zmiany kodu uzbrojenia
    {
      now=0;
      after=u;
      Serial.println("ZMIANA HASLA UZBRAJANIA PRZEZ KONSOLE");
      Serial.print("OBECNE HASLO : ");
      Serial.println(String(pinC1)+String(pinC2)+String(pinC3)+String(pinC4));
      Serial.println("PODAJ NOWY KOD UZBRAJANIA - 4 CYFRY!!!");
      Serial.println("ZAKONCZ ENTEREM");
      Serial.setTimeout(3000);
      serChng=Serial.readString();
      
      if(serChng==passRest)                                          //warunek niepowtarzalnosci kodu
      {
        Serial.println("!!!HASLO POKRYWA SIE Z HASLEM STANU NIEAKTYWNEGO!!!");
        Serial.println("NIE ZMIENIONO HASLA UZBRAJANIA");
        Serial.print("HASLO UZBRAJANIA TO NADAL : ");
        Serial.println(String(pinC1)+String(pinC2)+String(pinC3)+String(pinC4));      
      }
      else if(serChng[3]=='\0')
      {
        Serial.println("WPISANO BLEDNE ZNAKI LUB BRAK WPISU");
        Serial.println("HASLO NIE ZMIENIONE");
      }
      else
      {           
       passUzb=serChng;                                              //zamiana poprzedniego na nowy
       pinC1=serChng[0];
       pinC2=serChng[1];
       pinC3=serChng[2];
       pinC4=serChng[3];
      
       Serial.println("HASLO ZMIENIONE");
       passUzb=String(pinC1)+String(pinC2)+String(pinC3)+String(pinC4);
       Serial.print("NOWY KOD : ");
       Serial.println(passUzb);
       status=5;
       Serial.println("!!SYSTEM NIEAKTYWNY!!");
       Serial.println("ABY UZBROIC SYSTEM UZYJ NOWEGO KODU");
      zone=1;
     }
    }
    else if(readCode=="zmDez")                                       //jezeli polecenie zmiany kodu dezaktywujacego
    {
      now=0;
      after=u;
      Serial.println("ZMIANA HASLA DEZAKTYWACJI PRZEZ KONSOLE");
      Serial.print("OBECNE HASLO : ");
      Serial.println(String(pinQ1)+String(pinQ2)+String(pinQ3)+String(pinQ4));
      Serial.println("PODAJ NOWY KOD DEZAKTYWUJACY - 4 CYFRY!!!");
      Serial.println("ZAKONCZ ENTEREM");
      Serial.setTimeout(3000);
      serChng=Serial.readString();
      
      if(serChng==passUzb)                                           //warunek niepowtarzalnosci kodu
      {
        Serial.println("!!!HASLO POKRYWA SIE Z HASLEM UZBRAJANIA!!!");
        Serial.println("NIE ZMIENIONO HASLA DEZAKTYWUJACEGO");
        Serial.print("HASLO DEAKTYWUJACE TO NADAL : ");
        Serial.println(String(pinQ1)+String(pinQ2)+String(pinQ3)+String(pinQ4));     
      }
      else if(serChng[3]=='\0')
      {
        Serial.println("WPISANO BLEDNE ZNAKI LUB BRAK WPISU");
        Serial.println("HASLO NIE ZMIENIONE");
      }
      else
      {           
       passRest=serChng;                                                   //przypisanie nowego kodu
       pinQ1=serChng[0];
       pinQ2=serChng[1];
       pinQ3=serChng[2];
       pinQ4=serChng[3];
       
       Serial.println("HASLO ZMIENIONE");
       passRest=String(pinQ1)+String(pinQ2)+String(pinQ3)+String(pinQ4);
       Serial.print("NOWY KOD : ");
       Serial.println(passRest);
       status=5;
       Serial.println("!!SYSTEM NIEAKTYWNY!!");
       Serial.println("ABY DEZAKTYWOWAC SYSTEM UZYJ NOWEGO KODU");
      zone=1;
     }
    
    }
    else
      {
      Serial.println("PODANO ZLY KOD");                                      //jezeli podano gdziekolwiek zly kod
      status=4;
      now=0;
      after=u;
      }
  }
  
}   
