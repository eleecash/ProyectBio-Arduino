/*
 * Nombre del fichero: PuertoSerie.h
 * Descripción: Definición de la clase PuertoSerie para la comunicación a través del puerto serie.
 * Autores: Carla Rumeu Montesinos y Elena Ruiz de la Blanca
 * Fecha: 30 de septiembre de 2024
 *
 * Este archivo ha sido realizado por Carla Rumeu Montesinos y Elena Ruiz de la Blanca el 30 de septiembre de 2024.
 * Contiene la implementación de la clase PuertoSerie, que simplifica la inicialización y la escritura en el puerto serie
 * de un microcontrolador como un Arduino.
 * 
 * Todos los derechos reservados.
 */

#ifndef PUERTO_SERIE_H_INCLUIDO
#define PUERTO_SERIE_H_INCLUIDO

/**
 * Clase PuertoSerie para la comunicación a través del puerto serie.
 * 
 * Esta clase simplifica la inicialización y la escritura en el 
 * puerto serie de un microcontrolador, como un Arduino.
 */
class PuertoSerie  {

public:
  /**
   * Constructor de la clase PuertoSerie.
   * 
   * Inicializa la comunicación serie a la velocidad especificada.
   * 
   * @param baudios Velocidad de transmisión en baudios (por ejemplo, 9600).
   */
  PuertoSerie (long baudios) {
	Serial.begin( baudios );
	// mejor no poner esto aquí: while ( !Serial ) delay(10);   
  } // ()

  /**
   * Espera a que la comunicación serie esté disponible.
   * 
   * Este método bloquea la ejecución hasta que la conexión serie 
   * esté lista para ser utilizada.
   */
  void esperarDisponible() {

	while ( !Serial ) {
	  delay(10);   
	}

  } // ()

  /**
   * Envía un mensaje a través del puerto serie.
   * 
   * Este método utiliza la sobrecarga de funciones para permitir 
   * el envío de diferentes tipos de datos.
   * 
   * @tparam T Tipo del mensaje a enviar.
   * @param mensaje Mensaje que se desea enviar al puerto serie.
   */
  template<typename T>
  void escribir (T mensaje) {
	Serial.print( mensaje );
  } // ()
  
}; // class PuertoSerie

// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
#endif
