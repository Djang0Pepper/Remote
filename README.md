Remote(traduction en francais)
ESP8266 - Télécommande universelle
Première version stable 0.5

Description
Mise en place d'une télécommande universelle à l'aide du module ESP-12E connecté à un émetteur / récepteur RF 315/433 MHz et YS-IRTM Serial Infrared. Cela devrait vous permettre de contrôler un grand nombre d'appareils, qu'ils utilisent RF 315Mhz et 433Mhz, ou infrarouge (IR). Dans le cas d'appareils IR, il ne doit y avoir aucun obstacle entre l'émetteur IR et l'appareil à contrôler.

La page Web de configuration vous permet de définir différentes télécommandes (appareils) de type IR ou RF433 . Pour chaque télécommande, il sera possible de définir les touches (touches) en saisissant manuellement les codes relatifs ou en utilisant la détection automatique: pointez la télécommande d'origine vers le récepteur de l' ESP-12E et appuyez sur la touche à détecter.

La configuration est stockée en permanence dans la mémoire flash de l' ESP-12E .

Une fois les télécommandes et les touches correspondantes configurées, l'appareil est capable d'envoyer le code correct à l'appareil simplement en utilisant l'url:

http: // <ip_esp-12e> / sendKey / <devicename> / <keyname>

la mise en oeuvre
Le programme a été développé en utilisant l'IDE Arduino configuré pour ESP-12E . Les bibliothèques suivantes sont utilisées:

interrupteur rc (2.6.2)
ESP8266WiFi (1.0)
ESP8266WebServer (1.0)
ESP8266mDNS
DNSServer (1.1.0)
Il utilise également la bibliothèque FS ( SPIFFS ) pour gérer la mémoire Flash de l'appareil comme s'il s'agissait d'une mémoire de masse. Ce dernier sert à la fois à sauvegarder les configurations de la télécommande et à stocker un fichier contenant certaines fonctions javascript nécessaires au fonctionnement des pages de configuration.

Toutes les bibliothèques précédentes doivent être installées sur l'IDE Arduino avant de compiler le projet.

Module RF315 / 433Mhz
A cet effet, les modules MX-05V et MX-FS-03V ont été utilisés, respectivement un récepteur et un émetteur RF314 / 433Mth facilement disponibles sur Amazon ou Ebay. Le récepteur était connecté au PIN D2 de l' ESP-12E tandis que l'émetteur était connecté au PIN D1 . Ils sont pilotés par la bibliothèque rc-switch qui devrait être capable de gérer des émetteurs / récepteurs RF fonctionnant à des fréquences de 315 et 433 MHz. La bibliothèque prend en charge les chipsets suivants:

SC5262 / SC5272
HX2262 / HX2272
PT2262 / PT2272
EV1527 / RT1527 / FP1527 / HS1527
Points de vente Intertechno
Module YS-IRTM
Il s'agit d'une carte de fabrication chinoise qui comprend à la fois un récepteur et un émetteur IR (infrarouge). Celui-ci doit être connecté à une interface UART (série). Cependant, pour éviter d'engager les codes PIN série de l' ESP-12E, la bibliothèque SoftwareSerial a été utilisée qui permet d'utiliser deux GPIO pour envoyer ou recevoir des signaux de type série. Plus précisément, le PIN D5 a été utilisé comme RXD et doit être connecté au PIN IR-TXD du module, le PIN D6 a été utilisé à la place comme émetteur TXD et doit être connecté au PIN IR-RXDdu module. Le circuit de réception IR du module est toujours actif et décode automatiquement chaque signal infrarouge en envoyant le code relatif sous forme de paires de nombres hexadécimaux sur la série.

JavaScript
Les pages Web servies par l' ESP-12E utilisent largement certaines fonctions Javascript stockées dans un fichier écrit dans la mémoire Flash de l'ESP. Plus précisément, les pages de configuration ( Setup ) utilisant le framework Bootsrtap et JQuery, servaient de références externes permanentes. Par conséquent, l'appareil à partir duquel vous configurez l' ESP-12E doit être connecté à Internet.

Toutes les fonctions javascript écrites ad hoc ont été rassemblées dans le fichier devicesFunc.js dans le répertoire data du projet. Ce fichier doit être transféré dans la mémoire flash de l' ESP-12E , pour cette opération il est possible d'utiliser le plugin Sketch Data Uploader de l'environnement Arduino.

Opération
Pour que la page de configuration de la télécommande fonctionne correctement, l' ESP-12E doit d' abord être connecté au réseau Wi-Fi domestique en tant que client Wi-Fi .

Mode sans échec
Au premier démarrage, ou lorsque l' ESP-12E ne parvient pas à se connecter au réseau Wi-Fi configuré, il passe en mode de configuration sans échec . Dans ce mode, l' ESP-12E fonctionne comme un point d'accès sans fil rendant disponible un réseau Wi-Fi isolé dont le SSID est esp8266_remote et l' administrateur de mot de passe . Dans ce mode, il existe également un service DHCP pour attribuer des adresses IP et un serveur DNS minimal qui résout tout nom Web en adresse IP de l' ESP-12E .

En mode temporaire, les appareils Wi-Fi (PC, tablette, téléphone portable) connectés au réseau esp8266_remote recevront une configuration IP et pourront accéder à la page de configuration en ouvrant n'importe quelle page Web dans un navigateur. La page Web de configuration minimale de l' ESP-12E vous permet de définir le SSID et le mot de passe du réseau Wi-Fi principal auquel connecter l' ESP-12E pour un fonctionnement normal.

Pour faciliter l'identification du réseau Wi-Fi, l' ESP-12E effectue une analyse affichant tous les SSID détectés dans un contrôle Web de sélection / option . Pour le moment, il n'est pas possible de saisir manuellement un SSID.Par conséquent, les réseaux Wi-Fi qui n'exposent pas leur SSID (c'est-à-dire masqué) ne sont pas pris en charge . De plus, la configuration manuelle de l'adresse IP et d'autres paramètres Internet n'est pas prise en charge, il est donc nécessaire que le réseau Wi-Fi auquel l' ESP-12E sera connecté dispose d'un serveur DHCP qui libère ces paramètres automatiquement.

Fonctionnement normal
Uniquement en mode client Wi-Fi , l' ESP-12E servira les pages de configuration complètes. Cependant, l'envoi des clés précédemment configurées reste disponible via la page sendKey .

Traduction avec google traduction.
Ajout de la librairie Softwareserial adéquate, ancienne.