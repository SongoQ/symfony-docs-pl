.. index::
   single: Requirements
   
Wymagania do uruchomienia Symfony2
=================================

Aby uruchomić Symfony2, Twój system musi stosować się do listy wymagań. 
Możesz w łatwy sposób zobaczyć czy Twój system spełnia wszystkie wymagania poprzez uruchomienie pliku `web/config.php`
w swojej dystrybucji symfony. Od kiedy CLI często używa różnych plików ``php.ini``, dobrym pomysłem jest sprawdzenie 
wymagań z linii komend:

.. code-block:: bash

    php app/check.php

Poniżej znajduje się lista wymaganych oraz opcjonalnych wymagań które musi spełniać Twój system.

Wymagane
--------

* PHP w wersji minimum 5.3.2
* Twój PHP.ini musi mieć ustawioną wartość date.timezone

Opcjonalne
----------

* Powinieneś mieć zainstalowany moduł PHP-XML
* Powinieneś mieć bibliotekę libxml w wersji minimum 2.6.21
* Tokenizer PHP powinien być włączony
* mbstring powinen być włączony
* iconv powinien być włączony
* POSIX powinien być włączony
* Intl powinien być zainstalowany
* APC (lub też inny akcelerator powinien być zainstalowany)
* PHP.ini - zalecane ustawienia

    * short_open_tags: off
    * magic_quotes_gpc: off
    * register_globals: off
    * session.autostart: off

Doctrine
--------

Jeśli chcesz używać Doctrine, musisz mieć zainstalowane PDO. Dodatkowo,
musisz mieć zainstalowany driver PDO dla typu bazy danych którą chcesz używać.
