.. index::
   single: wymagania
   
Wymagania do uruchomienia Symfony2
==================================

Do uruchomienia aplikacji Symfony2 system komputerowy musi spełniać wymagania
okreśłone na liście wymagań. 
Można w łatwy sposób sprawdzić, czy system spełnia spełnia te wymagania poprzez
uruchomienie pliku `web/config.php` w swojej dystrybucji Symfony.
Ponieważ CLI często używa różnej konfiguracji pliku ``php.ini``, dobrym pomysłem
jest sprawdzenie wymagań z linii poleceń:

.. code-block:: bash

    php app/check.php

Poniżej znajduje się lista koniecznych oraz opcjonalnych wymagań.

Obowiązkowe
-----------

* PHP w wersji minimum 5.3.2;
* Włączenie JSON;
* Włączenie ctype;
* Prawidłowe ustawienie wartość opcji date.timezone w PHP.ini;

Opcjonalne
----------

* Zainstalowanie modułi PHP-XML;
* Dostępna biblioteka libxml w wersji minimum 2.6.21;
* Włączony tokenizer PHP;
* Włączony mbstring;
* Włączony iconv;
* Włączony POSIX (tylko na systemach *nixowych);
* Zainstalowany intl z ICU 4+
* Zainstalowany APC 3.0.17+ (lub inny akcelerator PHP)
* Zalecane ustawienia PHP.ini:

   * ``short_open_tag = Off``
   * ``magic_quotes_gpc = Off``
   * ``register_globals = Off``
   * ``session.auto_start = Off``

Doctrine
--------

Jeśli chce się używać Doctrine, musisz się zainstalowane PDO. Dodatkowo,
trzeba mieć zainstalowany sterownik PDO dla typu bazy danych którą chce
się używać.