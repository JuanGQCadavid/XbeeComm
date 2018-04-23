#include <TimerOne.h>

int unsigned numero_xbee = 2;
int unsigned mine = 1;

byte conexiones [2][8] = {{0x00,0x13,0xA2,0x00,0x40,0xB5,0xE7,0x5C},
                          {0x00,0x13,0xA2,0x00,0x40,0xB5,0xEA,0x16}};

String messesges[100];
int millis_messeges[100];
//  from_number type recive_opt data_id time_id estado(0->NoConfirmado, 1->Confirmado)
byte data_Gram [1000][6];

int data_readed = 0, data_received = 0,
    index_millis = 0, index_data_gram = 0;


boolean debug = false;

void setup(){
  Serial.begin(9600);
}

void loop(){
  Serial.println("Hola");
  
  data_Gram[0][1] = time_i;
  Serial.println(data_Gram[0][1]);
  
  delay(5000);
  
  

}

void mensajeConfirmado(int time_recivido){
  
  if(millis_messeges[])

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
  
  

void leer(){ 
  // Delimiter MSB LSB TYPE {DIR_source*8} FF FE Receive_op Data Checksum
  
  if (Serial3.available() >= 21) {
    int time_recivido = millis();
    byte sum_checkSum = 0;
    byte msb, lsb, type, receive_op,checksum;
    byte dir_source[8];
    byte dir_source_16 [2];
    byte data_space = 0;
    String mensaje = ""
    byte id_from;
      
    // Nos aseguramos que ha llegado el mensaje completo
    if (Serial3.read() == 0x7E) { // 7E es el byte de inicio
      Serial.print("");
      Serial.println("*********************");
      Serial.println("Ha llegado un mensaje");
      Serial.print("");
      
      msb = (byte) Serial3.read(); // Lee el primer byte del dato análogo
      if(debug){
        Serial.print("MSB -> ");
        Serial.print(msb, HEX);
        Serial.print(" ");
      } 
      lsb = (byte) Serial3.read(); // Lee el segundo byte del dato análogo
      if(debug){
        Serial.print("LSB -> ");
        Serial.print(lsb, HEX);
        Serial.print(" ");
      }
      data_space = lsb;
      
      type = (byte) Serial3.read();
      sum_checkSum = sum_checkSum + type;
      
      //Falta comprobar  

      
      
      if(debug){
        Serial.print("TYPE -> ");
        Serial.print(type, HEX);
        Serial.print(" ");
      }
      
      if(type == 0x8B){
        mensajeConfirmado(time_recivido);
      
      }
      
      for (int i = 0; i < 8; i = i + 1){
        byte actual_byte = (byte) Serial3.read();
        sum_checkSum = sum_checkSum + actual_byte;
        
        if(actual_byte == 0x7D){
          dir_source[i] = 0x13;
          byte trash = (byte)Serial3.read();
          continue;
        }
        
        dir_source[i] = actual_byte;
        if(debug){
          Serial.print("Dir pos - ");Serial.print(i,HEX);Serial.print(" ");
          Serial.print(dir_source[i],HEX);
          Serial.print(" ");
        }
      }
      
      //id
      
      for(int i = 0; i < numero_xbee; i = i + 1){
        if(numero_xbee == mine)continue;
        bool bandera = true;
        for(int j = 0; j < 8; j = j + 1){
          if(conexiones[i][j] != dir_source[j]){
            bandera = false;
            break;
          }
        
        }
        
        if(bandera){
          id_from = 0xi;
          break;
        }
        
        
      }
      
      
      
      
      dir_source_16 [0] = (byte) Serial3.read();
      sum_checkSum = sum_checkSum + dir_source_16[0];
      if(debug){
        Serial.print("FF -> ");
        Serial.print(dir_source_16 [0], HEX);
        Serial.print(" ");
      }
        
      dir_source_16 [1] = (byte) Serial3.read();
      sum_checkSum = sum_checkSum + dir_source_16[1];
      
      if(debug){
        Serial.print("FE -> ");
        Serial.print(dir_source_16 [1],HEX);
        Serial.print(" ");
      }
      
      receive_op = (byte) Serial3.read();
      sum_checkSum = sum_checkSum + receive_op;
      if(debug){
        Serial.print("Receive Op -> ");
        Serial.print(receive_op,HEX);
        Serial.print(" ");
      }


      do{
        byte dara_recive = (byte) Serial3.read();
        if(dara_recive == 0xFF)break;
        char dara_recive_b = dara_recive;
        String mensaje = String(dara_recive_b);
        if(debug){
          Serial.print(mensaje);
        }
      }while(true);
      
      if(debug){
        Serial.print(mensaje);
      }
      
      Serial.print("");
      //Serial.println("DONE");
     
      time_recivido = millis();
      messesges[data_received] = mensaje;
      millis_messeges[index_millis] = time_recivido;
      //  from_number type recive_opt data_id time_id estado(0->NoConfirmado, 1->Confirmado)
      data_Gram [index_data_gram][0] = id_from;
      data_Gram [index_data_gram][1] = type;
      data_Gram [index_data_gram][2] = receive_op;
      data_Gram [index_data_gram][3] = data_received;
      data_Gram [index_data_gram][0] = index_millis;
      data_Gram [index_data_gram][0] = 0x00;
      
      data_received = data_received + 1;
      index_millis = index_millis + 1;
      index_data_gram = index_data_gram + 1;
      
      confirmar_recivido();
      Serial.println("*********************");
      Serial.print("");
      
    }
  }  

  
}

