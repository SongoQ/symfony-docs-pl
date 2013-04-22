.. highlight:: php
   :linenothreshold: 2

.. index::
   pair: Symfony2 Standard Edition; instalacja

Instalacja oraz konfiguracja Symfony
====================================

Jest to dość wierne tłumaczenie anglojęzycznego artykułu `Installing and Configuring
Symfony`_, ale uzupełnione w trakcie tłumaczenia o informacje, które wydają się
niezbędne do przekazania w tym temacie. Poprawiono też kilka nieścisłości
występujących w oryginale.

Co do konwencji. W całym podręczniku stosowane są ścieżki w formacie uniksowym.
Jeżeli chcesz zainstalować Symfony w środowisku Windows, to musisz sobie je
odpowiednio przekształcić, co nie jest filozofią.

Rozdział ten traktuje o sposobach pobrania i uruchomienia aplikacji roboczej
zbudowanej na Symfony2. Na szczęście Symfony oferuje "dystrybucje", będące
funkcjonalnymi projektami "startowymi" Symfony, które można pobrać i rozpocząć
natychmiast na ich bazie tworzenie własnej aplikacji.

.. tip::

    Jeśli szukasz instrukcji jak najlepiej utworzyć nowy projekt
    i przechowywać go poprzez system kontroli wersji, zobacz
    :ref:`using-source-control`.

Pobieranie Dystrybucji Symfony2
-------------------------------

.. tip::

    Po pierwsze, sprawdz czy masz zainstalowany oraz skonfigurowany
    serwer (np. Apache) z najnowszą wersją PHP (zalecane jest PHP 5.3.2 lub wersja
    wyższa). Aby uzyskać więcej informacji o wymaganiach Symfony2, przeczytaj
    :doc:`Wymagania do uruchomienia Symfony2</reference/requirements>`.
        
Symfony2 dystrybuowany jest w pakietach zwanych "dystrybucjami", będącymi w pełni
funkcjonalnymi aplikacjami, zawierającymi biblioteki rdzenia Symfony2, wybór użytecznych
pakietów-wtyczek (ang. bundles), sensowną strukturę katalogów i domyślna konfigurację.
Gdy pobierze się dystrybucję Symfony2, to uzyskuje się funkcjonalny szkielet aplikacyjny,
który może zostać natychmiast wykorzystany do rozpoczęcia tworzenia własnej aplikacji.

Zacznij od odwiedzenia strony Symfony2 z której pobierzesz dystrybucję
http://symfony.com/download/. Na tej stronie zobaczysz odnośnik do *Symfony Standard
Edition* - głównej dystrybucji Sumfon2. Są dwa sposoby pobierania projektu startowego:

.. index::
   pair: Composer; instalacja

Opcja 1 - Composer
~~~~~~~~~~~~~~~~~~

`Composer`_ jest biblioteką zarządzającą zależnościami w PHP, przy użyciu której
można pobrać i zainstalować dystrybucję Symfony2 Standard Edition (jak i inne
dystrybucje).

Zacznij od `pobrania Composera`_ na swój komputer. Istnieją dwa sposoby zainstalowania
tej biblioteki. Pierwszy, to instalacja lokalna, w konkretnym katalogu roboczym.
Drugi, to instalacja globalna, dostępna w całym systemie.

.. _composer-installation:

.. sidebar:: Instalacja biblioteki Composer 

   **Instalacja lokalna**
   
   Jeżeli masz zainstalowany *curl*, to sprawa jest prosta:

   .. code-block:: bash
      
      $ cd /scieżka/do/katalogu/roboczego
      $ curl -s https://getcomposer.org/installer | php

   Polecenie to sprawdzi kilka ustawień PHP i i pobierze plik *composer.phar*
   do katalogu roboczego. Jest to binarny plik programu Composer o formacie PHAR
   (PHP archive), mogący być uruchamianym z linii poleceń.
      
   Jeśli nie ma się zainstalowanej biblioteki *curl*, to można ręcznie pobrać plik
   instalatora ze strony http://getcomposer.org/installer, następnie umieścić go
   w projekcie i uruchomić:
      
   .. code-block:: bash
       
      $ php installer
      $ sudo php composer.phar install
         
   Można zainstalować Composer w określonym katalogu przez użycie opcji ``--install-dir``
   i podanie ścieżki do katalogu docelowego (może być to ścieżka bezwzględna lub względna):
      
   .. code-block:: bash
         
      $ sudo curl -s https://getcomposer.org/installer | php -- --install-dir=bin
         
   gdzie ``bin``, to katalog *bin* znajdujący sie w katalogu roboczym.

   **Instalacja globalna** 

   Plik *composer.phar* można umieścić gdziekolwiek się chce. Jeżeli umieści się
   ścieżkę katalogu docelowego w zmiennej systemowej *PATH*, to można uzyskać dostęp
   globalny. W systemach uniksowych można nawet wywoływać ten plik poza poleceniem php.
      
   Aby w uruchamiać Composer prostym poleceniem ``composer`` a nie ``php composer.phar``
   z dowolnego miejsca systemu (uniksowego) trzeba wykonać dwa polecenia:
      
   .. code-block:: bash
         
      $ sudo curl -s https://getcomposer.org/installer | php
      $ sudo mv composer.phar /usr/local/bin/composer
      
   Konieczne jest jeszcze umieszczenie sieżki */usr/local/bin* w zmiennej *PATH*,
   co można zrobić, w systemie takim jak Ubuntu, przez edycję pliku *~/.profile*:
      
   .. code-block:: bash
         
      $ sudo gedit ~/.profile
         
   i dopisanie ścieżki do zmiennej *PATH*, przykładowo:
      
   .. code-block:: bash
            
      PATH="$HOME/bin:$PATH/usr/local/bin"
      
   Teraz można uruchamiać program prostym poleceniem ``composer``.      

.. note::
        
   Jeżeli komputer nie jest odpowiednio przygotowany na użycie Composera, to po
   wywołaniu tego programu zobaczysz na ekranie pewne zalecenia. Dostosowanie się
   do nich powinno sprawić, ze Composer będzie pracował prawidłowo.

Composer jest wykonywalnym plikiem PHAR, który można użyć do pobrania dystrybucji
standardowej Symfony:

.. code-block:: bash
   
   $ php composer.phar create-project symfony/framework-standard-edition /path/to/webroot/Symfony 2.1.x-dev
   
.. note::
   
   W celu skonkretyzowania wersji trzeba zamienić ``2.1.x-dev`` na łańcuch odpowiadający
   najnowszej wersji Symfony (np. ``2.1.1-dev``). Dla poznania szczegółów przeczytaj
   sytronę `Symfony Installation`_.

.. note::
   
   Aby szybciej załadować pliki dostawców i bez dodatkowych katalogów (np. "Tests"),
   trzeba dodać do polecenia composer opcją ``--prefer-dist``.

Polecenie to może być wykonywane nawet przez kilka minut gdy uruchomia pobieranie
dystrybucji standardowej Symfony przez Composer, wraz ze wszystkimi zalecanymi
bibliotekami dostawców. Po zakończeniu działania programu powinieneś mieć zapisane
wszystkie wymagane pliki wraz ze strukturą katalogów, która wygląda mniej więcej tak:

.. code-block:: text

    path/to/webroot/ <- your web root directory
        Symfony/ <- the new directory
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

Opcja 2 - Pobranie archiwum
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Można również pobrać archiwum dystrybucji Synfony2 Standard Edition. W tym celu
trzeba pobrać archiwum .tgz albo .zip. Oba są równoważne, więc decyzja zależy tylko
od Twoich preferencj

Trzeba zdecydować się na pobranie archiwum z lub bez dostawców (*ang. vendors*).
Jeżeli planujesz używanie bibliotek lub pakietów (*ang. bundles*) niezależnych
dostawców i zarządzać nimi za pośrednictwem Composera, to przypuszczalnie lepszym
wyborem będzie pobranie dystrybucji *without vendors*.

Pobierz jedno z archiwów i rozpakuj go gdzieś w katalogu głównym serwera internetowego.
W systemie uniksowym można użyć w terminalu jedno z poniższych poleceń (zamieniając
``###`` na rzeczywistą nazwę pliku):

.. code-block:: bash

   # dla pliku .tgz
   $ tar -zxvf Symfony_Standard_Vendors_2.2.###.tgz
   
   # dla pliku .zip
   $ unzip Symfony_Standard_Vendors_2.2.###.zip

Jeśli pobrałeś archiwum *without vendors*, to koniecznie przeczytaj następny rozdział.

.. note::
   
   Można łatwo zastąpić domyślną strukturę katalogów. Przeczytaj artykuł
   :doc:`/cookbook/configuration/override_dir_structure` w celu uzyskania więcej
   informacji.

Aktualizacja bibliotek dostawców
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

W tym momencie powinieneś mieć pobrany i zainstalowany w pełni funkcjonalny projekt
Symfony, z którym możesz rozpocząć tworzenie własnej aplikacji. Projekt Symfony
zależy od wielu zewnętrznych bibliotek. Są one pobierane do katalogu *vendor/*
projektu. poprzez bibliotekę `Composer`_, o której była mowa w poprzednim rozdziale.

W zależności o sposobu pobrania Symfony, może być konieczne pobranie bibliotek
dostawców lub nie (bo znajdowały się w pliku archiwum instalacyjnego). Aktualizacja
bibliotek dostawców jest zawsze bezpieczna i gwarantuje, że ma się wszystkie potrzebne
biblioteki.

Instalacja Composer została dokładnie omówiona w rodziale :ref:`poprzednim<composer-installation>`.

Zainstalowanie lub zaktualizowanie bibliotek dostawców można osiągnąć poleceniem (pełna składnia):

.. code-block:: bash
   
   $cd /ścieżka/do/katalogu/symfony
   $ [sudo -u www-data] php composer.phar install

Powyższe polecenie instalujące (lub polecenie skrócone ``$ composer install``)
musi być uruchomione w katalogu, w którym znajduje się plik *composer.json* - domyślnie
jest to katalog główny projektu Symfony. Spowoduje ono pobranie lub zaktualizowanie
wszystkich bibliotek dostawców w katalogu *vendor/*. Instalacja lub aktualizacja
może się nie powieść, ze względu na brak uprawnień użytkownika dokonujacego instalacji
(aktualizacji) do zapisu katalogów *app/cache* i *app/logs*. Dlatego wcześniej należy
odpowiednio skonfigurować aplikację. Jest to omówione nieco dalej, w przypisie
"Konfiguracja uprawnień". Gdy użytkownkiem serwera jest ``www-data``  a użytkownik
linii poleceń należy do grupy mającej uprawnienia zapisu do w/w katalogów, to w podanym
poleceniu trzeba użyć opcji ``sudo -u www-data`` (w Ubuntu i podobnych systemach),
lub analogicznego.

Jeśli ma się zainstalowane biblioteki dostawców, to można wykonać tylko polecenie
aktualizujące:

.. code-block:: bash
   
   $ [sudo -u www-data] php composer.phar update

.. tip::
   
   Po zrealizowaniu polecenia ``php composer.phar install`` lub ``php composer.phar update``,
   Composer automatycznie wykonuje czyszczenie pamięci podręcznej i instalację zasobów.
   Zasoby są domyślnie kopiowane do katalogu „web”. Zamiast później przekopiowywać
   te zasoby, lepiej jest spowodować automatyczne utworzenie dowiązania symbolicznego
   poprzez wykonanie odpowiedniego wpisu w pliku composer.json z kluczem ``symfony-assets-install``
   a wartością ``symlink``:
   
   .. code-block:: json
      :linenos:
      
      "extra": {
         "symfony-app-dir": "app",
         "symfony-web-dir": "web",
         "symfony-assets-install": "symlink"
      }
   
   Jeżeli zamiast wpisu symlink zastosuje się wpis ``relative`` w wartości klucza
   ``symfony-assets-install``, to polecenie będzie generowało względne dowiązanie
   symboliczne.


Konfiguracja i ustawienie
~~~~~~~~~~~~~~~~~~~~~~~~~

W tym momęcie wszystkie zewnętrzne biblioteki umiejscowione są w katalogu ``vendor/``.
Masz także wstępnie skonfigurowany projekt w katalogu ``app/`` wg ustawień domyślnych
oraz przykładowy kod w katalogu ``src/``.

Symfony2 dostarczane jest z wizualnym testerem konfiguracji serwera, aby pomóc w
sprawdzeniu prawidłowości konfiguracji serwera internetowego i PHP pod kątem działania
Symfony. Zakładając, że Symfony zostało zainstalowane w katalogu
/ścieżka/do/katalogu/wwwroot/symfony, użyj w przeglądarce następującego adresu URL,
aby sprawdzić swoją konfigurację:

.. code-block:: text

    http://localhost/Symfony/web/config.php

Jeśli są jakieś problemy, rozwiąż je teraz, zanim przejdziesz dalej.

.. sidebar:: Ustawienie Uprawnień
   
   Jednym z powszechnych problemów jest to, że katalogi *app/cache* i *app/logs*
   muszą być zapisywalne zarówno dla serwera internetowego, jak i dla użytkownika
   linii poleceń. Na systemie uniksowym, jeżeli użytkownik serwera internetowego
   jest inny niż użytkownik linii poleceń, to można uruchomić tylko raz następujące
   polecenia w swoim projekcie, aby spowodować prawidłowość ustawień uprawnień.
   
   **Należy mieć na uwadze, że nie wszystkie serwery internetowe uruchamiane są
   w procesie należącym do użytkownika** ``www-data``, tak jak to przyjęto w poniższych
   przykładach. Zamiast tego, sprawdź jaki użytkownik jest właścicielem procesów
   stosowanego serwera internetowego i użyj go w miejsce ``www-data``.
   
   W systemie uniksowym można to zrobić przy pomocy następujących poleceń:
   
   .. code-block:: bash
   
      $ ps aux | grep httpd
   
   lub
      
   .. code-block:: bash

      $ ps aux | grep apache
    
   **1. Użycie ACL na systemach obsługujących ``chmod +a``**

   Wiele systemów umożliwia użycie polecenia ``chmod +a``. Spróbuj najpierw tego
   i jeżeli wystąpi błąd, spróbuj następnego sposobu:
   
   .. code-block:: bash
      
      $ rm -rf app/cache/*
      $ rm -rf app/logs/*
      
      $ sudo chmod +a "www-data allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs
      $ sudo chmod +a "`whoami` allow delete,write,append,file_inherit,directory_inherit" app/cache app/logs
   
   **2. Użycie ACL w systemach nie obsługujących ``chmod +a``**
      
   Niektóre systemy nie obsługują polecenia ``chmod +a``, ale obsługują inne narzędzie
   o nazwie ``setfacl``. Możesz spróbować `włączyć obsługę ACL`_ na partycji i
   zainstalować ``setfacl`` (w Ubuntu jest on zainstalowany domyślnie), a następnie
   uruchomić polecenia podobne do tych:
   
   .. code-block:: bash
      
      $ sudo setfacl -R -m u:www-data:rwx -m u:`whoami`:rwx app/cache app/logs
      $ sudo setfacl -dR -m u:www-data:rwx -m u:`whoami`:rwx app/cache app/logs
      
   W systemie takim jak Ubuntu, można to zrobić też inaczej:
    
   .. code-block:: bash
          
      # zmiana właściciela i grupy dla całego projektu
      $ sudo chown -R www-data:www-data /var/www/symfony
      # dopisanie siebie do grupy www-data (jeżeli się tego wcześniej nie uczyniło)
      $ sudo usermod -aG www-data `whoami`  
      # nadanie uprawnień zapisu do app/cache i app/logs
      $ sudo chmod -R 775 app/cache app/logs
     
   **3. Bez użycia ACL**
   
   Jeśli nie ma się dostępu do zmian ACL katalogów, to pozostaje zmiana ``umask``,
   tak aby katalogi *cache* i *log* były zapisywalne dla grupy lub każdego
   (w zależności od tego czy użytkownik serwera internetowego i użytkownik linii
   poleceń należą do tej samej grupy). Aby to osiągnąć należy wstawić następującą
   linię na samym początku plików *app/console*, *web/app.php* i *web/app_dev.php*:

   .. code-block:: php

      umask(0002); // To nadaje uprawnienia 0775
      
      // lub
      
      umask(0000); // To nadaje uprawnienia 0777

   Proszę zauważyć, że zalecaną metodą jest zastosowanie ACL, gdy ma się do niego
   dostęp na serwerze, ponieważ zmiana ``umask`` nie jest całkiem bezpieczna.

Gdy wszystko jest w porządku, kliknij na "Go to the Welcome page" aby zażądać
pierwszą "prawdziwą" strony Symfony2:

.. code-block:: text

   http://localhost/Symfony/web/app_dev.php/

Symfony2 przywita nas ekranem, takim jak ten:

.. image:: /images/quick_tour/welcome.jpg

.. tip::

   Aby uzyskać ładne i krótkie adresy URL należy wskazać katalog ``Symfony/web/``
   jako katalog główny dokumentów (*document root*) swojego serwera internetowego
   lub wirtualnego hosta. Choć nie jest to konieczne dla prac programistycznych,
   jest to zalecane już na tym etapie, nim aplikacja trafi do produkcji, gdyż później
   trzeba będzie dokonać zmian we wszystkich plikach konfiguracyjnych systemu aby
   zasoby były dostępne dla klientów. W celu uzyskania informacji o konfiguracji
   katalogu głównego dokumentów w określonym serwerze internetowym, proszę zapoznać
   się z dokumentacją: `Apache`_ lub `Nginx`_ .


Rozpoczęcie programowania
-------------------------

Teraz, gdy już masz w pełni funkcjonalną aplikację Symfony2, możesz rozpocząć jej
dalsze tworzenie. Twoja dystrybucja może zwierać trochę przykładowego kodu – sprawdź
plik README.md zawarty w katalogu głównym aplikacji (otwórz go jak zwykły plik tekstowy)
aby poznać informacje o zawartym w dystrybucji przykładowym kodzie i jak można go usunąć.

Jeśli jesteś nowicjuszem w Symfony, to zapoznaj się z rozdziałem ":doc:`page_creation`"
dokumentacji, gdzie poznasz sposoby tworzenia stron, zmieniania konfiguracji i wszystko,
co jest potrzebne do zbudowania nowej aplikacji.

Należy się również zapoznać z :doc:`Receptariuszem</cookbook/index>`, która to część
dokumentacji zawiera szeroki wybór artykułów o rozwiązywaniu konkretnych problemów
w Symfony.

.. _using-source-control:

Używanie systemu kontroli wersji
--------------------------------

Jeśli używasz systemu kontroli wersji, takiego jak *Git* lub *Subversion*,
możesz skonfigurować swój system kontroli wersji oraz rozpocząć wysyłanie
tam swojego projektu. Symfony Standard edition jest startowym punktem
dla nowego projektu.

Aby dowiedzieć się jak najlepiej ustawić swój projekt do przechowywania go
w git, przeczytaj :doc:`/cookbook/workflow/new_project_git`.

Ignorowanie katalogu ``vendor/``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jeśli pobrałeś archiwum *without vendors*, możesz zignorować całą
zawartość katalogu ``vendor/`` i nie zgłaszać go do systemu kontroli wersji.
W *Git* robi się to przez utworzenie pliku *.gitignore* i dodanie do niego
nastęþującej linii:

.. code-block:: text

    vendor/

Teraz, katalog *vendor* nie będzie zgłaszany do systemu kontroli wersji.
Tak jest dobrze (nawet bardzo dobrze) ponieważ gdy ktoś klonuje lub sprawdza
projekt z repozytorium, może po prostu uruchomić skrypt
``php composer.phar install``, który zainstaluje wszystkie wszystkie niezbędne
zależności projektu.

.. _`włączyć obsługę ACL`: https://help.ubuntu.com/community/FilePermissions#ACLs
.. _`Gita`: http://git-scm.com/
.. _`GitHub Bootcamp`: http://help.github.com/set-up-git-redirect
.. _`pobrania Composera`: http://getcomposer.org/download/
.. _`Composer`: http://getcomposer.org/download/
.. _`Installing and Configuring Symfony`: http://symfony.com/doc/current/book/installation.html
.. _`Symfony Installation`: http://symfony.com/download
.. _`Apache`: http://httpd.apache.org/docs/current/mod/core.html#documentroot
.. _`Nginx`: http://wiki.nginx.org/Symfony