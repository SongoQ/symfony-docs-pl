.. highlight:: php
   :linenothreshold: 2

.. index::
   single: szablonowanie

Tworzenie i stosowanie szablonów
================================

Wiemy już, że :doc:`kontroler </book/controller>` jest odpowiedzialny za obsługę
każdego żądania dostarczanego do aplikacji Symfony2. W rzeczywistości kontroler
przekazuje większość cięższych prac w inne miejsca, których kod może zostać
przetestowany i ponownie wykorzystany. Kiedy kontroler ma wygenerować HTML, CSS
lub inną treść, to przekazuje to działanie silnikowi szablonowania. W tym rozdziale
dowiemy się, jak napisać pełnowartościowe szablony, które mogą być użyte do zwracania
treści do użytkownika, wypełniania treści wiadomości e-mail i wiele więcej. Dowiemy
się o skrótach, pomysłowych sposobach na rozszerzanie szablonów i o tym, jak ponownie
wykorzystywać kod szablonu.

.. note::

    Jak są przetwarzane szablony, jest opisane w rozdziale
    :ref:`Kontroler <controller-rendering-templates>` tego podręcznika.


.. index::
   single: szablonowanie; Co to jest szablon?

Szablony
--------

Szablon jest plikiem tekstowym mogącym wygenerować dowolny format tekstowy
(HTML, XML, CSV, LaTeX itd.). Najbardziej popularnym typem szablonu jet *szablon
PHP* - plik tekstowy parsowany przez PHP, który zawiera mieszankę tekstu i kodu PHP:

.. code-block:: html+php
   :linenos:

    <!DOCTYPE html>
    <html>
        <head>
            <title>Welcome to Symfony!</title>
        </head>
        <body>
            <h1><?php echo $page_title ?></h1>

            <ul id="navigation">
                <?php foreach ($navigation as $item): ?>
                    <li>
                        <a href="<?php echo $item->getHref() ?>">
                            <?php echo $item->getCaption() ?>
                        </a>
                    </li>
                <?php endforeach; ?>
            </ul>
        </body>
    </html>

.. index:: single: Twig; wprowadzenie

Lecz pakiety Symfony2 są wyposażone w jeszcze bardziej silniejszy język szablonowania
o nazwie `Twig`_. Twig pozwala pisać zwięzłe, czytelne szablony na kilka sposbów,
które są bardziej przyjazne dla projektantów stron i są bardziej wydajne niż szablony PHP:

.. code-block:: html+jinja
   :linenos:

    <!DOCTYPE html>
    <html>
        <head>
            <title>Welcome to Symfony!</title>
        </head>
        <body>
            <h1>{{ page_title }}</h1>

            <ul id="navigation">
                {% for item in navigation %}
                    <li><a href="{{ item.href }}">{{ item.caption }}</a></li>
                {% endfor %}
            </ul>
        </body>
    </html>

Twig definiuje dwa rodzaje specjalnej składni:

* ``{{ ... }}``: "Przekaż coś": drukuje wartość zmiennej lub wynik wyrażenia do szablonu;

* ``{% ... %}``: "Zrób coś": znacznik kontrolujący logikę szablonu - jest stosowany
  do wykonywania instrukcji, takich jak na przykład pętla ``for``.

.. note::

   Jest jeszcze trzecia składnia używana do tworzenia komentarzy: ``{# to jest komentarz #}``.
   Składnia ta może obejmować wiele linii, stanowiąc ekwiwalent komentarza ``/* komentarz */``
   składni PHP.

Twig zawiera również **filtry**, które modyfikuja zawartość przed rozpoczęciem
renderowania. Poniższe działanie powoduje zmianę znaków wartości zmiennej ``title``
na duże litery, przed renderowaniem:

.. code-block:: jinja

    {{ title|upper }}

Twig dostarczany jest z wieloma `znacznikami <http://twig.sensiolabs.org/doc/tags/index.html>`_
i `filtrami <http://twig.sensiolabs.org/doc/filters/index.html>`_,
które są dostępne domyślnie. Do Twig można nawet `dodać własne rozszerzenia`_ , gdy jest to niezbędne.

.. tip::

    Rejestrowanie rozszerzenia Twiga jest tak proste jak tworzenie nowej usługi
    i jej zakodowanie poprzez :ref:`znacznik<reference-dic-tags-twig-extension>`
    ``twig.extension``.

Jak zobaczymy dalszej części dokumentacji, Twig również obsługuje funkcje, które
również mogą być łatwo dodawane przez użytkownika. Na przyjkład, w poniższym kodzie
użyto standardowy znacznik ``for`` i funkcję ``cycle`` do wydrukowania dziesięciu
znaczników div, na przemian z klasami ``odd``, ``even``:

.. code-block:: html+jinja
   :linenos:

    {% for i in 0..10 %}
        <div class="{{ cycle(['odd', 'even'], i) }}">
          <!-- some HTML here -->
        </div>
    {% endfor %}

W tym rozdziale przykłady szablonów będą pokazywane zarówno jako szablony Twiga jak i PHP.

.. tip::

    Jeśli zdecydujesz się nie używać Twiga i wyłączy go, to musisz zaimplementować
    własną obsługę wyjątków poprzez zdarzenie ``kernel.exception``.

.. sidebar:: Dlaczego Twig?

    Szablony Twig są proste i nie przetwarzają znaczników PHP. Jest to zgodne
    z zasadami projektownia. System szablonów Twig przeznaczony jest do szybkiej
    prezentacji, a nie do przetwarzania logiki. Im dłużej będziesz stosować Twig,
    tym bardziej doceniać zaczniesz zalety tego systemu. I oczywiście będziesz
    kochany przez projektantów na całym świecie.

    Twig może również wykonywać rzeczy, które nie można wykonać w szablonach PHP,
    jak prawdziwe dziedziczenie szablonów (szablony Twiga kompilują je do klas PHP,
    które z kolei dziedziczą po sobie), kontrola białych znaków, testowanie
    i dołączanie własnych funkcji i fitrów, które działają tylko w szablonach.
    Twig zawiera trochę cech, które czynią pisanie szablonów łatwym i bardziej
    przystępnym. Rozpatrzmy następujący przykład, który łączy pętlę z wyrażeniem
    logicznym ``if``:

    .. code-block:: html+jinja
       :linenos:

        <ul>
            {% for user in users if user.active %}
                <li>{{ user.username }}</li>
            {% else %}
                <li>No users found</li>
            {% endfor %}
        </ul>

.. index::
   single: Twig; bufor

Buforowanie szablonów Twig
~~~~~~~~~~~~~~~~~~~~~~~~~~

Twig jest szybki. Każdy szablon Twiga jest kompilowany do natywnej klasy PHP
przetwarzanej w czasie rzeczywistym. Skompilowane klasy są umieszczone w katalogu
``app/cache/{environment}/twig`` (gdzie ``{environment}``, to środowisko, takie
jak ``dev`` lub ``prod``) i w wielu przypadkach może być użyteczne podczas debugowania.
W celu uzyskania więcej informacji proszę przeczytać rozdział :ref:`environments-summary`.

Gdy włączony jest tryb ``debug`` (najczęściej w środowisku ``dev``), szablon Twiga
będzie automatycznie rekompilowany podczas wprowadzania do niego zmian. Oznacza to,
że w czasie programowania można szczęśliwie dokonać zmian w szablonie Twiga oraz
natychmiast zobaczyć zmiany, bez potrzeby martwienia się o czyszczenie jakiejkolwiek
pamięci podręcznej.

Kiedy wyłączony jest tryb ``debug`` (najczęściej w środowisku ``prod``), to po
dokonaniu zmian w szablonie Twiga konieczne jest wyczyszczenie katalogu buforowego
Twiga, tak aby szablony Twiga mogły zostać zregenerowane. Pamiętaj o tym podczas
wdrażania aplikacji.

.. index::
   single: szablonowanie; dziedziczenie

Dziedziczenie szablonów a układ strony
--------------------------------------

Niejednokrotnie szablony w projekcie współdzielą te same elementy, takie jak
nagłówek, stopka, pasek boczny i inne. W Symfony2 myślimy o tym problemie inaczej -
szablon może być dekorowany przez inny szablon. Działa to dokładnie tak samo jak
klasa PHP - dziedziczenie szablonowe umożliwia zbudowanie szablonu podstawowego
"układu strony" (ang. layout), który zawiera wszystkie wspólne elementy strony,
określane jako bloki (myśl, że to "klasa PHP z podstawowymi metodami").
Szablon potomny może rozszerzać podstawowy układ strony i przesłaniać niektóre
z jego bloków (myśl o tym jak o "podklasie PHP przesłaniającej określone metody
swojej klasy nadrzędnej").

Po pierwsze, zbuduj podstawowy plik układu strony:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        {# app/Resources/views/base.html.twig #}
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title>{% block title %}Test Application{% endblock %}</title>
            </head>
            <body>
                <div id="sidebar">
                    {% block sidebar %}
                    <ul>
                        <li><a href="/">Home</a></li>
                        <li><a href="/blog">Blog</a></li>
                    </ul>
                    {% endblock %}
                </div>

                <div id="content">
                    {% block body %}{% endblock %}
                </div>
            </body>
        </html>

    .. code-block:: html+php
       :linenos:

        <!-- app/Resources/views/base.html.php -->
        <!DOCTYPE html>
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title><?php $view['slots']->output('title', 'Test Application') ?></title>
            </head>
            <body>
                <div id="sidebar">
                    <?php if ($view['slots']->has('sidebar')): ?>
                        <?php $view['slots']->output('sidebar') ?>
                    <?php else: ?>
                        <ul>
                            <li><a href="/">Home</a></li>
                            <li><a href="/blog">Blog</a></li>
                        </ul>
                    <?php endif; ?>
                </div>

                <div id="content">
                    <?php $view['slots']->output('body') ?>
                </div>
            </body>
        </html>

.. note::

    Choć dziedziczenie szablonów jest tutaj demonstrowane w kontekście Twiga,
    to filozofia ta jest taka sama zarówno dla szablonów Twiga jak i PHP.


Szablon ten definiuje podstawowy szkielet dokumentu HTML prostej dwukolumnowej strony.
W tym przykładzie trzy obszary ``{% block %}`` są określone dla ``title``,
``sidebar`` i ``body``. Każdy blok może być przesłonięty przez szablon potomny
lub pozostawiony z domyślną implementacją. Szablon ten może być również zrenderowany
bezpośrednio. W takim przypadku bloki ``title``, ``sidebar`` i ``body`` zachowają
domyślne wartości użyte w szablonie.

Szablon potomny może wyglądać tak:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        {# src/Acme/BlogBundle/Resources/views/Blog/index.html.twig #}
        {% extends '::base.html.twig' %}

        {% block title %}My cool blog posts{% endblock %}

        {% block body %}
            {% for entry in blog_entries %}
                <h2>{{ entry.title }}</h2>
                <p>{{ entry.body }}</p>
            {% endfor %}
        {% endblock %}

    .. code-block:: html+php
       :linenos:

        <!-- src/Acme/BlogBundle/Resources/views/Blog/index.html.php -->
        <?php $view->extend('::base.html.php') ?>

        <?php $view['slots']->set('title', 'My cool blog posts') ?>

        <?php $view['slots']->start('body') ?>
            <?php foreach ($blog_entries as $entry): ?>
                <h2><?php echo $entry->getTitle() ?></h2>
                <p><?php echo $entry->getBody() ?></p>
            <?php endforeach; ?>
        <?php $view['slots']->stop() ?>

.. note::

   Szablon nadrzędny jest identyfikowany tutaj przez specjalne wyrażenie tekstowe
   składni Twiga (``::base.html.twig``), które wskazuje, że lokalizacją szablonu
   jest katalog ``app/Resources/views`` projektu. To nazewnictwo jest w pełni
   wyjaśnione w :ref:`template-naming-locations`.

Kluczem do dziedziczenia szablonów jest znacznik ``{% extends %}``. Powiadamia
on silnik szablonowania aby najpierw ocenił szablon podstawowy, który ustawia
układ strony i definiuje kilka bloków. Następnie jest przetwarzany szablon potomny
i w tym momencie bloki ``title`` i ``body`` szablonu nadrzędnego są zamienione
przez bloki z szablonu potomnego. W zależności od wartości ``blog_entries`` wyjście
może wyglądać następująco:

.. code-block:: html
   :linenos:

    <!DOCTYPE html>
    <html>
        <head>
            <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
            <title>My cool blog posts</title>
        </head>
        <body>
            <div id="sidebar">
                <ul>
                    <li><a href="/">Home</a></li>
                    <li><a href="/blog">Blog</a></li>
                </ul>
            </div>

            <div id="content">
                <h2>My first post</h2>
                <p>The body of the first post.</p>

                <h2>Another post</h2>
                <p>The body of the second post.</p>
            </div>
        </body>
    </html>

Proszę zauważyć, że skoro szablon potomny nie definiuje bloku ``sidebar``,
to używana jest zamiast tego zwartość z szablonu nadrzędnego. Zawartość ze znacznika
``{% block %}`` w szablonie nadrzędnym jest zawsze stosowana domyślnie.

Można używać wiele poziomów dziedziczenia, jeżeli jest to potrzebne. W następnym
rozdziale wyjaśniony jest trójpoziomowy model dziedziczenia oraz to, jak szablony
są organizowane wewnątrz projektu Symfony2.

Oto kilka wskazówek o których trzeba pamietać przy pracy z dziedziczeniem szablonów:

* Jeżeli używa się w szablonie znacznika ``{% extends %}``,  musi to być pierwszy
  znacznik w szablonie;

* Im więcej znaczników ``{% block %}`` stosuje się w szablonie podstawowym,
  to tym lepiej. Proszę pamiętać, że szablony potomne nie muszą definiować
  wszystkich bloków nadrzędnych, tak więc można tworzyć w szablonie podstawowym
  tyle bloków ile się potrzebuje. Im więcej ma sie bloków w szablonie podstawowym,
  tym bardziej elastyczny jest układ szablonu;

* Jeśli w szablonie znajdują się powtarzające się treści z kilku innych szablonów,
  to prawdopodobnie można przenieść taką treść do ``{% block %}`` w szablonie
  nadrzędnym. W niektórych przypadkach lepiej jest przenieść treści do nowego
  szablonu i go dołączyć (patrz :ref:`including-templates`);

* Jeśli zachodzi potrzeba pobrania treści bloku z szablonu nadrzędnego, to można
  użyć funkcji ``{{ parent() }}``. Jest to przydatne, gdy chce się dodać treść
  bloku nadrzędnego zamiast go całkowicie przesłonić:

    .. code-block:: html+jinja
       :linenos:

        {% block sidebar %}
            <h3>Table of Contents</h3>

            {# ... #}

            {{ parent() }}
        {% endblock %}

.. index::
   single: szablonowanie; konwencja nazewnicza
   single: szablonowanie; lokalizacja plików

.. _template-naming-locations:

Nazewnictwo szablonów i lokalizacje
-----------------------------------

.. versionadded:: 2.2
    W wersji 2.2 została dodana obsługa ścieżek przestrzeni, co pozwala na
    stosowanie nazw szablonów takich jak ``@AcmeDemo/layout.html.twig``.
    Patrz :doc:`/cookbook/templating/namespaced_paths` dla poznania szczegółów.

Domyślnie szablony mogą zostać umieszczone w dwu różnych lokalizacjach:

* ``app/Resources/views/``: katalog ``views`` aplikacji może zawierać szablony
  podstawowe dla całej aplikacji (tj. układy stron) a także szablony, które
  przesłaniają szablony pakietu (patrz :ref:`overriding-bundle-templates`); 

* ``path/to/bundle/Resources/views/``: każdy pakiet przechowuje swoje szablony
  w swoim katalogu ``Resources/views`` (i podkatalogach). Większość szablonów
  funkcjonuje wewnątrz pakietu.

Symfony2 używa dla odwoływania się do szablonów składni **pakiet**:**kontroler**:**szablon**.
Umożliwia to na wiele różnych typów szablonów, z których każdy znajduje się w określonej
lokalizacji:

* ``AcmeBlogBundle:Blog:index.html.twig``: Ta składnia jest używana do określenia
  szablonu dla określonej strony. Trzy części łańcucha, każdy oddzielony dwukropkiem
  (``:``) ma następujace znaczenie:

    * ``AcmeBlogBundle``: (*pakiet*) szablon znajduje się wewnątrz ``AcmeBlogBundle``
      (np. ``src/Acme/BlogBundle``);

    * ``Blog``: (*kontroler*) wskazuje, że szablon znajduje się wewnątrz podkatalogu
      ``Blog`` katalogu ``Resources/views``;

    * ``index.html.twig``: (*szablon*) aktualna nazwa pliku, to ``index.html.twig``.

  Zakładając, że ``AcmeBlogBundle`` umieszczony jest w ``src/Acme/BlogBundle``,
  to ostateczną ścieżką do układu strony będzie ``src/Acme/BlogBundle/Resources/views/Blog/index.html.twig``.

* ``AcmeBlogBundle::layout.html.twig``: Składnia ta odwołuje się do szablonu podstawowego,
  który jest specyficzny dla ``AcmeBlogBundle``. Ponieważ brakuje części "kontroler"
  (np. ``Blog``), to szablon znajduje się na ścieżce ``Resources/views/layout.html.twig``
  wewnątrz pakietu ``AcmeBlogBundle``.

* ``::base.html.twig``: Składnia ta odwołuje się do szablonu lub ogólnego układu
  strony. Proszę zauważyć, że łańcuch rozpoczyna się od dwóch dwukropków
  (``::``), co oznacza, że zarówno część "pakiet" jak część "kontroler" nie występują.
  Oznacza to, że szablon nie jest zlokalizowany w jakimkolwiek pakiecie, ale w głównej
  gałęzi w katalogu ``app/Resources/views/``.

W rozdziale :ref:`overriding-bundle-templates` dowiemy się, jak każdy szablon
umieszczony, na przykład, w ``AcmeBlogBundle``, może zostać przesłonięty przez
umieszczenie szalonu z tą samą nazwą w katalogu ``app/Resources/AcmeBlogBundle/views/``.
Daje to możliwość przesłonięcia wszystkich szablonów w pakiecie dostawcy.

.. tip::

    Proszę zwrócić uwagę, że składnia nazewnicza szablonów wygląda podobnie do
    konwencji omówionej w rozdziale :ref:`controller-string-syntax`.

Końcówka nazwy szablonu
~~~~~~~~~~~~~~~~~~~~~~~

Format **pakiet**:**kontroler**:**szablon** każdego szablonu określa gdzie znajduje
się plik szablonu. Każda nazwa szablonu ma też dwa rozszerzenia, które określają
*format* i *silnik* dla tego szablonu.

* **AcmeBlogBundle:Blog:index.html.twig** - format HTML, silnik Twig

* **AcmeBlogBundle:Blog:index.html.php** - format HTML, silnik PHP

* **AcmeBlogBundle:Blog:index.css.twig** - dormat CSS, silnik Twig

Domyślnie każdy szablon Symfony2 może być napisany dla silnika Twig albo PHP
i mieć ostatnie rozszerzenie (np. ``.twig`` albo ``.php``).
Pierwsza część rozszerzenia (np. ``.html``, ``.css`` itd.) jest ostatecznym
formatem w jakim ma zostać wygenerowany szablon. Inaczej niż rozszerzenie wskazujące
silnik, które determinuje jak parsowany będzie szablon Symfony2 , rozszerzenie
formatu jest organizacyjną taktyką stosowaną w przypadku tego samego aktywu
(*ang. asset*), który może zostać przetworzony jako HTML (``index.html.twig``),
XML (``index.xml.twig``), lub inny format. Dla uzyskania więcej informacji
proszę przeczytać rozdział :ref:`template-formats`.

.. note::

   The available "engines" can be configured and even new engines added.
   See :ref:`Templating Configuration<template-configuration>` for more details.
   Można konfigurować dostępne "silniki" a nawet dodawać nowe. W celu uzyskania
   więcej informacji proszę przeczytać rozdział
   :ref:`Konfiguracja szablonowania<template-configuration>`.

.. index::
   single: szablonowanie; znaczniki i helpery
   single: szablonowanie; helpery PHP

Znaczniki i helpery
-------------------

Już rozumiemy podstawy szablonów, jak się je nazywa i jak stosuje się dziedziczenie.
Najtrudniejsze elementy są już za nami. W tym rozdziale nauczymy się o sporej
grupie narzędzi, dostępnych aby pomóc w wykonaniu większości wspólnych zadań
wykonywanych przez szablony, takich jak dołączanie innych szablonów, tworzenie
łączy do stron, czy dołączanie obrazów.

Symfony2 dostarczany jest w pakietach zawierających kilka wyspecjalizowanych
znaczników i funkcji Twiga, które ułatwiają pracę projektantom szablonów.
System szablonowania w PHP dostarcza rozszerzalny system *helperów*, które
umożliwiających skorzystanie z użytecznych funkcjonalności w kontekście szablonu.

Już widzieliśmy kilka wbudowanych znaczników Twiga (``{% block %}`` i ``{% extends %}``),
jak też przykład helpera PHP (``$view['slots']``). Nauczmy sie więcej.

.. index::
   single: szablonowanie; dołączanie innych szablonów

.. _including-templates:

Dołączanie innych szablonów
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Często występuje potrzeba dołączenia tego samego szablonu lub tego samego fragmentu
kodu na różnych stronach. Na przykład, w aplikacji z "artykułami prasowymi" kod
szablonu wyświetla streszczenie artykułu mogące być użyte na stronie szczegółowego
artykułu, na stronie wyświetlającej najpardziej popularne artykuły lub na liście
najnowszych artykułów.

Kiedy trzeba użyć wielokrotnie porcji kodu PHP, to zazwyczaj przenosi się ten kod
do nowej klasy PHP lub funkcji. Podobnie jest w przypadku szablonów. Przenosząc
wielokrotnie wykorzystywany kod do odrębnego szablonu można ten szablon dołączać
do każdego innego szablonu. Najpierw trzeba utworzyć szablon, który będzie mógł
być wykorzystywany wielokrotnie.

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        {# src/Acme/ArticleBundle/Resources/views/Article/articleDetails.html.twig #}
        <h2>{{ article.title }}</h2>
        <h3 class="byline">by {{ article.authorName }}</h3>

        <p>
            {{ article.body }}
        </p>

    .. code-block:: html+php
       :linenos:

        <!-- src/Acme/ArticleBundle/Resources/views/Article/articleDetails.html.php -->
        <h2><?php echo $article->getTitle() ?></h2>
        <h3 class="byline">by <?php echo $article->getAuthorName() ?></h3>

        <p>
            <?php echo $article->getBody() ?>
        </p>

Dołączanie tego szablonu do innego jest proste:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        {# src/Acme/ArticleBundle/Resources/views/Article/list.html.twig #}
        {% extends 'AcmeArticleBundle::layout.html.twig' %}

        {% block body %}
            <h1>Recent Articles<h1>

            {% for article in articles %}
                {{ include('AcmeArticleBundle:Article:articleDetails.html.twig', {'article': article}) }}
            {% endfor %}
        {% endblock %}

    .. code-block:: html+php
       :linenos:

        <!-- src/Acme/ArticleBundle/Resources/Article/list.html.php -->
        <?php $view->extend('AcmeArticleBundle::layout.html.php') ?>

        <?php $view['slots']->start('body') ?>
            <h1>Recent Articles</h1>

            <?php foreach ($articles as $article): ?>
                <?php echo $view->render(
                    'AcmeArticleBundle:Article:articleDetails.html.php',
                    array('article' => $article)
                ) ?>
            <?php endforeach; ?>
        <?php $view['slots']->stop() ?>

Szablon jest dołączany przy użyciu znacznika ``{% include %}``. Proszę zwrócić uwagę,
że nazwa szablonu składana jest według tej samej typowej konwencji.
Szablon ``articleDetails.html.twig`` używa zmiennej ``article``.
Ta jest przekazywana przez szablon ``list.html.twig`` przy użyciu polecenia ``with``.

.. tip::

    Składnia ``{'article': article}`` jest standardową składnią Twiga dla map asocjacyjnych
    (czyli tablic z nazwanymi kluczami). Jeśli trzeba przekazać wiele elementów,
    będzie to wygladać tak: ``{'foo': foo, 'bar': bar}``.

.. versionadded:: 2.2
    Funkcja ``include()`` jest nową funkcją Twiga, udostępnioną w Symfony 2.2.
    Poprzednio stosowany był znacznik ``{% include %}``.

.. index::
   single: szablonowanie; osadzanie kontrolerów 

.. _templating-embedding-controller:

Osadzanie kontrolerów
~~~~~~~~~~~~~~~~~~~~~

W niektórych przypadkach trzeba zrobić więcej niż tylko prosty szablon.
Powiedzmy, że mamy w układzie strony pasek boczny, który zawiera trzy najnowsze
artykuły. Pobieranie tych trzech artykułów obejmuje zapytania do bazy danych
lub wykonanie innej skomplikowanej logiki, których to elementów nie da się zrobić
wewnątrz szablonu.

Rozwiązaniem jest osadzenie w szablonie wyniku działania całego kontrolera.
Najpierw trzeba utworzyć kontroler, który przetwarza pewną liczbę najnowszych
artykułów::

    // src/Acme/ArticleBundle/Controller/ArticleController.php
    class ArticleController extends Controller
    {
        public function recentArticlesAction($max = 3)
        {
            // make a database call or other logic
            // to get the "$max" most recent articles
            $articles = ...;

            return $this->render(
                'AcmeArticleBundle:Article:recentList.html.twig',
                array('articles' => $articles)
            );
        }
    }

Szablon ``recentList`` jest bardzo prosty:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        {# src/Acme/ArticleBundle/Resources/views/Article/recentList.html.twig #}
        {% for article in articles %}
            <a href="/article/{{ article.slug }}">
                {{ article.title }}
            </a>
        {% endfor %}

    .. code-block:: html+php
       :linenos:

        <!-- src/Acme/ArticleBundle/Resources/views/Article/recentList.html.php -->
        <?php foreach ($articles as $article): ?>
            <a href="/article/<?php echo $article->getSlug() ?>">
                <?php echo $article->getTitle() ?>
            </a>
        <?php endforeach; ?>

.. note::

    Proszę zauważyć, że w tym przykładzie adres URL jest zakodowany sztywno
    (tj. ``/article/*slug*``). Jest to zła praktyka. W następnym rozdziale poznamy
    jak to wykonać prawidłowo.

Aby dołączyć kontroler, trzeba się do niego odwołać używając standardowej składni
(tj. **pakiet**:**kontroler**:**akcja**):

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        {# app/Resources/views/base.html.twig #}

        {# ... #}
        <div id="sidebar">
            {{ render(controller('AcmeArticleBundle:Article:recentArticles', { 'max': 3 })) }}
        </div>

    .. code-block:: html+php
       :linenos:

        <!-- app/Resources/views/base.html.php -->

        <!-- ... -->
        <div id="sidebar">
            <?php echo $view['actions']->render(
                new ControllerReference('AcmeArticleBundle:Article:recentArticles', array('max' => 3))
            ) ?>
        </div>

Ilekroć zajdzie potrzeba użycia zmiennej lub porcji informacji do których nie ma
się dostępu w szablonie, to warto rozważyć przetwarzanie kontrolerem. Kontrolery
są szybkie w wykonaniu i promują dobrą organizacje kodu i możliwość jego wielokrotnego
wykorzystania.

.. index:: hinclude.js
      single: szablonowanie; hinclude.js
      single: szablonowanie; render
      single: helper; render 


Asynchroniczna zawartość z hinclude.js
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.1
    W Symfony 2.1 została dodana obsługa biblioteki hinclude.js.

Kontrolery mogą być osadzana asynchronicznie przy wykorzystaniu biblioteki
JavaScript `hinclude.js`_. Jako że osadzana treść pochodzi z innej strony (lub
kontrolera w tym przypadku), to Symfony2 używa standardowego helpera ``render``
do konfigurowania znaczników ``hinclude.js``:

.. configuration-block::

    .. code-block:: jinja
       :linenos:

        {{ render_hinclude(controller('...')) }}

        {{ render_hinclude(url('...')) }}

    .. code-block:: php
       :linenos:

        <?php echo $view['actions']->render(
            new ControllerReference('...'),
            array('renderer' => 'hinclude')
        ) ?>

        <?php echo $view['actions']->render(
            $view['router']->generate('...'),
            array('renderer' => 'hinclude')
        ) ?>

.. note::

   Aby działać, hinclude.js musi zosytać dołączona do strony.

.. note::

   Podczas używania kontrolera zamiast adresu URL, należy włączyć opcję ``fragments``
   w konfiguracji Symfony:

    .. configuration-block::

        .. code-block:: yaml
           :linenos:

            # app/config/config.yml
            framework:
                # ...
                fragments: { path: /_fragment }

        .. code-block:: xml
           :linenos:

            <!-- app/config/config.xml -->
            <framework:config>
                <framework:fragments path="/_fragment" />
            </framework:config>

        .. code-block:: php
           :linenos:

            // app/config/config.php
            $container->loadFromExtension('framework', array(
                // ...
                'fragments' => array('path' => '/_fragment'),
            ));

Domyślną zawartość (wyświetlaną w czasie ładowania lub gdy wyłączona jest obsługa
JavaScript) można ustawić w konfiguracji aplikacji:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        framework:
            # ...
            templating:
                hinclude_default_template: AcmeDemoBundle::hinclude.html.twig

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:templating hinclude-default-template="AcmeDemoBundle::hinclude.html.twig" />
        </framework:config>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'templating'      => array(
                'hinclude_default_template' => array('AcmeDemoBundle::hinclude.html.twig'),
            ),
        ));

.. versionadded:: 2.2
      W Symfony 2.2 zostały dodane domyślne szablony z funkcją render 
    

Można zdefiniować domyślne szablony z funkcją ``render`` (które przesłaniają
wszystkie zdefiniowane globalne szablony):


.. configuration-block::

    .. code-block:: jinja
       :linenos:

        {{ render_hinclude(controller('...'),  {'default': 'AcmeDemoBundle:Default:content.html.twig'}) }}

    .. code-block:: php
       :linenos:

        <?php echo $view['actions']->render(
            new ControllerReference('...'),
            array(
                'renderer' => 'hinclude',
                'default' => 'AcmeDemoBundle:Default:content.html.twig',
            )
        ) ?>

Albo można również określić łańcuch tekstowy do wyświetlenia jako domyślną zawartość:

.. configuration-block::

    .. code-block:: jinja
       :linenos:

        {{ render_hinclude(controller('...'), {'default': 'Loading...'}) }}

    .. code-block:: php
       :linenos:

        <?php echo $view['actions']->render(
            new ControllerReference('...'),
            array(
                'renderer' => 'hinclude',
                'default' => 'Loading...',
            )
        ) ?>

.. index::
   pair: szablonowanie; odnośniki do stron
   single: szablonowanie; funkcja path()
   single: funkcje szablonowe; path()

.. _book-templating-pages:

Odnośniki do stron
~~~~~~~~~~~~~~~~~~

Tworzenie łączy do innych stron aplikacji jest jedną z najczęstrzych czynności
przy wykonywaniu szablonu. Aby wygenerować adresy URL oparte o konfigurację trasowania,
to zamiast umieszczać w szablonie sztywne adresy URL, należy wykorzystywać funkcję
``path`` Twiga (lub helper ``router`` w szablonie PHP). Później, jeśli chce się
zmodyfikować adres URL danej strony, to wystarczy zmienić konfigurację trasowania.
Szablony wygenerują wówczas automatycznie nowy adres URL.

Najpierw zlinkujmy stronę "_welcome", która jest dostępna poprzez następującą
konfigurację trasowania:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        _welcome:
            path:     /
            defaults: { _controller: AcmeDemoBundle:Welcome:index }

    .. code-block:: xml
       :linenos:

        <route id="_welcome" path="/">
            <default key="_controller">AcmeDemoBundle:Welcome:index</default>
        </route>

    .. code-block:: php
       :linenos:

        $collection = new RouteCollection();
        $collection->add('_welcome', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Welcome:index',
        )));

        return $collection;

Aby utworzyć łącze do strony, wystarczy użyć funkcji ``path`` Twiga i odnieść się
do odpowiedniej trasy:

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('_welcome') }}">Home</a>

    .. code-block:: html+php

        <a href="<?php echo $view['router']->generate('_welcome') ?>">Home</a>

Zgodnie z oczkiwaniami wygenuruje to adres URL ``/``. Zobaczmy jak działa to
z bardziej skomplikowaną trasą:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        article_show:
            path:     /article/{slug}
            defaults: { _controller: AcmeArticleBundle:Article:show }

    .. code-block:: xml
       :linenos:

        <route id="article_show" path="/article/{slug}">
            <default key="_controller">AcmeArticleBundle:Article:show</default>
        </route>

    .. code-block:: php
       :linenos:

        $collection = new RouteCollection();
        $collection->add('article_show', new Route('/article/{slug}', array(
            '_controller' => 'AcmeArticleBundle:Article:show',
        )));

        return $collection;

W tym przypadku, należy określić zarówno nazwę trasy (``article_show``),
jak i wartość parametru ``{slug}``. Używając tej trasy, przeróbmy szablon
``recentList`` z poprzedniego rozdziału i stwórzmy prawidłowe odnośnik do artykułów:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        {# src/Acme/ArticleBundle/Resources/views/Article/recentList.html.twig #}
        {% for article in articles %}
            <a href="{{ path('article_show', {'slug': article.slug}) }}">
                {{ article.title }}
            </a>
        {% endfor %}

    .. code-block:: html+php
       :linenos:

        <!-- src/Acme/ArticleBundle/Resources/views/Article/recentList.html.php -->
        <?php foreach ($articles in $article): ?>
            <a href="<?php echo $view['router']->generate('article_show', array('slug' => $article->getSlug()) ?>">
                <?php echo $article->getTitle() ?>
            </a>
        <?php endforeach; ?>

.. tip::

    Można również wygenerować bezwzględny adres URL stosując funkcję ``url`` Twiga:

    .. code-block:: html+jinja

        <a href="{{ url('_welcome') }}">Home</a>

    To samo można zrobić w szablonach PHP przez przekazanie do metody trzeciego
    argumentu ``generate()``:

    .. code-block:: html+php
       :linenos:

        <a href="<?php echo $view['router']->generate(
            '_welcome',
            array(),
            true
        ) ?>">Home</a>

.. index::
   single: szablonowanie; odnośniki do aktywów
   single: szablonowanie; funkcja assets()
   single: funkcje szablonowe; assetss()

.. _book-templating-assets:

Odnośniki do aktywów
~~~~~~~~~~~~~~~~~~~~

Szablony często również odwołują się do obrazów, skryptów Javascript, arkuszy stylów
i innych :term:`aktywów<aktywa>`. Oczywiście można podawać sztywne ścieżki dostępu do
tych aktywów (np. ``/images/logo.png``), ale Symfony2 oferuje bardziej dynamiczny sposób
poprzez funkcję ``assets``:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        <img src="{{ asset('images/logo.png') }}" alt="Symfony!" />

        <link href="{{ asset('css/blog.css') }}" rel="stylesheet" type="text/css" />

    .. code-block:: html+php
       :linenos:

        <img src="<?php echo $view['assets']->getUrl('images/logo.png') ?>" alt="Symfony!" />

        <link href="<?php echo $view['assets']->getUrl('css/blog.css') ?>" rel="stylesheet" type="text/css" />

Głównym celem funkcji ``asset`` jest uczynienie aplikacji bardziej przenośną.
Jeżeli aplikacja zlokalizowana jest w głównym katalogu hosta (np. ``http://example.com``),
wówczas generowaną ścieżką powinno być ``/images/logo.png``. Lecz jeśli aplikacja
umieszczona jest w podkatalogu (np. ``http://example.com/my_app``), to ścieżka
każdego aktywu powinna zostać wygenerowana z podkatalogiem (np. ``/my_app/images/logo.png``).
Funkcja ``asset`` rozwiązuje ten problem i generuje odpowiednie ścieżki.

Dodatkowo, w przypadku korzystania z funkcji ``asset``, Symfony może automatycznie
dołączać łańcuch zapytania do :term:`aktywu<aktywa>`, w celu zagwarantowania, że aktualizowane
statyczne aktywa nie będą buforowane w czasie wykorzystywania.
Na przykład, ``/images/logo.png`` wyglądać jak ``/images/logo.png?v2``.
Więcej informacji na tenm temat można znależć w :ref:`ref-framework-assets-version`.

.. index::
   single: szablonowanie; dołączanie arkuszy stylów
   single: szablonowanie; dołączanie skryptów JavaScript 
   single: arkusze stylów; dołączanie arkuszy stylów
   single: JavaScript; dołączanie skryptów JavaScript

Dołącznie w Twig arkuszy stylów i skryptów JavaScript
-----------------------------------------------------

Żadna strona nie byłaby kompletna bez dołaczonych plików Javascript i arkuszy stylów.
W Symfony dołączanie tych :term:`aktywów<aktywa>` jest obsługiwane elegancko przez
wykorzystanie zaawansowanego dziedziczenia szablonów.

.. tip::

    Ten rozdział traktuje o filozofii stojącej za dołączaniem w Symfony arkuszy
    stylów i aktywów Javascript. Symfony posiada również pakiet o nazwie Assetic,
    któremu towarzyszy ta filozofia, ale też pozwala na wykonanie wielu interesujacych
    rzeczy z tymi aktywami. Więcej informacji o stosowaniu Assetic można znaleźć
    w artykule :doc:`Jak używać Assetic do zarządzania aktywami</cookbook/assetic/asset_management>`.


Rozpocznijmy od dodania dwóch bloków do podstawowego szablonu, który będzie
przejmował aktywa: jeden o nazwie ``stylesheets`` wewnątrz znacznika ``head`` a drugi
o nazwie ``javascripts`` zaraz powyżej znacznika zamykającego ``body``.
Bloki te będę zawierać wszystkie arkusze stylów i skrypty Javascripts jakie są
potrzebne w całej witrynie:

.. code-block:: html+jinja
   :linenos:

    {# app/Resources/views/base.html.twig #}
    <html>
        <head>
            {# ... #}

            {% block stylesheets %}
                <link href="{{ asset('/css/main.css') }}" type="text/css" rel="stylesheet" />
            {% endblock %}
        </head>
        <body>
            {# ... #}

            {% block javascripts %}
                <script src="{{ asset('/js/main.js') }}" type="text/javascript"></script>
            {% endblock %}
        </body>
    </html>

To proste! Ale co, gdy zajdzie potrzeba dołączenia w szablonie potomnym dodatkowego
arkusza stylów lub pliku Javascript? Na przykład załóżmy, że mamy stronę kontaktową
i potrzebujemy dołączyć arkusz stylów ``contact.css`` tylko na tej stronie.
Wewnątrz szablonu strony kontaktowej trzeba zrobić co następuje:

.. code-block:: html+jinja
   :linenos:

    {# src/Acme/DemoBundle/Resources/views/Contact/contact.html.twig #}
    {% extends '::base.html.twig' %}

    {% block stylesheets %}
        {{ parent() }}

        <link href="{{ asset('/css/contact.css') }}" type="text/css" rel="stylesheet" />
    {% endblock %}

    {# ... #}

.. index::
      single: szablonowanie; funkcja parent()
      single: funkcje szablonowe parent() 

W szablonie potomnym można przesłonić blok ``stylesheets`` szablonu podstawowego.
W tym celu trzeba umieścić w szablonie potomnym blok ``stylesheets`` a w nim odwołanie
do nowego pliku arkusza stylów. Oczywiście nie chcemy, aby nowy plik arkusza stylów
zastępował style określone w szablonie podstawowym - chcemy tylko dodać dodatkowy
arkusze stylów. Dlatego też, w szablonie potomnym, przed odwołaniem się do nowego
pliku arkusza stylów musimy umieścić funkcję ``parent()`` Twiga, aby dołaczyć wszystko
z bloku stylesheets z szablonu podstawowego.

Można również dołączyć aktywa zlokalizowane w folderze ``Resources/public``
swojego pakietu.
Trzeba też będzie uruchomić polecenie ``php app/console assets:install target [--symlink]``,
które przeniesie (lub dowiąże) pliki do prawidłowej lokalizacji. Parametr ``target``
to domyślnie "web". Użycie parametru ``--symlink`` spowoduje utworzenie dowiązania
symbolicznego.

Wiersz linkujący w szablonie w naszym przykładzie teraz wyglądał będzie tak:

.. code-block:: html+jinja

   <link href="{{ asset('bundles/acmedemo/css/contact.css') }}" type="text/css" rel="stylesheet" />

Wynikiem końcowym jest strona, która zawiera arkusze stylów, zarówno ``main.css``
jak i ``contact.css``.

.. index:: zmienne globalne szablonu
      single: szablonowanie; zmienne globalne szablonu
      single: zmienna globalna; app

Zmienne globalne szablonu
-------------------------

During each request, Symfony2 will set a global template variable ``app``
in both Twig and PHP template engines by default.  The ``app`` variable
is a :class:`Symfony\\Bundle\\FrameworkBundle\\Templating\\GlobalVariables`
instance which will give you access to some application specific variables
automatically:
Podczas każdego żądania Symfony2 ustawia domyślnie szablonową zmienną globalną ``app``,
zarówno dla silnika szablonowego Twig jak i PHP. Zmienna ``app`` jest instancją
:class:`Symfony\\Bundle\\FrameworkBundle\\Templating\\GlobalVariables`
dającej automatyczny dostęp do określonych zmiennych:

* ``app.security`` - kontekst systemu bezpieczeństwa;
* ``app.user`` - obiekt bieżącego użytkownika;
* ``app.request`` - obiekt żądania;
* ``app.session`` - obiekt sesji;
* ``app.environment`` - bieżace środowisko (dev, prod, itd.).
* ``app.debug`` - ``true`` jeżeli aplikacja jest w trybie debug, w przeciwnym razie ``false``.

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        <p>Username: {{ app.user.username }}</p>
        {% if app.debug %}
            <p>Request method: {{ app.request.method }}</p>
            <p>Application Environment: {{ app.environment }}</p>
        {% endif %}

    .. code-block:: html+php
       :linenos:

        <p>Username: <?php echo $app->getUser()->getUsername() ?></p>
        <?php if ($app->getDebug()): ?>
            <p>Request method: <?php echo $app->getRequest()->getMethod() ?></p>
            <p>Application Environment: <?php echo $app->getEnvironment() ?></p>
        <?php endif; ?>

.. tip::

     Można dodawać własne globalne zmienne szablonowe. Zobacz przykład na
     :doc:`Zmienne globalne</cookbook/templating/global_variables>`.

.. index::
   single: szablonowanie; usługa templating
   single: usługa; templating

Konfigurowanie i używanie usługi templating
-------------------------------------------

Sercem systemu szablonów Symfony2 jest obiekt ``Engine``. Ten szczególny obiekt
jest odpowiedzialny za przetwarzanie szablonów i zwracanie ich zawartości.
Podczas przetwarzania szablonu w kontrolerze, w rzeczywistości wykorzystywana jest
usługa silnika szablonowania. Na przykład::

    return $this->render('AcmeArticleBundle:Article:index.html.twig');

jest równoważne z::

    use Symfony\Component\HttpFoundation\Response;

    $engine = $this->container->get('templating');
    $content = $engine->render('AcmeArticleBundle:Article:index.html.twig');

    return $response = new Response($content);

.. _template-configuration:

Ten silnik szablonowania (lub "usługa") jest wstępnie skonfigurowany do automatycznej
pracy wewnątrz Symfony2. Można oczywiście to skonfigurować samemu w pliku konfiguracyjnym
aplikacji:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        framework:
            # ...
            templating: { engines: ['twig'] }

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <framework:templating>
            <framework:engine id="twig" />
        </framework:templating>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...

            'templating' => array(
                'engines' => array('twig'),
            ),
        ));

Dostępne są różne opcje konfiguracyjne i omówione są one w
:doc:`dodatku Konfiguracja</reference/configuration/framework>`.

.. note::

   Silnik ``twig`` jest obowiązkowy do używania webprofilera (jak również wielu
   niezależnych pakietów).

.. index::
    single: szablonowanie; przesłanianie szablonów

.. _overriding-bundle-templates:

Przesłanianie szablonów pakietowych
-----------------------------------

Społeczność Symfony2 szczyci się tworzeniem i utrzymywaniem wysokiej jakości pakietów
(zobacz `KnpBundles.org`_ aby zapoznać się z wielką ilością różnych funkcjonalności).
W razie użycia niezależnego pakietu często trzeba przesłonić i dostosować jeden lub
więcej jego szablonów.

Załóżmy, że dodaliśmy do swojego projektu wyimaginowany pakiet ``AcmeBlogBundle``
o otwartym kodzie (np. w katalogu ``src/Acme/BlogBundle``). Następnie zdecydowaliśmy
się na przesłonięcie strony "list" blogu, tak aby dostosować specyficzne znaczniki
do naszej aplikacji. Badając kontroler Blog pakietu ``AcmeBlogBundle``,
znaleźliśmy to::

    public function indexAction()
    {
        // some logic to retrieve the blogs
        $blogs = ...;

        $this->render(
            'AcmeBlogBundle:Blog:index.html.twig',
            array('blogs' => $blogs)
        );
    }


Kiedy przetwarzany jest szablon ``AcmeBlogBundle:Blog:index.html.twig,``
to Symfony2 wyszukuje szablony w dwóch różnych lokalizacjach:

#. ``app/Resources/AcmeBlogBundle/views/Blog/index.html.twig``
#. ``src/Acme/BlogBundle/Resources/views/Blog/index.html.twig``

Aby przesłonić szablon pakietu wystarczy skopiować szablon ``index.html.twig``
z pakietu do ``app/Resources/AcmeBlogBundle/views/Blog/index.html.twig``
(katalog ``app/Resources/AcmeBlogBundle`` nie będzie istniał, więc trzeba go utworzyć).
Można teraz dowolnie dostosować ten szablon do swoich wymagań.

.. caution::

    Jeśli doda się szablon w nowym miejscu, może okazać się konieczne wyczyszczenie
    pamięci podręcznej ( ``php app/console cache:clear`` ), nawet jeśli się jest
    w trybie debugowania.

Logika ta ma również zastosowanie do podstawowych szablonów pakietów. Załóżmy, że
każdy szablon w ``AcmeBlogBundle`` dziedziczy z szablonu podstawowego o nazwie
``AcmeBlogBundle::layout.html.twig``. Podobnie jak wcześniej, Symfony2 będzie
wyszukiwało szablony w dwóch miejscach:

#. ``app/Resources/AcmeBlogBundle/views/layout.html.twig``
#. ``src/Acme/BlogBundle/Resources/views/layout.html.twig``

Po raz kolejny, aby przesłonić szablon, wystarczy skopiować go z pakietu do
``app/Resources/AcmeBlogBundle/views/layout.html.twig``. Można teraz swobodnie
przystosować kopię do swoich potrzeb.

Symfony2 zawsze rozpoczyna wyszukiwanie szablonów w katalogu ``app/Resources/{BUNDLE_NAME}/views/``.
Jeśli szablon nie istnieje tam, to kontynuuje i sprawdza wewnątrz katalogu
``Resources/views`` pakietu. Oznacza to, że wszystkie szablony pakietu mogą zostać
przesłoniete przez umieszczenie ich w odpowiednim podkatalogu ``app/Resources``.

.. note::

    Można również przesłaniać szablony w pakietach stosując dziedziczenie pakietowe.
    Więcej informacji na ten temat uzyskasz w artykule :doc:`/cookbook/bundles/inheritance`

.. _templating-overriding-core-templates:

.. index::
    single: szablonowanie; nadpisywanie szablonów wyjątków

Przesłanianie szablonów rdzenia
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Począwszy od Symfony2 rdzeń jest sam w sobie pakietem, tak więc szablony rdzenia
mogą być przesłaniane w ten sam sposób. Na przykład, rdzenny ``TwigBundle`` zawiera
szereg różnych szablonów dla "wyjątków" i "błędów", które mogą zostać przesłonięte
przez skopiowanie ich z katalogu ``Resources/views/Exception`` pakietu ``TwigBundle``
do katalogu ``app/Resources/TwigBundle/views/Exception``.

.. index::
   single: szablonowanie; trzy poziomy dziedziczenia

Trzy poziomy dziedziczenia
--------------------------

Jednym ze sposobów zastosowania dziedziczenia jest użycie podejścia trójpoziomowego.
Ta metoda działa doskonale z trzema różnymi typami szablonów, które właśnie omówimy:

* Utwórzmy plik ``app/Resources/views/base.html.twig``, który zawiera główny układ
  dla aplikacji (podobnie jak w poprzednim przykładzie). Wewnętrznie do szablonu
  tego będziemy się odwoływać przez ``::base.html.twig``;

* Utwórzmy szablon dla każdej "sekcji" witryny. Na przykład, ``AcmeBlogBundle``,
  miałby szablon o nazwie ``AcmeBlogBundle::layout.html.twig``, zawierający tylko
  elementy specyficzne dla blogu:

  .. code-block:: html+jinja
     :linenos:

      {# src/Acme/BlogBundle/Resources/views/layout.html.twig #}
      {% extends '::base.html.twig' %}

      {% block body %}
          <h1>Blog Application</h1>

          {% block content %}{% endblock %}
      {% endblock %}

* Utwórzmy indywidualny szablon dla każdej strony i rozrzerzmy szablon każdej sekcji.
  Na przykład, strona "index" będzie wywoływana przez coś takiego, jak
  ``AcmeBlogBundle:Blog:index.html.twig`` i zawierać będzie wykaz aktualnych wpisów blogu:

  .. code-block:: html+jinja
     :linenos:

      {# src/Acme/BlogBundle/Resources/views/Blog/index.html.twig #}
      {% extends 'AcmeBlogBundle::layout.html.twig' %}

      {% block content %}
          {% for entry in blog_entries %}
              <h2>{{ entry.title }}</h2>
              <p>{{ entry.body }}</p>
          {% endfor %}
      {% endblock %}

Proszę zauważyć, że szablon ten rozszerza szablon sekcji - (``AcmeBlogBundle::layout.html.twig``)
który z kolei rozszerza bazowy układ aplikacji (``::base.html.twig``). Jest to typowy
model dziedziczenia trójpoziomowego.

Budując aplikację, można wybrać tą metodę lub po prostu wykonać sazablon każdej strony,
rozszerzając bezpośrednio bazowy szablon aplikacji (np. ``{% extends '::base.html.twig' %}``).
Model trójpoziomowy jest metodą dobrych praktyk stosowaną przez dostawcą pakietów,
tak aby szablon bazowy pakietu mógł być łatwo przesłaniany aby odpowiednio rozszerzyć
podstawowy układ aplikacji.

.. index::
   single: szablonowanie; zabezpieczenie zmiennych

Zabezpieczenie zmiennych
------------------------

Podczas generowania kodu HTML z szablonu zawsze istnieje ryzyko, że zmienna szablonowa
może wyprowadzić niezamierzony kod HTML lub niebezpieczny kod wprowadzony przez klienta.
W efekcie dynamiczna zawartość może załamać kod HTML strony lub umożliwić złośliwemu
użytkownikowi przeprowadzenie ataku `Cross Site Scripting`_ (XSS). Rozważmy następujacy
przykład:

.. configuration-block::

    .. code-block:: jinja

        Hello {{ name }}

    .. code-block:: html+php

        Hello <?php echo $name ?>

Wyobraźmy sobie, że użytkownik wprowadza następujący kod jako swoją nazwę::

    <script>alert('hello!')</script>

Bez zastosownia jakiegokolwiek zabezpieczenia zmiennych, wynikowy szablon wyprowadzi
kod wyskakującego okienka alertu JavaScript::

    Hello <script>alert('hello!')</script>

Choć wydaje się to nieszkodliwe, to jednak użytkownik taki może pójść dalej
i wprowadzić kod JavaScript, który wykona szkodliwe działania.

Rozwiązaniem problemu jest tzw. zabezpieczenie zmiennych (*ang. escaping*).
Przy zabezpieczeniu zmiennych dane wyjściowe tego samego szablonu będą przetwarzane
bezpiecznie, drukując na ekranie literalnie znacznik script::

    Hello &lt;script&gt;alert(&#39;helloe&#39;)&lt;/script&gt;

Systemy szablonowania Twig i PHP podchodzą do tego problemu w odmienny sposób.
Jeżeli używa się Twig, zabezpieczenie zmiennych jest domyślnie włączone i jest
się chronionym. W PHP zabezpieczenie zmiennych nie jest automatyczne, co oznacza,
że trzeba ręcznie zabezpieczać zmienne, gdy jest to potrzebne.

Zabezpieczenie zmiennych w Twig
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jeżeli używa się szablonów Twig, wówczas zabezpieczanie zmiennych jest domyślnie
włączone. Oznacza to, że jest się chronionym od momentu instalacji przed niezamierzonymi
konsekwencjami kodu wprowadzanego przez użytkownika. Zakłada się domyślnie, że
zabezpieczenie zmiennych obejmuje wyjściowy kod w formacie HTML.

W niektórych przypadkach zachodzi potrzeba wyłączenia zabezpieczenia zmiennych,
które są zaufane i zawierają znaczniki, które nie powinny być zamieniane na encje
znakowe. Załóżmy, że użytkownicy grupy administratorów mogą pisać artykuły, które
zawierają kod HTML. Domyślnie Twig będzie zabezpieczał ciało artykułu, zamieniając
znaki charakterystyczne dla znaczników HTML na odpowiednie encje znakowe. Aby to
normalnie przetworzyć (bez zamiany na encje), trzeba dodać filtr ``raw``:
``{{ article.body | raw }}``.

Można również wyłączyć zabezpieczenie zmiennych wewnątrz obszaru ``{% block %}``
lub dla całego szablonu. Więcej informacji na ten temat można znaleźć w rozdziale
`Output Escaping`_ w dokumentacji Twig.

Zabezpieczenie zmiennych w PHP
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

W szablonach PHP zabezpieczenie zmiennych nie jest automatyczne. Oznacza to, że
jeśli trzeba zastosować zabezpieczenie zmiennych, to trzeba to uczynić samemu.
Aby zastosować zabezpieczenie zmiennej należy użyć specjalnej metody widoku
``escape()``::

    Hello <?php echo $view->escape($name) ?>

Domyślnie metoda ``escape()`` zakłada, że zmienna zostanie przetworzona w kontekście
HTML (a więc zmienna jest zabezpieczana pod kątem bezpieczeństwa kodu HTML). Drugi
argument pozwala zmienić kontekst. Na przykład, aby wyprowadzić zabezpieczenie przed
kodem JavaScript, należy użyć kontekst ``js``:

.. code-block:: js

    var myMsg = 'Hello <?php echo $view->escape($name, 'js') ?>';

.. index::
   single: szablonowanie; formaty

.. _template-formats:

Debugowanie
-----------

.. versionadded:: 2.0.9
    Funkcjonalność ta jest dostępna od Twig 1.5.x, który po raz pierwszy został
    włączony do Symfony 2.0.9.
     

Gdy stosuje się PHP, można użyć ``var_dump()`` do szybkiego znalezienia wartości
jakiejś przekazanej zmiennej. Jest tu użyteczne, na przykład wewnątrz kontrolera.
To samo można uzyskać przy stosowaniu Twig poprzez wykorzystanie rozszerzenia Debug.
Trzeba to włączyć w konfiguracji:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        services:
            acme_hello.twig.extension.debug:
                class:        Twig_Extension_Debug
                tags:
                     - { name: 'twig.extension' }

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <services>
            <service id="acme_hello.twig.extension.debug" class="Twig_Extension_Debug">
                <tag name="twig.extension" />
            </service>
        </services>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition('Twig_Extension_Debug');
        $definition->addTag('twig.extension');
        $container->setDefinition('acme_hello.twig.extension.debug', $definition);

Parametry szablonowe mogą być następnie zrzucane przy użyciu funkcji ``dump``:

.. code-block:: html+jinja
   :linenos:

    {# src/Acme/ArticleBundle/Resources/views/Article/recentList.html.twig #}
    {{ dump(articles) }}

    {% for article in articles %}
        <a href="/article/{{ article.slug }}">
            {{ article.title }}
        </a>
    {% endfor %}


Zmienne będę zrzucane tylko gdy ustawienie ``debug`` Twiga (w ``config.yml``)
ma wartość ``true``. Domyślnie oznacza to, że zmienne będą zrzucane w środowisku
``dev`` ale nie ``prod``.

Sprawdzanie składni
-------------------

.. versionadded:: 2.1
      Polecenie ``twig:lint`` zostało dodane w Symfony 2.1

Można sprawdzić poprawność składni w szablonie Twig stosując polecenie konsoli
``twig:lint``:

.. code-block:: bash

    # Można sprawdzić przez nazwę pliku:
    $ php app/console twig:lint src/Acme/ArticleBundle/Resources/views/Article/recentList.html.twig

    # lub przez katalog:
    $ php app/console twig:lint src/Acme/ArticleBundle/Resources/views

    # lib używając nazwy pakietu:
    $ php app/console twig:lint @AcmeArticleBundle

Formaty szablonów
-----------------

Szablony są ogólnym sposobem na generowania zawartości w dowolnym formacie.
Choć w większości przypadków stosować będziemy szablony generujące zawartość HTML,
to szablon może łatwo wygenerować JavaScript, CSS, XML lub inny format jaki może
być potrzebny.

Na przykład, sam "zasób" jest często generowany w różnych formatach. Aby wygenerować
stronę indeksową artykułu w XML, należy zawrzeć ten format w nazwie szablonu:

* *Nazwa szablonu XML*: ``AcmeArticleBundle:Article:index.xml.twig``
* *Nazwa pliku XML*: ``index.xml.twig``

W rzeczywistości jest to nic innego jak konwencja nazewnicza i szablon nie jest
rzeczywiście przetwarzane na podstawie rozszerzenia wskazującego na format.

W wielu przypadkach może być wygodne użycie jednego kontrolera do wygenerowania
wielu różnych formatów w oparciu o "format żądania". Z tego powodu typowy wzorzec
jest zrobiony następująco::

    public function indexAction()
    {
        $format = $this->getRequest()->getRequestFormat();

        return $this->render('AcmeBlogBundle:Blog:index.'.$format.'.twig');
    }

Metoda ``getRequestFormat`` w obiekcie ``Request`` domyślnie zwraca format ``html``,
ale może zwrócić dowolny inny format na podstawie formatu żądanego przez użytkownika.
Format żądania jest najczęściej zarządzany przez trasowanie gdzie trasa może być
skonfigurowana tak, że ``/contact`` ustawia format żadania na ``html``, podczas gdy
``/contact.xml`` ustawia format na ``xml``. W celu uzyskania więcej informacji proszę
przeczytać rozdział :ref:`Zaawansowany przykład trasowania <advanced-routing-example>`.

Aby utworzyć linki, które zawierają parametr formatu, należy dołączyć klucz ``_format``
z parametrem asocjacyjnym:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        <a href="{{ path('article_show', {'id': 123, '_format': 'pdf'}) }}">
            PDF Version
        </a>

    .. code-block:: html+php
       :linenos:

        <a href="<?php echo $view['router']->generate('article_show', array('id' => 123, '_format' => 'pdf')) ?>">
            PDF Version
        </a>

Wnioski końcowe
---------------

Silnik szablonowania w Symfony, to bardzo silne narzędzie mogące zostać użyte
w każdej chwili, gdy zajdzie potrzeba wygenerowania prezentacyjnej zawartości
w formacie HTML, XML lub w każdym innym. Chociaż szablony są typowym sposobem
generowania zawartości stron w kontrolerze, to ich używanie nie jest obowiązkowe.
Obiekt ``Response`` zwracany przez kontroler może być utworzony bez stosowania szablonu::

    // utworzenie obiektu Response, którego zawartością jest przetworzony szablon
    $response = $this->render('AcmeArticleBundle:Article:index.html.twig');

    // utworzenie obiektu Response, którego zawartością jest prosty tekst
    $response = new Response('response content');

Silnik szablonowania Symfony jest bardzo elastyczny i dostępne są domyślnie dwa
różne typy szablonów: tradycyjne szablony PHP oraz eleganckie i wydajne szablony
Twig. Obydwa typy obsługują hierarchię szablonów i są dostarczane z bogatym
zestawem pomocniczych funkcji, zdolnych do wykonywania najpardziej typowych zadań.

Ogólnie rzecz biorąc, system szablonowania w Symfony2 powinien być traktowany jako
potężne narzędzie, które ma się do dyspozycji. W niektórych przypadkach nie ma
potrzeby renderowania szablonów i w Symfony2 jest to absoluynie dopuszczalne.

Do nauczenia się więcej w Receptariuszu
---------------------------------------

* :doc:`/cookbook/templating/PHP`
* :doc:`/cookbook/controller/error_pages`
* :doc:`/cookbook/templating/twig_extension`

.. _`Twig`: http://twig.sensiolabs.org
.. _`KnpBundles.com`: http://knpbundles.com
.. _`Cross Site Scripting`: http://en.wikipedia.org/wiki/Cross-site_scripting
.. _`Output Escaping`: http://twig.sensiolabs.org/doc/api.html#escaper-extension
.. _`tags`: http://twig.sensiolabs.org/doc/tags/index.html
.. _`filters`: http://twig.sensiolabs.org/doc/filters/index.html
.. _`dodać własne rozszerzenia`: http://twig.sensiolabs.org/doc/advanced.html#creating-an-extension
.. _`hinclude.js`: http://mnot.github.com/hinclude/
