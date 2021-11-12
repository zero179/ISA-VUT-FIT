Projekt: ISA - Klient POP3 s podporou TLS
Autor: Šimon Feňko (xfenko01)

Popis programu:
Program popcl umožňuje sťahovanie e-mailových správ z e-mailovej schránky pomocou protokolu POP3. Správy sú ukladané ako jednotlivé súbory vo formáte "mail_[poradové číslo správy].txt" do zvolenej výstupnej zložky. Je podporované šifrované aj nešifrované spojenie s prípadnou možnosťou prechodu na šifrované pomocou STARTLS. Existuje možnosť sťahovania len nových a zároveň neprečítaných správ za pomoci parametru -n a takisto ak je k parametru -n pridaný aj parameter -d, tak dochádza k zmazaniu všetkých nových správ, ktoré sú neprečítané.

Příklad spuštění:

    ./popcl <server> -a <auth_file> -o <out_dir> [-p <port>] [-T|-S [-c <certfile>] [-C <certaddr]] [-d] [-n] [--help]
    <server>       - Povinný parameter špecifikujúci URL nebo IPv4/IPv6 adresu servera 
    -a <auth_file> - povinný parameter špecifikujúci súbor s prihlasovacím menom a heslom v stanovenom formáte
    -o <out_dir>   - povinný parameter špecifikujúci výstupnú zložku na ukladanie sťahovaných správ
    -p <port>      - volitelný parameter špecifikujúci číslo portu (východzí 995 pre TLS a 110 pro nezabezbečené/STARTTLS pripojenie)
    -T             - volitelný parameter špecifikujúci TLS pripojenie (nedá sa kombinovať s -S)
    -S             - volitelný parameter špecifikujúci STARTTLS pripojenie (nedá sa kombinovat s -T)
    -c <certfile>  - voliteľný parameter špecifikujúci cestu k cetifikátu. (dá sa použiť iba s -S alebo -T)
    -C <certaddr>  - voliteľný parameter špecifikujúci cestu k adresári s certifikátmi (dá sa použiť iba s -S alebo -T)
    -d             - voliteľný parameter špecifikujúci či se majú stiahnuté správy zo serveru zmazat
    -n             - voliteľný parameter špecifikujúci stiahnutie alebo vymazanie iba nových (doposiaľ nestiahnutých správ)
    --help    - voliteľný parameter pre výpis nápovedy

Návratové hodnoty:

	- 0 Všetko prebehlo správne
	- 1 Nesprávne vstupné parametre
	- 2 Nesprávna štruktúra <auth_file>
	- 3 Problém pri spojení so serverom
	- 4 Nesprávne prihlasovacie údaje
	- 5 Chyba na strane serveru
	- 6 Nenájdený súbor alebo zložka

Odevzdané soubory:

    - Makefile
	- manual.pdf
	- README
	- main.cpp 
	- Pop3.h
	- Pop3.cpp
	- ParseParams.h
	- ParseParams.cpp
