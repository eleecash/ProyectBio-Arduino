/*
 * Nombre del fichero: Publicador.h
 * Descripción: Definición de la clase Publicador para emitir anuncios de datos ambientales utilizando una emisora BLE.
 * Autores: Carla Rumeu Montesinos y Elena Ruiz de la Blanca
 * Fecha: 30 de septiembre de 2024
 *
 * Este archivo ha sido realizado por Carla Rumeu Montesinos y Elena Ruiz de la Blanca el 30 de septiembre de 2024.
 * Contiene la implementación de la clase Publicador, que permite publicar datos como niveles de CO2 y temperatura 
 * utilizando una emisora BLE (Bluetooth Low Energy).
 * 
 * Todos los derechos reservados.
 */

#ifndef PUBLICADOR_H_INCLUIDO
#define PUBLICADOR_H_INCLUIDO

/** -------------------------------------------------------------- 
 * Clase Publicador para emitir anuncios de datos ambientales.
 * 
 * Esta clase permite publicar datos como niveles de CO2 y 
 * temperatura utilizando una emisora BLE (Bluetooth Low Energy).
 * -------------------------------------------------------------- */
class Publicador {

  // ............................................................
  // ............................................................
private:

  uint8_t beaconUUID[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

  // ............................................................
  // ............................................................
public:
  EmisoraBLE laEmisora {
    "yesyes",   ///< Nombre de la emisora.
    0x004c,    ///< Identificación del fabricante (Apple).
    4           ///< Potencia de transmisión (txPower).
	  };
  
  const int RSSI = -53; ///< Valor RSSI (Received Signal Strength Indicator).

  // ............................................................
  // ............................................................
public:

  /** --------------------------------------------------------------
   * Enumeración para las ID de las mediciones.
   * 
   * Incluye identificadores para CO2, temperatura y ruido.
   * -------------------------------------------------------------- */
  enum MedicionesID  {
	  CO2 = 11,           ///< ID para la medición de CO2.
    TEMPERATURA = 12,  ///< ID para la medición de temperatura.
    RUIDO = 13         ///< ID para la medición de ruido.
  };

  /** --------------------------------------------------------------
   * Constructor de la clase Publicador.
   * 
   * ATENCIÓN: no encender la emisora en el constructor. 
   * Se debe llamar a encenderEmisora() desde el método setup() más tarde.
   -------------------------------------------------------------- */
  
  Publicador( ) {
	// ATENCION: no hacerlo aquí. (*this).laEmisora.encenderEmisora();
	// Pondremos un método para llamarlo desde el setup() más tarde
  } // ()

  // ............................................................
  // Método para encender la emisora BLE.
  void encenderEmisora() {
	(*this).laEmisora.encenderEmisora();
  } // ()

  /** --------------------------------------------------------------
   * Publica el nivel de CO2.
   * 
   * Emite un anuncio IBeacon con el valor de CO2.
   * 
   * @param valorCO2 El valor de CO2 a publicar.
   * @param contador Un contador que se puede utilizar para el seguimiento.
   * @param tiempoEspera El tiempo en milisegundos a esperar 
   *                     antes de detener el anuncio.
   -------------------------------------------------------------- */
  void publicarCO2( double valorCO2, uint8_t contador, long tiempoEspera ) {

	//
	// 1. empezamos anuncio
	//
	uint16_t major = (uint16_t) (valorCO2 * 10);
	(*this).laEmisora.emitirAnuncioIBeacon( (*this).beaconUUID, major, valorCO2, (*this).RSSI);
  
  /*
	Globales::elPuerto.escribir( "   publicarCO2(): valor=" );
	Globales::elPuerto.escribir( valorCO2 );
	Globales::elPuerto.escribir( "   contador=" );
	Globales::elPuerto.escribir( contador );
	Globales::elPuerto.escribir( "   todo="  );
	Globales::elPuerto.escribir( major );
	Globales::elPuerto.escribir( "\n" );
	*/

	//
	// 2. esperamos el tiempo que nos digan
	//
	esperar( tiempoEspera );

	//
	// 3. paramos anuncio
	//
	(*this).laEmisora.detenerAnuncio();
  } // ()

  /** --------------------------------------------------------------
   * Publica la temperatura.
   * 
   * Emite un anuncio IBeacon con el valor de temperatura.
   * 
   * @param valorTemperatura El valor de temperatura a publicar.
   * @param contador Un contador que se puede utilizar para el seguimiento.
   * @param tiempoEspera El tiempo en milisegundos a esperar 
   *                     antes de detener el anuncio.
   -------------------------------------------------------------- */
  void publicarTemperatura( int16_t valorTemperatura, uint8_t contador, long tiempoEspera ) {

	uint16_t major = (MedicionesID::TEMPERATURA << 8) + contador;
	(*this).laEmisora.emitirAnuncioIBeacon( (*this).beaconUUID, major, valorTemperatura, (*this).RSSI);
	
  esperar( tiempoEspera );
  
  (*this).laEmisora.detenerAnuncio();
  } // ()
	
}; // class

// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------
#endif
