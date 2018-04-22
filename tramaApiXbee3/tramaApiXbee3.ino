//verde,azul
byte conexiones [5][8] = {{0x00,0x13,0xA2,0x00,0x40,0xB5,0xE7,0x5C},
                          {0x00,0x13,0xA2,0x00,0x40,0xB5,0xEA,0x16}};
int mine = 1;

byte tramaMensajeCast(String cadena);
byte tramaIdDireccion(int pos_array);
void createTrama(int ente_mensaje, String cadena);
int longitud_cadena(String cadena);
int longitud_dir(int ente_mensaje);
void leer();
void setup() {

  Serial3.begin(9600);
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  createTrama(0,"hola");
  delay(1000);
}

void leer(){
  
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

