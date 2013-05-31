.. highlight:: php
   :linenothreshold: 2

Architektura
============

Jestem pełen uznania dla Ciebie! Miałem obawy, że nie znajdziesz sie tutaj po
lekturze trzech pierwszych części przewodnika? Twoje starania zostaną niedługo
nagrodzone. Pierwsze trzy części nie zagłębiały się za głęboko w architekturę frameworka.
Bo to ona sprawia, że ​​Symfony2 wyróżnia się z tłumu innych frameworków.
Zanurzmy się więc teraz w architekturę Symfony2.

Zrozumienie Struktury Katalogów
-------------------------------

Struktura katalogów Symfony2 jest dość elastyczna, ale struktura katalogów
dystrybucji *Standard Edition* odzwierciedla typową oraz rekomendowaną strukturę
aplikacji Symfony2:

* ``app/``:    Konfiguracja aplikacji;
* ``src/``:    Kod PHP projektu;
* ``vendor/``: Biblioteki zewnętrzne;
* ``web/``:    Katalog główny serwera.

Katalog ``web/``
~~~~~~~~~~~~~~~~

Katalog ``web`` jest miejscem wszystkich publicznych oraz statycznych
plików takich jak obrazy, arkusze stylów oraz pliki JavaScript. Tam także
umiejscowiony jest :term:`kontroler wejścia <kontroler wejścia>`::

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();

P pierwszej kolejności jądro oczekuje istnienie pliku ``bootstrap.php.cache``,
który inicjuje framework oraz rejestruje ``autoloader`` (zobacz niżej).

Jak każdy kontroler wejścia, ``app.php`` używa klasy jądra ``AppKernel`` do
rozruchu aplikacji.

.. _the-app-dir:

Katalog ``app/``
~~~~~~~~~~~~~~~~

Klasa ``AppKernel`` jest głównym punktem wejścia do konfiguracji aplikacji
i jako takie, jest przechowywany w folderze ``app/``.

Ta klasa musi implementować dwie metody:

* ``registerBundles()`` zwraca tablicę wszystkich pakietów potrzebnych do uruchomienia
  aplikacji;

* ``registerContainerConfiguration()`` wczytuje konfigurację aplikacji
  (więcej na ten temat później).

Autoładowanie jest obsługiwane automatycznie poprzez `Composer`_, co oznacza, że
można użyć dowolnych  klas PHP nie robiąc nic. Jeśli potrzeba więcej elastyczności,
to można rozszerzyć autoloader w pliku ``app/autoload.php``. Wszystkie zależności
są przechowywane w katalogu ``vendor/``, ale jest to tylko konwencja. Można je
przechowywać wszędzie tam, gdzie się chce -  globalnie na serwerze lub lokalnie
w swoich projektach.

.. note::

    Jeżeli chcesz nauczyć się więcej o programie Composer, przeczytaj `Composer-Autoloader`_.
    Symfony ma również komponent autoładowania - czytaj ":doc:`/components/class_loader`".


Zrozumienie systemu pakietów
----------------------------

Rozdział ten wprowadza do jedenej z największych i najpotężniejszych cech Symfony2 -
systemu :term:`pakietów <pakiet>`.

Pakiet jest czymś w rodzaju wtyczki w innych programach. Więc dlaczego został nazwany
pakietem (*ang. bundle*) a nie wtyczką (*ang. plugin*)? To dlatego, że wszystko w Symfony2
należy do jakiegoś pakietu, od funkcji rdzenia frameworka po kod napisany dla Twojej aplikacji.
Pakiety są obywatelem numer jeden w Symfony2. Zapewnia to elastyczność w używaniu
wbudowanych pakietów funkcyjnych rozpowszechnianych przez osoby trzecie lub w dystrybucji
Twoich własnych pakietów. Stwarza to możliwość łatwego doboru i wyboru odpowiednich
dla swojej aplikacji funkcjonalności i umożliwia łatwą optymalizację całości.
Tak na koniec - w Symfony2 Twój kod jest tak samo ważny jak kod frameworka.

.. note::

   W rzeczywistości pojęcie pakietu (*ang. bundle*), nie jest pojęciem wyłącznym
   dla Symfony2. Pakiety są ściśle związane z przestrzenią nazw i w niektórych
   językach (jak np. w Java) są sformalizowane od początku. W PHP pojęcia te nie
   istniały, co było powodem wielu problemów. Zmuszało to programistów do stosowania
   własnych konwencji nazewniczych. Pakiety i przestrzenie nazw zostały formalnie
   wprowadzone w PHP 5.3 i tym samym pojawiły się w Symfony2.

Rejestrowanie pakietów
~~~~~~~~~~~~~~~~~~~~~~

Aplikacja składa się z pakietów określonych przez metodę ``registerBundles()``
klasy ``AppKernel``. Każdy pakiet jest katalogiem zawierającym pojedyńczą klasę
``Bundle`` opisującą ten pakiet::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            new Symfony\Bundle\FrameworkBundle\FrameworkBundle(),
            new Symfony\Bundle\SecurityBundle\SecurityBundle(),
            new Symfony\Bundle\TwigBundle\TwigBundle(),
            new Symfony\Bundle\MonologBundle\MonologBundle(),
            new Symfony\Bundle\SwiftmailerBundle\SwiftmailerBundle(),
            new Symfony\Bundle\DoctrineBundle\DoctrineBundle(),
            new Symfony\Bundle\AsseticBundle\AsseticBundle(),
            new Sensio\Bundle\FrameworkExtraBundle\SensioFrameworkExtraBundle(),
        );

        if (in_array($this->getEnvironment(), array('dev', 'test'))) {
            $bundles[] = new Acme\DemoBundle\AcmeDemoBundle();
            $bundles[] = new Symfony\Bundle\WebProfilerBundle\WebProfilerBundle();
            $bundles[] = new Sensio\Bundle\DistributionBundle\SensioDistributionBundle();
            $bundles[] = new Sensio\Bundle\GeneratorBundle\SensioGeneratorBundle();
        }

        return $bundles;
    }

Proszę zauważyć, że oprócz pakietu ``AcmeDemoBundle``, który już był omawiany, jądro
udostępnia również inne pakiety, takie jak ``FrameworkBundle``, ``DoctrineBundle``,
``SwiftmailerBundle`` czy ``AsseticBundle``. Są one częścią rdzenia szkieletu.

Konfiguracja pakietu
~~~~~~~~~~~~~~~~~~~~

Każdy pakiet może być dostosowywany poprzez pliki konfiguracyjne w języku YAML,
XML, czy też PHP. Wystarczy popatrzeć na domyślną konfigurację:

.. code-block:: yaml
   :linenos:

    # app/config/config.yml
    imports:
        - { resource: parameters.yml }
        - { resource: security.yml }

    framework:
        #esi:             ~
        #translator:      { fallback: "%locale%" }
        secret:          "%secret%"
        router:
            resource: "%kernel.root_dir%/config/routing.yml"
            strict_requirements: "%kernel.debug%"
        form:            true
        csrf_protection: true
        validation:      { enable_annotations: true }
        templating:      { engines: ['twig'] } #assets_version: SomeVersionScheme
        default_locale:  "%locale%"
        trusted_proxies: ~
        session:         ~

    # Twig Configuration
    twig:
        debug:            "%kernel.debug%"
        strict_variables: "%kernel.debug%"

    # Assetic Configuration
    assetic:
        debug:          "%kernel.debug%"
        use_controller: false
        bundles:        [ ]
        #java: /usr/bin/java
        filters:
            cssrewrite: ~
            #closure:
            #    jar: "%kernel.root_dir%/Resources/java/compiler.jar"
            #yui_css:
            #    jar: "%kernel.root_dir%/Resources/java/yuicompressor-2.4.7.jar"

    # Doctrine Configuration
    doctrine:
        dbal:
            driver:   "%database_driver%"
            host:     "%database_host%"
            port:     "%database_port%"
            dbname:   "%database_name%"
            user:     "%database_user%"
            password: "%database_password%"
            charset:  UTF8

        orm:
            auto_generate_proxy_classes: "%kernel.debug%"
            auto_mapping: true

    # Swiftmailer Configuration
    swiftmailer:
        transport: "%mailer_transport%"
        host:      "%mailer_host%"
        username:  "%mailer_user%"
        password:  "%mailer_password%"
        spool:     { type: memory }

Każdy wpisów jak np. ``framework`` definiuje konfigurację dla określonego pakietu.
Dla przykładu, ``framework`` konfiguruje pakiet ``FrameworkBundle`` a ``swiftmailer``
konfiguruje ``SwiftmailerBundle``.

Każde :term:`środowisko` może nadpisać domyślną konfigurację poprzez dostarczenie
odpowiedniego pliku konfiguracyjnego. Dla przykładu, środowisko ``dev`` wczytuje plik
``config_dev.yml``, który to wczytuje główną konfigurację (np. ``config.yml``) oraz
modyfikuje go w celu dodania narzędzi do debugowania:

.. code-block:: yaml
   :linenos:

    # app/config/config_dev.yml
    imports:
        - { resource: config.yml }

    framework:
        router:   { resource: "%kernel.root_dir%/config/routing_dev.yml" }
        profiler: { only_exceptions: false }

    web_profiler:
        toolbar: true
        intercept_redirects: false

    monolog:
        handlers:
            main:
                type:  stream
                path:  %kernel.logs_dir%/%kernel.environment%.log
                level: debug
            firephp:
                type:  firephp
                level: info

    assetic:
        use_controller: true


Rozszerzanie pakietu
~~~~~~~~~~~~~~~~~~~~

Oprócz tego że pakiety są dobrym sposobem na zorganizowanie i skonfigurowanie kodu,
pakiet może rozszerzać inny pakiet. Dziedziczenie pakietu umożliwia zamienienie
istniejącego pakietu w celu dostosowania jego kontrolerów, szablonów lub każdego
z jego plików. Tu właśnie przydadzą się logiczne nazwy
(np. ``@AcmeDemoBundle/Controller/SecuredController.php``) - są abstraktem,
niezależnym od rzeczywistego miejsca przechowywania zasobu.

Logiczne nazwy plików
.....................

Kiedy chce się odwołać do pliku pakietu, trzeba użyj notacji:
``@BUNDLE_NAME/path/to/file``. Symfony2 zamieni ``@BUNDLE_NAME`` na
realną ścieżkę do pakietu. Na przykład, logiczna ścieżka
``@AcmeDemoBundle/Controller/DemoController.php`` zostanie przekształcona
do ``src/Acme/DemoBundle/Controller/DemoController.php`` ponieważ Symfony
zna lokalizację ``AcmeDemoBundle``.

Logiczne nazwy kontrolerów
..........................

W przypadku kontrolerów trzeba odwołać się do metod stosując notację
``BUNDLE_NAME:CONTROLLER_NAME:ACTION_NAME``. Dla przykładu,
``AcmeDemoBundle:Welcome:index`` wskazuje na metodę ``indexAction``
z klasy ``Acme\DemoBundle\Controller\WelcomeController``.

Logiczne nazwy szablonów
........................

Dla szablonów, logiczna nazwa ``AcmeDemoBundle:Welcome:index.html.twig`` zostanie
przekształcona na ścieżkę do pliku ``src/Acme/DemoBundle/Resources/views/Welcome/index.html.twig``.
Szablony staną się jeszcze bardziej interesujące kiedy zdasz sobie sprawę że nie
musisz je przechowywać w systemie plików. Dla przykładu, możesz w prosty sposób
przechowywać je w bazie danych.

Rozszerzenie pakietów
.....................

Stosując tą konwencję, można następnie wykorzystać
:doc:`dziedziczenia pakietów </cookbook/bundles/inheritance>` do "napisania" plików,
kontrolerów lub szablonów. Na przykład, można utworzyć pakiet ``AcmeNewBundle``
i  określić, że zastępuje on pakiet ``AcmeDemoBundle``. Gdy Symfony ładuje kontroler
``AcmeDemoBundle:Welcome:index``, to najpierw będzie wyszukiwał klasy ``WelcomeController``
w pakiecie ``AcmeNewBundle`` i jeśli jej nie znajdzie, to rozpocznie przeszukiwanie
pakietu ``AcmeDemoBundle``. Oznacza to, że pakiet może zastąpić prawie każdą część
innego pakietu.

Rozumiesz teraz dlaczego Symfony2 jest tak elastyczny? Współdziel swoje pakiety
pomiędzy aplikacjami, przechowuj je lokalnie lub globalnie, to zależy od tylko
Ciebie.

.. _using-vendors:

Korzystanie ze żródeł dostawców
-------------------------------

Jest bardzo prawdopodobne, że Twoja aplikacja będzie zależeć od bibliotek i pakietów
osób trzecich. Powinny być one przechowywane w katalogu ``vendor/``. Katalog ten już
zawiera biblioteki Symfony2, biblioteki ``SwiftMailer``, ``Doctrine ORM``,
system szablonów Twig i trochę innych bibliotek i pakietów osób trzecich
(zwanych też dostawcami).

Zrozumienie buforowania i dzienników zdarzeń
--------------------------------------------

Symfony2 jest prawdopodobnie jednym z najszybszych pełnych frameworków PHP.
Ale jak może tak szybko działać, skoro parsuje oraz interpretuje kilkadziesiąt
plików YAML oraz XML dla każdego zapytania. Prędkość jest po części związana
z systemem buforowania. Konfiguracja aplikacji jest parsowana tylko dla pierwszego
żądania i przetwarzana do kodu PHP przechowywanego w katalogu ``app/cache/``.
W środowisku programistycznym, Symfony2 jest wystarczająco inteligentny aby czyścić
pamięć podręczną po zmianie pliku. Ale w środowisku produkcyjnym, to do 
do zadań programisty należy czyszczenie pamięci podręcznej zmianie kodu lub
konfiguracji.

Podczas tworzenia aplikacji, dużo rzeczy może pójść źle. Pliki dzienników zdarzeń,
znajdujące się w katalogu ``app/logs/``, informują o wszystkich żądaniach
i mogą pomóc w naprawieniu napotkanych problemów.

Sosowanie interfejsu linii poleceń
----------------------------------

Każda aplikacja posiada narzędzie interfejsu linii poleceń (``app/console``)
który pomaga w utrzymywaniu aplikacji. Udostęþnia on polecenia które zwiększają
wydajność prac programistycznych i administracyjnych poprzez automatyzację żmudnych
i powtarzających się zadań.

Uruchom go bez żadnych argumentów aby dowiedzieć się więcej o jego możliwościach:

.. code-block:: bash

    php app/console

Opcja ``--help`` pomaga w poznaniu dostęþnych poleceń:

.. code-block:: bash

    php app/console router:debug --help

Podsumowanie
------------

Nazwij mnie szaleńcem, bo twierdzę, że po przeczytaniu tego przewodnika powinieneś czuć
się komfortowo w pracy z Symfony2. Wszystko w Symfony2 jest zaprojektowane tak by sprostać Twoim
oczekiwaniom. Zatem, zmieniaj nazwy, przenoś katalogi zgodnie z swoimi upodobaniami.

I to wszystko jeśli chodzi o szybkie wprowadzenie. Musisz się jeszcze dużo nauczyć
o Symfony2 by stać się mistrzem, od testowania do wysyłania poczty e-mail. Gotowy
jesteś do dokopania się do tych tematów? Nie musisz specjalnie szukać - przejdź
do oficjalnej książki i wybierz dowolny temat.


.. _`standardy`:               http://groups.google.com/group/php-standards/web/psr-0-final-proposal
.. _`konwencji`:               http://pear.php.net/
.. _`Composer`:                http://getcomposer.org
.. _`Composer-Autoloader`:     http://getcomposer.org/doc/01-basic-usage.md#autoloading