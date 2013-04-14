.. highlight:: php
   :linenothreshold: 2

.. index::
   single: Strony

Tworzenie stron w Symfony2
==========================

Procedura tworzenia stron w Symfony2 składa sie z dwóch etapów

  * *Utworzenie trasy*: Trasa (*ang. route*) odpowiada części adresu URL określającej
    ścieżkę dostępu do zasobu (np. ``/about``) i wyznacza też kontroler, który Symfony2
    ma wykonać, gdy ścieżka dostępu przychodzącego żądania HTTP zostanie dopasowany do
    wzorca trasy;

  * *Utworzenie kontrolera*: Kontroler jest funkcją PHP, która pobiera przychodzące
    żądanie HTTP i przekształca je w obiekt Symfony2 o nazwie *Response*, który jest
    zwracany klientowi.

To proste podejście jest piękne, gdyż pasuje do sposobu funkcjonowania internetu.
Każda interakcja w internecie jest inicjowana przez żądanie HTTP. Zadaniem aplikacji
jest interpretacja żądania i zwrócenie odpowiedzi HTTP.

Symfony2 dostosowuje się do tej filozofii i dostarcza narzędzia oraz konwencje,
które pomogaja w utrzymaniu aplikacji w miarę wzrostu liczby użytkowników
i złożoności aplikacji.

.. index::
   single: tworzenie stron; przykład

Strona "Witaj Symfony!"
-----------------------

Zacznijmy od klasycznej aplikacji "Witaj Świecie!". Kiedy skończymy, użytkownik będzie
miał możliwość ujrzeć osobiste pozdrowienie (np. "Witaj Symfony") przesyłając
następujący adres URL:

.. code-block:: text

    http://localhost/app_dev.php/hello/Symfony

Faktycznie będziesz mógł zastąpić *Symfony* jakimkolwiek inną nazwą, które ma być
użyte w pozdrowieniu. Aby utworzyć stronę, wykonamy wspomnianą już tą dwuetapową
procedurę.

.. note::

    Poradnik ten zakłada, że już pobrałeś Symfony2 i skonfigurowałeś swój serwer
    internetowy. Powyższy URL przyjmuje, że ``localhost`` wskazuje na katalog
    ``web`` Twojego nowego projektu Symfony2. Jest to tylko przykład. Szczegółowe
    informacje dotyczące konfiguracji katalogu głównego serwera znajdziesz w dokumentacji
    serwera, który używasz. Poniżej podane są odnośniki do odpowiedniej dokumentacji:
    
    * dla serwera Apache HTTP jest to dokumentacja`Apache's DirectoryIndex`_
    * dla Nginx jest to dokumentacja umiejscawiania `Nginx HttpCoreModule`_

Zanim zaczniesz: utwórz pakiet
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Zanim zaczniesz, musisz utworzyć pakiet (*ang. bundle*). W Symfony2 :term:`pakiet`
jest jak wtyczka, tyle, że cały kod aplikacji składa się z takich pakietów,
odpowiedzialnych za konkretne funkcjonalności.

Pakiet to nic innego jak katalog, który przechowuje wszystko wszystko co związane
jest z określoną funkcjonalnością, włączając w to klasy PHP, konfigurację, a nawet
arkusze stylów i pliki Javascript (zobacz :ref:`System pakietów<page-creation-bundles>`).

Aby utworzyć pakiet o nazwie ``AcmeHelloBundle`` (cwiczebny pakiet, który będziemy
budować tym rozdziale), wykonaj poniższe polecenie i postępuj ze wskazówkami
na ekranie (stosując wszystkie domyślne opcje):

.. code-block:: bash

    php app/console generate:bundle --namespace=Acme/HelloBundle --format=yml

Tworzy to katalog pakietu o lokalizacji ``src/Acme/HelloBundle``. Zostanie też
automatycznie dodana linia w pliku ``app/AppKernel.php`` rejestrujaca pakiet w jądrze::

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

Właśnie skonfigurowałeś swój pakiet, teraz możesz zacząć budowę swojej aplikacji
wewnątrz katalogu tego pakietu.

Krok 1: Utwórz trasę
~~~~~~~~~~~~~~~~~~~~

Domyślnie, plik konfiguracyjny trasowania aplikacji Symfony2 znajduje się w katalogu
``app/config/routing.yml``. Podobnie jak w całej konfiguracji Symfony2, może to być
również plik w formacie XML lub PHP.

Jeśli spojrzysz na główny plik trasowania, zobaczysz, że Symfony już dodało wpis,
podczas generowania ``AcmeHelloBundle``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/routing.yml
        AcmeHelloBundle:
            resource: "@AcmeHelloBundle/Resources/config/routing.yml"
            prefix:   /

    .. code-block:: xml
       :linenos:

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml" prefix="/" />
        </routes>

    .. code-block:: php
       :linenos:

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->addCollection(
            $loader->import('@AcmeHelloBundle/Resources/config/routing.php'),
            '/',
        );

        return $collection;

Ten wpis jest dość prosty: informuje Symfony, aby załadował konfigurację trasowania
z pliku ``Resources/config/routing.yml``, który jest umieszczony wewnątrz ``AcmeHelloBundle``.
Oznacza to, że konfiguracja trasowania jest umieszczona bezpośrednio w ``app/config/routing.yml``
lub zorganizowano trasowanie dla całej aplikacji i zaimportowno je tutaj.

Teraz gdy plik ``routing.yml`` został zaimportowany z pakietu, dodaj nową trasę
określającą ścieżkę dostępu do strony, którą właśnie tworzysz:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # src/Acme/HelloBundle/Resources/config/routing.yml
        hello:
            pattern:  /hello/{name}
            defaults: { _controller: AcmeHelloBundle:Hello:index }

    .. code-block:: xml
       :linenos:

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
       :linenos:

        // src/Acme/HelloBundle/Resources/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
        )));

        return $collection;

Trasowanie składa się z dwóch części: ``pattern``, która jest wzorcem ścieżki
dostępu porównywanym z żądaniem HTTP i tablicy ``defaults``, która określa kontroler
jaki ma być wykonany. Składnia symbolu ``{name}`` we wzorcu jest wieloznaczna.
Oznacza to, że zostaną dopasowane do wzorca trasy ścieżki takie jak ``/hello/Ryan``,
``/hello/Fabien`` i tym podobne. Parametr wieloznacznika ``{name}`` będzie również
pasował do kontrolera, dzięki czemu można używać jego wartości do indywidualizowania
powitania.

.. note::

  System trasowania ma o wiele więcej możliwości tworzenia elastycznej i rozbudowanej
  struktury ścieżek dostępu w aplikacji. W celu poznania szczegółów przeczytaj rozdział
  :doc:`Trasowanie</book/routing>`.

Krok 2: Utwórz kontroler
~~~~~~~~~~~~~~~~~~~~~~~~

Kiedy adres URL, taki jak ``/hello/Ryan``, jest obsługiwany przez aplikację dopasowywana
zostaje trasa ``hello`` i zostaje wykonany kontroler ``AcmeHelloBundle:Hello:index``.
Drugim krokiem procedury tworzenia strony jest utworzenie tego własnie kontrolera.

Łańcuch ``AcmeHelloBundle:Hello:index`` jest **logiczną nazwą** kontrolera i odwzorowuje
metodę ``indexAction`` klasy PHP o nazwie ``Acme\HelloBundle\Controller\Hello``.
Zacznij od utworzenia tego pliku wewnątrz ``AcmeHelloBundle``::

    // src/Acme/HelloBundle/Controller/HelloController.php
    namespace Acme\HelloBundle\Controller;

    use Symfony\Component\HttpFoundation\Response;

    class HelloController
    {
    }

W rzeczywistości, kontroler to nic innego jak metoda PHP, którą tworzysz a Symfony
ją wykonuje. To tutaj kod uzyskuje informację z żądania HTTP aby zbudować
i przygotować odpowiedź w postaci określonego zasobu. Z wyjątkiem kilku szczególnych
przypadków, końcowy produkt produkt kontrolera jest zawsze taki sam: obiekt
``Response`` Symfony2.

Utwórzmy metodę ``indexAction``, którą wykona Symfony, gdu zostanie dopasowana trasa
``hello``::

    // src/Acme/HelloBundle/Controller/HelloController.php

    // ...
    class HelloController
    {
        public function indexAction($name)
        {
            return new Response('<html><body>Hello '.$name.'!</body></html>');
        }
    }

Kontroler ten jest prosty: tworzy nowy obiekt ``Response``, którego pierwszym argumentem
jest treść, która będzie użyta w odpowiedzi (w tym przykładzie jest to niewielka
strona HTML).

Gratulacje! Jedynie po utworzeniu trasy i kontrolera masz już funkcjonującą stronę.
Jeżeli skonfigurowałeś wszystko prawidłowo, to bedziesz mógł wywołać swoja aplikację
w ten sposób:

.. code-block:: text

    http://localhost/app_dev.php/hello/Ryan

.. tip::

    Możesz również zobaczyć swoją aplikację w :ref:`środowisku<environments-summary>`
    "prod" (produkcyjnym) odwiedzając:

    .. code-block:: text

        http://localhost/app.php/hello/Ryan

    Jeśli otrzymujesz błąd, prawdopodobnie musisz wyczyścić pamięć podręczną za
    pomocą polecenia:

    .. code-block:: bash

        php app/console cache:clear --env=prod --no-debug

Opcjonalnie, ale zazwyczaj, wykonywany jest trzeci krok - jest nim utworzenie szablonu.

.. note::

   Kontrolery są głównym punktem wejścia do kodu oraz kluczowym składnikiem
   podczas tworzenia stron. Więcej informacji można znaleźć w rozdziale
   :doc:`Kontroler</book/controller>`,

Opcjonalny krok 3: Utwórz szablon
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Szablony umożliwiają przeniesienie całej prezentacji (czyli kodu HTML) do oddzielnego
pliku i wielokrotne wykorzystywania fragmentół układu strony. Zamiast pisać kod
HTML wewnątrz kontrolera, należy wygenerować szablon:

.. code-block:: php
   :linenos:

   // src/Acme/HelloBundle/Controller/HelloController.php
    namespace Acme\HelloBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
            return $this->render(
                'AcmeHelloBundle:Hello:index.html.twig',
                array('name' => $name)
            );

            // renderowanie szablonu PHP zamiast
            // return $this->render(
            //     'AcmeHelloBundle:Hello:index.html.php',
            //     array('name' => $name)
            // );
        }
    } 

.. note::

   Aby można używać metodę ``render()``, kontroler musi rozszerzać klasę
   ``Symfony\Bundle\FrameworkBundle\Controller\Controller`` (Dokumentacja API
   :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`),
   która dodaje skróty do typowych zadań wykonywanych wewnąrz kontrolera.
   W powyższym przykładzie zostało to wykonane przez dodanie wyrażenia ``use``
   linii 4, a następnie ``extends Controller`` w linii 6.

Metoda ``render()`` tworzy obiekt ``Response`` wypełniony zawartością przetworzonego
szablonu. Jak każdy kontroler, zwróci ona ostatecznie obiekt ``Response``.

Proszę zwrócić uwagę, że są dwa różne przykłady przetwarzania szablonów. Domyślnie
Symfony2 wspiera dwa różne języki szablonów: klasyczne szablony PHP oraz zwięzłe, lecz
potężne szablonowanie `Twig`_. Nie przejmuj się - możesz wybrać jeden, lub nawet
oba z nich w tym samym projekcie.

Kontroler renderuje szablon ``AcmeHelloBundle:Hello:index.html.twig`` używając
następującej konwencji nazewniczej:

    **NazwaPakietu**:**NazwaKontrolera**:**NazwaSzablonu**

Jest to *logiczna nazwa* szablonu odwzorowana na fizyczną lokację, zgodnie z następującą
konwencją:

    /ścieżka/do/**NazwaPakietu**/Resources/views/**NazwaKontrolera**/**NazwaSzablonu**

W tym przypadku, ``AcmeHelloBundle`` jest nazwą pakietu, ``Hello`` to kontroler, a
``index.html.twig`` jest nazwą szablonu:

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

Omówmy ten kod szablon Twig linia po linii:

* *linia 2*: Literał ``extends`` określa szablon nadrzędny. Wskazywany jest tu
  jawnie plik układu strony, w którym będzie umieszczony szablon

* *linia 4*: Literał ``block`` wskazuje, że całe wnętrze powinno zostać umieszczone
  w bloku o nazwie ``body``. Jak się później przekonasz, to szablon nadrzędny
  (*base.html.twig*) jest odpowiedzialny za ostateczne wygenerowanie bloku o nazwie
  ``body``.

Szablon nadrzędny ``::base.html.twig`` nie posiada w swojej nazwie oznaczeń
**NazwaPakietu**, jak i **NazwaKontrolera** (stąd podwójny dwukropek (``::``)
na początku). Oznacza to, że plik szablonu znajduje się na zewnątrz pakietów,
wewnątrz katalogu ``app``:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

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

Plik podstawowego szablonu okresła układ strony HTML i renderuje blok ``body``, który
został zdefiniowany w szablonie ``index.html.twig``. Ponieważ w szablonie potomnym
nie został określony blok ``title``, to domyślnie treścią tego bloku będzie "Welcome!".

Szablony są zaawamsowanym sposobem renderowania i organizowania treści dla strony.
Szablon może przetwatrzać wszystko, od znaczników HTML po kod CSS, czy cokolwiek
co może zwrócić kontroler.

W cyklu przetwarzania żądania, silnik szablonów jest po prostu dodatkowym narzędziem.
Przypomnijmy, że celem każdego kontrolera jest zwrócić obiekt ``Response``.
Szablony są silnym, ale też opcjonalnym narzędziem do tworzenia treści dla
obiektu ``Response``.

.. index::
   single: struktura katalogów

Struktura katalogów
-------------------

Po przeczytaniu tych kilku krótkich podrozdziałów zapoznałeś się ze sposobem tworzenia
i generowania stron w Symfony2. Zobaczyłeś też jak projekty Symfony2 są strukturyzowane
i organizowane. W dalszej części rozdziału poznasz gdzie umieszczać pliki i dlaczego tam.

Chociaż framework Symfony jest bardzo elastyczny, to jednak każda jego :term:`aplikacja`
ma taką samą podstawową, zalecaną strukturę katalogową:

* ``app/``: zawiera konfigurację aplikacji;

* ``src/``: w tym katalogu zawarty jest cały kod PHP projektu;

* ``vendor/``: zgodnie z konwencja, w tym katalogu umieszczane są jakiekolwiek biblioteki dostawców;

* ``web/``: jest to główny katalog internetowy, zawierający wszystkie publicznie dostępne pliki.

Katalog web
~~~~~~~~~~~

Katalog główny ``web`` jest miejscem dla wszystkich publicznych i statycznych plików,
w tym dla obrazów, arkuszy stylów i plików JavaScript. Jest to również miejsce
lokalizacji każdego :term:`kontrolera wejścia<kontroler wejścia>`::

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();

Plik kontrolera wejścia (w tym przykładzie ``app.php``) jest rzeczywistym plikiem
PHP, który jest wykonywany podczas wywołania aplikacji Symfony2, a jego zadaniem
jest wczytanie klas jądra ``AppKernel`` w celu przeprowadzenia rozruchu aplikacji.

.. tip::

    Zastosowanie kontrolera wejściowego umożliwa używania innych i bardziej elastycznych
    adresów URL, niż te w zwykłych aplikacjach ze zwykłym PHP. Przy zastosowaniu
    kontrolera wejścia adresy URL są formatowane w następujący sposób:

    .. code-block:: text

        http://localhost/app.php/hello/Ryan

    Kontroler wejścia ``app.php`` jest wykonywany również "wewnętrznie": adres URL
    ``/hello/Ryan`` jest kierowany wewnętrznie przy użyciu konfiguracji trasowania.
    Stosując regułę ``mod_rewrite`` Apache można wymusić aby plik ``app.php`` był
    wykonywany bez potrzeby specyfikowania go w adresie URL:

    .. code-block:: text

        http://localhost/hello/Ryan

Wprawdzie kontrolery wejściowe są niezbędne w obsłudze każdego żądania, to nie ma
potrzeby ich modyfikowania lub nawet zaglądania do nich. Powrócimy do tego tematu
w rozdziale ":ref:`environments-summary`".

Katalog aplikacji (app)
~~~~~~~~~~~~~~~~~~~~~~~

Jak mogłeś zobaczyć w kodzie kontrolera wejścia klasa ``AppKernel`` jest głównym
punktem wejścia do aplikacji i jest odpowiedzialna za całą konfigurację. Jako taka,
jest ona przechowywana w katalogu ``app/``.

Klasa ta musi implemetować dwie metody definiują wszystko, co Symfony potrzebuje
wiedzieć o aplikacji. Nawet nie trzeba się martwić o te metody, gdy się rozpoczyna
pracę z Symfony - kod jądra konstruuje je z rozsądnymi wartościami domyślnymi.

* ``registerBundles()``: zwraca tablicę wszystkich pakietów niezbędnych do uruchomienia
  aplikacji. Pakiet jest czymś więcej niż tylko katalogiem przechowującym wszystko
  co wiąże sie z określoną funkcjonalnością, włączając w to klasy PHP, konfiguracje
  a nawet arkusze stylów i pliki Javascript (zobacz rozdział ":ref:`page-creation-bundles`");

* ``registerContainerConfiguration()``: ładuje główny plik konfiguracji zasobów
  aplikacji (zobacz rozdział ":ref:`application-configuration`). 

W miarę zaangażowania się w programowanie aplikacji Symfony2 będziesz coraz częściej
wykorzystywał katalog ``app/`` do modyfikowania konfiguracji i plików trasowania
w katalogu ``app/config/`` (zobacz rozdział Konfiguracja aplikacji).
Katalog aplikacji zawiera również katalog pamięci podręcznej (``app/cache``),
katalog dziennika (``app/logs``) oraz katalog dla plików zasobów na poziomie
aplikacji, takich jak szablony (``app/Resources``). Dowiesz się więcej o każdym
z tych podkatalogów w dalszej części rozdziału.

.. _autoloading-introduction-sidebar:

.. sidebar:: Automatyczne ładowanie plików

    Podczas ładowania Symfony dołączany jest specjalny plik ``app/autoload.php``.
    Plik ten odpowiedzialny jest za konfigurację *autoloadera*, który będzie automatycznie
    ładował pliki aplikacji z katalogu ``src/`` oraz dodatkowe biblioteki z katalogu
    ``vendor/``.

    Dzięki autoloaderowi, nigdy nie musisz martwić się o używanie wyrażeń ``include``
    czy ``require``. Zamiast tego, Symfony2 wykorzystuje przestrzeń nazw klasy do
    określenia lokalizacji pliku i automatycznego dołączenia go, kiedy zachodzi
    potrzeba użycia danej klasy.

    Autoloader jest domyślnie skonfigurowany wyszukiwać klasy PHP w katalogu ``src/``.
    Aby to działało, nazwy klas i ścieżki do plików muszą być tworzone wg. poniższego
    wzorca:

    .. code-block:: text

        **Nazwa klasy**:
        
            Acme\HelloBundle\Controller\HelloController
        
        **Ścieżka**:
        
            src/Acme/HelloBundle/Controller/HelloController.php

    Zazwyczaj, tylko czasami trzeba się martwić o plik ``app/autoload.php``.
    Ma to miejsce, gdy dołącza się nową bibliotekę niezależnego dostawcy z katalogu
    ``vendor/``. Więcej informacji o automatycznym ładowaniu znajdziesz w artykule
    :doc:`Jak automatycznie ładować klasy </components/class_loader>`.

Katalog źródeł (``src``)
~~~~~~~~~~~~~~~~~~~~~~~~

Katalog ``src/`` zawiera po prostu cały właściwy kod (PHP, szablony, pliki
konfiguracyjne, arkusze stylów, itd.), który obsługuje aplikację.
W trakcie rozwoju projektu, zdecydowana większość pracy będzie wykonywana wewnątrz
jednego lub więcej pakietów, które zostaną utworzone w tym katalogu.

Lecz co właściwie oznacza termin :term:`pakiet`?

.. _page-creation-bundles:

System pakietów
---------------

Pakiet (*ang. bundle*) jest podobny do wtyczki w innym oprogramowaniu, ale jest
czymś lepszym. Kluczową różnicą jest to, że w Symfony wszystko jest jakimś pakietem,
włączając w to funkcjonalności rdzenia frameworka i kod napisany dla kazdej aplikacji.
Pakiety, to w Symfony2 obywatel pierwszej klasy. Daje to elastyczność w wykorzystaniu
zabudowanych funkcjonalności, pakowanych w dodatkowe pakiety lub w tworzeniu dystrybucji
własnych pakietów. Sprawia, że łatwo jest wybrać funkcjonalności udostępniane w aplikacji
i zoptymalizować je według potrzeb.

.. note::

   Podczas gdy tutaj nauczysz się podstaw, w Receptariuszu znajdziesz cały rozdział
   poświęcony organizacji i najlepszym praktykom związanym z korzystana
   z :doc:`pakietów</cookbook/bundles/best_practises>`.

Pakiet jest ustrukturyzowanym zbiorem plików wewnątrz katalogu, który
implementuje pojedynczą funkcjonalność. Możesz utworzyć ``BlogBundle``, ``ForumBundle``
czy pakiet do zarządzania użytkownikami (wiele z nich już istnieje i jest dystrubowane
jako pakiety o otwartym kodzie). Każdy katalog pakietu zawiera wszystko co związane
jest z daną funkcjonalnościa, włączając w to pliki PHP, szablony, arkusze stylów,
JavaScript, testy i całą resztę. Każdy aspekt danej funkcjonalności zawarty jest
w pakiecie, jak również każda funkcjonalność przynależy do jakiegośc pakietu.

Aplikacja składa się z pakietów zdefiniowanych w metodzie ``registerBundles()``
klasy ``AppKernel``::

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

Z pomocą metody ``registerBundles()`` ma się całkowitą kontrolę nad tym, które
pakiety będą używane przez aplikacją (włączając w to rdzenne pakiety Symfony).

.. tip::

   Pakiety mogą być zlokalizowane gdziekolwiek o ile mogą być z tego miejsca
   ładowane automatycznie (poprzez konfigurację autoloadera ``app/autoload.php``).

Tworzenie pakietu
~~~~~~~~~~~~~~~~~

Dystrybucja Symfony Standard Edition dostarczana jest z poręcznym zadaniem
tworzącym dla w pełni funkcjonalny pakiet. Własnoręczne utworzenie pakietu jest
dość proste.

Aby pokazać jakie to proste, utworzymy nowy pakiet o nazwie ``AcmeTestBundle``
i go udostępnimy.

.. tip::

    Fraza ``Acme`` to tylko atrapa. Nazwa ta powinna być zastąpiona przez nazwę
    jakiegoś "dostawcy", która wskazuje na Ciebie lub Twoją organizację (np.
    ``ABCTestBundle`` dla przedsiębiorstwa o nazwie ``ABC``).

Zaczniemy od utworzenia katalogu ``src/Acme/TestBundle/`` i dodania w nim pliku
o nazwie ``AcmeTestBundle.php``::

    // src/Acme/TestBundle/AcmeTestBundle.php
    namespace Acme\TestBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;

    class AcmeTestBundle extends Bundle
    {
    }

.. tip::

   Nazwa ``AcmeTestBundle`` zgodna jest ze standardem :ref:`konwencji nazewniczej
   pakietów<bundles-naming-conventions>`. Można również posłużyć się nazwą krótszą,
   po prostu ``TestBundle`` nazywając klasę ``TestBundle`` (a plik nazwą ``TestBundle.php``).

Ta pusta klasa jest tylko częścią tego co jest potrzebne do utworzenia pakietu.
Chociaż często nawet pusta klasa jest potrzebna i może być wykorzystana do dostosowania
zachowań pakietu.

Teraz, kiedy już został stworzony pakiet, trzeba go udostępnić za pomocą klasy ``AppKernel``::

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

Pakiet ``AcmeTestBundle`` choć jeszcze nic nie robi, jest już gotowy do użycia.

Symfony udostępnia również interfejs linii poleceń dla wygenerowania podstawowego
szkieletu pakietu:

.. code-block:: bash

    php app/console generate:bundle --namespace=Acme/TestBundle

Szkielet pakietu generowany jest wraz z podstawowym kontrolerem, szablonem oraz
zasobem trasowania, które można dostosować. Dowiesz się później więcej o narzędziu
linii poleceń Symfony2.

.. tip::

   Ilekroć tworzysz nowy pakiet lub stosujesz pakiet niezależnego dostawcy,
   zawsze upewnij się, że pakiet ten został udostępniony w ``registerBundles()``.
   Podczas użycia polecenia konsoli ``generate:bundle`` jest to robione za Ciebie.

Struktura katalogowa pakietów
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Struktura katalogowa pakietu jest prosta i elastyczna. Domyślnie system pakietów
rozumie zbiór konwencji, które pomagają utrzymać zachować spójność kodu pomiędzy
wszystkimi pakietami Symfony2. Spójrz na ``AcmeHelloBundle``, zawiera on
kilka najczęściej wykorzystywanych elementów pakietu:

* ``Controller/`` zawiera kontrolery pakietu (np. ``HelloController.php``);

* ``Resources/config/`` przechowuje konfigurację, włączając w to konfigurację
  trasowania (np. ``routing.yml``);

* ``Resources/views/`` zawiera szablony uporządkowane wg nazw kontrolerów (np.
  ``Hello/index.html.twig``);

* ``Resources/public/`` przechowuje obrazy, arkusze stylów, itd. oraz jest kopiowany
  lub dowiązywany symbolicznie do katalogu ``web/`` projektu poprzez polecenie
  konsoli ``assets:install`;

* ``Tests/`` zawiera wszystkie testy dla pakietu.

Pakiet może być mały lub duży, w zależności od implementowanej funkcjonalności.
Zawiera on tylko niezbędne pliki i nic więcej.

W miarę przemieszczania się po podręczniku, dowiesz się jak utrzymywać obiekty w
bazie danych, tworzyć i walidować formularze, tworzyć tłumaczenia swoich aplikacji,
pisać testy i wiele więcej. Każde z tych tematów ma swoje miejsce i rolę w systemie
pakietów.

.. _application-configuration:

Konfiguracja aplikacji
----------------------

Aplikacja składa się z kolekcji pakietów reprezentujących wszystkie funkcjonalności
i możliwości aplikacji. Każdy pakiet może być dostosowany poprzez pliki konfiguracyjne
napisane w formacie YAML, XML lub PHP. Główny plik konfiguracyjny domyślnie zlokalizowany
jest w katalogu ``app/config/`` i nosi nazwę albo ``config.yml``, albo ``config.xml``
albo ``config.php`` w zależności od wybranego formatu:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

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
       :linenos:

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
       :linenos:

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

   O tym jak załadować każdy plik (format) nauczysz się dokładniej w następnym
   rozdziale ":ref:`environments-summary`".

Każdy zapis najwyższego poziomu, na takim jak ``framework`` lub ``twig`` określa
konfigurację dla danego pakietu. Na przykład, klucz ``framework`` określa konfigurację
rdzenia Symfony ``FrameworkBundle`` i dołacza konfigurację dla trasowania, szablonowania
i innych systemów rdzenia.

Na razie nie martw się opcjami określonej konfiguracji w każdej sekcji. Konfiguracja
wypełniana jest wartościami domyślnymi, umożliwiającymi działanie aplikacji.
Gdy przeczytasz więcej i poznasz każdy komponet Symfony2, to nauczysz się też
określonych opcji konfiguracyjnych dla każdej funkcjonalności.

.. sidebar:: Formaty konfiguracji

    W wszystkich rozdziałach przykłady konfiguracji są wyświetlane dla wszystkich
    trzech formatów (YAML, XML i PHP). Każdy z tych formatów ma swoje zalety i wady. Wybór któregoś z nich zależy tylko od Ciebie:

    * *YAML*: prosty, przejrzysty i czytelny;

    * *XML*: czasami mocniejszy niż YAML i obsługuje autouzupełnianie środowisk IDE.

    * *PHP*: bardzo mocny, lecz mniej czytelny niż inne standardowe formaty konfiguracji.

Zrzut domyślnej konfiguracji
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.1
    W Symfony 2.1 zostało dodane polecenie ``config:dump-reference``  

Można zrzucić domyślną konfigurację pakietu w formacie yaml do konsoli używając
polecenia ``config:dump-reference``. Oto przykład zrzutu domyślnej konfiguracji
FrameworkBundle:

.. code-block:: text

    app/console config:dump-reference FrameworkBundle

Może również zostać użyty alias rozszerzenia (klucz konfiguracyjny):

.. code-block:: text

    app/console config:dump-reference framework

.. note::

    Przczytaj artykuł :doc:`How to expose a Semantic Configuration for
    a Bundle</cookbook/bundles/extension>` aby uzyskać więcej informacji o
    dodawaniu konfiguracji do pakietu.

.. index::
   single: środowiska; wprowadzenie

.. _environments-summary:

Środowiska
----------

Aplikacja może być uruchomiona w różnych środowiskach. Różne środowiska
dzielą ten sam kod PHP (oprócz kontrolera wejścia), lecz używają różnej konfiguracji.
Dla przykładu, środowisko ``dev`` będzie rejestrował ostrzeżenia i błędy, podczas gdy
środowisko ``prod`` będzie rejestrował wyłącznie błędy. Niektóre pliki są
tworzone ponownie przy każdym żądaniu w środowisku ``dev`` (dla wygody programisty),
zaś w środowisku ``prod`` są buforowane. Wszystkie środowiska działają razem na
tym samym komputerze i uruchamiają tą samą aplikację.

Projekt Symfony2 zazwyczaj rozpoczyna się z trzema środowiskami (``dev`, ``test``
i ``prod``), jednakże tworzenie nowych środowisk jest proste. Możesz łatwo przegladać
swoją aplikację w różnych środowiskach zmieniając kontroler wejścia w żądaniu HTTP.
Aby obejrzeć aplikację w środowisku ``dev``, trzeba uzyskać dostęp do aplikacji poprzez
programistyczny kontroler wejścia:

.. code-block:: text

    http://localhost/app_dev.php/hello/Ryan

Jeśli chce się zobaczyć jak aplikacja zachowa się w środowisku produkcyjnym,
trzeba wywołać zamiast tego kontroler wejścia ``prod``:

.. code-block:: text

    http://localhost/app.php/hello/Ryan

Ponieważ środowisko ``prod`` jest zoptymalizowane pod względem szybkości, to konfiguracja
trasowanie i szablony Twiga są kompilowane do postaci klas PHP i buforowane.
Podczas zmiany widoków w środowisku ``prod`` zachodzi potrzeba wyczyszczenia
pamieci podręcznej i ponownego przebudowania plików::

    php app/console cache:clear --env=prod --no-debug

.. note::

   Jeśli otworzysz plik ``web/app.php``, zobaczysz, że jest on skonfigurowany specjalnie
   do uzywania środowiska ``prod``::

       $kernel = new AppKernel('prod', false);

   Można utworzyć nowy kontroler wejścia dla nowego środowiska kopiując ten plik
   i zmieniając wartość ``prod`` na inną.

.. note::

    Środowisko ``test`` jest używane podczas uruchamiania automatycznych testów
    i nie jest dostępne bezpośrednio z przeglądarki. Przeczytaj rozdział
    :doc:`"Testowanie"</book/testing>` w celu poznania szczegółów.

.. index::
   pair: środowiska; konfiguracja

Konfiguracja środowiska
~~~~~~~~~~~~~~~~~~~~~~~

Klasa ``AppKernel`` jest odpowiedzialna za faktyczne załadowanie pliku konfiguracyjnego
wybranego środowiska::

    // app/AppKernel.php
    public function registerContainerConfiguration(LoaderInterface $loader)
    {
        $loader->load(__DIR__.'/config/config_'.$this->getEnvironment().'.yml');
    }

Wiesz już, że rozszerzenie ``.yml`` może być zamienione na ``.xml`` lub ``.php``,
jeśli preferuje się używanie HML lub PHP do tworzenia konfiguracji.
Proszę też zwrócić uwagę, że każde środowisko ładuje swój własny plik konfiguracyjny.
Przyjrzyjmy się plikowi konfiguracyjnemu środowiska ``dev``.

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config_dev.yml
        imports:
            - { resource: config.yml }

        framework:
            router:   { resource: "%kernel.root_dir%/config/routing_dev.yml" }
            profiler: { only_exceptions: false }

        # ...

    .. code-block:: xml
       :linenos:

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
       :linenos:

        // app/config/config_dev.php
        $loader->import('config.php');

        $container->loadFromExtension('framework', array(
            'router'   => array('resource' => '%kernel.root_dir%/config/routing_dev.php'),
            'profiler' => array('only-exceptions' => false),
        ));

        // ...

Klucz ``imports`` jest podobny do wyrażenia ``include`` w PHP i gwarantuje,
że główny plik konfiguracji (``config.yml``) jest ładowany jako pierwszy.
Reszta pliku dostosowuje konfigurację poprzez zapisy z wcięciem zawierające
ustawienia sprzyjające środowisku programistycznemu.

Oba środowiska ``prod`` i ``test`` działają wg tego samego wzorca: każde środowisko
importuje podstawowy plik konfiguracyjny i modyfikuje jego wartości konfiguracyjne,
aby dopasować je do potrzeb danego środowiska. To tylko konwencja, lecz pozwala na
ponowne wykorzystanie większości zapisów konfiguracji i dostosowywania tylko niektórych
jej części pomiędzy środowiskami.

Podsumowanie
------------

Gratulacje! Zapoznałeś się z każdym podstawowym aspektem Symfony2 i mamy nadzieję,
że odkryłeś, jak jest on łatwy i elastyczny. Choć istnieje jeszcze wiele tematów
do omówienia, należy tu zapamietać następujące kwestie:

* tworzenie strony jest trzyetapowym procesem obejmujacym stworzenie **trasy**,
  **kontrolera** i (opcjonalnie) **szablonu**;

* każdy projekt zawiera kilka głównych katalogów: ``web/`` (pliki statyczne i kontroler
  wejścia), ``app/`` (konfiguracja), ``src/`` (Twoje pakiety), oraz ``vendor/``
  (kod osób trzecich); jest tam jeszcze katalog ``bin/``, który używany jest do
  aktualizacji bibliotek dostawców);

* każda funkcjonalność w Symfony2 (włączając to rdzeń frameworka Symfony2) zorganizowana
  jest w **pakiet**, który jest uporządkowanym zbiorem plików dla tej funkcjonalności;

* **plik konfiguracykny** każdego pakietu znajduje się w katalogu ``app/config``
  i może mieć format YAML, XML lub PHP;

* każde **środowisko** jest dostępne przez kontroler wejścia (np. ``app.php`` i
  ``app_dev.php``) i wczytuje oddzielny plik konfiguracji.

Z tego miejsca, kazdy następny rozdział wprowadzi Cie w coraz to bardziej zaawansowane
tematy. Im więcej będziesz wiedział o Symfony2, to tym bardziej będziesz doceniał
elastyczność jego architektury i możliwości szybkiego tworzenia aplikacji.

.. _`Twig`: http://twig.sensiolabs.org
.. _`third-party bundles`: http://symfony2bundles.org/
.. _`Symfony Standard Edition`: http://symfony.com/download
.. _`Apache's DirectoryIndex`: http://httpd.apache.org/docs/2.0/mod/mod_dir.html
.. _`Nginx HttpCoreModule`: http://wiki.nginx.org/HttpCoreModule#location