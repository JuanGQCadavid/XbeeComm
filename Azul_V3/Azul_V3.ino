//verde,azul
int unsigned numero_xbee = 2;
int unsigned mine = 1;
#include <TimerOne.h>
byte conexiones [2][8] = {{0x00,0x13,0xA2,0x00,0x40,0xB5,0xE7,0x5C},
                          {0x00,0x13,0xA2,0x00,0x40,0xB5,0xEA,0x16}};



byte tramaMensajeCast(String cadena);
byte tramaIdDireccion(int pos_array);
void createTrama(int ente_mensaje, String cadena);
int longitud_cadena(String cadena);
int longitud_dir(int ente_mensaje);
void leer();
void confirmar_recivido();
bool check_trama();
void modulo_print(int pos_conexiones);

void serialEvent3(){
  Serial.print("So");
  leer();
  
}

void setup() {
  Timer1.initialize(500);
  Timer1.attachInterrupt(leer);
  Serial3.begin(9600);
  Serial.begin(9600);
  
  // put your setup code here, to run once:

}

void loop() {
  
  Serial.println("Hola. Por favor seleccione una opcion.");
  Serial.println("1 -> Enviar mensaje");
  Serial.println("2 -> Verificar Conexion");
  Serial.println("3 -> Prender LED");
  Serial.println("4 -> Terminar Conexion");
  
  while(true){
    if (Serial.available() >= 1){
      String eleccion = Serial.readString();
      if(eleccion == "1"){Serial.println("Opcion 1");opcion_Enviar_Mensaje();}
      else if(eleccion == "2"){Serial.println("Opcion 2");mostrarModulosDisponibles();}
      else if(eleccion == "3"){Serial.println("Opcion 3");}
      else if(eleccion == "4"){Serial.println("Opcion 4");}
      else{Serial.println("Opcion incorrecta, vea bien las opciones mijo."); break;}
      break;
      }
    
    }
   
  //createTrama(0,"hola");
  //delay(1000);
}

void opcion_Enviar_Mensaje(){
  Serial.println("Seleccione por favor el modulo a mandar mensaje.");
  mostrarModulosDisponibles();
  while(true){
    if(Serial.available()){
      int number = (Serial.readString()).toInt();
        if(number <= numero_xbee){
          Serial.println("Introdusca el mensaje.");
          while(not(Serial.available())){}
          String mensaje = Serial.readString();

          Serial.println(number);
          Serial.println(mensaje);
          
          bool conectado = false; int times = 0, tiempo_lectura;
          
          //while(times <= 6){
            createTrama(number,mensaje);
            conectado = check_trama();

            //if(conectado == true) break;
            
            //tiempo_lectura = millis();

           // while((millis() - tiempo_lectura) < 500){}
            //times = times + 1;
            
          //}

          if(conectado){

              Serial.println("");
                Serial.println("El mensaje fue enviado con EXITO");
              Serial.println("");
            
            }else{
              Serial.println("");
                Serial.println("Error en el envio.");
              Serial.println("");
              
              }
          break;
          
          }else{
           Serial.println("El Xbee no existe");   
           break;
        }
      
      }
    
    
    }

  Serial.println("Fin de la Transmision");
  
}

void mostrarModulosDisponibles(){
  bool conectado = false;
  Serial.println();
  Serial.println("*************************");
  Serial.println("*Modulos Xbee*");
  
    for(int i = 0; i < numero_xbee; i = i + 1){
      
      
      if(i == mine) continue;
      
      long actual_time = millis();
      createTrama(i,"ping");
      while((millis() - actual_time ) < 5000){
          
          conectado = check_trama();
        }

      if(conectado == true){
        Serial.print(i);Serial.print(" -> ");modulo_print(i);
        Serial.println("ESTADO -> ON ");
      }else{
        Serial.print(i);Serial.print(" -> ");modulo_print(i);
        Serial.println("ESTADO -> OFF ");
        
      }

      conectado = false;
   }
   Serial.println("*************************");
   Serial.println();
}

void modulo_print(int pos_conexiones){
  for(int i = 0; i < 8; i = i + 1 ){
    byte dato = conexiones [pos_conexiones][i];
    Serial.print(dato, HEX);
    Serial.print(" ");
    
    }  
    Serial.println();
  
}

bool check_trama(){
  int times = millis();
  while((millis - times)< 1000){
  if ((Serial3.available()) && Serial3.read() == 0x7E){
    //Serial.println("Intersting");
    
 
  //7E 00 07 8B 01 FF FE 00 00 00 76
  //Serial.println("0");
  byte valor = Serial3.read();
  while(valor == 255){
    valor = Serial3.read();
    //Serial.println(valor);
    
  }
  
  
  if( valor != (byte)0x00) return false;
  //Serial.println("1");
  valor = Serial3.read();
  //Serial.println(valor);
  if(valor != (byte)0x07) return false;
  //Serial.println("2");
  if(Serial3.read() != (byte)0x8B) return false;
  //Serial.println("3");
  if(Serial3.read() != (byte)0x01) return false;
  //Serial.println("4");
  if(Serial3.read() != (byte)0xFF) return false;
  //Serial.println("5");
  if(Serial3.read() != (byte)0xFE) return false;
  //Serial.println("6");
  if(Serial3.read() != (byte)0x00) return false;
  //Serial.println("7");
  if(Serial3.read() != (byte)0x00) return false;
  //Serial.println("8");
  if(Serial3.read() != (byte)0x00) return false;
  //Serial.println("9");
  if(Serial3.read() != (byte)0x76) return false;
  return true;
  //Serial.println("10");
   }}
   return false;
  
}


void confirmar_recivido(){
  //Transmit Status (API 2)

    Serial3.write(0x7E);//Delimiter
    Serial3.write(0x00);
    Serial3.write(0x07); //Length lsb - 7
    Serial3.write(0x8B); //Frime type -> 8B -> Trasmit status
    Serial3.write(0X01); //Frame id
    Serial3.write(0xFF); //
    Serial3.write(0xFE); // 16 bit dest
    Serial3.write(0x00); // TX retry count -> 0
    Serial3.write(0x00); // Succes -> 0
    Serial3.write(0x00); // (No discovery overhead) -> 0
    Serial3.write(0x76); //Checksum
  }


void createTrama(int ente_mensaje, String cadena){
  int longitud_c= longitud_cadena(cadena) + 2 + longitud_dir(ente_mensaje) + 2 + 1 + 1;
                        // Long String + Frame Id and Type + id Network +  + bitaddress + broadcast ra + options
  //Start Delimiter
  Serial3.write(0x7E); 

  //lENGTH
  Serial3.write((byte)00); // Largo MSB (siempre 0)
  Serial3.write((byte)longitud_c); // LSB


  // Frame Type - 00.
  Serial3.write(0x10); 
  // Frame ID - Transmision.
  Serial3.write(0x01); 
  
  // Envía los 64 bit de la dirección de destino
  byte trama_Id_Length = tramaIdDireccion(ente_mensaje);
  
  //16 bits 
  Serial3.write(0xFF);

  
  Serial3.write(0xFE);
  
  //Broadcast Radius
  Serial3.write(0x00);

  //Options
  Serial3.write(0x00);



  byte trama_length = tramaMensajeCast(cadena);
                  // type + id + trama_dir + 16 bits Options + trama_length + Broadcast Radius
  byte sum_checkSum = 0x10 + 0x01 + trama_Id_Length +0xFF + 0xFE + 0x00 +  0x00 + trama_length;
  
  byte joder = (sum_checkSum & 0xFF);
  byte checksum = (byte)(0xFF - joder);
  
  Serial3.write(checksum); // Checksum


}
int longitud_cadena(String cadena){
  int longitud_c = 0;
  for(int i = 0; i < cadena.length() -1; i = i + 1){
    byte epecial_caracter = (byte)cadena.charAt(i);
    if(epecial_caracter == 0x13){
      longitud_c = longitud_c + 2;
    }else{longitud_c = longitud_c + 1;}   
    }
    return longitud_c;
}

int longitud_dir(int ente_mensaje){
  if(ente_mensaje == -1){
      return 8;
    }
  int sum = 0;
  for(int i = 0; i < 8; i = i + 1){
      byte dir = (byte)conexiones[ente_mensaje][i];
      if(dir == 0x13){
        //Serial3.writeln("Si senor");
        // 13 -> 7D 33
        sum = sum + 2;
      }else{
        sum = sum + 1;
      }
    }
  return sum;
  }
/*
 * -1 Broadcast
 */
 
byte tramaIdDireccion(int pos_array){
  byte sum = 0;
  if(pos_array == -1){// (Enviando 0x000000000000FFFF (broadcast))
    Serial3.write((byte)00);
    Serial3.write((byte)00); 
    Serial3.write((byte)00);
    Serial3.write((byte)00);
    Serial3.write((byte)00);
    Serial3.write((byte)00);
    Serial3.write((byte)00);
    Serial3.write(0xFF);
    Serial3.write(0xFF);
    sum = 0x0 + 0x0 + 0x0 + 0x0 + 0x0 + 0x0 + 0x0 + 0xFF + 0xFF;
    
  }else{
      for(int i = 0; i < 8; i = i + 1){
        byte dir = (byte)conexiones[pos_array][i];
        if(dir == 0x13){
          //Serial3.writeln("Si senor");
          // 13 -> 7D 33
          sum = sum + dir;
          Serial3.write(0x7D);
          Serial3.write(0x33);
        }else{
          sum = sum + dir;
          Serial3.write(dir);
        }
        
      }
  }
  return sum;
}

byte tramaMensajeCast(String cadena){
  byte number = 0;
  for(int i = 0; i < cadena.length(); i = i + 1){
    char caracter = cadena.charAt(i);
    number = number + (byte)caracter;
    Serial3.write((byte)caracter);
  }
  return number;

}



void leer(){ 
  // Delimiter MSB LSB TYPE {DIR_source*8} FF FE Receive_op Data Checksum
  
  if (Serial3.available() >= 21) {
    byte sum_checkSum = 0;
    byte msb, lsb, type, receive_op,checksum;
    byte dir_source[8];
    byte dir_source_16 [2];
    byte data_space = 0;
    String mensaje = "";
      
    // Nos aseguramos que ha llegado el mensaje completo
    if (Serial3.read() == 0x7E) { // 7E es el byte de inicio
      Serial.print("");
      Serial.println("*********************");
      Serial.println("Ha llegado un mensaje");
      Serial.print("");
      
      msb = (byte) Serial3.read(); // Lee el primer byte del dato análogo
      Serial.print("MSB -> ");
      Serial.print(msb);
      Serial.print(" ");
      
      lsb = (byte) Serial3.read(); // Lee el segundo byte del dato análogo
      Serial.print("LSB -> ");
      Serial.print(lsb);
      Serial.print(" ");
      data_space = lsb;
      
      type = (byte) Serial3.read();
      sum_checkSum = sum_checkSum + type;
      Serial.print("TYPE -> ");
      Serial.print(type);
      Serial.print(" ");

      
      for (int i = 0; i < 8; i = i + 1){
        byte actual_byte = (byte) Serial3.read();
        sum_checkSum = sum_checkSum + actual_byte;
        
        if(actual_byte == 0x7D){
            i = i - 1;
            continue;
        }
        
        dir_source[i] = actual_byte;
        Serial.print("Dir pos - ");Serial.print(i);Serial.print(" ");
        Serial.print(dir_source[i]);
        Serial.print(" ");
      }
      
      dir_source_16 [0] = (byte) Serial3.read();
      sum_checkSum = sum_checkSum + dir_source_16[0];
      Serial.print("FF -> ");
      Serial.print(dir_source_16 [0]);
      Serial.print(" ");
        
      dir_source_16 [1] = (byte) Serial3.read();
      sum_checkSum = sum_checkSum + dir_source_16[1];
      Serial.print("FE -> ");
      Serial.print(dir_source_16 [1]);
      Serial.print(" ");
      
      receive_op = (byte) Serial3.read();
      sum_checkSum = sum_checkSum + receive_op;
      Serial.print("Receive Op -> ");
      Serial.print(receive_op);
      Serial.print(" ");



      do{
        byte dara_recive = (byte) Serial3.read();
        if(dara_recive == 0xFF)break;
        char dara_recive_b = dara_recive;
        String mensaje = String(dara_recive_b);
        Serial.print(mensaje);
        
      }while(true);

      Serial.print(mensaje);

      Serial.print("");
      Serial.println("DONE");
      confirmar_recivido();
      Serial.println("*********************");
      Serial.print("");
      
    }
  }  

  
}


