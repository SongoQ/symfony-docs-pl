.. index::
   single: Strony

Tworzenie stron w Symfony2
==========================

Tworzenie nowej strony w Symfony2 to prosty, dwuetapowy proces:

  * *Utwórz trasę (ang. route)*: Trasa definiuje URL (np. ``/o-nas``) do Twojej strony
    oraz określa kontroler (który jest funkcją PHP), który Symfony2 powinno wykonać,
    kiedy URL przychodzącego żądania pasuje do wzorca trasy.

  * *Utwórz kontroler*: Kontroler jest funkcją PHP, która pobiera przychodzące żądanie
    i transformuje je w obiekt ``Odpowiedzi`` (ang. ``Response``) Symfony2, który jest zwracany
    użytkownikowi.


To proste podejście jest piękne, gdyż pasuje do sposobu funkcjonowania Sieci.
Każda interakcja w Sieci jest inicjowana przez żądanie HTTP. Zadaniem Twojej aplikacji jest
po prostu zinterpretować żądanie i zwrócić właściwą odpowiedź HTTP.

Symfony2 stosuje tą filozofię i dostarcza Ci narzędzia oraz konwencje, które pomogą Ci utrzymać
swoją aplikację zorganizowaną wraz ze wzrostem liczby użytkowników i złożoności.

Brzmi wystarczająco prosto? Do dzieła!

.. index::
   single: Strony; Przykład

Strona "Witaj Symfony!"
-----------------------

Zacznijmy od klasycznej aplikacji "Witaj Świecie!". Kiedy skończysz, użytkownik będzie
miał możliwość ujrzeć osobiste pozdrowienie (np. "Witaj Symfony") wchodząc w poniższy URL:

.. code-block:: text

    http://localhost/app_dev.php/hello/Symfony

W rzeczywistości będziesz mógł zastąpić *Symfony* innym imieniem, które ma być użyte w pozdrowieniu.
Aby utworzyć stronę, wykonaj prosty, dwuetapowy proces.

.. note::

    Ten tutorial zakłada, że już pobrałeś Symfony2 i skonfigurowałeś swój serwer. Powyższy URL przyjmuje,
    że ``localhost`` wskazuje na katalog ``web`` Twojego nowego projektu Symfony2.
    Po więcej informacji na temat tego procesu zobacz: :doc:`Instalacja Symfony2</book/installation>`.

Zanim zaczniesz: Utwórz Bundle
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Zanim zaczniesz, musisz utworzyć *bundle*. W Symfony2 :term:`bundle`
jest jak wtyczka, tyle, że cały kod Twojej aplikacji będzie znajdował
się wewnątrz bundle.

Bundle to nic innego jak katalog, który przechowuje wszystko związane
z daną funkcją, włączając w to klasy PHP, konfigurację, a nawet arkusze stylów
czy pliki Javascript (zobacz :ref:`page-creation-bundles`).

Aby utworzyć bundle o nazwie ``AcmeHelloBundle`` (testowy bundle, który
zbudujesz w tym rozdziale), wykonaj poniższe polecenie i postępuj ze wskazówkami
na ekranie (zastosuj wszystkie domyślne opcje):

.. code-block:: bash

    php app/console generate:bundle --namespace=Acme/HelloBundle --format=yml

Za kulisami tworzony jest katalog dla bundle ``src/Acme/HelloBundle``.
Ponadto do ``app/AppKernel.php`` automatycznie dodana jest linia, dzięki której bundle
jest rejestrowany w jądrze::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Acme\HelloBundle\AcmeHelloBundle(),
        );
        // ...

        return $bundles;
    }

Właśnie skonfigurowałeś swój bundle, teraz możesz zacząć budować swoją aplikację
wewnątrz bundle.

Krok 1: Utwórz trasę
~~~~~~~~~~~~~~~~~~~~

Domyślnie, plik konfiguracyjny routingu w aplikacji Symfony2 znajduje się w katalogu
``app/config/routing.yml``. Podobnie jak w całej konfiguracji Symfony2, możesz również
wybrać XML lub PHP "out of the box" do konfiguracji tras.

Jeśli spojrzysz na główny plik routingu, zobaczysz, że Symfony już dodało wpis, kiedy
generowałeś ``AcmeHelloBundle``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        AcmeHelloBundle:
            resource: "@AcmeHelloBundle/Resources/config/routing.yml"
            prefix:   /

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml" prefix="/" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->addCollection(
            $loader->import('@AcmeHelloBundle/Resources/config/routing.php'),
            '/',
        );

        return $collection;

Ten wpis jest bardzo prosty: mówi on Symfony, aby wczytywał konfigurację routingu
z pliku ``Resources/config/routing.yml``, który żyje wewnątrz ``AcmeHelloBundle``.
Oznacza to, że możesz umieścić konfigurację routingu bezpośrednio w ``app/config/routing.yml``
lub zorganizować swoje trasy po całej swojej aplikacji i importować je tutaj.

Teraz, kiedy plik ``routing.yml`` bundle'a jest importowany dodaj nową trasę, która
zdefiniuje URL strony którą chcesz utworzyć:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/routing.yml
        hello:
            pattern:  /hello/{name}
            defaults: { _controller: AcmeHelloBundle:Hello:index }

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="hello" pattern="/hello/{name}">
                <default key="_controller">AcmeHelloBundle:Hello:index</default>
            </route>
        </routes>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
        )));

        return $collection;


Routing składa się z dwóch podstawowych części: ``wzorca``, którym jest URL
do którego prowadzi trasa, oraz z tablicy ``defaults``, która definiuje
kontroler, który ma być wykonany. Składnia symbolu zastępczego we wzorcu
(``{name}``) jest wieloznacznikiem. Oznacza to, że ``/hello/Ryan``, ``/hello/Fabien``
oraz każdy inny podobny URL będzie pasował do tej trasy. Parametr symbolu ``{name}``
będzie również przekazany do kontrolera, więc możesz używać jego wartość, aby
pozdrowić użytkownika po imieniu.

.. note::

  System routingu posiada wiele świetnych funkcji do tworzenia elastycznych
  i potężnych struktur URL w Twojej aplikacji. W celu uzyskania więcej informacji,
  zobacz rozdział dotyczący :doc:`Routingu </book/routing>`.

Krok 2: Utwórz kontroler
~~~~~~~~~~~~~~~~~~~~~~~~

Kiedy URL taki jak ``/hello/Ryan`` jest obsługiwany przez aplikację, dopasowana jest
trasa ``hello``, a kontroler ``AcmeHelloBundle:Hello:index`` jest wykonywany przez
framework. Drugim krokiem procestu tworzenia strony jest utworzenie tego kontrolera.

Kontroler - ``AcmeHelloBundle:Hello:index`` jest *logiczną* nazwą kontrolera i mapuje
ona metodę ``indexAction`` klasy PHP o nazwie ``Acme\HelloBundle\Controller\Hello``.
Zacznij od utworzenia tego pliku wewnątrz Twojego ``AcmeHelloBundle``::

    // src/Acme/HelloBundle/Controller/HelloController.php
    namespace Acme\HelloBundle\Controller;

    use Symfony\Component\HttpFoundation\Response;

    class HelloController
    {
    }

W rzeczywistości, kontroler to nic innego jak metoda PHP, którą Ty tworzysz,
a Symfony ją wykonuje. To tutaj Twój kod używa informacje z żądania, aby zbudować
i przygotować żądany zasób. Z wyjątkiem szczególnych przypadków, końcowy produkt
kontrolera jest zawsze taki sam: obiekt ``Odpowiedzi`` Symfony2.

Utwórz akcję ``indexAction``, którą Symfony uruchomi, kiedy trasa ``hello`` będzie
dopasowana::

    // src/Acme/HelloBundle/Controller/HelloController.php

    // ...
    class HelloController
    {
        public function indexAction($name)
        {
            return new Response('<html><body>Hello '.$name.'!</body></html>');
        }
    }

Kontroler jest prosty: tworzy nowy obiekt ``Response``, którego pierwszym argumentem
jest treść będzie użyta w odpowiedzi (w tym przykładzie jest to niewielka strona HTML).

Gratulacje! Po utworzeniu wyłącznie trasy oraz kontrolera, masz już w pełni funkcjonalną
stronę. Jeśli ustawiłeś wszystko poprawnie, Twoja aplikacja powinna Cię pozdrowić:

.. code-block:: text

    http://localhost/app_dev.php/hello/Ryan

.. tip::

    Możesz również zobaczyć swoją aplikację w środowisku "prod" (produkcyjnym) :ref:`środowiska <environments-summary>`
    odwiedzając:

    .. code-block:: text

        http://localhost/app.php/hello/Ryan

    Jeśli otrzymujesz błąd, prawdopodobnie musisz wyczyścić cache za pomocą komendy:

    .. code-block:: bash

        php app/console cache:clear --env=prod --no-debug

Opcjonalnym, lecz częstym, trzecim krokiem procesu tworzenia strony jest utworzenie szablonu.

.. note::

   Kontrolery są głównym punktem wejścia Twojego kodu oraz kluczowym składnikiem
   podczas tworzenia stron. Więcej informacji można znaleźć w rozdziale :doc:`Kontroler </book/controller>`,

Opcjonalny Krok 3: Utwórz szablon
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Szablon pozwala Ci przenieść całą warstwę prezentacji (np. kod HTML) do oddzielnego pliku
i ponownie wykorzystywać różne części układu strony. Zamiast pisać kod HTML wewnątrz
kontrolera, wyrenderuj szablon:

.. code-block:: php
    :linenos:

    // src/Acme/HelloBundle/Controller/HelloController.php
    namespace Acme\HelloBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
            return $this->render('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

            // render a PHP template instead
            // return $this->render('AcmeHelloBundle:Hello:index.html.php', array('name' => $name));
        }
    }

.. note::

   Aby móc używać metody ``render()``, Twój kontroler musi dziedziczyć klasę
   ``Symfony\Bundle\FrameworkBundle\Controller\Controller`` (API
   docs: :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`),
   która dodaje skróty do częstych zadań wykonywanych w kontrolerach. Jest to
   zastosowane w powyższym przykładzie poprzez dodanie wyrażenia ``use`` w 4
   linii, a następnie rozszerzenia ``Controller`` w linii 6.

Metoda ``render()`` tworzy nową ``Odpowiedź`` (ang. ``Response``) wypełnioną
zawartością danego, wyrenderowanego szablonu. Podobnie jak inny kontroler,
ostatecznie zwróci on obiekt ``Response``.

Zauważ, że są dwa różne przykłady renderowania szablonów. Domyślnie Symfony2
wspiera dwa różne języki szablonów: klasyczne szablony PHP oraz zwięzłe, lecz
potężne szablony `Twig`_. Nie przejmuj się - możesz wybrać jeden, lub nawet
oba z nich w tym samym projekcie.

Kontroler renderuje szablon ``AcmeHelloBundle:Hello:index.html.twig``, który
używa następującej konwencji nazewnictwa:

    **NazwaBundle**:**NazwaKontrolera**:**NazwaSzablonu**

To jest *logiczna* nazwa szablonu, który jest zmapowany do fizycznej lokacji
używając następującej konwencji:

    **/path/to/NazwaBundle**/Resources/views/**NazwaKontrolera**/**NazwaSzablonu**

W tym przypadku, ``AcmeHelloBundle`` jest nazwą Bundle, ``Hello`` to kontroler, a
``index.html.twig`` jest szablonem:

.. configuration-block::

    .. code-block:: jinja
       :linenos:

        {# src/Acme/HelloBundle/Resources/views/Hello/index.html.twig #}
        {% extends '::base.html.twig' %}

        {% block body %}
            Hello {{ name }}!
        {% endblock %}

    .. code-block:: php

        <!-- src/Acme/HelloBundle/Resources/views/Hello/index.html.php -->
        <?php $view->extend('::base.html.php') ?>

        Hello <?php echo $view->escape($name) ?>!

Przeanalizujmy szablon Twig krok po kroku:

* *linia 2*: Znacznik ``extends`` definiuje szablon rodzica. Szablon wyraźnie
  definiuje plik układu strony, wewnątrz którego ma być on umieszczony.

* *linia 4*: Znacznik ``block`` mówi, że wszystko wewnątrz szablonu powinno zostać
  umieszczone wewnątrz bloku ``body``. Jak się później przekonasz, to szablon-rodzic
  (``base.html.twig``) jest odpowiedzialny za ostatecznie wyrenderowanie bloku ``body``.

Szablon nadrzędny ``::base.html.twig`` nie posiada w swojej nazwie zarówno **NazwaBundle**,
jak i **NazwaKontrolera** (stąd podwójny dwukropek (``::``) na początku). Oznacza to, że
szablon żyje na zewnątrz bundle, wewnątrz katalogu ``app``:

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/base.html.twig #}
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title>{% block title %}Welcome!{% endblock %}</title>
                {% block stylesheets %}{% endblock %}
                <link rel="shortcut icon" href="{{ asset('favicon.ico') }}" />
            </head>
            <body>
                {% block body %}{% endblock %}
                {% block javascripts %}{% endblock %}
            </body>
        </html>

    .. code-block:: php

        <!-- app/Resources/views/base.html.php -->
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title><?php $view['slots']->output('title', 'Welcome!') ?></title>
                <?php $view['slots']->output('stylesheets') ?>
                <link rel="shortcut icon" href="<?php echo $view['assets']->getUrl('favicon.ico') ?>" />
            </head>
            <body>
                <?php $view['slots']->output('_content') ?>
                <?php $view['slots']->output('stylesheets') ?>
            </body>
        </html>

Podstawowy plik szablonu definiuje układ strony HTML i renderuje blok ``body``, który
zdefiniowałeś w szablonie ``index.html.twig``. W przypadku, gdy nie zdefiniujesz bloku
``title`` w szablonie-dziecku, domyślnie zwraca on "Welcome!".

Szablony to potężny sposób renderowania i organizacji treści dla Twojej strony.
Szablon może wyrenderować wszystko, od znaczników HTML do kodu CSS, czy cokolwiek
innego, co kontroler potrzebuje zwracać.

W cyklu życia obsługi żądania, silnik szablonów jest po prostu dodatkowym narzędziem.
Przypomnij sobie, że celem każdego kontrolera jest zwrócić obiekt ``Response``.
Szablony są potężnym, lecz opcjonalnym narzędziem do tworzenia treści dla
obiektu ``Response``.

.. index::
   single: Struktura katalogów

Struktura katalogów
-------------------

Już po kilku krótkich akapitach rozumiesz filozofię tworzenia i renderowania stron
w Symfony2. Zobaczyłeś również jak projekty Symfony2 są zbudowane i zorganizowane.
Na koniec tej sekcji będziesz wiedział gdzie znaleźć i umieścić różne typy
plików, oraz dlaczego.

Pomimo całkowitej elastyczności, domyślnie każda :term:`aplikacja` symfony
ma tą samą podstawę oraz zalecaną strukturę katalogów:

* ``app/``: Ten katalog zawiera konfigurację aplikacji;

* ``src/``: Cały kod PHP projektu przechowywany jest w tym katalogu;

* ``vendor/``: Wszelkie dodatkowe biblioteki umieszczone są tutaj w celu zachowania konwencji;

* ``web/``: To jest główny katalog strony (web root), który zawiera wszystkie dostępne publicznie pliki;

Katalog Web
~~~~~~~~~~~

Katalog web root jest głównym katalogiem wszystkich publicznych i statycznych
plików, włączając w to obrazki, arkusze stylów oraz pliki JavaScript. Tutaj
również znajduje się :term:`front kontroler`::

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();

Plik front kontrolera (w tym przypadku ``app.php``) jest właściwym plikiem PHP,
który jest wykonywany przy każdym użyciu aplikacji Symfony2, a jego zadaniem
jest użyć klasy Jądra (ang. Kernel) ``AppKernel``, aby wykonać rozruch aplikacji.

.. tip::

    Posiadanie front kontrolera oznacza różne i bardziej elastyczne URLe niż te
    używane w typowej aplikacji PHP. Wykorzystując front kontroler, URLe formatowane
    są w następujący sposób:

    .. code-block:: text

        http://localhost/app.php/hello/Ryan

    Wykonywany jest front kontroler ``app.php``, a "wewnętrzny:" URL ``/hello/Ryan``
    jest trasowany wewnętrzenie przy użyciu konfiguracji routingu.
    Dzięki zastosowaniu reguł ``mod_rewrite`` serwera Apache, możesz wymusić, aby plik
    ``app.php`` był uruchamiany bez konieczności podawania jego nazwy w adresie URL:

    .. code-block:: text

        http://localhost/hello/Ryan

Pomimo, że front kontrolery są niezbędne w obsłudze każdego żądania, bardzo rzadko
będziesz potrzebował je modyfikować, czy nawet myśleć o nich. Wspomnimy o nich
pokrótce w sekcji `Środowiska`_.

Katalog aplikacji (``app``)
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jak mogłeś zobaczyć we front kontrolerze, klasa ``AppKernel`` jest głównym punktem
wejścia aplikacji i jest odpowiedzialna za całą konfigurację. Jako taki,
jest on przechowywany w katalogu ``app/``.

Ta klasa musi zawierać dwie metody, które definiują wszystko, co Symfony potrzebuje
wiedzieć o Twojej aplikacji. Nie musisz nawet martwić się o te metody kiedy
zaczynasz - Symfony uzupełni je za Ciebie odpowiednimi wartościami domyślnymi.

* ``registerBundles()``: Zwraca tablicę wszystkich bundli potrzebnych do uruchomienia
  aplikacji (zobacz :ref:`page-creation-bundles`);

* ``registerContainerConfiguration()``: Ładuje główny plik konfiguracji aplikacji
  (zobacz dział `Konfiguracja aplikacji`_).

W codziennej pracy w większości będziesz używał katalogu ``app/`` do modyfikacji
konfiguracji, oraz plików routingu w katalogu ``app/config`` (zobacz `Konfiguracja aplikacji`_).
Katalog ten zawiera również katalog cache (``app/cache``), katalog logów (``app/logs``),
a także katalog dla plików zasobów aplikacji, takich jak szablony (``app/Resources``).
O każdym z nich dowiesz się więcej w kolejnych rozdziałach.

.. _autoloading-introduction-sidebar:

.. sidebar:: Autoloading

    Kiedy Symfony jest uruchamiany, dołączany jest specjalny plik - ``app/autoload.php``.
    Ten plik odpowiedzialny jest za konfigurację autoloadera, który dołączy pliki
    Twojej aplikacji z katalogu ``src/`` oraz dodatkowe biblioteki z katalogu ``vendor/``.

    Dzięki autoloaderowi, nigdy nie musisz martwić się o używanie wyrażeń ``include``
    czy ``require``. Zamiast tego, Symfony2 używa przestrzeni nazw klasy do określenia
    jej lokalizacji i automatycznie dołącza plik, kiedy potrzebujesz zawartej w nim klasy.

    Autoloader jest domyślnie skonfigurowany aby szukać Twoich klas PHP w katalogu ``src/``.
    Aby autoloading działał, nazwa klasy oraz ścieżka do pliku powinny mieć tą samą formę:

    .. code-block:: text

        Nazwa klasy:
            Acme\HelloBundle\Controller\HelloController
        Ścieżka:
            src/Acme/HelloBundle/Controller/HelloController.php

    Zwykle jedyny przypadek, kiedy musisz martwić się o plik ``app/autoload.php`` jest wtedy,
    kiedy dołączasz zewnętrzne biblioteki w katalogu ``vendor/``. Aby dowiedzieć się więcej
    o autoloadingu, zobacz :doc:`Jak dołączać klasy </components/class_loader>`.

Katalog źródeł (``src``)
~~~~~~~~~~~~~~~~~~~~~~~~

Katalog ``src/`` zawiera po prostu cały właściwy kod (PHP, szablony, pliki konfiguracyjne,
arkusze stylów, itd.), który obsługuje *Twoją* aplikację.
W trakcie rozwoju projektu, zdecydowana większość Twojej pracy będzie wykonana wewnątrz
jednego lub więcej bundli, które utworzysz w tym katalogu.

Lecz co właściwie oznacza termin :term:`bundle`?

.. _page-creation-bundles:

System Bundli
-------------

Bundle jest podobny do pluginów czy podobnego oprogramowania, jednakże jest od nich znacznie lepszy.
Kluczową różnicą jest to, że *wszystko* jest bundlem w Symfony2, włączając w to
rdzenną funkcjonalność frameworka oraz kod napisany dla Twojej aplikacji.
Bundle jest typem pierwszoklasowym w Symfony2. Daje Ci to elastyczność
użycia wbudowanych funkcji spakowanych w bundle osób trzecich, lub możliwość
rozprowadzania swoich własnych bundli. Ułatwia to wybierać które funkcje włączyć
w Twojej aplikacji, oraz optymalizować je na swój własny sposób.

.. note::

   Podczas gdy tutaj nauczysz się podstaw, w cookbook znajduje się cały rozdział
   poświęcony organizacji i najlepszym praktykom podczas korzystana z :doc:`bundli</cookbook/bundles/best_practises>`.

Bundle jest po prostu uporządkowanym zbiorem plików wewnątrz katalogu, który
implementuje pojedynczą funkcję. Możesz utworzyć ``BlogBundle``, ``ForumBundle``
czy bundle do zarządzania użytkownikami (wiele z nich już istnieje jako bundle
open source). Każdy katalog posiada wszystko związane z daną funkcją, włączając w
to pliki PHP, szablony, arkusze stylów, JavaScript, testy i całą resztę.
Każdy aspekt danej funkcji zawarty jest w bundle, jak również każda funkcja żyje w bundle.

Aplikacja składa się z bundli zdefiniowanych w metodzie ``registerBundles()`` klasy ``AppKernel``::

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

Za pomocą metody ``registerBundles()`` masz całkowitą kontrolę nad tym, które bundle
będą używane przez Twoją aplikacją (włączając w to rdzenne bundle Symfony).

.. tip::

   Bundle może znajdować się *wszędzie* dopóty, dopóki może być automatycznie ładowany
   (przez autoloader skonfigurowany w ``app/autoload.php``).

Tworzenie Bundle
~~~~~~~~~~~~~~~~~

Symfony Standard Edition przychodzi z poręcznym zadaniem, które tworzy w pełni funkcjonalny
bundle za Ciebie. Oczywiście tworzenie bundle ręcznie jest również bardzo proste.

Aby pokazać Ci jak prosty jest system bundli, utwórz nowy bundle o nazwie ``AcmeTestBundle``
i włącz go.

.. tip::

    Część ``Acme`` jest po prostu przykładową nazwą, która powinna zostać zastąpiona
    przez unikalną nazwę, która reprezentuje Ciebie lub Twoją organizację (np. ``ABCTestBundle``)
    dla firmy o nazwie ``ABC``).

Zacznij od utworzenia katalogu ``src/Acme/TestBundle/`` i dodania nowego pliku o nazwie
``AcmeTestBundle.php``::

    // src/Acme/TestBundle/AcmeTestBundle.php
    namespace Acme\TestBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;

    class AcmeTestBundle extends Bundle
    {
    }

.. tip::

   Nazwa ``AcmeTestBundle`` podąża za standardem :ref:`Konwencji nazewnictwa bundli<bundles-naming-conventions>`.
   Możesz również zdecydować, by skrócić nazwę bundla do prostego ``TestBundle``
   nazywając tą klasę ``TestBundle`` (i nazywając plik ``TestBundle.php``).

Ta pusta klasa jest jedyną rzeczą, jaką musisz zrobić, by utworzyć bundle. Pomimo, iż
jest całkowicie pusta, jest ona w pełni skuteczna i może zostać użyta do dostosowania
zachowań bundla.

Teraz, kiedy już utworzyłeś bundle, włącz za pomocą klasy ``AppKernel``::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            // ...

            // register your bundles
            new Acme\TestBundle\AcmeTestBundle(),
        );
        // ...

        return $bundles;
    }

Pomimo, że jeszcze nic on nie robi, ``AcmeTestBundle`` jest już gotowy do użycia.

Tak łatwo jak to, Symfony dostarcza również interfejs linii poleceń do
generowania podstawowego szkieletu bundle:

.. code-block:: bash

    php app/console generate:bundle --namespace=Acme/TestBundle

Szkielet bundla generowany jest wraz z podstawowym kotrolerem, szablonem i
zasobem routingu, który może być dostosowywany do Twoich potrzeb.
Więcej o linii poleceń Symfony2 dowiesz się później.

.. tip::

   Za każdym razem, kiedy tworzysz nowy bundle lub używasz bundle osób trzecich,
   upewnij się, że został on włączony w ``registerBundles()``. Kiedy używasz
   komendy ``generate:bundle``, to wszystko jest zrobione za Ciebie.

Struktura katalogów bundli
~~~~~~~~~~~~~~~~~~~~~~~~~~

Struktura katalogów bundla jest prosta i elastyczna. Domyślnie system bundli
podąża za zbiorem konwencji, które pomagają utrzymać kod zwarty pomiędzy
wszystkimi bundlami Symfony2. Spójrz na ``AcmeHelloBundle``, zawiera on
kilka najczęściej wykorzystywanych elementów bundla:

* ``Controller/`` zawiera kontrolery bundla (np. ``HelloController.php``);

* ``Resources/config/`` przechowuje konfigurację, włączając w to konfigurację
  routingu (np. ``routing.yml``);

* ``Resources/views/`` zawiera szablony uporządkowane wg nazw kontrolerów (np.
  ``Hello/index.html.twig``);

* ``Resources/public/`` przechowuje obrazki, arkusze stylów, itd. oraz jest kopiowany
  lub linkowany symbolicznie do katalogu ``web/`` projektu poprzez komendę
  ``assets:install`` w konsoli.

* ``Tests/`` Zawiera wszystkie testy dla bundla.

Bundle może być tak mały czy duży, jak funkcja, którą on implementuje. Zawiera on
tylko te pliki, które potrzebujesz - i nic więcej.

W miarę jak poruszasz się po książce, nauczysz się wstawiać obiekty do bazy danych,
tworzyć i walidować formularze, tworzyć tłumaczenia dla Twojej aplikacji, pisać testy
i wiele innych. Każdy z tych elementów ma swoje miejsce i rolę w bundlu.

Konfiguracja aplikacji
----------------------

Aplikacja składa się z kolekcji bundli reprezentujących wszystkie funkcje
i zdolności Twojej aplikacji. Każdy bundle może być dostosowany poprzez
pliki konfiguracyjne napisane w YAML, XML lub PHP. Domyślnie, główny plik
konfiguracyjny znajduje się w katalogu ``app/config/`` i jest nazwany odpowiednio
``config.yml``, ``config.xml`` lub ``config.php`` zależnie od tego, który format preferujesz:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            - { resource: parameters.yml }
            - { resource: security.yml }

        framework:
            secret:          %secret%
            charset:         UTF-8
            router:          { resource: "%kernel.root_dir%/config/routing.yml" }
            # ...

        # Twig Configuration
        twig:
            debug:            %kernel.debug%
            strict_variables: %kernel.debug%

        # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <imports>
            <import resource="parameters.yml" />
            <import resource="security.yml" />
        </imports>

        <framework:config charset="UTF-8" secret="%secret%">
            <framework:router resource="%kernel.root_dir%/config/routing.xml" />
            <!-- ... -->
        </framework:config>

        <!-- Twig Configuration -->
        <twig:config debug="%kernel.debug%" strict-variables="%kernel.debug%" />

        <!-- ... -->

    .. code-block:: php

        $this->import('parameters.yml');
        $this->import('security.yml');

        $container->loadFromExtension('framework', array(
            'secret'          => '%secret%',
            'charset'         => 'UTF-8',
            'router'          => array('resource' => '%kernel.root_dir%/config/routing.php'),
            // ...
            ),
        ));

        // Twig Configuration
        $container->loadFromExtension('twig', array(
            'debug'            => '%kernel.debug%',
            'strict_variables' => '%kernel.debug%',
        ));

        // ...

.. note::

   O tym jak ładować każdy plik/format nauczysz się dokładniej w następnym dziale
   `Środowiska`_.

Każa główna pozycja jak ``framework`` czy ``twig`` definiują konfigurację dla
pojedynczego bundla. Dla przykładu, klucz ``framework`` definiuje konfigurację
dla rdzennego ``FrameworkBundle`` Symfony i ładuje konfigurację routingu, szablonów oraz
pozostałych systemów rdzenia.

Na chwilę obecną nie martw się o szczegółowe opcje konfiguracji w każdej sekcji.
Plik konfiguracyjny ładuje się z rozsądnymi wartościami domyślnymi. W miarę dalszego
czytania oraz odkrywana każdej części Symfony2, nauczysz się więcej o szczegółowych
opcjach konfiguracji każdej funkcji.

.. sidebar:: Formaty konfiguracji

    Przez wszystkie rozdziały, wszystkie przykłady konfiguracji będą pokazane
    we wszystkich trzech formatach (YAML, XML i PHP). Każdy z nich ma swoje wady
    i zalety. Decyzja, który z nich wybrać, należy do Ciebie:

    * *YAML*: Prosty, przejrzysty i czytelny;

    * *XML*: Czasami potężniejszy niż YAML i wspiera autouzupełnianie środowisk IDE.

    * *PHP*: Najbardziej potężny, lecz najmniej czytelny ze standardowych formatów konfiguracji.

Zrzut domyślnej konfiguracji
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.1
    Polecenie ``config:dump-reference`` zostało dodane w Symfony 2.1

Możesz zrzucić domyślną konfigurację bundla w yaml do konsoli używając polecenia
``config:dump-reference``. Oto przykład zrzutu domyślnej konfiguracji FrameworkBundle:

.. code-block:: text

    app/console config:dump-reference FrameworkBundle

.. note::

    Zobacz artykuł w cookbook: :doc:`H`How to expose a Semantic Configuration for
    a Bundle</cookbook/bundles/extension>` aby uzyskać więcej informacji nt. dodawania
    konfiguracji do Twojego bundla.

.. index::
   single: Środowiska; Wprowadzenie

.. _environments-summary:

Środowiska
----------

Aplikacja może być uruchomiona w różnych środowiskach. Różne środowiska
dzielą ten sam kod PHP (oprócz front kontrolera), lecz używają różnej konfiguracji.
Dla przykładu, środowisko ``dev`` będzie logował ostrzeżenia i błędy, podczas gdy
środowisko ``prod`` będzie logował wyłącznie błędy. Niektóre pliki są
tworzone ponownie podczas każdego żądania w środowisku ``dev`` (dla wygody deweloperów),
zaś w środowisku ``prod`` są cache'owane. Wszystkie środowiska żyją razem na
tej samej maszynie i uruchamiają tą samą aplikację.

Projekt Symfony2 generalnie ma początek z trzeba środowiskami (``dev`, ``test`` i ``prod``),
jednakże tworzenie nowych środowisk jest proste. Możesz łatwo zobaczyć swoją aplikację w różnych
środowiskach zmieniając front kontroler w przeglądarce. Aby ujrzeć aplikację
w środowisku ``dev``, uruchom aplikację przez deweloperski front kontroler:

.. code-block:: text

    http://localhost/app_dev.php/hello/Ryan

Jeśli chcesz zobaczyć jak Twoja aplikacja zachowa się w środowisku produkcyjnym,
wywołaj front kontroler ``prod``:

.. code-block:: text

    http://localhost/app.php/hello/Ryan

Ponieważ środowisko ``prod`` jest optymalizowane pod względem szybkości; konfiguracja,
routing i szablony Twig są kompilowane do postaci klas PHP i cache'owane.
Aby zobaczyć zmiany w środowisku ``prod``, musisz wyczyścić zcache'owane pliki
i pozwolić im wygenerować się od nowa::

    php app/console cache:clear --env=prod --no-debug

.. note::

   Jeśli otworzysz plik ``web/app.php``, zobaczysz, że jest on skonfigurowany specjalnie,
   aby używać środowiska ``prod``::

       $kernel = new AppKernel('prod', false);

   Możesz utworzyć nowy front kontroler dla nowego środowiska kopiując ten plik
   i zmieniając wartość ``prod`` na inną.

.. note::

    Środowisko ``test`` jest używane podczas uruchamiania zautomatyzowanych testów
    i nie jest dostępny bezpośrednio z przeglądarki. Po więcej informacji zobacz rozdział
    :doc:`Testowanie</book/testing>`.

.. index::
   single: Środowiska; Konfiguracja

Konfiguracja środowiska
~~~~~~~~~~~~~~~~~~~~~~~~~

Klasa ``AppKernel`` jest odpowiedzialna za właściwe ładowanie wybranego przez Ciebie
pliku konfiguracyjnego::

    // app/AppKernel.php
    public function registerContainerConfiguration(LoaderInterface $loader)
    {
        $loader->load(__DIR__.'/config/config_'.$this->getEnvironment().'.yml');
    }

Wiesz już, że rozszerzenie ``.yml`` może być zamienione na ``.xml`` lub ``.php``,
jeśli preferujesz używać HML lub PHP do pisania Twojej konfiguracji.
Zauważ również, że każde środowisko ładuje swój własny plik konfiguracyjny. Zwróćmy
uwagę na plik konfiguracyjny środowiska ``dev``.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        imports:
            - { resource: config.yml }

        framework:
            router:   { resource: "%kernel.root_dir%/config/routing_dev.yml" }
            profiler: { only_exceptions: false }

        # ...

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->
        <imports>
            <import resource="config.xml" />
        </imports>

        <framework:config>
            <framework:router resource="%kernel.root_dir%/config/routing_dev.xml" />
            <framework:profiler only-exceptions="false" />
        </framework:config>

        <!-- ... -->

    .. code-block:: php

        // app/config/config_dev.php
        $loader->import('config.php');

        $container->loadFromExtension('framework', array(
            'router'   => array('resource' => '%kernel.root_dir%/config/routing_dev.php'),
            'profiler' => array('only-exceptions' => false),
        ));

        // ...

Klucz ``imports`` jest podobny do wyrażenia ``include`` w PHP i gwarantuje,
że główny plik konfiguracji (``config.yml``) jest ładowany jako pierwszy.
Pozostała część pliku podkręca domyślną konfigurację o zwiększone logowanie
i inne ustawienia sprzyjające środowisku deweloperskiemu.

Oba środowiska ``prod`` i ``test`` działają wg tego samego modelu: każde środowisko
importuje podstawowy plik konfiguracyjny i modyfikuje jego wartości konfiguracyjne,
aby dopasować je do potrzeb danego środowiska. To tylko konwencja, lecz pozwala to na
ponowne wykorzystanie większości Twojej konfiguracji i dostosowywania tylko niektórych jej części
pomiędzy środowiskami.

Podsumowanie
------------

Gratulacje! Poznałeś już każdy fundamentalny aspekt Symfony2 i szczęśliwie odkryłeś
jak łatwe i elastyczne może ono być. Pomimo, iż przed nami jest jeszcze *wiele* funkcji,
upewnij się, by mieć poniższe elementy podstaw w swojej pamięci:

* tworzenie strony to trzyetapowy proces dotyczący **trasy** (ang. **route**),
  **kontrolera** i (opcjonalnie) **szablonu**;

* każdy projekt zawiera kilka głównych katalogów: ``web/`` (pliki statyczne i front
  kontrolery), ``app/`` (konfiguracja), ``src/`` (Twoje bundle), oraz ``vendor/``
  (kod osób trzecich) (jest jeszcze katalog ``bin/``, który używany jest w celu aktualizacji
  bibliotek osób trzecich);

* każda funkcja w Symfony2 (włączając to rdzeń frameworka Symfony2) zorganizowana jest
  w *bundle*, który jest uporządkowanym zbiorem plików dla tej funkcji;

* **konfiguracja** każdego bundla znajduje się w katalogu ``app/config`` i może być
  określona w YAML, XML lub PHP;

* każde **środowisko** jest dostępne przez różny front kontroler (np. ``app.php`` i
  ``app_dev.php``) i wczytuje oddzielny plik konfiguracji.

Od teraz, każdy rodział będzie przedstawiał Ci coraz bardziej i bardziej potężne
narzędzia i zaawansowane koncepcje. Czym więcej wiesz o Symfony2, tym więcej docenisz
elastyczność jego architektury i moc, jaką Ci daje do szybkiego tworzenia aplikacji.

.. _`Twig`: http://twig.sensiolabs.org
.. _`third-party bundles`: http://symfony2bundles.org/
.. _`Symfony Standard Edition`: http://symfony.com/download