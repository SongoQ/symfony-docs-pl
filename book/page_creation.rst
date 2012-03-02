.. index::
   single: Page creation

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
   single: Page creation; Example

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

Templates are a powerful way to render and organize the content for your
page. A template can render anything, from HTML markup, to CSS code, or anything
else that the controller may need to return.

In the lifecycle of handling a request, the templating engine is simply
an optional tool. Recall that the goal of each controller is to return a
``Response`` object. Templates are a powerful, but optional, tool for creating
the content for that ``Response`` object.

.. index::
   single: Directory Structure

The Directory Structure
-----------------------

After just a few short sections, you already understand the philosophy behind
creating and rendering pages in Symfony2. You've also already begun to see
how Symfony2 projects are structured and organized. By the end of this section,
you'll know where to find and put different types of files and why.

Though entirely flexible, by default, each Symfony :term:`application` has
the same basic and recommended directory structure:

* ``app/``: This directory contains the application configuration;

* ``src/``: All the project PHP code is stored under this directory;

* ``vendor/``: Any vendor libraries are placed here by convention;

* ``web/``: This is the web root directory and contains any publicly accessible files;

The Web Directory
~~~~~~~~~~~~~~~~~

The web root directory is the home of all public and static files including
images, stylesheets, and JavaScript files. It is also where each
:term:`front controller` lives::

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();

The front controller file (``app.php`` in this example) is the actual PHP
file that's executed when using a Symfony2 application and its job is to
use a Kernel class, ``AppKernel``, to bootstrap the application.

.. tip::

    Having a front controller means different and more flexible URLs than
    are used in a typical flat PHP application. When using a front controller,
    URLs are formatted in the following way:

    .. code-block:: text

        http://localhost/app.php/hello/Ryan

    The front controller, ``app.php``, is executed and the "internal:" URL
    ``/hello/Ryan`` is routed internally using the routing configuration.
    By using Apache ``mod_rewrite`` rules, you can force the ``app.php`` file
    to be executed without needing to specify it in the URL:

    .. code-block:: text

        http://localhost/hello/Ryan

Though front controllers are essential in handling every request, you'll
rarely need to modify or even think about them. We'll mention them again
briefly in the `Environments`_ section.

The Application (``app``) Directory
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

As you saw in the front controller, the ``AppKernel`` class is the main entry
point of the application and is responsible for all configuration. As such,
it is stored in the ``app/`` directory.

This class must implement two methods that define everything that Symfony
needs to know about your application. You don't even need to worry about
these methods when starting - Symfony fills them in for you with sensible
defaults.

* ``registerBundles()``: Returns an array of all bundles needed to run the
  application (see :ref:`page-creation-bundles`);

* ``registerContainerConfiguration()``: Loads the main application configuration
  resource file (see the `Application Configuration`_ section).

In day-to-day development, you'll mostly use the ``app/`` directory to modify
configuration and routing files in the ``app/config/`` directory (see
`Application Configuration`_). It also contains the application cache
directory (``app/cache``), a log directory (``app/logs``) and a directory
for application-level resource files, such as templates (``app/Resources``).
You'll learn more about each of these directories in later chapters.

.. _autoloading-introduction-sidebar:

.. sidebar:: Autoloading

    When Symfony is loading, a special file - ``app/autoload.php`` - is included.
    This file is responsible for configuring the autoloader, which will autoload
    your application files from the ``src/`` directory and third-party libraries
    from the ``vendor/`` directory.

    Because of the autoloader, you never need to worry about using ``include``
    or ``require`` statements. Instead, Symfony2 uses the namespace of a class
    to determine its location and automatically includes the file on your
    behalf the instant you need a class.

    The autoloader is already configured to look in the ``src/`` directory
    for any of your PHP classes. For autoloading to work, the class name and
    path to the file have to follow the same pattern:

    .. code-block:: text

        Class Name:
            Acme\HelloBundle\Controller\HelloController
        Path:
            src/Acme/HelloBundle/Controller/HelloController.php

    Typically, the only time you'll need to worry about the ``app/autoload.php``
    file is when you're including a new third-party library in the ``vendor/``
    directory. For more information on autoloading, see
    :doc:`How to autoload Classes</components/class_loader>`.

The Source (``src``) Directory
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Put simply, the ``src/`` directory contains all of the actual code (PHP code,
templates, configuration files, stylesheets, etc) that drives *your* application.
When developing, the vast majority of your work will be done inside one or
more bundles that you create in this directory.

But what exactly is a :term:`bundle`?

.. _page-creation-bundles:

The Bundle System
-----------------

A bundle is similar to a plugin in other software, but even better. The key
difference is that *everything* is a bundle in Symfony2, including both the
core framework functionality and the code written for your application.
Bundles are first-class citizens in Symfony2. This gives you the flexibility
to use pre-built features packaged in `third-party bundles`_ or to distribute
your own bundles. It makes it easy to pick and choose which features to enable
in your application and to optimize them the way you want.

.. note::

   While you'll learn the basics here, an entire cookbook entry is devoted
   to the organization and best practices of :doc:`bundles</cookbook/bundles/best_practices>`.

A bundle is simply a structured set of files within a directory that implement
a single feature. You might create a ``BlogBundle``, a ``ForumBundle`` or
a bundle for user management (many of these exist already as open source
bundles). Each directory contains everything related to that feature, including
PHP files, templates, stylesheets, JavaScripts, tests and anything else.
Every aspect of a feature exists in a bundle and every feature lives in a
bundle.

An application is made up of bundles as defined in the ``registerBundles()``
method of the ``AppKernel`` class::

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

With the ``registerBundles()`` method, you have total control over which bundles
are used by your application (including the core Symfony bundles).

.. tip::

   A bundle can live *anywhere* as long as it can be autoloaded (via the
   autoloader configured at ``app/autoload.php``).

Creating a Bundle
~~~~~~~~~~~~~~~~~

The Symfony Standard Edition comes with a handy task that creates a fully-functional
bundle for you. Of course, creating a bundle by hand is pretty easy as well.

To show you how simple the bundle system is, create a new bundle called
``AcmeTestBundle`` and enable it.

.. tip::

    The ``Acme`` portion is just a dummy name that should be replaced by
    some "vendor" name that represents you or your organization (e.g. ``ABCTestBundle``
    for some company named ``ABC``).

Start by creating a ``src/Acme/TestBundle/`` directory and adding a new file
called ``AcmeTestBundle.php``::

    // src/Acme/TestBundle/AcmeTestBundle.php
    namespace Acme\TestBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;

    class AcmeTestBundle extends Bundle
    {
    }

.. tip::

   The name ``AcmeTestBundle`` follows the standard :ref:`Bundle naming conventions<bundles-naming-conventions>`.
   You could also choose to shorten the name of the bundle to simply ``TestBundle``
   by naming this class ``TestBundle`` (and naming the file ``TestBundle.php``).

This empty class is the only piece you need to create the new bundle. Though
commonly empty, this class is powerful and can be used to customize the behavior
of the bundle.

Now that you've created the bundle, enable it via the ``AppKernel`` class::

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

And while it doesn't do anything yet, ``AcmeTestBundle`` is now ready to
be used.

And as easy as this is, Symfony also provides a command-line interface for
generating a basic bundle skeleton:

.. code-block:: bash

    php app/console generate:bundle --namespace=Acme/TestBundle

The bundle skeleton generates with a basic controller, template and routing
resource that can be customized. You'll learn more about Symfony2's command-line
tools later.

.. tip::

   Whenever creating a new bundle or using a third-party bundle, always make
   sure the bundle has been enabled in ``registerBundles()``. When using
   the ``generate:bundle`` command, this is done for you.

Bundle Directory Structure
~~~~~~~~~~~~~~~~~~~~~~~~~~

The directory structure of a bundle is simple and flexible. By default, the
bundle system follows a set of conventions that help to keep code consistent
between all Symfony2 bundles. Take a look at ``AcmeHelloBundle``, as it contains
some of the most common elements of a bundle:

* ``Controller/`` contains the controllers of the bundle (e.g. ``HelloController.php``);

* ``Resources/config/`` houses configuration, including routing configuration
  (e.g. ``routing.yml``);

* ``Resources/views/`` holds templates organized by controller name (e.g.
  ``Hello/index.html.twig``);

* ``Resources/public/`` contains web assets (images, stylesheets, etc) and is
  copied or symbolically linked into the project ``web/`` directory via
  the ``assets:install`` console command;

* ``Tests/`` holds all tests for the bundle.

A bundle can be as small or large as the feature it implements. It contains
only the files you need and nothing else.

As you move through the book, you'll learn how to persist objects to a database,
create and validate forms, create translations for your application, write
tests and much more. Each of these has their own place and role within the
bundle.

Application Configuration
-------------------------

An application consists of a collection of bundles representing all of the
features and capabilities of your application. Each bundle can be customized
via configuration files written in YAML, XML or PHP. By default, the main
configuration file lives in the ``app/config/`` directory and is called
either ``config.yml``, ``config.xml`` or ``config.php`` depending on which
format you prefer:

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

   You'll learn exactly how to load each file/format in the next section
   `Environments`_.

Each top-level entry like ``framework`` or ``twig`` defines the configuration
for a particular bundle. For example, the ``framework`` key defines the configuration
for the core Symfony ``FrameworkBundle`` and includes configuration for the
routing, templating, and other core systems.

For now, don't worry about the specific configuration options in each section.
The configuration file ships with sensible defaults. As you read more and
explore each part of Symfony2, you'll learn about the specific configuration
options of each feature.

.. sidebar:: Configuration Formats

    Throughout the chapters, all configuration examples will be shown in all
    three formats (YAML, XML and PHP). Each has its own advantages and
    disadvantages. The choice of which to use is up to you:

    * *YAML*: Simple, clean and readable;

    * *XML*: More powerful than YAML at times and supports IDE autocompletion;

    * *PHP*: Very powerful but less readable than standard configuration formats.

Default Configuration Dump
~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.1
    The ``config:dump-reference`` command was added in Symfony 2.1

You can dump the default configuration for a bundle in yaml to the console using
the ``config:dump-reference`` command.  Here is an example of dumping the default
FrameworkBundle configuration:

.. code-block:: text

    app/console config:dump-reference FrameworkBundle

.. note::

    See the cookbook article: :doc:`How to expose a Semantic Configuration for
    a Bundle</cookbook/bundles/extension>` for information on adding
    configuration for your own bundle.

.. index::
   single: Environments; Introduction

.. _environments-summary:

Environments
------------

An application can run in various environments. The different environments
share the same PHP code (apart from the front controller), but use different
configuration. For instance, a ``dev`` environment will log warnings and
errors, while a ``prod`` environment will only log errors. Some files are
rebuilt on each request in the ``dev`` environment (for the developer's convenience),
but cached in the ``prod`` environment. All environments live together on
the same machine and execute the same application.

A Symfony2 project generally begins with three environments (``dev``, ``test``
and ``prod``), though creating new environments is easy. You can view your
application in different environments simply by changing the front controller
in your browser. To see the application in the ``dev`` environment, access
the application via the development front controller:

.. code-block:: text

    http://localhost/app_dev.php/hello/Ryan

If you'd like to see how your application will behave in the production environment,
call the ``prod`` front controller instead:

.. code-block:: text

    http://localhost/app.php/hello/Ryan

Since the ``prod`` environment is optimized for speed; the configuration,
routing and Twig templates are compiled into flat PHP classes and cached.
When viewing changes in the ``prod`` environment, you'll need to clear these
cached files and allow them to rebuild::

    php app/console cache:clear --env=prod --no-debug

.. note::

   If you open the ``web/app.php`` file, you'll find that it's configured explicitly
   to use the ``prod`` environment::

       $kernel = new AppKernel('prod', false);

   You can create a new front controller for a new environment by copying
   this file and changing ``prod`` to some other value.

.. note::

    The ``test`` environment is used when running automated tests and cannot
    be accessed directly through the browser. See the :doc:`testing chapter</book/testing>`
    for more details.

.. index::
   single: Environments; Configuration

Environment Configuration
~~~~~~~~~~~~~~~~~~~~~~~~~

The ``AppKernel`` class is responsible for actually loading the configuration
file of your choice::

    // app/AppKernel.php
    public function registerContainerConfiguration(LoaderInterface $loader)
    {
        $loader->load(__DIR__.'/config/config_'.$this->getEnvironment().'.yml');
    }

You already know that the ``.yml`` extension can be changed to ``.xml`` or
``.php`` if you prefer to use either XML or PHP to write your configuration.
Notice also that each environment loads its own configuration file. Consider
the configuration file for the ``dev`` environment.

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

The ``imports`` key is similar to a PHP ``include`` statement and guarantees
that the main configuration file (``config.yml``) is loaded first. The rest
of the file tweaks the default configuration for increased logging and other
settings conducive to a development environment.

Both the ``prod`` and ``test`` environments follow the same model: each environment
imports the base configuration file and then modifies its configuration values
to fit the needs of the specific environment. This is just a convention,
but one that allows you to reuse most of your configuration and customize
just pieces of it between environments.

Summary
-------

Congratulations! You've now seen every fundamental aspect of Symfony2 and have
hopefully discovered how easy and flexible it can be. And while there are
*a lot* of features still to come, be sure to keep the following basic points
in mind:

* creating a page is a three-step process involving a **route**, a **controller**
  and (optionally) a **template**.

* each project contains just a few main directories: ``web/`` (web assets and
  the front controllers), ``app/`` (configuration), ``src/`` (your bundles),
  and ``vendor/`` (third-party code) (there's also a ``bin/`` directory that's
  used to help updated vendor libraries);

* each feature in Symfony2 (including the Symfony2 framework core) is organized
  into a *bundle*, which is a structured set of files for that feature;

* the **configuration** for each bundle lives in the ``app/config`` directory
  and can be specified in YAML, XML or PHP;

* each **environment** is accessible via a different front controller (e.g.
  ``app.php`` and ``app_dev.php``) and loads a different configuration file.

From here, each chapter will introduce you to more and more powerful tools
and advanced concepts. The more you know about Symfony2, the more you'll
appreciate the flexibility of its architecture and the power it gives you
to rapidly develop applications.

.. _`Twig`: http://twig.sensiolabs.org
.. _`third-party bundles`: http://symfony2bundles.org/
.. _`Symfony Standard Edition`: http://symfony.com/download