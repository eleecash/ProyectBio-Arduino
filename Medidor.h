/*
 * Nombre del fichero: Medidor.h
 * Descripción: Definición de la clase Medidor para medir gas y temperatura.
 * Autores: Carla Rumeu Montesinos y Elena Ruiz de la Blanca
 * Fecha: 30 de septiembre de 2024
 *
 * Este archivo ha sido realizado por Carla Rumeu Montesinos y Elena Ruiz de la Blanca el 30 de septiembre de 2024.
 * Contiene la implementación de la clase Medidor, que permite medir la concentración de ozono en partes por millón (ppm) 
 * y proporciona un método de ejemplo para medir la temperatura.
 * 
 * Todos los derechos reservados.
 */

#ifndef MEDIDOR_H_INCLUIDO
#define MEDIDOR_H_INCLUIDO

#include <Arduino.h> // Incluir la librería de Arduino para funciones como analogRead, pinMode, etc.

/**
 * ------------------------------------------------------
 * Clase Medidor para medir gas y temperatura
 * 
 * Esta clase permite medir la concentración de ozono en partes por millón (ppm) 
 * y proporciona un método de ejemplo para medir la temperatura.
 * ------------------------------------------------------
 */
class Medidor {

private:
    uint8_t pinVref;    ///< Pin para referencia de voltaje.
    uint8_t pinVgas;    ///< Pin para leer el voltaje del gas.
    double ppmOzono;    ///< Partes por millón de Ozono (O3).
    float vref;         ///< Voltaje de referencia.
    float vgas;         ///< Voltaje del gas.

    /**
     * ------------------------------------------------------
     * Convierte un valor digital a voltios.
     * 
     * @param Vin Valor digital a convertir.
     * @return Valor en voltios.
     */
    float digToVolt(int Vin) { 
        return ((Vin * 3.3) / 1024);
    }
    
    /**
     * ------------------------------------------------------
     * Realiza una calibración lineal de la lectura.
     * 
     * Esta función toma el valor medido y ajusta la lectura usando 
     * una recta y = m * x + b, donde m es la pendiente y b es la intersección.
     * 
     * @param valorMedido Valor medido a calibrar.
     * @param m Referencia a la pendiente (se modifica).
     * @return Valor calibrado.
     */
    double calibrarLectura(double valorMedido, double &m) {
        // Ajuste manual basado en los datos proporcionados por las pruebas
        m = 0.3;  ///< Pendiente de la recta, ajustar según sea necesario.
        const double b = -1.5; ///< Intersección de la recta, ajustar según sea necesario.
        double valorCalibrado = m * valorMedido + b; // Calcula el valor calibrado

        // Si el valor calibrado es negativo, devuelve 0
        return valorCalibrado > 0 ? valorCalibrado : 0;
    }

public:

    // Constructor vacío
    Medidor(){

    }

    /**
     * Constructor
     * @param pinVgas Pin para leer el voltaje del gas.
     * @param pinVref Pin para referencia de voltaje.
     */
    Medidor(uint8_t pinVgas, uint8_t pinVref)
        : pinVref(pinVref), pinVgas(pinVgas), ppmOzono(0), vref(0), vgas(0) {
    }

    // Inicializa el medidor configurando los pines
    void iniciarMedidor() {
        vgas = 0;
        vref = 0;
        ppmOzono = 0;
        pinMode(pinVref, INPUT);
        pinMode(pinVgas, INPUT);
    }

    /**
     * Mide el gas y devuelve el valor de ppm de ozono calibrado
     * 
     * @return Valor calibrado de ppm de ozono.
     */
    double medirGas() {
        // Lee el valor de los pines del sensor
        int Agas = analogRead(pinVgas);
        int Aref = analogRead(pinVref);

        // Convierte el valor digital a voltios
        vgas = digToVolt(Agas);
        vref = digToVolt(Aref);

        // Constante de conversión basada en la especificación del sensor
        const double M = -41.96 * 499 * (0.000001);

        // Calcula las ppm de ozono
        double res = ((1 / M) * (vgas - vref));
        // Si el resultado es negativo, devuelve 0
        ppmOzono = res > 0 ? res : 0; 

        // Almacenar el valor de la pendiente
        double m;
        
        // Aplicamos la calibración lineal al valor de ppmOzono
        double ppmCalibrado = calibrarLectura(ppmOzono, m);

        // Calcular el valor de ppmOzono * 10
        int ppm10 = (int)(ppmOzono * 10);

        // Imprimir valores de referencia, gas leídos, m, ppmOzono * 10 y el valor calibrado
        Serial.print("VGAS: ");
        Serial.println(vgas);
        Serial.print("VREF: ");
        Serial.println(vref);
        Serial.print("M: ");
        Serial.println(M); // Mostrar el valor de M
        Serial.print("PPM Ozono * 10: ");
        Serial.println(ppm10); // Mostrar el valor de ppmOzono * 10
        Serial.print("PPM Ozono (calibrado): ");
        Serial.println(ppmCalibrado); // Mostrar solo el valor calibrado

        return ppmCalibrado; // Devuelve el valor calibrado
    }

    /**
     * Función de ejemplo para medir la temperatura
     * 
     * @return Valor simulado de temperatura (cambiar según el sensor de temperatura).
     */
    int medirTemperatura() {
        return -12; // Ejemplo, cambiar según el sensor de temperatura
    }

}; // class Medidor

#endif // MEDIDOR_H_INCLUIDO