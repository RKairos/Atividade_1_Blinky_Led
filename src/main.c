/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

//puxa as bibliotecas para dentro do código
#include <zephyr.h> // Funções básicas do Zephyr (ex: k_msleep)
#include <device.h>  // API para obter e usar dispositivos
#include <devicetree.h>  
#include <drivers/gpio.h>  // API para controle de GPIO

//cria definiçõs de constantes e nomes

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0) //DT_ALIAS(led0) ==> pega a info de o que é o led0 no device tree

//se o led0 estiver okay ele precegue
#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
//separa e nomeia cada caracteristica do nó LED0_NODE
#define LED0	DT_GPIO_LABEL(LED0_NODE, gpios)
//DT_GPIO_LABEL(ní, propriedades) --> um nó definido no device tree e a propriedade definida para ele 
//define LED0 como representando o nome do LED0_NODE (led0) 
#define PIN	DT_GPIO_PIN(LED0_NODE, gpios)
//DT_GPIO_PIN (ní, propriedades) --> um nó definido no device tree e a propriedade definida para ele 
//Define o número do pino de led0 como PIN
#define FLAGS	DT_GPIO_FLAGS(LED0_NODE, gpios)
//DT_GPIO_FLAGS (ní, propriedades) --> um nó definido no device tree e a propriedade definida para ele 
//Define o estado eletronico do led0 (high, low, up, down) como FLAG
#else //caso o led não seja encontrado
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
//caso ele não esteja ativo ==> placa errada, define mensagem de aviso
#define LED0	""
#define PIN	0
#define FLAGS	0
//assoscia tudo a vazio
#endif // finaliza defines


// criação da função main
void main(void)
{
	const struct device *dev;
	//cria um valor constante que aponta para dev
	bool led_is_on = true;
	//cria uma variável booliana
	int ret;
	//variável inteira com nome ret

	dev = device_get_binding(LED0);
	//busca o dispositivo GPIO com nome LED0. ==> se não achar dev = null
	if (dev == NULL) {
		return;
	//se não encontrar ele ==> sai da função main sem fazer nada
	}

	ret = gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
	// define configuração de pin para o ret ==> se bem sucedida vira >0 , se da errado <0
	if (ret < 0) {
		return;
	// se não conseguiu ativar, sai do main
	}

	while (1) {
	// loop infinito
		gpio_pin_set(dev, PIN, (int)led_is_on);
		//deixa o pin ligado [true(on), false(off) e int(inverte)]
		led_is_on = !led_is_on;
		//inverte o estado atual
		k_msleep(SLEEP_TIME_MS);
		//faz pausa no tempo de sleep_time_ms
	}
}
