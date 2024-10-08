/*
 * Nombre del fichero: ServicioEnEmisora.h
 * Descripción: Definición de una clase para gestionar servicios y características en una emisora BLE.
 * Autores: Carla Rumeu Montesinos y Elena Ruiz de la Blanca
 * Fecha: 30 de septiembre de 2024
 *
 * Este archivo ha sido realizado por Carla Rumeu Montesinos y Elena Ruiz de la Blanca el 30 de septiembre de 2024.
 * Contiene la implementación de la clase ServicioEnEmisora, que permite la configuración, activación y manejo de servicios
 * BLE, así como la gestión de características con permisos y propiedades.
 * 
 * Todos los derechos reservados.
 */


#ifndef SERVICIO_EMISORA_H_INCLUIDO
#define SERVICIO_EMISORA_H_INCLUIDO

// ----------------------------------------------------
// Incluir la biblioteca para trabajar con vectores
#include <vector>

/**
 * @brief Utilidad alReves() para invertir el contenido de un array
 * 
 * Esta función invierte el contenido de un array de tipo T.
 * 
 * @tparam T Tipo de los elementos del array.
 * @param p Puntero al array a invertir.
 * @param n Tamaño del array.
 * @return Puntero al array invertido.
 */
template< typename T >
T* alReves(T* p, int n) {
  T aux;

  for (int i = 0; i < n / 2; i++) {
    aux = p[i];
    p[i] = p[n - i - 1];
    p[n - i - 1] = aux;
  }
  return p;
}  // ()

/**
 * @brief Convierte un string a un array de uint8_t al revés.
 * 
 * Esta función copia el contenido de una cadena a un array de uint8_t en orden inverso.
 * 
 * @param pString Puntero a la cadena de entrada.
 * @param pUint Puntero al array de salida donde se copia el contenido.
 * @param tamMax Tamaño máximo del array de salida.
 * @return Puntero al array de salida.
 */
uint8_t* stringAUint8AlReves(const char* pString, uint8_t* pUint, int tamMax) {

  int longitudString = strlen(pString);
  int longitudCopiar = (longitudString > tamMax ? tamMax : longitudString);
  // copio nombreServicio -> uuidServicio pero al revés
  for (int i = 0; i <= longitudCopiar - 1; i++) {
    pUint[tamMax - i - 1] = pString[i];
  }  // for

  return pUint;
}  // ()

/**
 * @brief Clase ServicioEnEmisora para manejar servicios y características en una emisora BLE.
 * 
 * Esta clase permite definir un servicio y sus características, manejar propiedades y permisos, y activar el servicio.
 */
class ServicioEnEmisora {

public:
  /**
   * @brief Definición de un tipo para el callback de características escritas.
   */
  using CallbackCaracteristicaEscrita = void(uint16_t conn_handle, BLECharacteristic* chr, uint8_t* data, uint16_t len);
  /**
   * @brief Clase Caracteristica para manejar características de BLE.
   */
  class Caracteristica {
  private:
    uint8_t uuidCaracteristica[16] = {  // el uuid se copia aquí (al revés) a partir de un string-c
      // least signficant byte, el primero
      '0', '1', '2', '3',
      '4', '5', '6', '7',
      '8', '9', 'A', 'B',
      'C', 'D', 'E', 'F'
    };

    BLECharacteristic laCaracteristica;

  public:

    /**
     * @brief Constructor de Caracteristica que inicializa el UUID.
     * 
     * @param nombreCaracteristica_ Nombre de la característica.
     */
    Caracteristica(const char* nombreCaracteristica_) : laCaracteristica(stringAUint8AlReves(nombreCaracteristica_, &uuidCaracteristica[0], 16)) {

    }  // ()

    /**
     * @brief Constructor de Caracteristica que inicializa propiedades, permisos y tamaño de datos.
     * 
     * @param nombreCaracteristica_ Nombre de la característica.
     * @param props Propiedades de la característica.
     * @param permisoRead Permiso de lectura.
     * @param permisoWrite Permiso de escritura.
     * @param tam Tamaño de los datos.
     */
    Caracteristica(const char* nombreCaracteristica_, uint8_t props, SecureMode_t permisoRead, SecureMode_t permisoWrite, uint8_t tam): Caracteristica(nombreCaracteristica_)  // llamada al otro constructor
    {
      (*this).asignarPropiedadesPermisosYTamanyoDatos(props, permisoRead, permisoWrite, tam);
    }  // ()

  private:
    /**
     * @brief Asigna propiedades a la característica.
     * 
     * @param props Propiedades a asignar.
     */
    void asignarPropiedades(uint8_t props) {
      // no puedo escribir AUN si el constructor llama a esto: Serial.println( " laCaracteristica.setProperties( props ); ");
      (*this).laCaracteristica.setProperties(props);
    }  // ()

    /**
     * @brief Asigna permisos de lectura y escritura a la característica.
     * 
     * @param permisoRead Permiso de lectura.
     * @param permisoWrite Permiso de escritura.
     */
    void asignarPermisos(SecureMode_t permisoRead, SecureMode_t permisoWrite) {
      // no puedo escribir AUN si el constructor llama a esto: Serial.println( "laCaracteristica.setPermission( permisoRead, permisoWrite ); " );
      (*this).laCaracteristica.setPermission(permisoRead, permisoWrite);
    }  // ()

    /**
     * @brief Asigna el tamaño de los datos de la característica.
     * 
     * @param tam Tamaño de los datos.
     */
    void asignarTamanyoDatos(uint8_t tam) {
      // no puedo escribir AUN si el constructor llama a esto: Serial.print( " (*this).laCaracteristica.setFixedLen( tam = " );
      // no puedo escribir AUN si el constructor llama a esto: Serial.println( tam );
      // (*this).laCaracteristica.setFixedLen( tam );
      (*this).laCaracteristica.setMaxLen(tam);
    }  // ()

  public:
    /**
     * @brief Asigna propiedades, permisos y tamaño de datos a la característica.
     * 
     * @param props Propiedades de la característica.
     * @param permisoRead Permiso de lectura.
     * @param permisoWrite Permiso de escritura.
     * @param tam Tamaño de los datos.
     */
    void asignarPropiedadesPermisosYTamanyoDatos(uint8_t props, SecureMode_t permisoRead, SecureMode_t permisoWrite, uint8_t tam) {
      asignarPropiedades(props);
      asignarPermisos(permisoRead, permisoWrite);
      asignarTamanyoDatos(tam);
    }  // ()

    /**
     * @brief Escribe datos en la característica.
     * 
     * @param str Cadena a escribir en la característica.
     * @return Número de bytes escritos.
     */
    uint16_t escribirDatos(const char* str) {
      // Serial.print( " return (*this).laCaracteristica.write( str  = " );
      // Serial.println( str );

      uint16_t r = (*this).laCaracteristica.write(str);
      // Serial.print( ">>>Escritos " ); Serial.print( r ); Serial.println( " bytes con write() " );
      return r;

    }  // ()

    /**
     * @brief Notifica datos a los clientes conectados.
     * 
     * @param str Cadena a notificar.
     * @return Número de bytes notificados.
     */
    uint16_t notificarDatos(const char* str) {

      uint16_t r = laCaracteristica.notify(&str[0]);

      return r;
    }  //  ()

    /**
     * @brief Instala un callback que se ejecutará cuando la característica sea escrita.
     * 
     * @param cb Callback a instalar.
     */
    void instalarCallbackCaracteristicaEscrita(CallbackCaracteristicaEscrita cb) {
      (*this).laCaracteristica.setWriteCallback(cb);
    }  // ()

    /**
     * @brief Activa la característica.
     */
    void activar() {
      err_t error = (*this).laCaracteristica.begin();
      Globales::elPuerto.escribir(" (*this).laCaracteristica.begin(); error = ");
      Globales::elPuerto.escribir(error);
    }  // ()

  };  // class Caracteristica

  // --------------------------------------------------------
  // --------------------------------------------------------
private:

  uint8_t uuidServicio[16] = {  // el uuid se copia aquí (al revés) a partir de un string-c
    // least signficant byte, el primero
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'A', 'B',
    'C', 'D', 'E', 'F'
  };

  BLEService elServicio;

  std::vector< Caracteristica* > lasCaracteristicas;

public:
    /**
   * @brief Constructor de ServicioEnEmisora que inicializa el UUID del servicio.
   * 
   * @param nombreServicio_ Nombre del servicio.
   */
  ServicioEnEmisora(const char* nombreServicio_)
  : elServicio(stringAUint8AlReves(nombreServicio_, &uuidServicio[0], 16)) {

  }  // ()

  /**
   * @brief Escribe el UUID del servicio en el puerto serie.
   */
  void escribeUUID() {
    Serial.println("****");
    for (int i = 0; i <= 15; i++) {
      Serial.print((char)uuidServicio[i]);
    }
    Serial.println("\n****");
  }  // ()

  /**
   * @brief Añade una característica al servicio.
   * 
   * @param car Referencia a la característica a añadir.
   */
  void anyadirCaracteristica(Caracteristica& car) {
    (*this).lasCaracteristicas.push_back(&car);
  }  // ()

  /**
   * @brief Activa el servicio y todas sus características.
   */
  void activarServicio() {
    // Se supone que todo ya ha sido configurado: características y servicio
        
    err_t error = (*this).elServicio.begin();
    Serial.print(" (*this).elServicio.begin(); error = ");
    Serial.println(error);

    for (auto pCar : (*this).lasCaracteristicas) {
      (*pCar).activar();
    }  // for

  }  // ()

  /**
   * @brief Conversión de tipo implícita a BLEService.
   */
  operator BLEService&() {
    // "conversión de tipo": si pongo esta clase en un sitio donde necesitan un BLEService
    return elServicio;
  }  // ()

};  // class

#endif

// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------