# 🚗 Vehículo Autónomo y Controlado por Bluetooth

Proyecto de un vehículo robótico con doble modalidad: **control manual vía Bluetooth** y **navegación autónoma con evasión de obstáculos**. Desarrollado en Arduino Mega, integra un sensor ultrasónico y motores DC para movilidad inteligente.

[![Código en GitHub](https://img.shields.io/badge/Repositorio-GitHub-blue?logo=github)](https://github.com/tu-usuario/nombre-repositorio)

---

## 🔑 Características Clave
- **Dualidad de modos**:  
  - **Manual**: Control remoto mediante comandos Bluetooth (adelante, atrás, giros, velocidad).  
  - **Automático**: Navegación autónoma con detección de obstáculos mediante sensor ultrasónico (HC-SR04).  
- **Ajuste dinámico de velocidad**: Incremento/decremento en pasos de 25 (rango PWM: 0-255).  
- **Máquina de estados para autonomía**: Secuencia de evasión de obstáculos (detención → retroceso → giro → verificación).  

---

## 🛠️ Tecnologías Utilizadas
| Componente              | Detalles                          |
|-------------------------|-----------------------------------|
| **Placa**               | Arduino Mega 2560                |
| **Comunicación**        | Módulo Bluetooth HC-05           |
| **Sensores**            | HC-SR04 (ultrasónico)            |
| **Motores**             | 2x Motor DC con puente H (L298N) |
| **Entorno de Simulación**| Tinkercad (opcional)            |

---

## 📐 Diagrama del Circuito
```plaintext
Arduino Mega Pines:
- Motor A: IN1(25), IN2(26), ENA(5-PWM)
- Motor B: IN3(27), IN4(28), ENB(6-PWM)
- HC-SR04: Trig(30), Echo(31)
- HC-05: Serial1 (TX1/RX1)
