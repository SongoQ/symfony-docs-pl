.. index::
   single: Routing

Routing
=======

Piękne URL-e to absolutna konieczność dla każdej poważnej aplikacji internetowej.
Oznacza to porzucenie brzydkich adresów jak ``index.php?article_id=57``
na rzecz czegoś takiego jak ``/read/intro-to-symfony``.

Jeszcze ważniejsza jest elastyczność. Co, jeśli musisz zmienić URL strony
z ``/blog`` na ``/news``? Jak dużo linków musiałbyś odnaleźć i poprawić,
aby dokonać takiej zmiany? Jeśli używasz routera Symfony, taka zmiana jest
banalnie prosta.

Router Symfony2 pozwala Ci definiować kreatywne URL-e, które prowadzą do różnych
miejsc w Twojej aplikacji. Na końcu tego rozdziału będziesz umiał:

* Tworzyć kompleksowe trasy mapujące do kontrolerów
* Generować URL-e wewnątrz szablonów i kontrolerów
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
złożonych URL-i staje się łatwe.

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

   Warstwa routingu jest narzędziem, które tłumaczy przychodzące URL-e na określony
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
do URL-i, w których parameter ``{page}`` jest typu integer. Na szczęście,
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

Wzorzec wymagany ``\d+`` jest wyrażeniem regularnym, który mówi nam, że wartość
parametru ``{page}`` może zawierać wyłącznie cyfry. Ścieżka ``blog`` wciąż będzie
pasować do URL jak ``/blog/2`` (ponieważ 2 jest liczbą), ale nie będzie już pasować
do URL takich jak ``/blog/my-blog-post`` (ponieważ ``my-blog-post`` *nie* jest liczbą).

W efekcie końcowym, URL ``/blog/my-blog-post`` będzie odpowiednio pasować do trasy ``blog_show``.

+--------------------+-----------+-----------------------+
| URL                | route     | parameters            |
+====================+===========+=======================+
| /blog/2            | blog      | {page} = 2            |
+--------------------+-----------+-----------------------+
| /blog/my-blog-post | blog_show | {slug} = my-blog-post |
+--------------------+-----------+-----------------------+

.. sidebar:: Wcześniejsze trasy zawsze wygrywają

    Znaczy to tyle, że kolejność ścieżek jest bardzo istotna. Jeśli trasa
    ``blog_show`` jest umieszczona nad trasą ``blog``, URL ``/blog/2`` będzie
    pasować do ``blog_show``, zamiast do ``blog``, ponieważ parametr ``{slug}``
    ścieżki ``blog_show`` nie ma żadnych wymagań. Stosując odpowiednią kolejność
    oraz mądre wymagania, możesz osiągnąć niemal wszystko.

Ponieważ wymagania parametrów są wyrażeniami regularnymi, kompleksowość i elastyczność
każdego z wymagań należy całkowicie do Ciebie. Załóżmy, że strona główna Twojej
aplikacji jest dostępna w dwóch różnych językach, zależnie od adresu URL:

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

Dla nadchodzącego żądania, parametr ``{culture}`` jest dopasowany do wyrażenia
regularnego ``(en|fr)``.

+-----+---------------------------+
| /   | {culture} = en            |
+-----+---------------------------+
| /en | {culture} = en            |
+-----+---------------------------+
| /fr | {culture} = fr            |
+-----+---------------------------+
| /es | *nie pasuje do tej trasy* |
+-----+---------------------------+

.. index::
   single: Routing; Wymagania metod

Dodawanie wymagań metod HTTP
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Oprócz adresu URL, możesz również dopasować *metodę* nadchodzącego żądania
(np. GET, HEAD, POST, PUT, DELETE). Przypuśćmy, że masz formularz kontaktowy
z dwoma kontrolerami - jeden do wyświetlania formularza (dla żądania GET),
a drugi do przetwarzania formularza, gdy jest on wysłany (podczas żądania POST).
Można to osiągnąć poprzez następującą konfigurację routingu:

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

Pomimo faktu, iż te dwie trasy mają identyczne wzorce (``/contact``), pierwsza
z nich będzie pasować tylko do żądań GET, a druga tylko do żądań POST. Oznacza to,
że możesz wyświetlać i wysyłać formularz poprzez ten sam URL, jednocześnie wykorzystując
do tego oddzielne kontrolery do tych dwóch akcji.

.. note::
    Jeśli nie podano wymagań dla ``_method``, trasa będzie pasować do *wszystkich* metod HTTP.

Podobnie jak inne wymagania, parametrr ``_method`` jest parsowany jako wyrażenie regularne.
Aby dopasować żądania ``GET`` *albo* ``POST``, możesz użyć ``GET|POST``.

.. index::
   single: Routing; Zzawansowany przykład
   single: Routing; parametr _format

.. _advanced-routing-example:

Zaawansowany przykład routingu
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Na chwilę obecną masz do dyspozycji wszystko, co potrzebujesz, aby utworzyć
potężną strukturę routingu w Symfony. Poniższy przykład obrazuje jak elastyczny
może być system routingu:

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

Jak widisz, ta trasa będzie pasować tylko wtedy, kiedy parametr ``{culture}``
adresu URL będzie równy ``en`` lub ``fr``, a ``{year}`` jest liczbą. Ponadto
ta trasa pokazuje, jak możesz wykorzystać kropkę pomiędzy parametrami zamiast ukośnika.
Adresy URL pasujące do tej trasy mogą wyglądać np. tak:

* ``/articles/en/2010/my-post``
* ``/articles/fr/2010/my-post.rss``

.. _book-routing-format-param:

.. sidebar:: Specjalny parametr ``_format``

    Ten przykład prezentuje również specjalny parametr routingu ``_format``.
    Stosując ten parametr, dopasowany element staje się "formatem żądania"
    obiektu ``Request``. Ostatecznie format żądania jest używany do takich
    rzeczy jak ustawienie nagłówka ``Content-Type`` odpowiedzi (np. format żądania
    ``json`` jest zmieniany na ``Content-Type`` równy ``application/json``).
    Może być on również wykorzystany w kontrolerze do renderowania różnych
    szablonów dla każdej wartości parametru ``_format``. Jest on bardzo dobry
    sposób do renderowania tej samej treści w różnych formatach.

Specjalne parametry routingu
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jak mogłeś się przekonać, każdy parametr routingu, czy też wartość domyślna, mogą być
również dostępne jako argument metody kontrolera. Dodatkowo, istnieją jeszcze trzy specjalne
parametry: każdy z nich dodaje unikatową część funkcjonalności do Twojej aplikacji:

* ``_controller``: Jak już wiesz, ten parametr jest używany do określenia kontrolera,
  który ma być wykonany, kiedy trasa jest dopasowana;

* ``_format``: Używany do ustawienia formatu żądania (:ref:`read more<book-routing-format-param>`);

* ``_locale``: Używany do ustawienia języka żądania (:ref:`read more<book-translation-locale-url>`);

.. tip::

    Jeśli używasz parametru ``_locale``, jego wartość będzie również przechowywana
    w sesji, dzięki czemu kolejne żądania będą zawierać tą samą wartość.

.. index::
   single: Routing; Kontrolery
   single: Controller; Format nazewnictwa ciągów

.. _controller-string-syntax:

Wzór nazewnictwa Kontrolerów
----------------------------

Każda trasa musi posiadać parametr ``_controller``, który mówi Symfony,
który kontroler powinien zostać uruchomiony, gdy trasa zostanie dopasowana.
Ten parametr wykorzystuje ciąg w postaci prostego wzoru nazywanego *logiczna
nazwa kontrolera*, który Symfony dopasowuje do konkretnej metody PHP oraz klasy.
Ten wzór składa się z trzech części, każda z nich oddzielona jest dwukropkiem:

    **bundle**:**kontroler**:**akcja**

Na przykład, wartość ``AcmeBlogBundle:Blog:show`` parametru ``_controller_`` oznacza:
For example, a ``_controller`` value of ``AcmeBlogBundle:Blog:show`` means:

+----------------+------------------+--------------+
| Bundle         | Klasa kontrolera | Nazwa metody |
+================+==================+==============+
| AcmeBlogBundle | BlogController   | showAction   |
+----------------+------------------+--------------+

Kontroler może wyglądać np. tak:

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

Zauważ, że Symfony dodaje ciąg ``Controller`` do nazwy klasy (``Blog`` => ``BlogController``)
oraz ciąg ``Action`` do nazwy metody (``show`` => ``showAction``).

Możesz również odwołać się do tego kontrolera poprzez pełną nazwę klasy oraz metody:
``Acme\BlogBundle\Controller\BlogController::showAction``. Jeśli
będziesz przestrzegał kilka prostych konwencji, logiczna nazwa kontrolera jest
bardziej zwięzła i posiada większą elastyczność.

.. note::

   Oprócz używania logicznej nazwy oraz pełnej nazwy klasy, Symfony dostarcza
   trzeci sposób odwoływania się do kontrolera. Ta metoda używa tylko jednego
   dwukropka jako separatora (np. ``service_name:indexAction``) i odwołuje się
   do kontrolera jako usługi (patrz :doc:`/cookbook/controller/service`).

Parametry adresów oraz argumenty kontrolerów
--------------------------------------------

Parametry adresów (np. ``{slug}``) są szczególnie ważne, ponieważ każdy z nich
jest dostępny jako argument metody kontrolera:

.. code-block:: php

    public function showAction($slug)
    {
      // ...
    }

W rzeczywistości, cała kolekcja ``defaults``jest scalana z wartościami parametrów tak,
aby utworzyć prostą tablicę. Każdy klucz tej tablicy jest dostępny jako argument kontrolera.

Innymi słowy, dla kazdego argumentu metody Twojego kontrolera, Symfony szuka parametru
o nazwie takiej samej jak argument i przypisuje jego wartość do tego argumentu. W poniższym
bardziej zaawansowanym przykładzie, dowolna kombinacja (o dowolnej kolejności) poniższych
zmiennych może być użyta jako argumenty metody ``showAction()``:

* ``$culture``
* ``$year``
* ``$title``
* ``$_format``
* ``$_controller``

Jako, że parametry oraz kolekcja ``defaults`` są łączone razem, nawet zmienna
``$_controller`` jest dostępna. Po więcej informacji zasięgnij do
:ref:`route-parameters-controller-arguments`.

.. tip::

    Możesz również używać specjalnej zmiennej ``$_route``, która przechowuje
    nazwę trasy, która została dopasowana.

.. index::
   single: Routing; Importowanie zasobów routingu

.. _routing-include-external-resources:

Importowanie zewnętrznych zasobów routingu
------------------------------------------

Wszystkie trasy są ładowane poprzez prosty plik konfiguracyjny - zazwyczaj ``app/config/routing.yml``
(patrz `Tworzenie tras`_ ponizej). Jednakże najczęściej będziesz potrzebował ładować trasy
z innych miejsc, takich jak plik routingu z paczki (bundla). Można tego dokonać poprzez "importowanie"
tego pliku:

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

   Podczas importowania zasobów YAML, klucz (np. ``acme_hello``) jest bez znaczenia.
   Po prostu upewnij się, że jest unikatowy, przez co żadna inna linia nie nadpisze go.

Klucz ``resource`` wczytuje podany zasób routingu. W tym przypadku zasobem jest
pełna ścieżka do pliku, gdzie skrót ``@AcmeHelloBundle`` zwraca ścieżkę do danej paczki.
Importowany plik może wyglądać na przykład tak:

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

Adresy z tego pliku są parsowane i ładowane w ten sam sposób, jak główny plik
routingu.

Prefiksowanie importowanych tras
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Możesz również chcieć dołączać "prefix" do importowanych adresów. Na przykład, załóżmy,
że chcesz, aby trasa ``acme_hello`` miała ostateczny wzór ``/admin/hello/{name}``, zamiast
prostego ``/hello/{name}``:

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

Ciąg ``/admin`` będzie teraz poprzedzał wzór każdej trasy ładowanej z naszego
nowego zasobu routingu.

.. index::
   single: Routing; Debugowanie

Wizualizowanie i debugowanie adresów
------------------------------------

Dodając i dostosowując adresy, pomocna może okazać się możliwość wizualizacji
oraz uzyskania szczegółowej informacji na temat Twoich tras. Świetnym sposobem,
aby zobaczyć każdy adres Twojej aplikacji jest użycie polecenia ``router:debug``.
Uruchom do polecenie poprzez wpisanie go w linii poleceń w głównym katalogu
Twojego projektu, tak jak poniżej:

.. code-block:: bash

    php app/console router:debug

To polecenie wyświetli na ekranie pomocną listę *wszystkich* skonfigurowanych
adresów w Twojej aplikacji:

.. code-block:: text

    homepage              ANY       /
    contact               GET       /contact
    contact_process       POST      /contact
    article_show          ANY       /articles/{culture}/{year}/{title}.{_format}
    blog                  ANY       /blog/{page}
    blog_show             ANY       /blog/{slug}

Możesz również uzyskać dokładne informacje o pojedynczym adresie, dołączając
jego nazwę do powyższego polecenia:

.. code-block:: bash

    php app/console router:debug article_show

.. index::
   single: Routing; Generowanie adresów URL

Generowanie adresów URL
-----------------------

System routingu powinien również być używany do generowania URL-i. W rzeczywistości,
routing jest systemem dwukierunkowym: mapuje URL do kontrolera i parametrów, oraz
trasę i parametry z powrotem do URL. Metody :method:`Symfony\\Component\\Routing\\Router::match`
oraz :method:`Symfony\\Component\\Routing\\Router::generate` wykorzystują ten
dwukierunkowy system. Spójrz na poniższy przykład wykorzystujący wcześniejszą trasę
``blog_show``::

    $params = $router->match('/blog/my-blog-post');
    // array('slug' => 'my-blog-post', '_controller' => 'AcmeBlogBundle:Blog:show')

    $uri = $router->generate('blog_show', array('slug' => 'my-blog-post'));
    // /blog/my-blog-post

Aby wygenerować URL, musisz określić nazwę trasy (np. ``blog_show``) oraz wszystkie
parametry (np. ``slug = my-blog-post``) użyte we wzorze tego adresu. Dzięki tym informacjom,
każdy URL moze być łatwo wygenerowany:

.. code-block:: php

    class MainController extends Controller
    {
        public function showAction($slug)
        {
          // ...

          $url = $this->get('router')->generate('blog_show', array('slug' => 'my-blog-post'));
        }
    }

W kolejnej sekcji nauczysz się jak generować URL-e w szablonach.

.. tip::

    Jeśli frontend Twojej aplikacji wykorzystuje żądania AJAX, możesz chcieć
    mieć możliwość generowania URL-i w JavaScript na podstawie konfiguracji routingu.
    Używając `FOSJsRoutingBundle`_, mozesz robić dokładnie tak:

    .. code-block:: javascript

        var url = Routing.generate('blog_show', { "slug": 'my-blog-post'});

    Po więcej informacji zasięgnij do dokumentacji tej paczki.

.. index::
   single: Routing; Adresy absolutne

Generowanie adresów absolutnych
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Domyślnie router generuje relatywne adresy URL (np. ``/blog``). Aby wygenerować
absolutny URL, po prostu przekaż ``true`` jako trzeci argument metody ``generate()``:

.. code-block:: php

    $router->generate('blog_show', array('slug' => 'my-blog-post'), true);
    // http://www.example.com/blog/my-blog-post

.. note::

    Host używany podczas generowania aboslutnego URL jest hostem dla aktualnego
    obiektu ``Request`` (żądania). Jest on wykrywany automatycznie na podstawie informacji
    o serwerze dostarczanych przez PHP. Podczas generowania absolutnych URL-i dla
    skryptów uruchamianych z linii poleceń, musisz ręcznie podawać żądany
    host dla obiektu ``Request``:

    .. code-block:: php

        $request->headers->set('HOST', 'www.example.com');

.. index::
   single: Routing; Generowanie URL-i wewnątrz szablonów

Generowanie URL-i z QueryStrings
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Metoda ``generate`` wykorzystuje tablicę parametrów aby wygenerować URI.
Jednakże, jeśli podasz kilka dodatkowych, będą one dodane do URL jako query string (parametry GET)::

    $router->generate('blog', array('page' => 2, 'category' => 'Symfony'));
    // /blog/2?category=Symfony

Generowanie URL-i wewnątrz szablonów
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Najczęściej wykorzystywanym miejscem do generowanie URL-i wewnątrz szablonów są
linki pomiedzy stronami Twojej aplikacji. Jest to dokonywane tak samo jak powyżej,
lecz za pomocą funkcji helpera szablonu:

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('blog_show', { 'slug': 'my-blog-post' }) }}">
          Przeczytaj ten post bloga.
        </a>

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('blog_show', array('slug' => 'my-blog-post')) ?>">
            Przeczytaj ten post bloga.
        </a>

Można generować również absolutne adresy URL.

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ url('blog_show', { 'slug': 'my-blog-post' }) }}">
          Przeczytaj ten post bloga.
        </a>

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('blog_show', array('slug' => 'my-blog-post'), true) ?>">
            Przeczytaj ten post bloga.
        </a>

Podsumowanie
------------

Routing to system mapowania URL-i przychodzących żądań do funkcji kontrolera,
który ma być wywołany do przetworzenia żądania. Pozwala to zarówno na określanie
ładnych URL-i, a także oddziela funkcjonalność Twojej aplikacji od tych URL-i.
Routing jest dwukierunkowym mechanizmem, co oznacza, że może być również wykorzystywany
do generowania adresów URL.

Dowiedz się więcej z Cookbook'a
------------------------------

* :doc:`/cookbook/routing/scheme`

.. _`FOSJsRoutingBundle`: https://github.com/FriendsOfSymfony/FOSJsRoutingBundle
