# robot-arduino: Projeto de Robô baseado em Arduino
Esse projeto foi implementado para o Arduino Romeo All-In-One (que é baseado no Arduino Uno) da empresa DFRobot. 

# Peças utilizadas
* Romeo AIO
* Plataforma robótica Rover 5
* Base de montagem Rover 5
* Placa de expansão Rover 5
* Display LCD
* Módulo I2C para o Display LCD
* Módulos de seguidor de linha (4 no total)

# Intruções de Montagem
Em breve.

# Hardware
A base do robô foi construída toda em cima da plataforma Rover 5 de forma a criar um projeto simples.
A pinagem do Romeo AIO era insuficiente com tantas interfaces de I/O necessárias, por essa razão foi decidido utilizar um módulo I2C plugado no Display LCD para que assim houvesse pinagem disponível. 

# Software
O software foi implementado em um único sketch e utilizando as seguintes bibliotecas:
* Wire
* Liquid Crystal I2C 
* Menu System

Todos eles podem ser baixados pelo Arduino 1.6.x através do menu Sketch > Include Library > Manage Libraries...

O software inicialmente foi implementado para simplesmente seguir linha baseado no algoritmo de [PID](https://en.wikipedia.org/wiki/PID_controller "Wikipedia - PID Algorithm") (Proportional-Integral-Derivative).