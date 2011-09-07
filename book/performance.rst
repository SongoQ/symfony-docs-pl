.. index::
   single: Tests

Wydajność
=========

Symfony2 jest szybkie, od razu po wyjęciu z pudełka. Oczywiście jeśli potrzebujesz
jeszcze lepszej wydajności jest kilka sposobów na przyśpieszenie Symfony. W tym
rozdziale, poznasz wiele, z najbardziej potężnych i najczęściej stosowanych sposobów
na przyśpieszenie aplikacji opartych na Symfony.

Użyj Bajtowego Cache Kodu (np. APC)
------------------------------

Jednym z najlepszych (i najłatwiejszych) sposobów na zwiększenie wydajności jest
użycie "bajtowego cache kodu". Ideą bajtowego cache kodu jest wyeliminowanie ciągłej potrzeby
przekompilowywania kodu PHP. Dostępnych jest kilka `cachy tworzących bajtowy kod`_,
kilka z nich jest open sourcowych. Najszerzej stosowanym cachem jest prawdopodobnie `APC`_

Korzystanie z cache tego typu naprawdę nie ma wad, a Symfony2 zostało zaprojektowane tak
aby radzić sobie bardzo dobrze w takim środowisku.

Dalsze Optymalizacje
~~~~~~~~~~~~~~~~~~~~

Cache bajtowego kodu zwykle monitoruje czy źródło pliku się zmieniło. Zapewnia to,
że jeśli źródło pliku zostanie zmienione, kod bajtowy zostanie przekompilowany automatycznie.

Jest to naprawdę wygodnę, ale w oczywisty sposób dodaje narzutu czasowego.

Z tego powodu, niektóre cache bajt kodu oferują wyłączenie takiego sprawdzania.
Oczywiste jest że gdy wyłączymy tą funkcjonalność, sami będziemy musieli dbać o to aby
czyścić cache gdy źródło pliku się zmieni. W przeciwnym razie zmiany które zrobiłeś
nie będą widoczne.

Dla przykładu, aby wyłączyć sprawdzanie w APC, wystarczy dodać ``apc.stat=0``
w Twoim pliku php.ini.

.. index::
   single: Performance; Autoloader

Używaj Autoloadera z cachem (np. ``ApcUniversalClassLoader``)
-------------------------------------------------------------

Domyślnie Symfony2 standard edition używa klasy ``UniversalClassLoader``
w pliku `autoloader.php`_. Ten autoloader jest łatwy w użyciu, automatycznie
będzie wyszukiwał nowe klasy znajdujące się w zarejestrowanych folderach.

Niestety, wiąże się to z kosztami, ponieważ loader iteruje po wszystkich 
skonfigurowanych przestrzeniach nazw w celu znalezienia poszczególnego pliku, wywołując
za każdym razem funkcję ``file_exists``

Prostszym rozwiązaniem jest zapisanie lokalizacji znalezionego za pierwszym razem pliku
w cache. Symfony posiada klasę loadera - ``ApcUniversalClassLoader`` - która rozszerza
klasę ``UniversalClassLoader`` oraz składuje lokalizację klas w APC.

Aby użyć tego loadera, w prosty sposób dostosuj swój ``autoloader.php``:

.. code-block:: php

    // app/autoload.php
    require __DIR__.'/../vendor/symfony/src/Symfony/Component/ClassLoader/ApcUniversalClassLoader.php';

    use Symfony\Component\ClassLoader\ApcUniversalClassLoader;

    $loader = new ApcUniversalClassLoader('some caching unique prefix');
    // ...

.. note::

    Gdy używasz APC jako swojego autoloadera, i dodasz nową klasę, zostanie ona odnaleziona
    automatycznie i wszystko będzie działało jak dotychczas (nie potrzeba "czyścić" cache).
    Jeśli jednak zmienisz lokalizację przestrzeni nazw lub prefix, będziesz musiał zrzucić
    cache APC. W przeciwnym wypadku autoloader dla tej przestrzeni nazw będzie szukał klas
    w starej lokalizacji.

.. index::
   single: Performance; Bootstrap files

Użycie Plików Bootstrap
-----------------------

Aby zapewnić optymalną elastyczność i możliwość ponownego użycia kodu, Symfony2
posiada sporą różnorodność klas oraz komponentów zewnętrznych. Ale ładowanie
tych wszystkich klas z osobnych plików przy każdym wywołaniu (request) może
dawać narzut czasowy. Aby zminimalizować ten narzut, Symfony2 Standard Edition 
udostępnia skrypt do wygenerowania pliku `bootstrap`_, który zawiera definicję 
wielu klas w jednym miejscu.
Poprzez ładowanie tego pliku (który posiada kopię wielu klas z jądra), Symfony nie 
musi więcej ładować źródła plików zawierających te klasy. To trochę zredukuje 
operacje dyskowe IO.

Jeśli używasz Symfony2 Standard Edition, w takim razie zapewne używasz pliku bootstrap.
Aby być pewnym, otwórz swój front kontroller (zwykle ``app.php``) oraz sprawdź
czy na pewno istnieje następująca linia::

    require_once __DIR__.'/../app/bootstrap.php.cache';

Zauważ że używanie pliku bootstrap posiada dwie wady:

* plik musi zostać wygenerowany ponownie gdy jakiś z plików źródła się zmieni
  (np. kiedy robisz aktualizację kodu Symfony2 lub też bibliotek vendor);

* kiedy debugujesz, będziesz musiał ustawiać "break points" w środku pliku bootstrap.

Jeśli używasz Symfony2 Standard Edition, plik bootstrap jest automatycznie 
przebudowywany po aktualizacji bibliotek vendor poprzez polecenie
``php bin/vendors install``.

Pliki Bootstrap oraz Bajtowy Cache Kodu
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Nawet przy użyciu bajtowego cache kodu wydajnośc zostanie poprawiona poprzez
używanie pliku bootstrap ponieważ będzie mniej plików do monitorowania zmian.
Oczywiście jeśli ta funkcjonalność jest wyłączona w bajtowym cache kodzie
(np. ``apc.stat=0`` w APC), nie ma powodów aby dalej używać pliku bootstrap.

.. _`cachy tworzących bajtowy kod`: http://en.wikipedia.org/wiki/List_of_PHP_accelerators
.. _`APC`: http://php.net/manual/en/book.apc.php
.. _`autoloader.php`: https://github.com/symfony/symfony-standard/blob/master/app/autoload.php
.. _`bootstrap`: https://github.com/sensio/SensioDistributionBundle/blob/master/Resources/bin/build_bootstrap.php
