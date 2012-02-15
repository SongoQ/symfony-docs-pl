Jak utworzyć oraz przechowywać Projekt oparty na Symfony2 w git
===============================================================

.. tip::

    Pomimo tego że wpis ten jest o systemie kontroli wersji git, takie same
    ogólne zasady można zastosować podczas przechowywania projektu w Subversion.

Jeśli przeczytałeś :doc:`/book/page_creation` oraz zapoznałeś się z używaniem Symfony,
bez wątpienia jesteś gotowy do rozpoczęcia swojego projektu. W tym artykule, dowiesz
się jak najlepiej rozpocząć nowy projekt który będzie przechowywany przy użyciu
systemu kontroli wersji `git`_.

Wstępna Konfiguracja Projektu
-----------------------------

Aby zacząć, będziesz musiał pobrać Symfony oraz zainicjować lokalne repozytorium git:

1. Pobierz wersję `Symfony2 Standard Edition`_ bez dodatków ("without vendors").

2. Rozpakuj dystrybucję. Po rozpakowaniu zobaczysz folder o nazwie Symfony z 
   nową strukturą projektu, plikami konfiguracyjnymi, itd. Możesz dowolnie zmienić
   nazwę tego katalogu.

3. Utwórz nowy plik ``.gitignore`` w głównym katalogu Twojego nowego projektu (tam gdzie
   znajduje się plik ``deps``) oraz wklej poniższą treść. Pliki pasujące do tego wzorca
   zostaną zignorowane przez git:

    .. code-block:: text

        /web/bundles/
        /app/bootstrap*
        /app/cache/*
        /app/logs/*
        /vendor/  
        /app/config/parameters.ini

4. Skopiuj ``app/config/parameters.ini`` do ``app/config/parameters.ini.dist``.
   Plik ``parameters.ini`` jest ignorowany przez git (zobacz wyżej) więc ustawienia
   specyficzne dla danego komputera jak np. hasło bazy danych nie jest wysyłane do
   repozytorium. Poprzez utworzenie pliku ``parameters.ini.dist``, nowi deweloperzy mogą
   szybko sklonować projekt, skopiować ten plik do pliku ``parameters.ini``, dostosować go,
   i rozpocząć pracę.

5. Inicjalizowanie repozytorium git:

    .. code-block:: bash
    
        $ git init

6. Dodaj wszystkie początkowe pliki do git:

    .. code-block:: bash
    
        $ git add .

7. Utwórz pierwszy commit z swoim startowym projektem:

    .. code-block:: bash
    
        $ git commit -m "Initial commit"

8. Ostatecznie, pobierz wszystkie dodatkowe biblioteki:

    .. code-block:: bash
    
        $ php bin/vendors install

Od tego miejsca, masz w pełni funkcjonalny projekt w Symfony2 który jest
poprawnie zacommitowany do git. Możesz od razu rozpocząć pracę, oraz wysyłać
nowe zmiany do swojego repozytorium git.

Możesz kontynuować naukę z rozdziałem :doc:`/book/page_creation` aby dowiedzieć
się więcej na temat konfigurowania oraz rozwijania swojej aplikacji.

.. tip::

    Symfony2 Standard Edition posiada pewną ilość przykładowej funkcjonalności.
    Aby usunąć przykładowy kod, postępuj zgodnie z instrukcją `Standard Edition Readme`_.

.. _cookbook-managing-vendor-libraries:

Zarządzanie Bibliotekami Zewnętrznymi (Vendor) z bin/vendors oraz deps
----------------------------------------------------------------------

Każdy projekt Symfony wykorzystuje dużą grupę bibliotek zewnętrznych "vendor".

Domyślnie, biblioteki te są pobierane poprzez uruchomienie komendy ``php bin/vendors install``.
Skrypt ten czyta z pliku ``deps``, oraz pobiera zdefiniowane biblioteki do katalogu ``vendor/``.
Zaczytywana jest także zawartość pliku ``deps.lock``, która zawiera dokładny hash commitu
który ma zostać pobrany.

W tym ustawieniu, biblioteki "vendors" nie są częścią Twojego repozytorium git,
nie są nawet ustawione jako "submodule". Zamiast tego opieramy się na plikach ``deps``
oraz ``deps.lock`` oraz skrypcie ``bin/vendors`` do zarządzania nimi.
Pliki te są częścią Twojego repozytorium, więc odpowiednia wersja każdej z bibliotek
zewnętrznych jest trzymana w git, i to Ty możesz użyć skryptu do podniesienia tych
bibliotek do najnowszej wersji.

Gdy deweloper klonuje projekt, on/ona powinna wywołać komendę ``php bin/vendors install``
która pobierze wszystkie biblioteki zewnętrzne.

.. sidebar:: Aktualizacja Symfony

    Od kiedy Symfony jest po prostu grupą bibliotek zewnętrznych a te kontrolowane są
    poprzez pliki ``deps`` oraz ``deps.lock``, aktualizacja Symfony oznacza po prostu
    aktualizację każdego z plików do aktualnej wersji Symfony Standard Edition.

    Oczywiście, jeśli dodałeś nowe wpisy do ``deps`` lub ``deps.lock``, upewnij się
    że zmieniasz tylko oryginalne części (np. upewnij się czy przypadkiem nie usuwasz
    któryś z swoich niestandardowych wpisów).

.. caution::

    Istnieje także polecenie ``php bin/vendors update``, ale polecenie to nie ma nic
    wspólnego z aktualizacją Twojego projektu i normalnie nie będzie potrzeby abyś
    jej używał. Polecenie to jest używane do zamrożenia wersji wszystkich Twoich
    bibliotek zewnętrznych poprzez odczytanie ich aktualnego stanu oraz zapisaniu go do
    pliku ``deps.lock``.

Biblioteki Zewnętrzne (Vendors) oraz Submodules
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Zamiast używać systemów ``deps``, ``bin/vendors`` do zarządzania Twoimi bibliotekami
zewnętrznymi, możesz użyć natywnego `git submodules`_. Nie ma nic złego w tym podejściu,
choć system ``deps`` jest oficjalnym sposobem do zarządzania bibliotekami zewnętrznymi.
A używanie git submodules może być trudne do używania w dłuższym okresie czasu.

Przechowywanie Twojego Projektu na Serwerze Zewnętrznym
-------------------------------------------------------

Posiadasz teraz w pełni funkcjonalny projekt Symfony2 przechowywany w git.
Jednakże, w większości przypadków, będziesz także chciał przechowywać swój projekt na
serwerze zewnętrznym, z względu na kopie bezpieczeństwa, oraz aby inni deweloperzy mogli
współpracować przy projekcie.

Najprostszym sposobem przechowywania projektu na zewnętrznym serwerze udostępnia `GitHub`_.
Publiczne repozytoria są darmowe, ale będziesz musiał wnosić miesięczną opłatę jeśli
będziesz chciał utworzyć prywatne repozytoria.

Alternatywnie, możesz przechowywać swoje repozytorium git na dowolnym serwerze poprzez utworzenie
`repozytorium barebones`_ a następnie wysyłać do niego zmiany. Jedną z bibliotek która ułatwia
zarządzania nim jest `Gitosis`_.

.. _`git`: http://git-scm.com/
.. _`Symfony2 Standard Edition`: http://symfony.com/download
.. _`Standard Edition Readme`: https://github.com/symfony/symfony-standard/blob/master/README.md
.. _`git submodules`: http://book.git-scm.com/5_submodules.html
.. _`GitHub`: https://github.com/
.. _`repozytorium barebones`: http://progit.org/book/ch4-4.html
.. _`Gitosis`: https://github.com/res0nat0r/gitosis
