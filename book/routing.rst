.. highlight:: php
   :linenothreshold: 2

.. index::
   single: Routing

Trasowanie
==========

Piękne (przyjazne) adresy URL to absolutna konieczność dla każdej poważnej aplikacji
internetowej. Oznacza to porzucenie "brzydkich" (nieprzyjaznych) ścieżek, takich
jak ``index.php?article_id=57`` na rzecz czegoś takiego jak ``/read/intro-to-symfony``.

Jeszcze ważniejsza jest elastyczność. Co, jeśli musi się zmienić ścieżkę URL strony
z ``/blog`` na ``/news``? Ile odnośników trzeba odnaleźć i poprawić,
aby dokonać takiej zmiany? Jeśli korzysta się z mechanizmu trasowania Symfony,
taka zmiana jest bardzo prosta.

Mechanizm trasowania Symfony2 pozwala na dynamiczne określanie ścieżek URL (czyli
ścieżek dostępu, zawartych w adresie URL żądania HTTP) odwzorowywanych dla
różnych obszarów aplikacji. Pod koniec tego rozdziału, będziesz w stanie:

* tworzyć złożone trasy odwzorowujące kontrolery;
* generować ścieżki URL wewnątrz szablonów i kontrolerów;
* ładować zasoby trasowania z pakietów (lub z czegokolwiek innego);
* debugować swoje trasowania.


.. index::
   single: trasowanie; podstawy

Działanie trasowania
--------------------

**Trasa** (*ang. route*) jest mapą od ścieżki URL do kontrolera. Na przykład, załóżmy,
że chcemy dopasować ścieżkę URL jak ``/blog/moj-post`` czy ``/blog/wszystko-o-symfony``
i wysłać go do kontrolera, który może odnaleźć i wyświetlić dany wpis blogu. Trasa
jest prosta:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/routing.yml
        blog_show:
            path:   /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml
       :linenos:

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog_show" path="/blog/{slug}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php
       :linenos:

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog_show', new Route('/blog/{slug}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $collection;

.. versionadded:: 2.2
    Opcja ``path`` jest nowością w Symfony2.2 i zastępuje opcję ``pattern``
    z wersji wcześniejszych.



Ścieżka zdefiniowana w trasie ``blog_show`` działa jak ``/blog/*``, gdzie
znak wieloznaczny (``*``) otrzymuje nazwę ``slug``. Dla ścieżki URL ``/blog/moj-post``
zmienna ``slug`` przybierze wartość ``moj-post``, która jest dostępna z poziomu
kontrolera (czytaj dalej).

Parametr ``_controller`` jest specjalnym kluczem, który informuje Symfony jaki kontroler
powinien być uruchomiony, kiedy ścieżka URL zostanie dopasowana do wzorca trasy.
Wartością ``_controller`` jest ciąg znakowy określający
:ref:`nazwę logiczną<controller-string-syntax>`. Ma to zastosowanie do wzorców,
które wskazują określa klasę i metodę PHP:

.. code-block:: php
   :linenos:

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

W tym kodzie właśnie utworzyliśmy naszą pierwszą trasę i połączyliśmy ją z kontrolerem.
Teraz, kiedy odwiedzi się ``/blog/moj-post``, zostanie uruchomiony kontroler
``showAction``, a zmienna ``$slug`` przyjmie wartość ``moj-post``.

To jest właśnie zadanie mechanizmu trasowania Symfony2: odwzorować ścieżkę URL żądania
na kontroler. W dalszej części artykułu podanych jest  wiele sztuczek, które sprawiają,
że odwzorowanie nawet najbardziej skomplikowanych adresó URL staje się łatwe.


.. index::
   single: trasowanie; mechanizm

Trasowanie - pod maską
----------------------

Kiedy do aplikacji wysłane jest żądanie, zawiera ono dokładny adres do
"zasobu", który klient żąda. Ten adres nazywany jest ścieżką URL (lub ścieżką URI)
i może to być ``/kontakt``, ``/blog/informacje`` lub cokolwiek innego. Weźmy za
przykład poniższe żądanie HTTP:

.. code-block:: text

    GET /blog/moj-post

Zadaniem mechanizmu trasowania Symfony2 jest przetworzenie tej ścieżki URL
i określenie, który kontroler powinien zostać uruchomiony. Cały proces wygląda
mniej więcej tak:

#. Żądanie zostaje obsłużone przez kontroler wejścia Symfony2 (np. ``app.php``);

#. Rdzeń Symfony2 (czyli :term:`Kernel`) odpytuje mechaniz trasowania o treść żądania;

#. Mechanizm trasowania dopasowuje ścieżkę zawarta w przychodzącym adresie URL do
   konkretnej trasy i zwraca informacje o trasie, łącznie z nazwą kontrolera, który
   powinien zostać uruchomiony;

#. Rdzeń Symfony2 wykonuje kontroler, który ostatecznie zwraca obiekt ``Response``.

.. figure:: /images/request-flow.png
   :align: center
   :alt: Przepływ żądania w Symfony2

Warstwa trasowania jest narzędziem, które tłumaczy przychodzący adres URL na określony
kontroler jaki ma być wykonany.

.. index::
   single: trasowanie; tworzenie tras

.. _creating-routes:

Tworzenie tras
--------------

Symfony wczytuje wszystkie trasy dla aplikacji z pojedynczego pliku trasowania.
Ten plik to zazwyczaj ``app/config/routing.yml``, jednakże można
skonfigurować inny plik (nawet w formacie XML zy PHP) za pośrednictwem pliku
konfiguracyjnego aplikacji:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        framework:
            # ...
            router:        { resource: "%kernel.root_dir%/config/routing.yml" }

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <framework:config ...>
            <!-- ... -->
            <framework:router resource="%kernel.root_dir%/config/routing.xml" />
        </framework:config>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'router'        => array('resource' => '%kernel.root_dir%/config/routing.php'),
        ));

.. tip::

    Nawet, jeśli wszystkie trasy są wczytywane z pojedynczego pliku, dobrą praktyką
    jest dołączać dodatkowe zasoby trasowania z innych plików.
    Zobacz do rozdiału :ref:`routing-include-external-resources` w celu poznania
    szczegółów.

Podstawowa konfiguracja trasy
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Definiowanie tras jest proste, a typowa aplikacja będzie posiadała wiele tras.
Podstawowa trasa składa się z dwóch części: ``path`` (wzorca do dopasowania)
oraz z tablicy ``defaults`` przechowującej wartości domyślne:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        _welcome:
            path:   /
            defaults:  { _controller: AcmeDemoBundle:Main:homepage }

    .. code-block:: xml
       :linenos:

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="_welcome" path="/">
                <default key="_controller">AcmeDemoBundle:Main:homepage</default>
            </route>

        </routes>

    .. code-block:: php
       :linenos:

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('_welcome', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Main:homepage',
        )));

        return $collection;

Ta trasa dopasowuje do stronę główną aplikacji (``/``) i odwzorowuje kontroler
``AcmeDemoBundle:Main:homepage``. Ciąć znakowy ``_controller`` jest zamieniany
na nazwę odpowiedniej funkcji PHP, która następnie zostaje wykonana.
Ten proces będzie wyjaśniony w sekcji :ref:`controller-string-syntax`.

.. index::
   single: trasowanie; parametry

Trasowanie z wieloznacznikami
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Mechanizm trasowania obsługuje również trasy z wieloznacznikami.
Do określenia wielu tras można wykorzystać jedno lub więcej
"wyrażeń wieloznacznych" zwanych wieloznacznikami (*ang. placeholders*):

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        blog_show:
            path:   /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml
       :linenos:

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog_show" path="/blog/{slug}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php
       :linenos:
       
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog_show', new Route('/blog/{slug}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $collection;

Wzorzec będzie pasował do wszystkiego, co wygląda jak ``/blog/*``. Co więcej,
wartość przypisana do parametru ``{slug}`` będzie dostępna wewnątrz kontrolera.
Innymi słowy, jeśli ścieżka URL wygląda tak: ``/blog/hello-world``,
to zmienna ``$slug`` z wartością ``hello-world`` będzie dostępna w kontrolerze.
Może być to użyte np. do pobrania wpisu na blogu, którego adres pasuje do tego
ciągu znakowego.

Ten wzorzec jednakże nie będzie pasował do samego ``/blog``. Dzieje się tak,
ponieważ domyślnie wymagane jest okreśłenie wszystkich wieloznaczników.
Może być to zmienione poprzez dodanie do tablicy ``defaults`` następnej wartości
wieloznacznika.


Wieloznaczniki obowiązkowe i opcjonalne
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Aby było ciekawiej, dodamy nową trasę wyświetlającą listę wszystkich dostępnych
wpisów na blogu wymyślonej aplikacji blogowej:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        blog:
            path:   /blog
            defaults:  { _controller: AcmeBlogBundle:Blog:index }

    .. code-block:: xml
       :linenos:

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" path="/blog">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
            </route>
        </routes>

    .. code-block:: php
       :linenos:

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
        )));

        return $collection;

Jak dotąd, ta trasa jest tak prosta, jak to tylko możliwe - nie zawiera
żadnych wieloznaczników i pasuje tylko do jednej ścieżki URL ``/blog``. Ale co,
jeśli chce się, aby ta trasa obsługiwała stronicowanie, gdzie ``/blog/2``
wyświetlałby  drugą stronę wpisów blogu? Zmieńmy tą trasę, tak aby posiadała nowy
parameter ``{page}``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        blog:
            path:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index }

    .. code-block:: xml
       :linenos:

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" path="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
            </route>
        </routes>

    .. code-block:: php
       :linenos:

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
        )));

        return $collection;

Podobnie jak poprzedni wieloznacznik ``{slug}``, wartość pasująca do ``{page}``
będzie też dostępna dla kontrolera. Ta wartość może być użyta do określenia,
którą część wpisu na blogu wyświetlić dla danej strony.

Ale chwileczkę! Ponieważ wieloznaczniki są domyślnie wymagane, ta trasa już nie będzie
pasować do adresu ``/blog``. Ponadto, aby zobaczyć stronę 1 blogu, trzeba użyć
ścieżki URL ``/blog/1``. Ponieważ nie jest to dobry sposób dla bardziej złożonej
aplikacji internetowej, to zmodyfikujemy trasę tak aby wileoznacznik ``{page}``
był opcjonalny. Można tego dokonać dołączając do tablicy ``defaults``, taki oto
zapis:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        blog:
            path:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }

    .. code-block:: xml
       :linenos:

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" path="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="page">1</default>
            </route>
        </routes>

    .. code-block:: php
       :linenos:

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'page' => 1,
        )));

        return $collection;

Po dodaniu ``page`` do tablicy ``defaults``, wieloznacznik ``{page}`` już nie jest
wymagany. Ścieżka URL ``/blog`` będzie teraz pasowała do tej trasy, a wartość wieloznacznika
``page`` zostanie ustawiona na ``1``. Ścieżka URL ``/blog/2`` również będzie pasować,
dając wieloznacznikowi ``page`` wartość ``2``.

+---------+------------+
| /blog   | {page} = 1 |
+---------+------------+
| /blog/1 | {page} = 1 |
+---------+------------+
| /blog/2 | {page} = 2 |
+---------+------------+

.. index::
   single: trasowanie; wymagania

Dodawanie wymagań
~~~~~~~~~~~~~~~~~

Spójrzmy na utworzone przez nas wcześniej trasy:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        blog:
            path:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }

        blog_show:
            path:   /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml
       :linenos:

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" path="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="page">1</default>
            </route>

            <route id="blog_show" path="/blog/{slug}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php
       :linenos:

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

Czy nie występuje tu jakiś problem? Prosze zauważyć, że obie trasy mają wzorce,
do których pasują ścieżki URL takie jak ``/blog/*``. Mechanizm trasowania Symfony2
zawsze będzie wybierał **pierwszą** trasę, którą znajdzie. Innymi słowy, trasa
``blog_show`` nigdy nie zostanie dopasowana. Ponadto ścieżka URL taka jak
``/blog/my-blog-post`` będzie pasowała do pierwszej trasy (``blog``) i zwracała
bezsensowną wartość ``my-blog-post`` dla wieloznacznika ``{page}``.

+--------------------+-------+-----------------------+
| URL                | route | parameters            |
+====================+=======+=======================+
| /blog/2            | blog  | {page} = 2            |
+--------------------+-------+-----------------------+
| /blog/my-blog-post | blog  | {page} = my-blog-post |
+--------------------+-------+-----------------------+

Rozwiązaniem tego problemu jest dodanie do trasy **wymagań** (parametru ``requirements``).
Trasy w tym przypadku będą działały idealnie, jeśli wzorzec ``/blog/{page}`` będzie
pasował *wyłącznie* do ścieżek URL, w których wieloznacznik ``{page}`` jest typu
integer. Na szczęście można dodawać wyrażenie regularne do każdego parametru, w tym
do parametru ``requirements``. Na przykład:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        blog:
            path:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }
            requirements:
                page:  \d+

    .. code-block:: xml
       :linenos:

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" path="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="page">1</default>
                <requirement key="page">\d+</requirement>
            </route>
        </routes>

    .. code-block:: php
       :linenos:

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

Wymaganie ``\d+`` jest wyrażeniem regularnym, które dopuszcza jako wartość wieloznacznika
``{page}`` wyłącznie cyfry. Trasa ``blog`` wciąż będzie pasować do ścieżki URL,
takiej jak ``/blog/2`` (ponieważ 2 jest liczbą), ale nie będzie już pasować do
ścieżki URL takiego jak ``/blog/my-blog-post`` (ponieważ ``my-blog-post`` nie jest liczbą).

W efekcie końcowym scieżka URL ``/blog/my-blog-post`` będzie odpowiednio pasować do
trasy ``blog_show``.

+--------------------+-----------+-----------------------+
| URL                | route     | parameters            |
+====================+===========+=======================+
| /blog/2            | blog      | {page} = 2            |
+--------------------+-----------+-----------------------+
| /blog/my-blog-post | blog_show | {slug} = my-blog-post |
+--------------------+-----------+-----------------------+

.. sidebar:: Wcześniejsze trasy zawsze wygrywają

    Znaczy to tyle, że kolejność tras jest bardzo istotna. Jeśli trasa
    ``blog_show`` jest umieszczona nad trasą ``blog``, ścieżka URL ``/blog/2`` będzie
    pasować do ``blog_show``, zamiast do ``blog``, ponieważ wieloznacznik ``{slug}``
    ścieżki ``blog_show`` nie ma żadnych wymagań. Stosując odpowiednią kolejność
    oraz sprytne wymagania, można osiągnąć niemal wszystko.

Ponieważ parametr ``requirements`` jest wyrażeniem regularnym, kompleksowość i
elastyczność każdego z wymagań zależy całkowicie od programisty. Załóżmy, że
strona główna aplikacji jest dostępna w dwóch różnych językach, zależnie od ścieżki URL:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        homepage:
            path:   /{culture}
            defaults:  { _controller: AcmeDemoBundle:Main:homepage, culture: en }
            requirements:
                culture:  en|fr

    .. code-block:: xml
       :linenos:

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="homepage" path="/{culture}">
                <default key="_controller">AcmeDemoBundle:Main:homepage</default>
                <default key="culture">en</default>
                <requirement key="culture">en|fr</requirement>
            </route>
        </routes>

    .. code-block:: php
       :linenos:

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

Część ścieżki URL ``{culture}`` w przychodzącym żądaniu jest dopasowywana do wyrażenia
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
   single: trasowanie; wymagania metody HTTP

Dodawanie wymagania dotyczącego metody HTTP
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Oprócz ścieżki URL, można również dopasować metodę przychodzącego żądania
(tj. GET, HEAD, POST, PUT, DELETE). Załóżmy, że mamy formularz kontaktowy
z dwoma kontrolerami - jeden do wyświetlania formularza (dla żądania GET),
a drugi do przetwarzania formularza, gdy zostanie on zgłoszony (z metodą POST).
Można to osiągnąć poprzez następującą konfigurację trasowania:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        contact:
            path:  /contact
            defaults: { _controller: AcmeDemoBundle:Main:contact }
            methods:  GET

        contact_process:
            path:  /contact
            defaults: { _controller: AcmeDemoBundle:Main:contactProcess }
            methods:  POST

    .. code-block:: xml
       :linenos:

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="contact" path="/contact" methods="GET">
                <default key="_controller">AcmeDemoBundle:Main:contact</default>
            </route>

            <route id="contact_process" path="/contact" methods="POST">
                <default key="_controller">AcmeDemoBundle:Main:contactProcess</default>
            </route>
        </routes>

    .. code-block:: php
       :linenos:

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('contact', new Route('/contact', array(
            '_controller' => 'AcmeDemoBundle:Main:contact',
        ), array(), array(), '', array(), array('GET')));

        $collection->add('contact_process', new Route('/contact', array(
            '_controller' => 'AcmeDemoBundle:Main:contactProcess',
        ), array(), array(), '', array(), array('POST')));

        return $collection;

.. versionadded:: 2.2
    W Symfony2.2 została dodana opcja ``methods``. Użycie  ``_method`` wymagane
    jest tylko w starszych wersjach.

Pomimo faktu, iż te dwie trasy mają identyczne ścieżki (``/contact``), pierwsza
z nich będzie pasować tylko do żądań GET, a druga tylko do żądań POST. Oznacza to,
że można wyświetlać i zgłosić formularz poprzez ten sam adres URL, jednocześnie
wykorzystując do tego oddzielne kontrolery dla tych dwóch różnych akcji.

.. note::
    Jeśli nie zostanie podane wymaganie dla `methods``, trasa będzie pasować do
    wszystkich metod HTTP.


.. index::
   single: trasowanie; host
   
.. _adding-host:
   
Dodawanie hosta
~~~~~~~~~~~~~~~

.. versionadded:: 2.2
    W Symfony 2.2 dodano obsługę dopasowania hosta

Można również dopasowywać nagłówek HTTP `Host`_ przychodzącego żądania. Więcej
informacji można uzyskać a artykule :doc:`/components/routing/hostname_pattern`
w dokumentacji komponentu Routing.

.. index::
   single: Routing; Advanced example
   single: Routing; _format parameter

.. _advanced-routing-example:

Przykład zaawansowanego trasowania
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

W tym momencie mamy wszystko, co potrzebne jest do stworzenia pełno wartościowej
struktury trasowania w Symfony. Oto przykład tego, jak elastyczny może być system
trasowania:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        article_show:
          path:  /articles/{culture}/{year}/{title}.{_format}
          defaults: { _controller: AcmeDemoBundle:Article:show, _format: html }
          requirements:
              culture:  en|fr
              _format:  html|rss
              year:     \d+

    .. code-block:: xml
       :linenos:

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="article_show" path="/articles/{culture}/{year}/{title}.{_format}">
                <default key="_controller">AcmeDemoBundle:Article:show</default>
                <default key="_format">html</default>
                <requirement key="culture">en|fr</requirement>
                <requirement key="_format">html|rss</requirement>
                <requirement key="year">\d+</requirement>
            </route>
        </routes>

    .. code-block:: php
       :linenos:

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

Jak widać, ta trasa będzie pasować tylko wtedy, kiedy wieloznacznik ``{culture}``
w ścieżce URL będzie równy ``en`` lub ``fr``, a ``{year}`` jest liczbą. Ponadto
ta trasa pokazuje, jak można wykorzystać kropkę pomiędzy wieloznacznikami zamiast
ukośnika. Ścieżki URL pasujące do tej trasy mogą wyglądać np. tak:

* ``/articles/en/2010/my-post``
* ``/articles/fr/2010/my-post.rss``

.. _book-routing-format-param:

.. sidebar:: Specjalny parametr ``_format``

    Powyższy przykład pokazuje również specjalny parametr trasowania ``_format``.
    Przy zastosowaniu tego parametru dopasowaną wartością może być "format żądania"
    obiektu Request. Ostatecznie format żądania służy do takich rzeczy jak ustawienie
    nagłówka ``Content-Type`` odpowiedzi (np. żądany format json tłumaczony jest na
    ``Content-Type application/json``). W kontrolerze do renderowania może być
    również stosowany inny szablon dla każdej wartości ``_format``.
    Parametr ``_format`` jest bardzo skutecznym sposobem na renderowanie tej samej
    treści w różnych formatach.

.. note::
   
   Czasami można chcieć, aby niektóre części tras były konfigurowalne globalnie.
   Symfony 2.1 umożliwia zrobienie tego przez wykorzystanie parametrów poziomu
   kontenera usług. Więcej na ten temat można sie dowiedzieć w artykule
   ":doc:`Jak stosować parametry kontenera usług w trasowaniu</cookbook/routing/service_container_parameters>`".
   
   

Specjalne parametry trasowania
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jak mogliśmy sie przekonać, każda wartość parametr trasowania jest dostępna jako
argument w metododzie kontrolera. Dodatkowo istnieją jeszcze trzy specjalne
parametry - każdy z nich dodaje unikatową cząstkę funkcjonalności do aplikacji:

* ``_controller``: określa który kontroler ma zostać użyty gdy trasa zostanie dopasowana;

* ``_format``: służy do ustawienia formatu żądania (:ref:`czytaj więcej<book-routing-format-param>`);

* ``_locale``: służy do ustawienia języka sesji (:ref:`czytaj więcej<book-translation-locale-url>`).

.. tip::

    Jeśli używa się parametru ``_locale``, jego wartość będzie również przechowywana
    w sesji, dzięki czemu dla kolejnych żądań będą stosowane te same ustawinia
    regionalne.

.. index::
   single: trasowanie; kontrolery
   single: kontroler; format łańcucha nazewniczego

.. _controller-string-syntax:

Wzorzec nazwy kontrolera
------------------------

Każda trasa musi posiadać parametr ``_controller``, który informuje Symfony,
który kontroler powinien zostać uruchomiony, gdy trasa zostanie dopasowana.
Ten parametr używa prostego wzorca zwanego *logiczną nazwa kontrolera*, który
Symfony dopasowuje do nazwy konkretnej metody i klasy PHP.
Wzorzec ten składa się z trzech części, każda z nich oddzielona jest dwukropkiem:

    **pakiet**:**kontroler**:**akcja**

Na przykład, wartość ``AcmeBlogBundle:Blog:show`` parametru ``_controller_`` oznacza:

+----------------+------------------+--------------+
| Pakiet         | Klasa kontrolera | Nazwa metody |
+================+==================+==============+
| AcmeBlogBundle | BlogController   | showAction   |
+----------------+------------------+--------------+

Kontroler może wyglądać np. tak:

.. code-block:: php
   :linenos:

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

Prowszę zauważyć, że Symfony dodaje ciąg ``Controller`` do nazwy klasy (``Blog``
=> ``BlogController``) oraz ciąg ``Action`` do nazwy metody (``show`` => ``showAction``).

Można również odnieść się do kontrolera używając w pełni kwalifikowanej nazwy klasy
oraz metody:
``Acme\BlogBundle\Controller\BlogController::showAction``.
Jeśli przestrzega się kilku prostych konwencji, logiczna nazwa kontrolera jest
bardziej zwięzła i pozwala na większą elastyczność.

.. note::

   Oprócz używania logicznej nazwy oraz w pełni kwalifikowanej nazwy klasy,
   Symfony dostarcza trzeci sposób odwoływania się do kontrolera. Tą metoda używa
   tylko jednego dwukropka jako separatora (np. ``service_name:indexAction``)
   i odwołuje się do kontrolera jako usługi (patrz :doc:`/cookbook/controller/service`).

Parametry trasy a argumenty kontrolera
--------------------------------------

Parametry trasy (np. ``{slug}``) są szczególnie ważne, ponieważ każdy z nich
jest dostępny jako argument metody kontrolera:

.. code-block:: php
   :linenos:

    public function showAction($slug)
    {
      // ...
    }

W rzeczywistości, cała kolekcja ``defaults`` jest scalana z wartościami parametrów
w jedną pojedynczą tablicę. Każdy klucz tej tablicy jest dostępny jako argument
kontrolera.

Innymi słowy, dla każdego argumentu metody kontrolera, Symfony szuka parametru
o nazwie takiej samej jak argument i przypisuje jego wartość do tego argumentu.
W powyżej rezentowanym zaawansowanym przykładzie, dowolna kombinacja (o dowolnej
kolejności) nastęþujacych zmiennych może być użyta jako argumenty metody
``showAction()``:

* ``$culture``
* ``$year``
* ``$title``
* ``$_format``
* ``$_controller``

Ponieważ wieloznaczniki i kolekcja ``defaults`` są łączone razem, nawet zmienna
``$_controller`` jest dostępna. Więcej szczegółów jest omówionych w rozdziale
:ref:`route-parameters-controller-arguments`.

.. tip::

    Możesz również używać specjalnej zmiennej ``$_route``, która przechowuje
    nazwę trasy, która została dopasowana.

.. index::
   single: trasowanie; dołącznie zewnętrznych zasobów

.. _routing-include-external-resources:

Dołączanie zewnętrznych zasobów trasowania
------------------------------------------

Wszystkie trasy są ładowane z pojedyńczego pliku konfiguracyjnego - zazwyczaj
``app/config/routing.yml`` (czytaj rozdział :ref:`creating-routes`).
Często jednak zachodzi potrzeba ładowania trasy z innych miejsc, takich jak plik
trasowania umieszczonego w pakiecie. Można tego dokonać poprzez "importowanie"
tego pliku:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/routing.yml
        acme_hello:
            resource: "@AcmeHelloBundle/Resources/config/routing.yml"

    .. code-block:: xml
       :linenos:

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml" />
        </routes>

    .. code-block:: php
       :linenos:

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import("@AcmeHelloBundle/Resources/config/routing.php"));

        return $collection;

.. note::

   Podczas importowania zasobów YAML, klucz (np. ``acme_hello``) jest bez znaczenia.
   Wystarczy się upewnić, że jest on unikalny, przez co żadna inna linia nie nadpisze go.

Klucz ``resource`` wczytuje podany zasób trasowania. W tym przypadku zasobem jest
pełna ścieżka do pliku, gdzie skrót ``@AcmeHelloBundle`` przekształacany jest 
ścieżke do pakietu. Importowany plik może wyglądać na przykład tak:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # src/Acme/HelloBundle/Resources/config/routing.yml
       acme_hello:
            path:  /hello/{name}
            defaults: { _controller: AcmeHelloBundle:Hello:index }

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/HelloBundle/Resources/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="acme_hello" path="/hello/{name}">
                <default key="_controller">AcmeHelloBundle:Hello:index</default>
            </route>
        </routes>

    .. code-block:: php
       :linenos:

        // src/Acme/HelloBundle/Resources/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('acme_hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
        )));

        return $collection;

Trasy z tego pliku są przetwarzane i ładowane w ten sam sposób, jak główny plik
trasowania.

Przedrostki dla importowanych tras
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Można również zapewnić "przedrostek" dla importowanych tras. Na przykład załóżmy,
że trasa ``acme_hello`` ma ostateczną ścieżkę ``/admin/hello/{name}``, zamiast
prostego ``/hello/{name}``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/routing.yml
        acme_hello:
            resource: "@AcmeHelloBundle/Resources/config/routing.yml"
            prefix:   /admin

    .. code-block:: xml
       :linenos:

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml" prefix="/admin" />
        </routes>

    .. code-block:: php
       :linenos:

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import("@AcmeHelloBundle/Resources/config/routing.php"), '/admin');

        return $collection;

Ciąg ``/admin`` będzie teraz poprzedzał ścieżkę każdej trasy ładowanej z nowego
zasobu trasowania.

Dodawanie wyrażeń regularnych hosta do importowanych tras
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.2
    W Symfony 2.2 dodano obsługę dopasowywania hosta.

Można ustawić wyrażenie regularne hosta na importowanych trasach. Więcej informacji
można znaleźć w rozdziale :ref:`component-routing-host-imported`.


.. index::
   single: trasowanie; debugowanie

Wizualizowanie i debugowanie tras
---------------------------------

Dodając i dostosowując ścieżki, pomocna może okazać się możliwość wizualizacji
oraz uzyskania szczegółowej informacji o trasach. Dobrym sposobem,
na zobaczenie wszystkich tras aplikacji jest użycie polecenia ``router:debug``.
Polecenie należy wykonać głównym katalogu projektu, tak jak poniżej:

.. code-block:: bash

    $ php app/console router:debug

Polecenie to wyświetli na ekranie listę wszystkich skonfigurowanych
tras aplikacji:

.. code-block:: text

    homepage              ANY       /
    contact               GET       /contact
    contact_process       POST      /contact
    article_show          ANY       /articles/{culture}/{year}/{title}.{_format}
    blog                  ANY       /blog/{page}
    blog_show             ANY       /blog/{slug}

Można również uzyskać szczegółowe informacje o pojedynczej trasie, dołączając
jej nazwę do powyższego polecenia:

.. code-block:: bash

    $ php app/console router:debug article_show
    
.. versionadded:: 2.1
    W Symfony 2.1 dodano obsługe polecenia ``router:match``.

Można sprawdzić czy trasa pasuje do ścieżki posługując się poleceniem konsoli ``router:match``:

.. code-block:: bash

    $ php app/console router:match /articles/en/2012/article.rss
    Route "article_show" matches

.. index::
   single: trasowanie; generowanie ścieżek URL

Generowanie ścieżek URL
-----------------------

System trasowania powinien również być używany do generowania ścieżek URL.
W rzeczywistości, trasowanie jest systemem dwukierunkowym: odwzorowuje ścieżkę URL
na kontroler (i parametry), oraz z powrotem trasę (i parametry) na ścieżkę URL.
Ten dwukierunkowy system tworzony jest przez metody
:method:`Symfony\\Component\\Routing\\Router::match` oraz
:method:`Symfony\\Component\\Routing\\Router::generate`.
Przyjrzyjmy się poniższemu przykładowi wykorzystującemu wcześniejszą trasę
``blog_show``::

    $params = $router->match('/blog/my-blog-post');
    // array('slug' => 'my-blog-post', '_controller' => 'AcmeBlogBundle:Blog:show')

    $uri = $router->generate('blog_show', array('slug' => 'my-blog-post'));
    // /blog/my-blog-post

Aby wygenerować ścieżkę URL, musi się określić nazwę trasy (np. ``blog_show``) oraz
wszystkie wieloznaczniki (np. ``slug = my-blog-post``) użyte we wzorcu tej trasy.
Z tej informacji można wygenerować łatwo każdą ścieżkę URL:

.. code-block:: php

    class MainController extends Controller
    {
        public function showAction($slug)
        {
          // ...

          $url = $this->get('router')->generate('blog_show', array('slug' => 'my-blog-post'));
        }
    }

W kolejnym rozdziale poznasz jak generować ścieżki URL w szablonach.

.. tip::

    Jeśli fronton aplikacji wykorzystuje żądania AJAX, można generować ścieżki  URL
    w JavaScript na podstawie konfiguracji trasowania. Używając
    `FOSJsRoutingBundle`_, można to zrobić dokładnie tak:

    .. code-block:: javascript

        var url = Routing.generate('blog_show', { "slug": 'my-blog-post'});

    Więcej informacji mozna znaleźć w dokumentacji tego pakietu.

.. index::
   single: trasowanie; bezwględne ścieżki URL

Generowanie bezwzględnych ścieżek URL
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Domyślnie mechanizm trasowania generuje względne ścieżki URL (np. ``/blog``).
Aby wygenerować bezwzględną ścieżkę URL, trzeba przekazać ``true`` jako trzeci
argument metody ``generate()``:

.. code-block:: php

    $router->generate('blog_show', array('slug' => 'my-blog-post'), true);
    // http://www.example.com/blog/my-blog-post

.. note::

    Host używany podczas generowania bezwzględnego adresu URL jest hostem dla aktualnego
    obiektu ``Request``. Jest to wykrywane automatycznie na podstawie informacji
    o serwerze dostarczanych przez PHP. Podczas generowania bezwzglednych adresów
    URL dla skryptów uruchamianych z linii poleceń trzeba ręcznie podawać właściwy
    host dla obiektu ``Request``:

    .. code-block:: php

        $request->headers->set('HOST', 'www.example.com');

Generowanie ścieżek URL z łańcuchem zapytania
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Metoda ``generate`` pobiera tablicę wartości wieloznacznych dla generowania adresu
URI. Lecz jeśli przekaże się dodatkowe elementy tej tablicy, to zostaną one dodane
do adresu URI jako `łańcuch zapytania`_::

    $router->generate('blog', array('page' => 2, 'category' => 'Symfony'));
    // /blog/2?category=Symfony


.. index::
   single: trasowanie; generowanie adresów URL wewnątrz szablonów


Generowanie adresów URL wewnątrz szablonów
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Najczęściej wykorzystywanym miejscem do generowania adresów URL wewnątrz szablonów
są odnośniki pomiędzy stronami aplikacji. Odbywa sie to tak samo jak opisano wcześniej,
lecz za pomocą funkcji pomocniczej szablonu:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        <a href="{{ path('blog_show', { 'slug': 'my-blog-post' }) }}">
          Przeczytaj ten post bloga.
        </a>

    .. code-block:: php
       :linenos:

        <a href="<?php echo $view['router']->generate('blog_show', array('slug' => 'my-blog-post')) ?>">
            Przeczytaj ten post bloga.
        </a>

Można generować również bezwzględne ścieżki URL.

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        <a href="{{ url('blog_show', { 'slug': 'my-blog-post' }) }}">
          Przeczytaj ten post bloga.
        </a>

    .. code-block:: php
       :linenos:

        <a href="<?php echo $view['router']->generate('blog_show', array('slug' => 'my-blog-post'), true) ?>">
            Przeczytaj ten post bloga.
        </a>

Podsumowanie
------------

Trasowanie to system odwzorowania adresu URL przychodzącego żądania na funkcję kontrolera,
który ma być wywołany w celu przetworzenia żądania. Pozwala to zarówno na określanie
przyjaznych adresów URL, jak i oddzielenia funkcjonalności aplikacji od od struktury
adresów URL. Trasowanie jest dwukierunkowym mechanizmem, co oznacza, że może być
również wykorzystywany do generowania adresów URL.

Dowiedz się więcej w Receptariuszu
----------------------------------

* :doc:`/cookbook/routing/scheme`

.. _`FOSJsRoutingBundle`: https://github.com/FriendsOfSymfony/FOSJsRoutingBundle
.. _`Uniform Resource Locator`: http://pl.wikipedia.org/wiki/Uniform_Resource_Locator
.. _`Host`: http://pl.wikipedia.org/wiki/Lista_nag%C5%82%C3%B3wk%C3%B3w_HTTP
.. _`łańcuch zapytania`: http://pl.wikipedia.org/wiki/URI