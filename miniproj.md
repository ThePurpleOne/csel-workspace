# Module

Module noyau (`fanctl.ko`) qui expose un sysfs `/sys/class/fanctl/fanctl/` avec trois attributs : `mode` (auto/manual), `frequency` (1..20 Hz, R/W), `temperature` (millidegrés, RO). Un `timer_list` fait clignoter la Status LED (GPIO10) à la fréquence configurée. Un `delayed_work` lit la zone thermique `cpu-thermal` chaque seconde ; en mode auto, applique les seuils `<35->2`, `<40->5`, `<45->10`, `≥45->20` Hz.

```
# ls -la /sys/class/fanctl/fanctl/
total 0
drwxr-xr-x 3 root root    0 Jan  1 00:04 .
drwxr-xr-x 3 root root    0 Jan  1 00:04 ..
-rw-r--r-- 1 root root 4096 Jan  1 00:06 frequency
-rw-r--r-- 1 root root 4096 Jan  1 00:06 mode
drwxr-xr-x 2 root root    0 Jan  1 00:06 power
lrwxrwxrwx 1 root root    0 Jan  1 00:06 subsystem -> ../../../../class/fanctl
-r--r--r-- 1 root root 4096 Jan  1 00:06 temperature
-rw-r--r-- 1 root root 4096 Jan  1 00:04 uevent
# cd /sys/class/fanctl/fanctl/ 
# cat temperature 
33870
# cat frequency
2
# cat mode 
auto
# cat mode
auto
# echo "manual" > mode
# echo "20" > frequency
# echo "1" > frequency
# echo "100" > frequency
sh: write error: Invalid argument
```

# DAEMON

Daemon userspace lancé en foreground par un script init. Architecture en 5 modules autour d'une boucle `epoll` :
- `buttons` : lit S1/S2/S3 (GPIO 0/2/3) via sysfs + `EPOLLPRI`, pilote la LED Power
- `comm` : serveur TCP port 1337, protocole texte ligne (`MODE`, `FREQ`, `STATUS`, `QUIT`), multi-clients avec buffer pour reads partiels.
- `oled_view` : rendu OLED via I2C-0 (driver `ssd1306`), avec skip si l'affichage est inchangé.
- `fanctl_sysfs` : R/W des attributs du module noyau.
- `daemon.c` : orchestration, `timerfd` de rafraîchissement OLED (500 ms).

```
# cat /var/log/messages 
Jan  1 00:13:20 csel daemon.info fanctl-daemon[306]: starting
Jan  1 00:13:20 csel daemon.info fanctl-daemon[306]: listening on TCP port 1337
Jan  1 00:13:24 csel daemon.info fanctl-daemon[306]: K3: mode toggled
Jan  1 00:13:26 csel daemon.info fanctl-daemon[306]: K2: freq=9 (manual)
Jan  1 00:13:27 csel daemon.info fanctl-daemon[306]: K2: freq=8 (manual)
Jan  1 00:13:28 csel daemon.info fanctl-daemon[306]: K2: freq=7 (manual)
Jan  1 00:13:30 csel daemon.info fanctl-daemon[306]: K1: freq=8 (manual)
Jan  1 00:13:30 csel daemon.info fanctl-daemon[306]: K1: freq=9 (manual)
Jan  1 00:13:31 csel daemon.info fanctl-daemon[306]: K1: freq=10 (manual)

```

# CLI

Client TCP minimal : prend une commande en `argv`, l'envoie au daemon sur `127.0.0.1:1337`, affiche la réponse et retourne 0 (succès) ou 1 (`ERR`).

```
# ./fanctl-cli STATUS
mode=auto freq=10 temp=42445
# ./fanctl-cli MODE MANUAL
OK
# ./fanctl-cli FREQ 1
OK
# ./fanctl-cli FREQ 100
ERR freq out of range
# ./fanctl-cli FREQ 20
OK
# ./fanctl-cli STATUS
mode=manual freq=20 temp=42802
```
