# Aplicações de Monitoramento Ambientais e Aquáticas através de Redes de Sensores sem Fio – Um estudo de caso na região do Salgado.

Projeto do Programa Institucional de Bolsas de Iniciação Científica (PIBIC) financiado pelo Conselho Nacional de Desenvolvimento Científico e Tecnológico (CNPq) tem como proposta uma Avaliação de Desempenho em Cenários de Elevada e Baixa Taxa de Transferência para Redes de Sensores Sem Fio o trabalho também tem um objetivo fazer um estudo de caso no nordeste Paraense.

### Integrantes 

Hygor Jardim da Silva (Bolsita)

Dr. Tássio Costa de Carvalho (Orientador)

### Realização 


[Universidade Federal do Pará campus Universitário de Castanhal](http://www.campuscastanhal.ufpa.br/)

[Faculdade de Computação](http://facompcastanhal.ufpa.br/)

[Laboratório de Desenvolvimento de Sistemas](http://facompcastanhal.ufpa.br/lades/)

## Introdução:

Avanços recentes na fabricação de circuitos integrados tornam possível a integração de tecnologia de micro-sensores, computação de baixa potência e comunicação sem fio em um sistema compacto. As redes formadas por esses dispositivos, denominadas Rede de Sensores Sem Fio (RSSFs), criam oportunidades para diversas aplicações, tais como: monitoramento ambiental, monitoramento de tráfego e rodovias, agricultura de precisão, monitoramento de processos industriais, segurança pública e de ambientes em geral, transporte e controle militar [Kazem Sohraby e Daniel M, 2007].

Uma rede de sensores é uma infraestrutura composta de detecção (de medição), computação e elementos de comunicação que oferece ao administrador a capacidade de instrumentar, observar e reagir a eventos e fenômenos em um ambiente especificado. RSSFs geralmente incluem nós sensores, capazes de coletar dados e transmiti-los a outros nós da rede. Um grande número de nós sensores podem ser implantados aleatoriamente dentro ou perto da área de monitoramento, essas redes podem ser formuladas através de auto-organização dos nós [BRÖRING, 2011]. 

As atividades de sensoriamento estão relacionadas com a percepção do ambiente e a coleta de dados. De acordo com o tipo das aplicações e os tipos de sensores envolvidos, esta fase inclui a determinação de distância do alvo, ruídos do ambiente, tipo do dado coletado, volume de informação envolvida e frequência de amostragem. As RSSFs podem ter demandas de dados diferentes dependendo das dimensões, funcionalidades e quantidade dos nodos sensores. Por exemplo, as aplicações de monitoração de tráfego podem utilizar sensores de imagem e acústicos, embutidos no mesmo nodo sensor, e dependendo da quantidade de vezes que as informações são coletadas isso resultaria em uma rede com altas taxas de transferências. Por outro lado, em uma aplicação ambiental onde os sensores coletem dados a respeito da temperatura, pressão atmosférica, quantidade de luz e umidade relativa, com uma taxa de amostragem baixa (quantidade de vezes que a coleta é realizada), resultaria em uma rede com baixa taxas de transferências [Antônio A.F. Loureiro, 2010]. 

Algumas potenciais aplicações de rede sem fios, que possuem pequenas exigências de vazão e são frequentemente medidas em alguns bits por segundo, e incluem controles e monitoramentos; automação residencial e alguns eletrônicos de consumo; detecção de segurança e militar; rastreamento de ativos e gerenciamento de cadeia de energia/abastecimento; agricultura inteligente; e monitorização de saúde. Essas aplicações requerem um menor consumo de energia e uma menor complexidade de ligação sem fio. Um menor consumo energético é um problema crítico neste tipo de redes em desenvolvimento na subcamada MAC, da camada de enlace em RSSF [REF1, 2, 3]. O padrão IEEE 802.11 é um dos muitos padrões para a implantação de redes de sensores sem fio.

O IEEE 802.11 emprega um mecanismo portador de acesso múltiplo com que evita colisões (CSMA-CA) e suporta redes em estrela single-hop e redes peer-to-peer multi-hop. Em ambas as topologias de rede, o primeiro passo para iniciar está a definir o dispositivo coordenador da rede sem fio do sensor (RSSF) a fim controlar e manter recursos da rede. Este procedimento define o potencial gargalo da rede, além de definir o maior consumidor de energia de toda a rede.

Desta forma, este trabalho de pesquisa elabora um estudo do desempenho de diferentes protocolos existentes para o padrão IEEE 802.11 realizado para avaliar as redes de sensores sem fio ad hoc de taxa de dados baixas propostas por meio de vários conjuntos de simulação, incluindo o impacto das cargas de tráfego e tamanhos de carga de dados.

## Justificativa 

## Objetivos

### Objetivos Específicos

* [Aprender as abstrações do ambiente de simulação;]
* [Estudar cenários ambientais;]
* [Estudar cenários aquáticos e suas peculiaridades;]
* [Estudar cenários da região nordeste paraense;]
* [Estudar cenários de elevada e baixa transferência de dados;]
* [Realizar experimentos comparativos;]

## Materiais e Métodos

Para atingir os objetivos deste projeto, utilizou-se simulação de eventos discretos. A simulação é uma técnica baseada no desenvolvimento de modelos, isto é, abstrações da realidade, que capturam aspectos essenciais do objeto em estudo. Em geral, os modelos são especificados através de linguagens de programação de propósito geral com bibliotecas para geração de números aleatórios que serão utilizadas para o escalonamento de eventos, ou linguagens específicas. Para nosso estudo utilizou-se um simulador de eventos discretos, o Network Simulator 3 (ns-3), que é amplamente utilizado pela comunidade internacional de pesquisa em redes de computadores.

Além disso utilizou-se como fonte de informação a biblioteca digital IEEE Explorer (Institute of Electrical and Electronic Engineers) para buscar trabalhos relacionados com a temática de pesquisa, com finalidade fundamentar teoricamente e tecnicamente o projeto. Também foi usado o software GitKraken para o gerenciamento de versões dos códigos das simulações que são salvas dentro de um repositório público no Github, com intuito de facilitar o acompanhamento do orientador nas tarefas e também deixar público os artefatos gerados nesse projeto para contribuir com a comunidade acadêmica em geral. 