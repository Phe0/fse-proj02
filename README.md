# Trabalho 2 -FSE

## Introdução

Esse projeto diz respeito ao trabalho 2 da disciplina de Fundamentos de Sistemas Embarcados da Faculdade do Gama (UnB), do semestre 2021/1.

## Informações do aluno

|Nome|Matrícula|Git|
|----|---------|---|
|Pedro Henrique Andrade Féo|17/0020461|phe0|

## Como rodar

O projeto é dividido em duas partes, o servidor central e o servidor distribuído.


### Compilando

Para compilar o servidor central é necessário possuir o gcc e o make instalado.
Tendo esses requisitos cumpridosbasta utilizar o comando abaixo dentro da pasta /central_server.

```bash
make
```

Para compilar o servidor distribuído é necessário estar em um ambiente rapberryPi, além de ter o gcc e make instalados. Tendo esses requisitos cumpridos basta utilizar o comando abaixo dentro da pasta /distributed_server.

```bash
make
```

### Rodando

Para iniciar a aplicação é necessário rodar o servidor central primeiramente, então após compilar, bastarodar o seguinte comando:

```bash
./prog
```

Após iniciar o servidor central é possível iniciar o servidor distribuído, para isso é necessário rodar o comando abaixo:

```bash
./prog <json_de_configuração>
```

por exemplo:

```bash
./prog config_terro.json
```

```bash
./prog config_andar_1.json
```

### Utilizando o programa

Ao iniciar o servidor central ele abrirá duas áreas no seu terminal, um menu e uma área de informações.

O Menu da a opção de acionar dispositivos, acionar o alarme de presença ou acionar o alarme de fumaça.

Caso tenham dispositivos conectados, ao escolher a opção "Acionar dispositivos", o programa mostrará as possíveis ações que o usuário pode realizar, caso o dispositivo esteja ligado, aparecerá uma opção de desligar e caso esteja desligado, uma opção de ligar.

A área de informações mostra a temperatura, umidade e quantidade de pessoas atualizados a cada 1s.

Ao ligar o servidor distribuído ele irá tentar se conectar automaticamente ao servidor central e caso seja desligado, o servidor central irá removelo da sua lista de conexões automaticamente.