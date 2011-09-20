Uruchomienie Testów Symfony2
============================

Przed wysłaniem :doc:`poprawki <patches>`, musisz uruchomić zestaw testów Symfony2
aby się upewnić że nic nie zepsułeś.

PHPUnit
-------

Aby uruchomić zestaw testów Symfony2, `zainstaluj`_ najpierw PHPUnit 3.5.0 lub nowszą:

.. code-block:: bash

    $ pear channel-discover pear.phpunit.de
    $ pear channel-discover components.ez.no
    $ pear channel-discover pear.symfony-project.com
    $ pear install phpunit/PHPUnit

Zależności (opcjonalnie)
------------------------

Aby uruchomić cały zestaw testów, włączając w to testy które zależą
od zewnętrznych zależności, Symfony2 musi mieć możliwość wczytania ich. Domyślnie,
są one wczytywane z katalogu `vendor/` z głównego katalogu (zobacz `autoload.php.dist`).

Zestaw testów potrzebuje następujące zewnętrzne biblioteki:

* Doctrine
* Swiftmailer
* Twig
* Monolog

Aby zainstalować je wszystkie, uruchom skrypt `vendors`:

.. code-block:: bash

    $ php vendors install

.. note::

    Zauważ że skrypt potrzebuje trochę czasu aby zakończyć pracę.

Po instalacji, możesz zaktualizować biblioteki zewnętrzne do ich najnowszych wersji
przy użyciu komendy:

.. code-block:: bash

    $ php vendors update

Uruchamianie
------------

Po pierwsze, zaktualizuj biblioteki zewnętrzne (zobacz powyżej).

Następnie, uruchom zestaw testów z głównego katalogu Symfony2 przy użyciu
komendy:

.. code-block:: bash

    $ phpunit

Powinieneś zobaczyć `OK`. Jeśli nie, musisz zobaczyć co jest nie tak, i czy
przypadkiem jakaś z Twoich modyfikacji nie psuje czegoś.

.. tip::

    Uruchom zestaw testów przed zastosowaniem swoich zmian, aby upewnić się że
    testy przechodzą dobrze na Twojej konfiguracji.

Pokrycie Kodu (Code Coverage)
-----------------------------

Jeśli dodasz nową funkcjonalność, musisz także sprawdzić pokrycie kodu poprzez
użycie opcji `coverage-html`:

.. code-block:: bash

    $ phpunit --coverage-html=cov/

Pokrycie kodu możesz sprawdzić poprzez otwarcie wygenerowanego pliku `cov/index.html`
w swojej przeglądarce.

.. tip::

    Sprawdzanie pokrycia kodu działa tylko wtedy gdy masz włączone rozszerzenie XDebug
    oraz gdy wszystkie zależności są zaisntalowane.

.. _zainstaluj: http://www.phpunit.de/manual/current/en/installation.html
