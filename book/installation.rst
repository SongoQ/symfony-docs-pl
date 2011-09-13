.. index::
   single: Installation

Instalacja oraz Konfiguracja Symfony
====================================

Celem tego rozdziału jest uruchomienie aplikacji opartej na Symfony.
Na szczęście, Symfony oferuje "dystrybucje", które są funkcjonalnymi
"startowymi" projektami które możesz ściągnąć i rozpocząć pracę z nimi
natychmiast.

.. tip::

    Jeśli szukasz instrukcji jak najlepiej utworzyć nowy projekt
    i przechowywać go poprzez system kontrolii wersji, zobacz
    `Używanie Systemu Kontrolii Wersji`_.

Pobieranie Dystrybucji Symfony2
-------------------------------

.. tip::

    Po pierwsze, sprawdz czy masz zainstalowany oraz skonfigurowany
    serwer (np. Apache) z PHP 5.3.2 lub wyższym. Aby uzyskać więcej
    informacji o wymaganiach Symfony2, przeczytaj
    :doc:`Wymagania do uruchomienia Symfony2</reference/requirements>`.

Symfony2 popakowany jest w "dystrybucje" które to, są w pełni funkcjonalnymi
aplikacjami zawierającymi rdzenne biblioteki Symfony2, wyselekcjonowane
użyteczne bundle, sensowną strukturę katalogów oraz trochę domyślnej konfiguracji.
Gdy pobierasz dystrybucję Symfony, pobierasz funkcjonalny szkielet aplikacji
który może być użyty natychmiast do budowania Twojej aplikacji.

Zacznij od odwiedzenia strony Symfony2 z której pobierzesz dystrybucję
`http://symfony.com/download`_. Na tej stronie zobaczysz *Symfony Standard Edition*,
która to jest główną dystrybucją Symfony2. Tutaj musisz dokonać dwóch wyborów:

* Pobierz archiwum ``.tgz`` lub ``.zip`` - obydwa są identyczne, pobierz
  taką wersję archiwum która jest bardziej wygodna w użyciu dla Ciebie;

* Pobierz dystrybucję zawierającą lub też nie biblioteki zewnętrzne. Jeśli masz 
  zainstalowanego `Gita`_ na swoim komputerze, powinieneś pobrać Symfony2 
  "without vendors", jako że dodaje to większej elastyczności przy wyborze
  bibliotek zewnętrznych.

Pobierz archiwum gdzieś do głównego katalogu swojego serwera i rozpakuj je.
W UNIXowej linii poleceń, przy użyciu jednej z poniższych komend
(zamień ``###`` na swoją aktualną nazwę pliku):

.. code-block:: bash

    # dla pliku .tgz
    tar zxvf Symfony_Standard_Vendors_2.0.###.tgz

    # dla pliku .zip
    unzip Symfony_Standard_Vendors_2.0.###.zip

Gdy skończysz, powinieneś mieć katalog ``Symfony/`` który
wygląda następująco:

.. code-block:: text

    www/ <- twój główny katalog serwera
        Symfony/ <- rozpakowane archiwum
            app/
                cache/
                config/
                logs/
            src/
                ...
            vendor/
                ...
            web/
                app.php
                ...

Aktualizacja Bibliotek Zewnętrznych (Vendors)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ostatecznie, jeśli pobrałeś archiwum "without vendors", zainstaluj
biblioteki zewnętrzne poprzez uruchomienie komendy z linii poleceń:

.. code-block:: bash

    php bin/vendors install

Komenda ta pobiera wszystkie potrzebne biblioteki zewnętrzne - włączając w to
Symfony - do katalogu ``vendor/``. Aby uzyskać więcej informacji jak biblioteki
zewnętrzne są zarządzane przez Symfony2, zobacz ":ref:`cookbook-managing-vendor-libraries`".

Konfiguracja oraz Instalacja
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

W tym momęcie wszystkie Twoje zewnętrzne biblioteki umiejscowione są w katalogu ``vendor/``.
Masz także domyślną instalację aplikacji w ``app/`` oraz przykładowy kod w katalogu ``src/``.

Symfony2 posiada wirtualny tester konfiguracji serwera, aby ułatwić Ci sprawdzenie czy Twój serwer
oraz konfiguracja PHP spełnia wymagania Symfony. Użyj poniższego URLa aby sprawdzić swoją
konfigurację:

.. code-block:: text

    http://localhost/Symfony/web/config.php

Jeśli są tam jakieś problemy, rozwiąż je zanim przejdziesz dalej.

.. sidebar:: Ustawienie Uprawnień

    Znanym problemem jest to że foldery ``app/cache`` oraz ``app/logs``
    muszą mieć uprawnienia do zapisu przez użytkownika web serwer oraz
    użytkowika linii komend. Na systemie UNIXowym jeśli Twój użytkownik
    web serwera jest inny aniżeli użytkownik linii komend, możesz odpalić
    poniższe komendy - tylko raz - w swoim projekcie, aby upewnić się że
    uprawnienia zostały nadane poprawnie. Zamień ``www-data`` na swojego
    użytkownika web serwera oraz ``yourname`` na swojego użytkownika linii
    poleceń:

    **1. Korzystanie z ACL na systemie który wspiera chmod +a**

    Wiele systemów umożliwia korzystanie z komendy ``chmod +a``.
    Spróbuj najpierw tego, jeśli otrzymasz błąd - spróbuj innej metody:

    .. code-block:: bash

        rm -rf app/cache/*
        rm -rf app/logs/*

        sudo chmod +a "www-data allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs
        sudo chmod +a "yourname allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs

    **2. Korzystanie z ACL na systemach nie wspierających chmod +a**

    Część systemów nie wspiera ``chmod +a``, ale wspierają inne narzędzie o
    nazwie ``setfacl``. Będziesz musiał `włączyć obsługę ACL`_ na swojej partycji
    oraz zainstalować setfacl zanim będziesz mógł go użyć (jak w tym przypadku na
    Ubuntu), tak jak tutaj:

    .. code-block:: bash

        sudo setfacl -R -m u:www-data:rwx -m u:yourname:rwx app/cache app/logs
        sudo setfacl -dR -m u:www-data:rwx -m u:yourname:rwx app/cache app/logs

    **3. Bez użycia ACL**

    Jeśli nie masz uprawnień do zmieniania ACL na katalogach, będziesz
    musiał zmienić umask aby katalogi cache oraz log były "group-writable"
    lub też "world-writable" (w zależności czy użytkownik web serwer oraz
    linii komend są w tej samej grupie lub też nie). Aby to osiągnąć,
    dodaj poniższą linię na początku plików ``app/console``,
    ``web/app.php`` oraz ``web/app_dev.php``:

    .. code-block:: php

        umask(0002); // This will let the permissions be 0775

        // or

        umask(0000); // This will let the permissions be 0777

    Zauważ że używanie ACL jest rekomendowane jeśli masz do niego dostęp
    na swoim serwerze, ponieważ zmiana umask nie jest bezpiecznym
    rozwiązaniem dla pracy na wątkach (thread-safe).

Gdy wszystko jest dobrze, kliknij na "Go to the Welcome page" aby zobaczyć
swoją pierwszą "prawdziwą" stronę Symfony2:

.. code-block:: text

    http://localhost/Symfony/web/app_dev.php/

Symfony2 powinno Cię powitać oraz pogratulować Ci za Twoją dotychczasową ciężką pracę!

.. image:: /images/quick_tour/welcome.jpg

Rozpoczęcie Prac Deweloperskich
-------------------------------

Teraz gdy posiadasz w pełni funkcjonalną aplikację Symfony2, możesz rozpocząć
prace deweloperskie! Twoja dystrybucja może posiadać przykładowy kod - przeczytaj
plik ``README.rst`` dostarczony z dystrybucją (otwórz go jako plik tekstowy)
aby dowiedzieć się jaki przykładowy kod został dostarczony do Twojej dystrybucji
oraz jak możesz go później usunąć.

Jeśli dopiero rozpoczynasz pracę z Symfony, dołącz do nas w ":doc:`page_creation`",
gdzie dowiesz się jak tworzyć strony, zmieniać konfigurację, i robić wszystkie inne
rzeczy potrzebne w Twojej aplikacji.

Używanie Systemu Kontrolii Wersji
---------------------------------

Jeśli używasz systemu kontrolii wersji jak ``Git`` lub ``Subversion``,
możesz skonfigurować swój system kontrolii wersji oraz rozpocząć wysyłanie
tam swojego projektu. Symfony Standard edition *jest* startowym punktem
dla Twojego projektu.

Aby dowiedzieć się jak najlepiej ustawić swój projekt do przechowywania go
w git, zobacz :doc:`/cookbook/workflow/new_project_git`.

Ignorowanie Katalogu ``vendor/``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jeśli pobrałeś archiwum *without vendors*, możesz na spokojnie zignorować
zawartość katalogu ``vendor/`` i nie wysyłać go do systemu kontrolii wersji.
Z ``Git`` możesz to zrobić poprzez stworzenie oraz dodanie do pliku ``.gitignore``
następującej linii:

.. code-block:: text

    vendor/

Teraz, katalog vendor nie będzie wysyłany do systemu kontrolii wersji. Tak jest dobrze
(nawet bardzo dobrze!) ponieważ gdy ktoś sklonuje lub też pobierze projekt,
on/ona może w prosty sposób odpalić skrypt ``php bin/vendors install`` który
zainstaluje wszystkie potrzebne biblioteki zewnętrzne.

.. _`włączyć obsługę ACL`: https://help.ubuntu.com/community/FilePermissions#ACLs
.. _`http://symfony.com/download`: http://symfony.com/download
.. _`Gita`: http://git-scm.com/
.. _`GitHub Bootcamp`: http://help.github.com/set-up-git-redirect
