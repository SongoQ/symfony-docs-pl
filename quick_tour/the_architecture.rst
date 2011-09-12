Architektura
============

Jesteś moim bohaterem! Kto by pomyślał że będziesz tutaj po trzech pierwszych częściach?
Twoje starania zostaną niedługo nagrodzone. Pierwsze trzy części nie zagłębiały się
za głęboko w architekturę frameworka. A ponieważ ona wyróżna Symfony2 z tłumu innych
frameworków, zanurkujmy w architekturę teraz.

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

Katalog główny serwera jest miejscem wszystkich publicznych oraz statycznych
plików takich jak obrazki, arkusze stylów, oraz pliki JavaScript. Tam także
umiejscowione są front kontrolery::

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();

Jądro wymaga przede wszystkim pliku ``bootstrap.php.cache``, który inicjuje
framework oraz rejestruje autoloader (zobacz poniżej).

Jak każdy front kontroler, ``app.php`` używa Klasy Jądra, ``AppKernel``, do
inicjalizacji aplikacji.

.. _the-app-dir:

Katalog ``app/``
~~~~~~~~~~~~~~~~

Klasa ``AppKernel`` jest głównym wejściem konfiguracji aplikacji
i jako takie, jest przechowywany w folderze ``app/``.

Ta klasa musi implementować dwie metody:

* ``registerBundles()`` musi zwracać tablicę wszystkich bundli do uruchomienia
  aplikacji;

* ``registerContainerConfiguration()`` wczytuje konfigurację aplikacji
  (więcej na ten temat później).

Autoload PHP może zostać skonfigurowany poprzez ``app/autoload.php``::

    // app/autoload.php
    use Symfony\Component\ClassLoader\UniversalClassLoader;

    $loader = new UniversalClassLoader();
    $loader->registerNamespaces(array(
        'Symfony'          => array(__DIR__.'/../vendor/symfony/src', __DIR__.'/../vendor/bundles'),
        'Sensio'           => __DIR__.'/../vendor/bundles',
        'JMS'              => __DIR__.'/../vendor/bundles',
        'Doctrine\\Common' => __DIR__.'/../vendor/doctrine-common/lib',
        'Doctrine\\DBAL'   => __DIR__.'/../vendor/doctrine-dbal/lib',
        'Doctrine'         => __DIR__.'/../vendor/doctrine/lib',
        'Monolog'          => __DIR__.'/../vendor/monolog/src',
        'Assetic'          => __DIR__.'/../vendor/assetic/src',
        'Metadata'         => __DIR__.'/../vendor/metadata/src',
    ));
    $loader->registerPrefixes(array(
        'Twig_Extensions_' => __DIR__.'/../vendor/twig-extensions/lib',
        'Twig_'            => __DIR__.'/../vendor/twig/lib',
    ));

    // ...

    $loader->registerNamespaceFallbacks(array(
        __DIR__.'/../src',
    ));
    $loader->register();

:class:`Symfony\\Component\\ClassLoader\\UniversalClassLoader` jest używana do ładowania
plików które respektują techniczne `standardy`_ dla przestrzeni nazw PHP 5.3
lub też `konwencji`_ nazewnictwa klas PEAR. Jak możesz zauważyć tutaj, wszystkie
zależności przechowywane są w katalogu ``vendor/``, jest to tylko konwencja.
Możesz przechowywać je gdziekolwiek chcesz, globalnie na Twoim serwerze lub
lokalnie w Twoich projektach.

.. note::

    Jeśli chcesz dowiedzieć się więcej o elastyczności autoloadera Symfony2, przeczytaj
    ":doc:`/cookbook/tools/autoloader`".

Zrozumienie Systemu Bundli
--------------------------

Ten rozdział przedstawia jeden z największych i najpotężniejszych cech Symfony2,
system :term:`bundle`.

Bundle jest czymś w rodzaju pluginu - w innych programach. Więc dlaczego nazywamy
to *bundle* a nie właśnie *plugin*? Ponieważ *wszystko* jest bundlem w Symfony2,
od rdzennych funkcjonalności frameworka po kod który piszesz dla swojej aplikacji.
Bundle są obywatelami pierwszej kategorii w Symfony2. Daje Ci to elastyczność
użycia gotowych funkcji popakowanych w bundlach zewnętrznych lub do
rozpowszechniania swoich własnych bundli. Umożliwia to łatwe wybranie funkcji
które chcesz włączyć w swojej aplikacji oraz dostosowanie ich do swoich potrzeb.
I na koniec dnia, kod Twojej aplikacji jest tak samo *ważny* jak kod jądra
frameworka.

Rejestracja Bundla
~~~~~~~~~~~~~~~~~~

Aplikacja składa się z bundli zdefiniowanych w metodzie ``registerBundles()``
klasy ``AppKernel``. Każdy bundle jest folderem zawierającym pojedyńczą klasę
``Bundle`` która go opisuje::

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
            new JMS\SecurityExtraBundle\JMSSecurityExtraBundle(),
        );

        if (in_array($this->getEnvironment(), array('dev', 'test'))) {
            $bundles[] = new Acme\DemoBundle\AcmeDemoBundle();
            $bundles[] = new Symfony\Bundle\WebProfilerBundle\WebProfilerBundle();
            $bundles[] = new Sensio\Bundle\DistributionBundle\SensioDistributionBundle();
            $bundles[] = new Sensio\Bundle\GeneratorBundle\SensioGeneratorBundle();
        }

        return $bundles;
    }

Oprócz ``AcmeDemoBundle`` o którym już rozmawialiśmy wcześniej, zauważ że jądro
także włącza inne bundle, takie jak ``FrameworkBundle``,
``DoctrineBundle``, ``SwiftmailerBundle``, oraz ``AsseticBundle``.
One wszystkie pochodzą z rdzenia frameworka.

Konfiguracja Bundla
~~~~~~~~~~~~~~~~~~~

Każdy z bundli może być dostosowany dzięki plikom konfiguracyjnym napisanym w
YAML, XML lub PHP. Popatrz na domyślną konfigurację:

.. code-block:: yaml

    # app/config/config.yml
    imports:
        - { resource: parameters.ini }
        - { resource: security.yml }

    framework:
        secret:          %secret%
        charset:         UTF-8
        router:          { resource: "%kernel.root_dir%/config/routing.yml" }
        form:            true
        csrf_protection: true
        validation:      { enable_annotations: true }
        templating:      { engines: ['twig'] } #assets_version: SomeVersionScheme
        session:
            default_locale: %locale%
            auto_start:     true

    # Konfiguracja Twig
    twig:
        debug:            %kernel.debug%
        strict_variables: %kernel.debug%

    # Konfiguracja Assetic
    assetic:
        debug:          %kernel.debug%
        use_controller: false
        filters:
            cssrewrite: ~
            # closure:
            #     jar: %kernel.root_dir%/java/compiler.jar
            # yui_css:
            #     jar: %kernel.root_dir%/java/yuicompressor-2.4.2.jar

    # Konfiguracja Doctrine
    doctrine:
        dbal:
            driver:   %database_driver%
            host:     %database_host%
            dbname:   %database_name%
            user:     %database_user%
            password: %database_password%
            charset:  UTF8

        orm:
            auto_generate_proxy_classes: %kernel.debug%
            auto_mapping: true

    # Konfiguracja Swiftmailer
    swiftmailer:
        transport: %mailer_transport%
        host:      %mailer_host%
        username:  %mailer_user%
        password:  %mailer_password%

    jms_security_extra:
        secure_controllers:  true
        secure_all_services: false

Każdy z wpisów jak np. ``framework`` definiuje konfigurację dla specyficznego bundla.
Dla przykładu, ``framework`` konfiguruje bundla ``FrameworkBundle`` a ``swiftmailer``
konfiguruje ``SwiftmailerBundle``.

Każde środowisko może nadpisać domyślną konfigurację poprzez dostarczenie odpowiedniego
pliku konfiguracyjnego. Dla przykładu, środowisko ``dev`` wczytuje plik ``config_dev.yml``,
który to wczytuje główną konfigurację (np. ``config.yml``) oraz modyfikuje go w celu
dodania narzędzi do debugowania:

.. code-block:: yaml

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

Rozszerzenie Bundla
~~~~~~~~~~~~~~~~~~~

Oprócz tego że bundle to dobry sposób na zorganizowanie i skonfigurowanie Twojego
kodu, bundle może rozszerzyć innego bundla. Dziedziczenie bundla umożliwia Ci nadpisanie
istniejącego bundla w celu dostosowania jego kontrolerów, szablonów lub każdego z jego plików.
To jest miejsce gdzie logiczne nazwy (np. ``@AcmeDemoBundle/Controller/SecuredController.php``)
są tak przydatne: są abstrakcją do aktualnego miejsca przechowywania zasobu.

Logiczne Nazwy Plików
.....................

Kiedy chcesz się odwołać do pliku bundla, użyj notacji:
``@BUNDLE_NAME/path/to/file``; Symfony2 zamieni ``@BUNDLE_NAME`` na
realną ścieżkę do bundla. Na przykład, logiczna ścieżka
``@AcmeDemoBundle/Controller/DemoController.php`` zostanie przekonwertowana
na ``src/Acme/DemoBundle/Controller/DemoController.php`` ponieważ Symfony
zna lokalizację ``AcmeDemoBundle``.

Logiczne Nazwy Kontrolera
.........................

Dla kontrolerów, do odwołania się do metod musisz użyć notacji
``BUNDLE_NAME:CONTROLLER_NAME:ACTION_NAME``. Dla przykładu,
``AcmeDemoBundle:Welcome:index`` wskazuje na metodę ``indexAction``
z klasy ``Acme\DemoBundle\Controller\WelcomeController``.

Logiczne Nazwy Szablonów
........................

Dla szablonów, logiczna nazwa ``AcmeDemoBundle:Welcome:index.html.twig`` zostanie
przekonwertowana na ścieżke do pliku ``src/Acme/DemoBundle/Resources/views/Welcome/index.html.twig``.
Szablony staną się jeszcze bardziej interesujące kiedy zdasz sobie sprawę że nie musisz je
przechowywać w systemie plików. Dla przykładu, możesz w prosty sposób przechowywać je w bazie danych.

Rozszerzenie Bundli
...................

Jeśli będziesz stosować tę konwencję, możesz użyć :doc:`bundle inheritance</cookbook/bundles/inheritance>`
do "nadpisania" plików, kontrolerów lub szablonów. Dla przykładu, możesz stworzyć bundle - ``AcmeNewBundle`` -
oraz określić że jego rodzicem jest ``AcmeDemoBundle``.
Kiedy Symfony będzie ładowało kontroler ``AcmeDemoBundle:Welcome:index``, najpierw poszukiwania
klasy ``WelcomeController`` rozpocznie w ``AcmeNewBundle`` a dopiero później w ``AcmeDemoBundle``.
To oznacza że jeden bundle może nadpisać praktycznie każdą część innego bundla!

Rozumiesz teraz dlaczego Symfony2 jest tak elastyczne? Wykorzystuj swoje bundle pomiędzy
aplikacjami, przechowuj je lokalnie lub globalnie, to zależy od Ciebie.

.. _using-vendors:

Używanie "Vendors"
------------------

Przeważnie Twoja aplikacja będzie zależała od bibliotek zewnętrznych.
Te powinny być składowane w folderze ``vendor/``. Katalog ten zawiera już biblioteki
Symfony2, bibliotekę SwiftMailer, Doctrine ORM, system szablonów Twig,
oraz kilka innych bibliotek zewnętrznych oraz bundli.

Zrozumienie Cache oraz Logów
----------------------------

Symfony2 jest prawdopodobnie jednym z najszybszych pełnych frameworków.
Ale jak może być tak szybki skoro parsuje oraz interpretuje kilkadziesiąt
plików YAML oraz XML dla każdego zapytania. Prędkość jest po części związana
z systemem cache. Konfiguracja aplikacji jest parsowana tylko za pierwszym zapytaniem
i przetwarzana do kodu PHP przechowywanego w katalogu ``app/cache/``.
W środowisku deweloperskim, Symfony2 jest wystarczająco inteligentny aby czyścić
cache po zmianie pliku. Ale w środowisku produkcyjnym, to do Twoich obowiązków
należy czyszczenie cache po zmianie kodu lub jego konfiguracji.

Podczas tworzenia aplikacji, dużo rzeczy może pójść źle. Pliki logów w katalogu
``app/logs/`` powiedzą Ci wszystko na temat zapytań (requests) oraz pomogą Ci
szybko naparwić napotkane problemy.

Korzystanie z Interfejsu Linii Poleceń
--------------------------------------

Każda aplikacja posiada narzędzie interfejsu linii poleceń (``app/console``)
który pomaga utrzymywać aplikację. Interfejs zapewnia polecenia które zwiększają
produktywność poprzez automatyzację żmudnych i powtarzających się zadań.

Uruchom go bez żadnych argumentów aby dowiedzieć się więcej o dostępnych możliwościach:

.. code-block:: bash

    php app/console

Opcja ``--help`` pomoże Ci odkryć sposoby używania komendy:

.. code-block:: bash

    php app/console router:debug --help

Podsumowanie
------------

Nazwij mnie szaleńcem, ale po przeczytaniu tej części powinieneś czuć się komfortowo
w pracy z Symfony2. Wszystko w Symfony2 jest zaprojektowane tak by sprostać Twoim
oczekiwaniom. Zatem, zmieniaj nazwy, przenoś katalogi zgodnie z swoimi upodobaniami.

I to wyszystko jeśli chodzi o szybkie wprowadzenie. Od testowania do wysyłania maili,
musisz jeszcze się wiele nauczyć aby zostać mistrzem Symfony2. Gotowy aby zagłębić
się w te tematy? Nie musisz szukać dalej - przejdź do :doc:`/book/index` i wybierz
temat który Cię interesuje.

.. _standardy:               http://groups.google.com/group/php-standards/web/psr-0-final-proposal
.. _konwencji:               http://pear.php.net/
