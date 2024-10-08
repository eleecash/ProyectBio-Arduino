/*
 * Nombre del fichero: Emisora.h
 * Descripción: Definición de la clase EmisoraBLE para gestionar la emisora Bluetooth de tipo BLE.
 * Autores: Carla Rumeu Montesinos y Elena Ruiz de la Blanca
 * Fecha: 3o de septiembre de 2024
 *
 * Este archivo ha sido realizado por Carla Rumeu Montesinos y Elena Ruiz de la Blanca el 30 de septiembre de 2024.
 * Contiene la implementación de la clase EmisoraBLE, que permite la configuración del nombre, 
 * el ID del fabricante y la potencia de transmisión de una emisora BLE.
 */
#ifndef EMISORA_H_INCLUIDO
#define EMISORA_H_INCLUIDO

// Buena introducción: https://learn.adafruit.com/introduction-to-bluetooth-low-energy/gap
// https://os.mbed.com/blog/entry/BLE-Beacons-URIBeacon-AltBeacons-iBeacon/

// fuente: https://www.instructables.com/id/Beaconeddystone-and-Adafruit-NRF52-Advertise-Your-/
// https://github.com/nkolban/ESP32_BLE_Arduino/blob/master/src/BLEBeacon.h

// https://os.mbed.com/blog/entry/BLE-Beacons-URIBeacon-AltBeacons-iBeacon/
// https://learn.adafruit.com/bluefruit-nrf52-feather-learning-guide/bleadvertising

// ----------------------------------------------------------
// ----------------------------------------------------------

/**
 * @brief Clase para gestionar la emisora Bluetooth de tipo BLE (Bluetooth Low Energy).
 * 
 * Esta clase encapsula las funcionalidades de la emisora BLE, permitiendo la configuración
 * del nombre, el ID del fabricante y la potencia de transmisión. Además, permite iniciar
 * la emisión de anuncios y gestionar conexiones.
 * 
 * @section diseño Diseño
 * Esta clase utiliza la biblioteca Bluefruit para interactuar con el hardware BLE. 
 * Implementa métodos para encender la emisora, emitir anuncios iBeacon y gestionar 
 * servicios y características asociadas.
 * 
 * @section ejemplos Ejemplo de uso
 * @code
 * EmisoraBLE emisora("MiEmisora", 0x4C00, 0xCA);
 * emisora.encenderEmisora();
 * @endcode
 */

#include "ServicioEnEmisora.h"

// ----------------------------------------------------------
/**
 * @class EmisoraBLE
 * @brief Clase para manejar una emisora BLE (Bluetooth Low Energy).
 */
class EmisoraBLE {
private:

    const char * nombreEmisora; ///< Nombre de la emisora BLE.
    const uint16_t fabricanteID; ///< ID del fabricante de la emisora.
    const int8_t txPower;        ///< Potencia de transmisión en dBm.
public:

  // .........................................................
  /// @brief Tipo de callback para conexión establecida.
  using CallbackConexionEstablecida = void ( uint16_t connHandle );

  /// @brief Tipo de callback para conexión terminada.
  using CallbackConexionTerminada = void ( uint16_t connHandle, uint8_t reason);

  // ......................................................... 
    /**
     * @brief Constructor de la clase EmisoraBLE.
     * 
     * Este constructor inicializa los parámetros de la emisora.
     * 
     * @param nombreEmisora_ Nombre de la emisora.
     * @param fabricanteID_ ID del fabricante.
     * @param txPower_ Potencia de transmisión.
     */
  EmisoraBLE( const char * nombreEmisora_, const uint16_t fabricanteID_,const int8_t txPower_ ):
	nombreEmisora( nombreEmisora_ ) ,
	fabricanteID( fabricanteID_ ) ,
	txPower( txPower_ )
  {
	// no encender ahora la emisora, tal vez sea por el println()
	// que hace que todo falle si lo llamo en el contructor
	// ( = antes que configuremos Serial )
	// No parece que sea por el println,
	// por tanto NO_encenderEmisora();
  } // ()

  // .........................................................
  // .........................................................
  /* creo que no me sirve esta versión porque parece
	 que no se instalen los callbacks si la emisora no está encendida,
	 pero no la puedo encender en el constructor 
  EmisoraBLE( const char * nombreEmisora_, const uint16_t fabricanteID_,
			  const int8_t txPower_,
			  CallbackConexionEstablecida cbce,
			  CallbackConexionTerminada cbct
			  ) 
	:
	EmisoraBLE ( nombreEmisora_, fabricanteID_, txPower_ )
  {
	instalarCallbackConexionEstablecida( cbce );
	instalarCallbackConexionTerminada( cbct );
  } // ()
  */
	
  // ......................................................... 
    /**
     * @brief Enciende la emisora BLE.
     * 
     * Inicializa la emisora y detiene cualquier anuncio previo.
     * @return void
     */
  void encenderEmisora() {
	// Serial.println ( "Bluefruit.begin() " );
	 Bluefruit.begin(); 

	 // por si acaso:
	 (*this).detenerAnuncio();
  } // ()

 // ......................................................... 
    /**
     * @brief Enciende la emisora y establece callbacks.
     * 
     * @param cbce Callback que se ejecuta cuando se establece una conexión.
     * @param cbct Callback que se ejecuta cuando se termina una conexión.
     * @return void
     */
  void encenderEmisora( CallbackConexionEstablecida cbce,
						CallbackConexionTerminada cbct ) {

	encenderEmisora();

	instalarCallbackConexionEstablecida( cbce );
	instalarCallbackConexionTerminada( cbct );

  } // ()

  // ......................................................... 
    /**
     * @brief Detiene la emisión de anuncios.
     * 
     * Si la emisora está actualmente anunciando, se detiene el anuncio.
     * @return void
     */
  void detenerAnuncio() {

	if ( (*this).estaAnunciando() ) {
	  // Serial.println ( "Bluefruit.Advertising.stop() " );
	  Bluefruit.Advertising.stop(); 
	}

  }  // ()
  
  // ......................................................... 
    /**
     * @brief Verifica si la emisora está anunciando.
     * 
     * @return true si está anunciando, false en caso contrario.
     */
  bool estaAnunciando() {
	return Bluefruit.Advertising.isRunning();
  } // ()

// ......................................................... 
    /**
     * @brief Emite un anuncio de tipo iBeacon.
     * 
     * @param beaconUUID Puntero al UUID del beacon.
     * @param major Número mayor del beacon.
     * @param minor Número menor del beacon.
     * @param rssi Potencia de la señal recibida.
     * @return void
     */
  void emitirAnuncioIBeacon( uint8_t * beaconUUID, int16_t major, int16_t minor, uint8_t rssi ) {

	//
	//
	//
	(*this).detenerAnuncio();
	
	//
	// creo el beacon 
	//
	BLEBeacon elBeacon( beaconUUID, major, minor, rssi );
	elBeacon.setManufacturer( (*this).fabricanteID );

	//
	// parece que esto debe ponerse todo aquí
	//

	Bluefruit.setTxPower( (*this).txPower );
	Bluefruit.setName( (*this).nombreEmisora );
	Bluefruit.ScanResponse.addName(); // para que envíe el nombre de emisora (?!)

	//
	// pongo el beacon
	//
	Bluefruit.Advertising.setBeacon( elBeacon );

	//
	// ? qué valorers poner aquí
	//
	Bluefruit.Advertising.restartOnDisconnect(true); // no hace falta, pero lo pongo
	Bluefruit.Advertising.setInterval(100, 100);    // in unit of 0.625 ms

	//
	// empieza el anuncio, 0 = tiempo indefinido (ya lo pararán)
	//
	Bluefruit.Advertising.start( 0 ); 
	
  } // ()

  // .........................................................
  //
  // Ejemplo de Beacon (31 bytes)
  //
  // https://os.mbed.com/blog/entry/BLE-Beacons-URIBeacon-AltBeacons-iBeacon/
  //
  // The iBeacon Prefix contains the hex data : 0x0201061AFF004C0215. This breaks down as follows:
  //
  // 0x020106 defines the advertising packet as BLE General Discoverable and BR/EDR high-speed incompatible.
  // Effectively it says this is only broadcasting, not connecting.
  //
  // 0x1AFF says the following data is 26 bytes long and is Manufacturer Specific Data.
  //
  // 0x004C is Apple’s Bluetooth Sig ID and is the part of this spec that makes it Apple-dependent.
  //
  // 0x02 is a secondary ID that denotes a proximity beacon, which is used by all iBeacons.
  //
  // 0x15 defines the remaining length to be 21 bytes (16+2+2+1).
  //
  // Por ejemmplo:
  //
  // 1. prefijo: 9bytes
  //       0x02, 0x01, 0x06,       // advFlags 3bytes
  //       0x1a, 0xff,             // advHeader 2 (0x1a = 26 = 25(lenght de 0x4c a 0xca)+1)   0xFF -> BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA
  //       0x4c, 0x00,             // companyID 2bytes
  //       0x02,                   // ibeacon type 1 byte
  //       0x15,                   // ibeacon length 1 byte (dec=21 lo que va a continuación: desde  la 'f' hasta 0x01)
  // 
  // 2. uuid: 16bytes
  // 'f', 'i', 's', 't', 'r', 'o', 'f', 'i', 's', 't', 'r', 'o', 0xa7, 0x10, 0x96, 0xe0
  // 
  // 2 major: 2bytes
  // 0x04, 0xd2,
  // 
  // minor: 2bytes
  // 0x10, 0xe1,
  // 
  // 0xca, // tx power : 1bytes
  //
  // 0x01, // este es el byte 31 = BLE_GAP_ADV_SET_DATA_SIZE_MAX, parece que sobra
  //
  // .........................................................
  // Para enviar como carga libre los últimos 21 bytes de un iBeacon (lo que normalmente sería uuid-16 major-2 minor-2 txPower-1)
  // .........................................................
  /*
  void emitirAnuncioIBeaconLibre( const char * carga ) {

	const uint8_t tamanyoCarga = strlen( carga );
  */


  // ......................................................... 
    /**
     * @brief Emite un anuncio de tipo iBeacon con carga libre.
     * 
     * Permite enviar una carga personalizada como parte del anuncio iBeacon.
     * 
     * @param carga Puntero a la carga a enviar.
     * @param tamanyoCarga Tamaño de la carga a enviar.
     * @return void
     */
  void emitirAnuncioIBeaconLibre( const char * carga, const uint8_t tamanyoCarga ) {

	(*this).detenerAnuncio(); 

	Bluefruit.Advertising.clearData();
	Bluefruit.ScanResponse.clearData(); // hace falta?

	// Bluefruit.setTxPower( (*this).txPower ); creo que no lo pongo porque es uno de los bytes de la parte de carga que utilizo
	Bluefruit.setName( (*this).nombreEmisora );
	Bluefruit.ScanResponse.addName();

	Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);

	// con este parece que no va  !
	// Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE);

	//
	// hasta ahora habrá, supongo, ya puestos los 5 primeros bytes. Efectivamente.
	// Falta poner 4 bytes fijos (company ID, beacon type, longitud) y 21 de carga
	//
	uint8_t restoPrefijoYCarga[4+21] = {
	  0x4c, 0x00, // companyID 2
	  0x02, // ibeacon type 1byte
	  21, // ibeacon length 1byte (dec=21)  longitud del resto // 0x15 // ibeacon length 1byte (dec=21)  longitud del resto
	  '-', '-', '-', '-', 
	  '-', '-', '-', '-', 
	  '-', '-', '-', '-', 
	  '-', '-', '-', '-', 
	  '-', '-', '-', '-', 
	  '-'
	};

	//
	// addData() hay que usarlo sólo una vez. Por eso copio la carga
	// en el anterior array, donde he dejado 21 sitios libres
	//
	memcpy( &restoPrefijoYCarga[4], &carga[0], ( tamanyoCarga > 21 ? 21 : tamanyoCarga ) ); 

	//
	// copio la carga para emitir
	//
	Bluefruit.Advertising.addData( BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA, &restoPrefijoYCarga[0], 4+21 );

	//
	// ? qué valores poner aquí ?
	//
	Bluefruit.Advertising.restartOnDisconnect(true);
	Bluefruit.Advertising.setInterval(100, 100);    // in unit of 0.625 ms

	Bluefruit.Advertising.setFastTimeout( 1 );      // number of seconds in fast mode
	//
	// empieza el anuncio, 0 = tiempo indefinido (ya lo pararán)
	//
	Bluefruit.Advertising.start( 0 ); 

	Globales::elPuerto.escribir( "emitiriBeacon libre  Bluefruit.Advertising.start( 0 );  \n");
  } // ()

  // ......................................................... 
    /**
     * @brief Añade un servicio a la emisora.
     * 
     * @param servicio Referencia al servicio a añadir.
     * @return true si se añadió correctamente, false en caso contrario.
     */
  bool anyadirServicio( ServicioEnEmisora & servicio ) {

	Globales::elPuerto.escribir( " Bluefruit.Advertising.addService( servicio ); \n");

	bool r = Bluefruit.Advertising.addService( servicio );

	if ( ! r ) {
	  Serial.println( " SERVICION NO AÑADIDO \n");
	}
	

	return r;
	 // nota: uso conversión de tipo de servicio (ServicioEnEmisora) a BLEService
	 // para addService()
  } // ()

// ......................................................... 
    /**
     * @brief Añade un servicio y sus características a la emisora.
     * 
     * @param servicio Referencia al servicio a añadir.
     * @return true si se añadió correctamente, false en caso contrario.
     */
  bool anyadirServicioConSusCaracteristicas( ServicioEnEmisora & servicio ) { 
	return (*this).anyadirServicio( servicio );
  } // 

  // ......................................................... 
    /**
     * @brief Añade un servicio y múltiples características a la emisora.
     * 
     * @tparam T Parámetros de tipo variable para características.
     * @param servicio Referencia al servicio a añadir.
     * @param caracteristica Referencia a la característica a añadir.
     * @param restoCaracteristicas Referencias a las características adicionales.
     * @return true si se añadieron correctamente, false en caso contrario.
     */
  template <typename ... T>
  bool anyadirServicioConSusCaracteristicas( ServicioEnEmisora & servicio,ServicioEnEmisora::Caracteristica & caracteristica, T& ... restoCaracteristicas) {

	servicio.anyadirCaracteristica( caracteristica );

	return anyadirServicioConSusCaracteristicas( servicio, restoCaracteristicas... );
	
  } // ()

  // .........................................................
    /** 
     * @brief Añade un servicio con sus características y lo activa.
     * Esta función permite añadir un servicio especificando sus características 
     * y lo activa una vez añadido.
     * @tparam T Tipos de las características a añadir.
     * @param servicio Referencia al servicio que se va a añadir.
     * @param restoCaracteristicas Referencia a las características adicionales
     * a añadir al servicio.
     * @return Devuelve true si el servicio se añadió correctamente, 
     * false en caso contrario.
     */

  template <typename ... T>
  bool anyadirServicioConSusCaracteristicasYActivar( ServicioEnEmisora & servicio,
													 // ServicioEnEmisora::Caracteristica & caracteristica,
													 T& ... restoCaracteristicas) {

	bool r = anyadirServicioConSusCaracteristicas( servicio, restoCaracteristicas... );

	servicio.activarServicio();

	return r;
	
  } // ()

  // .........................................................
    /**
     * @brief Instala un callback para la conexión establecida.
     * Esta función permite establecer un callback que se llamará 
     * cuando se establezca una conexión.
     * @param cb Callback que se invocará al establecer la conexión.
     */

  void instalarCallbackConexionEstablecida( CallbackConexionEstablecida cb ) {
	Bluefruit.Periph.setConnectCallback( cb );
  } // ()
  
  // .........................................................
    /**
     * @brief Instala un callback para la conexión terminada.
     * Esta función permite establecer un callback que se llamará 
     * cuando se termine una conexión.
     * @param cb Callback que se invocará al terminar la conexión.
     */
     
  void instalarCallbackConexionTerminada( CallbackConexionTerminada cb ) {
	Bluefruit.Periph.setDisconnectCallback( cb );
  } // ()

  // .........................................................
    /**
     * @brief Obtiene la conexión correspondiente a un identificador.
     * Esta función permite obtener un puntero a la conexión utilizando
     * el manejador de conexión especificado.
     * @param cb connHandle Manejador de conexión del cual se quiere obtener la conexión.
     * @return Puntero a la conexión correspondiente.
     */
  BLEConnection * getConexion( uint16_t connHandle ) {
	return Bluefruit.Connection( connHandle );
  } // ()

}; // class

#endif

// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------

