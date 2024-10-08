/*
 * Nombre del fichero: main.cpp
 * Descripción: Archivo principal del programa para la placa NRF52840.
 * Autores: Carla Rumeu Montesinos y Elena Ruiz de la Blanca
 * Fecha: 30 de septiembre de 2024
 *
 * Este archivo ha sido realizado por Carla Rumeu Montesinos y Elena Ruiz de la Blanca el 30 de septiembre de 2024.
 * Contiene la implementación del programa que se ejecuta en la placa NRF52840, incluyendo la inicialización 
 * de componentes, configuración de BLE, y medición de gas y temperatura.
 * 
 * Todos los derechos reservados.
 */

// https://learn.sparkfun.com/tutorials/nrf52840-development-with-arduino-and-circuitpython

// https://stackoverflow.com/questions/29246805/can-an-ibeacon-have-a-data-payload

// --------------------------------------------------------------
// --------------------------------------------------------------
#include <bluefruit.h>

#undef min // Redefinimos para evitar conflictos con la biblioteca estándar
#undef max // Redefinimos para evitar conflictos con la biblioteca estándar

// Definimos los pines para Medidor.h
#define PIN_VGAS 28 //!< Pin para el medidor de gas
#define PIN_VREF 29 //!< Pin para la referencia de voltaje

#include "LED.h" //!< Incluye la clase para controlar el LED
#include "PuertoSerie.h" //!< Incluye la clase para la comunicación serie

// --------------------------------------------------------------
// --------------------------------------------------------------
namespace Globales {
  
  LED elLED ( /* NUMERO DEL PIN LED = */ 7 ); //!< Objeto LED inicializado en el pin 7

  PuertoSerie elPuerto ( /* velocidad = */ 115200 ); //!< Objeto para la comunicación serie a 115200 bps

  // Serial1 en el ejemplo de Curro creo que es la conexión placa-sensor 
};

#include "EmisoraBLE.h"
#include "Publicador.h"
#include "Medidor.h"

// --------------------------------------------------------------
// --------------------------------------------------------------
namespace Globales {

  Publicador elPublicador;

  Medidor elMedidor= Medidor(PIN_VGAS, PIN_VREF);

}; // namespace

/**
 * @brief Inicializa la placa 
 * @details Esta función se utiliza para realizar configuraciones iniciales
 * que pueden ser necesarias antes de comenzar el programa principal.
 * 
 * @return No devuelve ningún valor.
 */
void inicializarPlaquita () {

  // de momento nada

} // ()

/**
 * @brief Función de configuración inicial
 * details Esta función se llama una vez al inicio del programa. 
 * Aquí se inicializan los objetos y se configuran los parámetros necesarios 
 * para el funcionamiento del sistema.
 * return No devuelve ningún valor.
 */
void setup() {
  Globales::elPuerto.esperarDisponible(); // Espera a que el puerto esté disponible

  inicializarPlaquita(); // Llama a la función de inicialización

  Globales::elPublicador.encenderEmisora(); // Enciende la emisora BLE

  Globales::elMedidor.iniciarMedidor(); // Inicia el medidor de gas y temperatura

  esperar( 1000 ); // Espera 1 segundo

  Globales::elPuerto.escribir( "---- setup(): fin ---- \n " ); // Indica el fin de la configuración
} // setup ()

/**
 * @brief Función para controlar el parpadeo del LED
 * @details Esta función enciende y apaga el LED en diferentes patrones 
 * para indicar actividad del programa.
 * @return No devuelve ningún valor.
 */

inline void lucecitas() {
  using namespace Globales;

  elLED.brillar( 100 ); // 100 encendido
  esperar ( 400 ); //  100 apagado
  elLED.brillar( 100 ); // 100 encendido
  esperar ( 400 ); //  100 apagado
  Globales::elLED.brillar( 100 ); // 100 encendido
  esperar ( 400 ); //  100 apagado
  Globales::elLED.brillar( 1000 ); // 1000 encendido
  esperar ( 1000 ); //  100 apagado
} // ()

namespace Loop {
  uint8_t cont = 0;
};

/**
 * @brief Función principal del ciclo de ejecución
 * @details Esta función se ejecuta repetidamente y contiene la lógica 
 * del programa. Se encarga de medir gases y temperaturas, 
 * así como de controlar la emisión BLE.
 * @return No devuelve ningún valor.
 */ 
 void loop () {

  using namespace Loop;
  using namespace Globales;

cont++; // Incrementa el contador

  elPuerto.escribir( "\n---- loop(): empieza " ); // Inicia el registro del ciclo
  elPuerto.escribir( cont ); // Muestra el contador
  elPuerto.escribir( "\n" );

  lucecitas(); // Llama a la función de parpadeo del LED

  // Mido y publico
  int valorCO2 = elMedidor.medirGas(); // Mide el valor de CO2
  elPublicador.publicarCO2( valorCO2, cont, 1000 ); // Publica el valor de CO2

  // Mido y publico
  int valorTemperatura = elMedidor.medirTemperatura(); // Mide la temperatura
  // elPublicador.publicarTemperatura( valorTemperatura, cont, 10002);

  // Prueba para emitir un iBeacon y poner en la carga (21 bytes = uuid 16 major 2 minor 2 txPower 1 )
  char datos[21] = {
    'H', 'o', 'l', 'a',
    'H', 'o', 'l', 'a',
    'H', 'o', 'l', 'a',
    'H', 'o', 'l', 'a',
    'H', 'o', 'l', 'a',
    'H'
  };

  // elPublicador.laEmisora.emitirAnuncioIBeaconLibre ( &datos[0], 21 );
  // elPublicador.laEmisora.emitirAnuncioIBeaconLibre ( "ELENAELENAELENAELENAE", 21 );

  esperar( 2000 ); // Espera 2 segundos

  elPublicador.laEmisora.detenerAnuncio(); // Detiene la emisión

  elPuerto.escribir( "---- loop(): acaba ** " ); // Indica el fin del ciclo
  elPuerto.escribir( cont ); // Muestra el contador final
  elPuerto.escribir( "\n" );
} // loop ()
// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------