.. index::
   single: Routing

Routing
=======

Piękne URLe to absolutna konieczność dla każdej poważnej aplikacji internetowej.
Oznacza to porzucenie brzydkich adresów jak ``index.php?article_id=57``
na rzecz czegoś takiego jak ``/read/intro-to-symfony``.

Jeszcze ważniejsza jest elastyczność. Co, jeśli musisz zmienić URL strony
z ``/blog`` na ``/news``? Jak dużo linków musiałbyś odnaleźć i poprawić,
aby dokonać takiej zmiany? Jeśli używasz routera Symfony, taka zmiana jest
banalnie prosta.

Router Symfony2 pozwala Ci definiować kreatywne URLe, które prowadzą do różnych
miejsc w Twojej aplikacji. Na końcu tego rozdziału będziesz umiał:

* Tworzyć kompleksowe trasy mapujące do kontrolerów
* Generować URLe wewnątrz szablonów i kontrolerów
* Wczytywać zasoby routingu z bundli (lub skądkolwiek indziej)
* Debugować swoje trasy

.. index::
   single: Routing; Podstawy

Routing w akcji
---------------

*Trasa* łączy wzorzec URL z kontrolerem. Na przykład, załóżmy, że chcesz dopasować
URL jak ``/blog/moj-post`` czy ``/blog/wszystko-o-symfony`` i wysłać go do kontrolera,
który może odnaleźć i wyświetlić dany wpis bloga. Ustawienie trasy jest banalne::

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        blog_show:
            pattern:   /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog_show" pattern="/blog/{slug}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog_show', new Route('/blog/{slug}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $collection;

Wzorzec zdefiniowany w trasie ``blog_show`` działa jak ``/blog/*``, gdzie
wieloznaczny jest parametr ``slug``. Dla adresu URL ``/blog/moj-post`` zmienna
``slug`` przybierze wartość ``moj-post``, która jest dostępna dla Ciebie z poziomu
kontrolera (czytaj dalej).

Parametr ``_controller`` jest specjalną wartością, która mówi Symfony który kontroler
powinien być uruchomiony, kiedy URL zostanie dopasowany do tej trasy. Ciąg ``_controller``
jest nazywany :ref:`logical name<controller-string-syntax>`. Stosuje on wzorzec, który
określa konkretną klasę PHP oraz metodę:

.. code-block:: php

    // src/Acme/BlogBundle/Controller/BlogController.php

    namespace Acme\BlogBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class BlogController extends Controller
    {
        public function showAction($slug)
        {
            $blog = // use the $slug variable to query the database

            return $this->render('AcmeBlogBundle:Blog:show.html.twig', array(
                'blog' => $blog,
            ));
        }
    }

Gratulacje! Właśnie utworzyłeś swoją pierwszą trasę i połączyłeś ją z kontrolerem.
Teraz, kiedy odwiedzisz ``/blog/moj-post``, zostanie uruchomiony kontroler ``showAction``,
a zmienna ``$slug`` przyjmie wartość ``moj-post``.

To jest właśnie zadanie routera Symfony2: zmapować URL żądania do kontrolera.
Po drodze nauczysz się wielu trików, które sprawiają, że mapowanie nawet najbardziej
złożonych URLi staje się łatwe.

.. versionadded:: 2.1

    W Symfony2.1, komponent Routing obsługuje również wartości Unicode, takie
    jak np.: /Жени/

.. index::
   single: Routing; Pod maską

Routing: Pod maską
-----------------------

Kiedy do Twojej aplikacji wysłane jest żądanie, zawiera ono adres do dokładnego
"zasobu", który klient żąda. Ten adres nazywany jest URL (lub URI) i może być
nim ``/kontakt``, ``/blog/informacje`` lub cokolwiek innego. Weźmy za przykład
poniższe żądanie HTTP:

.. code-block:: text

    GET /blog/moj-post

Zadaniem systemu routingu Symfony2 jest parsować ten URL i określić, który
kontroler powinien zostać uruchomiony. Cały proces wygląda mniej więcej tak:

#. Żądanie jest przetwarzane przez front kontroler Symfony2 (np. ``app.php``);

#. Rdzeń Symfony2 (np. Kernel) odpytuje router dla danego żądania;

#. Router dopasowuje przychodzący URL do konkretnej trasy i zwraca informacje o tej właśnie
   trasie, włączając w to kontroler, który powinien zostać uruchomiony;

#. Kernel (Jądro) Symfony2 uruchamia kontroler, który ostatecznie zwraca obiekt
   ``Response``.

.. figure:: /images/request-flow.png
   :align: center
   :alt: Przepływ żądania Symfony2

   Warstwa routingu jest narzędziem, które tłumaczy przychodzące URLe na określony
   kontroler do wykonania.

.. index::
   single: Routing; Tworzenie tras

Tworzenie tras
--------------

Symfony wczytuje wszystkie trasy dla Twojej aplikacji z pojedynczego pliku konfiguracyjnego
routingu. Ten plik to zazwyczaj ``app/config/routing.yml``, jednakże może on być skonfigurowany
w dowolny sposób (włączając w to plik XML czy PHP) przez plik konfiguracyjny aplikacji:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            router:        { resource: "%kernel.root_dir%/config/routing.yml" }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config ...>
            <!-- ... -->
            <framework:router resource="%kernel.root_dir%/config/routing.xml" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'router'        => array('resource' => '%kernel.root_dir%/config/routing.php'),
        ));

.. tip::

    Nawet, jeśli wszystkie trasy są wczytywane z pojedynczego pliku, dobrą praktyką
    jest dołączać dodatkowe dane routingu z innych plików. Zobacz :ref:`routing-include-external-resources`
    w celu uzyskania więcej informacji.

Podstawowa konfiguracja trasy
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Definiowanie tras jest proste, a typowa aplikacja będzie posiadała mnóstwo tras.
Podstawowa trasa składa się z dwóch części: ``wzoru`` do dopasowania oraz z tablicy
``defaults`` przechowującej wartości domyślne:

.. configuration-block::

    .. code-block:: yaml

        _welcome:
            pattern:   /
            defaults:  { _controller: AcmeDemoBundle:Main:homepage }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="_welcome" pattern="/">
                <default key="_controller">AcmeDemoBundle:Main:homepage</default>
            </route>

        </routes>

    ..  code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('_welcome', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Main:homepage',
        )));

        return $collection;

Ta trasa pasuje do strony głównej (``/``) i prowadzi do kontrolera ``AcmeDemoBundle:Main:homepage``.
Ciąg ``_controller`` jest zamieniany na nazwę odpowiedniej funkcji PHP, która następnie
zostaje uruchomiona. Ten proces będzie pokrótce wyjaśniony w sekcji :ref:`controller-string-syntax`.

.. index::
   single: Routing; Parametry

Routing z parametrami
~~~~~~~~~~~~~~~~~~~~~

Oczywiście system routingu wspiera o wiele więcej ciekawych tras. Wiele
z nich będzie posiadało jeden lub więcej parametrów (symboli zastępczych, ang. placeholder) jako
"wieloznacznik" (ang. wildcard):

.. configuration-block::

    .. code-block:: yaml

        blog_show:
            pattern:   /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog_show" pattern="/blog/{slug}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog_show', new Route('/blog/{slug}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $collection;

Wzorzec będzie pasował do wszystkiego, co wygląda jak ``/blog/*``. Co więcej,
wartość przypisana do parametru ``{slug}`` będzie dostępna wewnątrz Twojego
kontrolera. Innymi słowy, jeśli URL wygląda tak: ``/blog/hello-world``,
to zmienna ``$slug`` z wartością ``hello-world`` będzie dostępna dla kontrolera.
Może być to użyte np. do pobrania wpisu bloga, który pasuje do tego ciągu.

Ten wzorzec jednakże *nie* będzie pasował do samego ``/blog``. Dzieje się tak,
ponieważ domyślnie wszystkie parametry są wymagane. Może być to zmienione poprzez
dodanie domyślnej wartości tego parametru do tablicy ``defaults``.

Wymagane oraz opcjonalne parametry
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Aby uczynić to bardziej ekscytującym, dodaj nową trasę, która wyświetla
listę wszystkich dostępnych wpisów bloga:

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog
            defaults:  { _controller: AcmeBlogBundle:Blog:index }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
        )));

        return $collection;

Jak dotąd, ta trasa jest tak prosta, jak to tylko możliwe - nie zawiera
żadnych placeholderów i pasuje tylko do jednego URL ``/blog``. Ale co, jeśli
chcesz, aby ta trasa obsługiwała paginację, gdzie ``/blog/2`` wyświetla drugą
stronę wpisów bloga? Zmień tą trasę, aby posiadała nowy parameter ``{page}``:

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
        )));

        return $collection;

Podobnie jak poprzedni parameter ``{slug``, wartość pasująca do ``{page}``
będzie dostępna dla Twojego kontrolera. Ta wartość może być użyta do określenia,
którą część postów bloga wyświetlić dla danej strony.

Ale chwileczkę! Ponieważ parametry są domyślnie wymagane, ta trasa już nie będzie
pasować do adresu ``/blog``. Ponadto, aby zobaczyć stronę 1 bloga, musisz
wejść pod URL ``/blog/1``! Jako, że nie jest to właściwe zachowanie
dla aplikacji internetowej, zmodyfikuj trasę tak, aby parametr ``{page}``
był opcjonalny. Można tego dokonać dołączając kolekcję ``defaults``:

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="page">1</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'page' => 1,
        )));

        return $collection;

Po dodaniu ``page`` do tablicy ``defaults``, wieloznacznik ``{page}`` już nie jest
wymagany. URL ``/blog`` będzie teraz pasował do tej trasy, a wartość parametru
``page`` będzie ustawiona na ``1``. URL ``/blog/2`` również będzie pasować,
dając parametrowi ``page`` wartość ``2``. Idelanie.

+---------+------------+
| /blog   | {page} = 1 |
+---------+------------+
| /blog/1 | {page} = 1 |
+---------+------------+
| /blog/2 | {page} = 2 |
+---------+------------+

.. index::
   single: Routing; Wymagania

Dodawanie wymagań
~~~~~~~~~~~~~~~~~

Spójrz na utworzone przez nas wcześniej trasy:

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }

        blog_show:
            pattern:   /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="page">1</default>
            </route>

            <route id="blog_show" pattern="/blog/{slug}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'page' => 1,
        )));

        $collection->add('blog_show', new Route('/blog/{show}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $collection;

Widzisz problem? Zauważ, że obie trasy mają wzory, do których pasują adresy
URL takie jak ``/blog/*``. Router Symfony2 zawsze będzie wybierał **pierwszą**
trasę, którą znajdzie. Innymi słowy, trasa ``blog_show`` nigdy nie będzie pasować.
Ponadto, URL jak ``/blog/my-blog-post`` będzie pasował do pierwszej trasy
(``blog``) i zwracał bezsensowną wartość ``my-blog-post`` do parametru ``{page}``.

+--------------------+-------+-----------------------+
| URL                | route | parameters            |
+====================+=======+=======================+
| /blog/2            | blog  | {page} = 2            |
+--------------------+-------+-----------------------+
| /blog/my-blog-post | blog  | {page} = my-blog-post |
+--------------------+-------+-----------------------+

Rozwiązaniem tego problemu jest dodanie do trasy *wymagań*. Trasy w tym przypadku
będą działały idealnie, jeśli wzorzec ``/blog/{page}`` będzie pasował *wyłącznie*
do URLi, w których parameter ``{page}`` jest typu integer. Na szczęście,
wymagania w postaci wyrażeń regularnych mogą być łatwo dodane dla każdego parametru.
Na przykład:

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }
            requirements:
                page:  \d+

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="page">1</default>
                <requirement key="page">\d+</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'page' => 1,
        ), array(
            'page' => '\d+',
        )));

        return $collection;

The ``\d+`` requirement is a regular expression that says that the value of
the ``{page}`` parameter must be a digit (i.e. a number). The ``blog`` route
will still match on a URL like ``/blog/2`` (because 2 is a number), but it
will no longer match a URL like ``/blog/my-blog-post`` (because ``my-blog-post``
is *not* a number).

As a result, a URL like ``/blog/my-blog-post`` will now properly match the
``blog_show`` route.

+--------------------+-----------+-----------------------+
| URL                | route     | parameters            |
+====================+===========+=======================+
| /blog/2            | blog      | {page} = 2            |
+--------------------+-----------+-----------------------+
| /blog/my-blog-post | blog_show | {slug} = my-blog-post |
+--------------------+-----------+-----------------------+

.. sidebar:: Earlier Routes always Win

    What this all means is that the order of the routes is very important.
    If the ``blog_show`` route were placed above the ``blog`` route, the
    URL ``/blog/2`` would match ``blog_show`` instead of ``blog`` since the
    ``{slug}`` parameter of ``blog_show`` has no requirements. By using proper
    ordering and clever requirements, you can accomplish just about anything.

Since the parameter requirements are regular expressions, the complexity
and flexibility of each requirement is entirely up to you. Suppose the homepage
of your application is available in two different languages, based on the
URL:

.. configuration-block::

    .. code-block:: yaml

        homepage:
            pattern:   /{culture}
            defaults:  { _controller: AcmeDemoBundle:Main:homepage, culture: en }
            requirements:
                culture:  en|fr

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="homepage" pattern="/{culture}">
                <default key="_controller">AcmeDemoBundle:Main:homepage</default>
                <default key="culture">en</default>
                <requirement key="culture">en|fr</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('homepage', new Route('/{culture}', array(
            '_controller' => 'AcmeDemoBundle:Main:homepage',
            'culture' => 'en',
        ), array(
            'culture' => 'en|fr',
        )));

        return $collection;

For incoming requests, the ``{culture}`` portion of the URL is matched against
the regular expression ``(en|fr)``.

+-----+--------------------------+
| /   | {culture} = en           |
+-----+--------------------------+
| /en | {culture} = en           |
+-----+--------------------------+
| /fr | {culture} = fr           |
+-----+--------------------------+
| /es | *won't match this route* |
+-----+--------------------------+

.. index::
   single: Routing; Method requirement

Adding HTTP Method Requirements
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In addition to the URL, you can also match on the *method* of the incoming
request (i.e. GET, HEAD, POST, PUT, DELETE). Suppose you have a contact form
with two controllers - one for displaying the form (on a GET request) and one
for processing the form when it's submitted (on a POST request). This can
be accomplished with the following route configuration:

.. configuration-block::

    .. code-block:: yaml

        contact:
            pattern:  /contact
            defaults: { _controller: AcmeDemoBundle:Main:contact }
            requirements:
                _method:  GET

        contact_process:
            pattern:  /contact
            defaults: { _controller: AcmeDemoBundle:Main:contactProcess }
            requirements:
                _method:  POST

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="contact" pattern="/contact">
                <default key="_controller">AcmeDemoBundle:Main:contact</default>
                <requirement key="_method">GET</requirement>
            </route>

            <route id="contact_process" pattern="/contact">
                <default key="_controller">AcmeDemoBundle:Main:contactProcess</default>
                <requirement key="_method">POST</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('contact', new Route('/contact', array(
            '_controller' => 'AcmeDemoBundle:Main:contact',
        ), array(
            '_method' => 'GET',
        )));

        $collection->add('contact_process', new Route('/contact', array(
            '_controller' => 'AcmeDemoBundle:Main:contactProcess',
        ), array(
            '_method' => 'POST',
        )));

        return $collection;

Despite the fact that these two routes have identical patterns (``/contact``),
the first route will match only GET requests and the second route will match
only POST requests. This means that you can display the form and submit the
form via the same URL, while using distinct controllers for the two actions.

.. note::
    If no ``_method`` requirement is specified, the route will match on
    *all* methods.

Like the other requirements, the ``_method`` requirement is parsed as a regular
expression. To match ``GET`` *or* ``POST`` requests, you can use ``GET|POST``.

.. index::
   single: Routing; Advanced example
   single: Routing; _format parameter

.. _advanced-routing-example:

Advanced Routing Example
~~~~~~~~~~~~~~~~~~~~~~~~

At this point, you have everything you need to create a powerful routing
structure in Symfony. The following is an example of just how flexible the
routing system can be:

.. configuration-block::

    .. code-block:: yaml

        article_show:
          pattern:  /articles/{culture}/{year}/{title}.{_format}
          defaults: { _controller: AcmeDemoBundle:Article:show, _format: html }
          requirements:
              culture:  en|fr
              _format:  html|rss
              year:     \d+

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="article_show" pattern="/articles/{culture}/{year}/{title}.{_format}">
                <default key="_controller">AcmeDemoBundle:Article:show</default>
                <default key="_format">html</default>
                <requirement key="culture">en|fr</requirement>
                <requirement key="_format">html|rss</requirement>
                <requirement key="year">\d+</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('homepage', new Route('/articles/{culture}/{year}/{title}.{_format}', array(
            '_controller' => 'AcmeDemoBundle:Article:show',
            '_format' => 'html',
        ), array(
            'culture' => 'en|fr',
            '_format' => 'html|rss',
            'year' => '\d+',
        )));

        return $collection;

As you've seen, this route will only match if the ``{culture}`` portion of
the URL is either ``en`` or ``fr`` and if the ``{year}`` is a number. This
route also shows how you can use a period between placeholders instead of
a slash. URLs matching this route might look like:

* ``/articles/en/2010/my-post``
* ``/articles/fr/2010/my-post.rss``

.. _book-routing-format-param:

.. sidebar:: The Special ``_format`` Routing Parameter

    This example also highlights the special ``_format`` routing parameter.
    When using this parameter, the matched value becomes the "request format"
    of the ``Request`` object. Ultimately, the request format is used for such
    things such as setting the ``Content-Type`` of the response (e.g. a ``json``
    request format translates into a ``Content-Type`` of ``application/json``).
    It can also be used in the controller to render a different template for
    each value of ``_format``. The ``_format`` parameter is a very powerful way
    to render the same content in different formats.

Special Routing Parameters
~~~~~~~~~~~~~~~~~~~~~~~~~~

As you've seen, each routing parameter or default value is eventually available
as an argument in the controller method. Additionally, there are three parameters
that are special: each adds a unique piece of functionality inside your application:

* ``_controller``: As you've seen, this parameter is used to determine which
  controller is executed when the route is matched;

* ``_format``: Used to set the request format (:ref:`read more<book-routing-format-param>`);

* ``_locale``: Used to set the locale on the request (:ref:`read more<book-translation-locale-url>`);

.. tip::

    If you use the ``_locale`` parameter in a route, that value will also
    be stored on the session so that subsequent requests keep this same locale.

.. index::
   single: Routing; Controllers
   single: Controller; String naming format

.. _controller-string-syntax:

Controller Naming Pattern
-------------------------

Every route must have a ``_controller`` parameter, which dictates which
controller should be executed when that route is matched. This parameter
uses a simple string pattern called the *logical controller name*, which
Symfony maps to a specific PHP method and class. The pattern has three parts,
each separated by a colon:

    **bundle**:**controller**:**action**

For example, a ``_controller`` value of ``AcmeBlogBundle:Blog:show`` means:

+----------------+------------------+-------------+
| Bundle         | Controller Class | Method Name |
+================+==================+=============+
| AcmeBlogBundle | BlogController   | showAction  |
+----------------+------------------+-------------+

The controller might look like this:

.. code-block:: php

    // src/Acme/BlogBundle/Controller/BlogController.php

    namespace Acme\BlogBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class BlogController extends Controller
    {
        public function showAction($slug)
        {
            // ...
        }
    }

Notice that Symfony adds the string ``Controller`` to the class name (``Blog``
=> ``BlogController``) and ``Action`` to the method name (``show`` => ``showAction``).

You could also refer to this controller using its fully-qualified class name
and method: ``Acme\BlogBundle\Controller\BlogController::showAction``.
But if you follow some simple conventions, the logical name is more concise
and allows more flexibility.

.. note::

   In addition to using the logical name or the fully-qualified class name,
   Symfony supports a third way of referring to a controller. This method
   uses just one colon separator (e.g. ``service_name:indexAction``) and
   refers to the controller as a service (see :doc:`/cookbook/controller/service`).

Route Parameters and Controller Arguments
-----------------------------------------

The route parameters (e.g. ``{slug}``) are especially important because
each is made available as an argument to the controller method:

.. code-block:: php

    public function showAction($slug)
    {
      // ...
    }

In reality, the entire ``defaults`` collection is merged with the parameter
values to form a single array. Each key of that array is available as an
argument on the controller.

In other words, for each argument of your controller method, Symfony looks
for a route parameter of that name and assigns its value to that argument.
In the advanced example above, any combination (in any order) of the following
variables could be used as arguments to the ``showAction()`` method:

* ``$culture``
* ``$year``
* ``$title``
* ``$_format``
* ``$_controller``

Since the placeholders and ``defaults`` collection are merged together, even
the ``$_controller`` variable is available. For a more detailed discussion,
see :ref:`route-parameters-controller-arguments`.

.. tip::

    You can also use a special ``$_route`` variable, which is set to the
    name of the route that was matched.

.. index::
   single: Routing; Importing routing resources

.. _routing-include-external-resources:

Including External Routing Resources
------------------------------------

All routes are loaded via a single configuration file - usually ``app/config/routing.yml``
(see `Creating Routes`_ above). Commonly, however, you'll want to load routes
from other places, like a routing file that lives inside a bundle. This can
be done by "importing" that file:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        acme_hello:
            resource: "@AcmeHelloBundle/Resources/config/routing.yml"

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import("@AcmeHelloBundle/Resources/config/routing.php"));

        return $collection;

.. note::

   When importing resources from YAML, the key (e.g. ``acme_hello``) is meaningless.
   Just be sure that it's unique so no other lines override it.

The ``resource`` key loads the given routing resource. In this example the
resource is the full path to a file, where the ``@AcmeHelloBundle`` shortcut
syntax resolves to the path of that bundle. The imported file might look
like this:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/routing.yml
       acme_hello:
            pattern:  /hello/{name}
            defaults: { _controller: AcmeHelloBundle:Hello:index }

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="acme_hello" pattern="/hello/{name}">
                <default key="_controller">AcmeHelloBundle:Hello:index</default>
            </route>
        </routes>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('acme_hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
        )));

        return $collection;

The routes from this file are parsed and loaded in the same way as the main
routing file.

Prefixing Imported Routes
~~~~~~~~~~~~~~~~~~~~~~~~~

You can also choose to provide a "prefix" for the imported routes. For example,
suppose you want the ``acme_hello`` route to have a final pattern of ``/admin/hello/{name}``
instead of simply ``/hello/{name}``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        acme_hello:
            resource: "@AcmeHelloBundle/Resources/config/routing.yml"
            prefix:   /admin

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml" prefix="/admin" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import("@AcmeHelloBundle/Resources/config/routing.php"), '/admin');

        return $collection;

The string ``/admin`` will now be prepended to the pattern of each route
loaded from the new routing resource.

.. index::
   single: Routing; Debugging

Visualizing & Debugging Routes
------------------------------

While adding and customizing routes, it's helpful to be able to visualize
and get detailed information about your routes. A great way to see every route
in your application is via the ``router:debug`` console command. Execute
the command by running the following from the root of your project.

.. code-block:: bash

    php app/console router:debug

The command will print a helpful list of *all* the configured routes in
your application:

.. code-block:: text

    homepage              ANY       /
    contact               GET       /contact
    contact_process       POST      /contact
    article_show          ANY       /articles/{culture}/{year}/{title}.{_format}
    blog                  ANY       /blog/{page}
    blog_show             ANY       /blog/{slug}

You can also get very specific information on a single route by including
the route name after the command:

.. code-block:: bash

    php app/console router:debug article_show

.. index::
   single: Routing; Generating URLs

Generating URLs
---------------

The routing system should also be used to generate URLs. In reality, routing
is a bi-directional system: mapping the URL to a controller+parameters and
a route+parameters back to a URL. The
:method:`Symfony\\Component\\Routing\\Router::match` and
:method:`Symfony\\Component\\Routing\\Router::generate` methods form this bi-directional
system. Take the ``blog_show`` example route from earlier::

    $params = $router->match('/blog/my-blog-post');
    // array('slug' => 'my-blog-post', '_controller' => 'AcmeBlogBundle:Blog:show')

    $uri = $router->generate('blog_show', array('slug' => 'my-blog-post'));
    // /blog/my-blog-post

To generate a URL, you need to specify the name of the route (e.g. ``blog_show``)
and any wildcards (e.g. ``slug = my-blog-post``) used in the pattern for
that route. With this information, any URL can easily be generated:

.. code-block:: php

    class MainController extends Controller
    {
        public function showAction($slug)
        {
          // ...

          $url = $this->get('router')->generate('blog_show', array('slug' => 'my-blog-post'));
        }
    }

In an upcoming section, you'll learn how to generate URLs from inside templates.

.. tip::

    If the frontend of your application uses AJAX requests, you might want
    to be able to generate URLs in JavaScript based on your routing configuration.
    By using the `FOSJsRoutingBundle`_, you can do exactly that:

    .. code-block:: javascript

        var url = Routing.generate('blog_show', { "slug": 'my-blog-post'});

    For more information, see the documentation for that bundle.

.. index::
   single: Routing; Absolute URLs

Generating Absolute URLs
~~~~~~~~~~~~~~~~~~~~~~~~

By default, the router will generate relative URLs (e.g. ``/blog``). To generate
an absolute URL, simply pass ``true`` to the third argument of the ``generate()``
method:

.. code-block:: php

    $router->generate('blog_show', array('slug' => 'my-blog-post'), true);
    // http://www.example.com/blog/my-blog-post

.. note::

    The host that's used when generating an absolute URL is the host of
    the current ``Request`` object. This is detected automatically based
    on server information supplied by PHP. When generating absolute URLs for
    scripts run from the command line, you'll need to manually set the desired
    host on the ``Request`` object:

    .. code-block:: php

        $request->headers->set('HOST', 'www.example.com');

.. index::
   single: Routing; Generating URLs in a template

Generating URLs with Query Strings
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``generate`` method takes an array of wildcard values to generate the URI.
But if you pass extra ones, they will be added to the URI as a query string::

    $router->generate('blog', array('page' => 2, 'category' => 'Symfony'));
    // /blog/2?category=Symfony

Generating URLs from a template
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The most common place to generate a URL is from within a template when linking
between pages in your application. This is done just as before, but using
a template helper function:

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('blog_show', { 'slug': 'my-blog-post' }) }}">
          Read this blog post.
        </a>

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('blog_show', array('slug' => 'my-blog-post')) ?>">
            Read this blog post.
        </a>

Absolute URLs can also be generated.

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ url('blog_show', { 'slug': 'my-blog-post' }) }}">
          Read this blog post.
        </a>

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('blog_show', array('slug' => 'my-blog-post'), true) ?>">
            Read this blog post.
        </a>

Summary
-------

Routing is a system for mapping the URL of incoming requests to the controller
function that should be called to process the request. It both allows you
to specify beautiful URLs and keeps the functionality of your application
decoupled from those URLs. Routing is a two-way mechanism, meaning that it
should also be used to generate URLs.

Learn more from the Cookbook
----------------------------

* :doc:`/cookbook/routing/scheme`

.. _`FOSJsRoutingBundle`: https://github.com/FriendsOfSymfony/FOSJsRoutingBundle
