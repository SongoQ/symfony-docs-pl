.. highlight:: php
   :linenothreshold: 2

Widok (View)
============

Po przeczytaniu pierwszej części tego kursu, zdecydowałeś że możesz poświęcić
kolejne 10 minut dla Symfony2. Wspaniały wybór! W drugiej części, nauczysz się
więcej na temat silnika szablonów Symfony2, `Twig`_. Twig jest elastycznym,
szybkim oraz bezpiecznym systemem szablonów PHP. Sprawia że szablony
są bardo czytelne oraz zwięzłe. Czyni je także bardziej przyjaznymi dla
projektantów stron internetowych.

.. note::

    Zamiast używać systemu Twig, można również w swojej aplikacji użyć zwykłych
    :doc:`skryptów PHP </cookbook/templating/PHP>`.
    Obydwa silniki są wspierane przez Symfony2.

Zapoznanie się z Twig
---------------------

.. tip::

    Jeśli chcesz się nauczyć systemu Twig, to zalecamy przeczytanie oficjalnej
    `dokumentacji`_. Niniejszy rozdział jest tylko przeglądem głównych
    pojęć tego systemu.

Szablon Twig jest tekstowym plikiem który może generować każdy rodzaj
treści (HTML, XML, CSV, LaTeX, ...). Twig definiuje dwa rodzaje
ograniczników:

* ``{{ ... }}``: wypisuje zmienną lub też wynik wyrażenia;

* ``{% ... %}``: kontroluje logikę szablonu. Jest na przykład używany do pętli
  ``for`` lub  wyrażeń ``if``.

Poniżej znajduje się kod minimalnego szablonu, ilustrujący kilka podstawowych rzeczy:
użyte zostały dwie zmienne ``page_title`` oraz ``navigation``, które będą
przekazane do szablonu:

.. code-block:: html+jinja
   :linenos:

    <!DOCTYPE html>
    <html>
        <head>
            <title>My Webpage</title>
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

.. tip::

    Komentarze mogą być dołączone do szablonu poprzez użycie ogranicznika ``{# ... #}``.

Aby w Symfony wyrenderować szablon, trzeba z poziomu kontrolera użyć metodę ``render`` 
i przekazać do szablonu wszystkie potrzebne zmienne::

    $this->render('AcmeDemoBundle:Demo:hello.html.twig', array(
        'name' => $name,
    ));

Zmienne przekazane do szablonu mogą być ciągami znaków, tablicami lub nawet obiektami.
Twig rozpoznaje różnice między nimi i umożliwia łatwy dostęp do "atrybutów" zmiennej
poprzez notację kropkową (``.``):

.. code-block:: jinja
   :linenos:

    {# array('name' => 'Fabien') #}
    {{ name }}

    {# array('user' => array('name' => 'Fabien')) #}
    {{ user.name }}

    {# force array lookup #}
    {{ user['name'] }}

    {# array('user' => new User('Fabien')) #}
    {{ user.name }}
    {{ user.getName }}

    {# force method name lookup #}
    {{ user.name() }}
    {{ user.getName() }}

    {# pass arguments to a method #}
    {{ user.date('Y-m-d') }}

.. note::

    Należy pamiętać że nawiasy klamrowe nie są częścią zmiennej ale
    instrukcją drukarską. Jeśli umieszczasz zmienne wewnątrz znaczników,
    nie umieszczaj wokół nich nawiasów klamrowych.

Dekorowanie szablonów
---------------------

Przeważnie w projektach szablony posiadają elementy, takie jak powszechnie używany
nagłówek czy stopka. W Symfony2, patrzymy na ten problem inaczej - szablon może
być dekorowany przez inny szablon. Działa to tak samo jak klasy PHP. Dziedziczenie
szablonów umożliwia zbudowanie podstawowego szablonu "układu strony" (*ang. layout*),
który zawiera wszystkie typowe elementy strony oraz określa "bloki", które szablony
potomne mogą zamieniać.

Szablon ``hello.html.twig`` dziedziczy po szablonie ``layout.html.twig``,
dzięki znacznikowi ``extends``:

.. code-block:: html+jinja
   :linenos:

    {# src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig #}
    {% extends "AcmeDemoBundle::layout.html.twig" %}

    {% block title "Hello " ~ name %}

    {% block content %}
        <h1>Hello {{ name }}!</h1>
    {% endblock %}

Zapis ``AcmeDemoBundle::layout.html.twig`` wygląda znajomo, prawda? Jest to ta sama
notacja, jaka była zastosowana do regularnego szablonu. Część ``::`` oznacza, że
element kontrolera jest pusty, tak więc odpowiedni plik znajduje się w katalogu
``Resources/views/``.

Przyjrzyjmy sie uproszczonej wersji ``layout.html.twig``:

.. code-block:: html+jinja
   :linenos:

    {# src/Acme/DemoBundle/Resources/views/layout.html.twig #}
    <div class="symfony-content">
        {% block content %}
        {% endblock %}
    </div>

Znaczniki ``{% block %}`` określają bloki, które może wypełniać szablon potomny.
Znacznik ten informuje szablon potomny, że może zastąpić ten znacznik
własnym kodem.

W tym przykładzie, szablon ``hello.html.twig`` zastępuje blok ``content``,
co oznacza że tekst "Hello Fabien" jest renderowany w środku elementu
``div.symfony-content``.

Używanie znaczników, filtrów i funkcji
--------------------------------------

Jedną z najlepszych cech systemu Twig jest jego rozszerzalność poprzez zanaczniki,
filtry i funkcje. Symfony2 posiada wiele wbudowanych elementów ułatwiających pracę
nad projektowaniem szablonów.

Dołączenie innych szablonów
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Najlepszym sposobem, aby podzielić się fragmentem kodu pomiędzy różnymi
szablonami jest stworzenie nowego szablonu który może zostać dołączony
przez inne szablony.

Utwórzmy szablon ``embedded.html.twig``:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/embedded.html.twig #}
    Hello {{ name }}

i zmieńmy szablon ``index.html.twig``, tak aby dołączał nasz nowo utworzony szablon:

.. code-block:: jinja
   :linenos:

    {# src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig #}
    {% extends "AcmeDemoBundle::layout.html.twig" %}

    {# override the body block from embedded.html.twig #}
    {% block content %}
        {% include "AcmeDemoBundle:Demo:embedded.html.twig" %}
    {% endblock %}

Osadzanie innych kontrolerów
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A co, jeśli chcesz osadzić wynik innego kontrolera w szablonie? To bardzo przydatne
podczas pracy z Ajax, lub gdy osadzony szablon potrzebuje niektórych zmiennych
niedostępnych w głównym szablonie.

Załóżmy, że chcemy utworzyć akcję ``fancy`` i chcemy dołączyć ją do szablonu ``index``.
Aby to zrobić, trzeba użyć znacznik ``render``:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/index.html.twig #}
    {{ render(controller("AcmeDemoBundle:Demo:fancy", {'name': name, 'color': 'green'})) }}

Załóżmy, że utworzyliśmy metodę kontrolera ``fancyAction`` i chcemy "renderować"
ją w szablonie ``index``, co oznacza osadzenie wyniku kontrolera (np. ``HTML``)
w renderowanej z szablonu stronie. Aby to zrobić, użyjemy funkcji``render``::

    // src/Acme/DemoBundle/Controller/DemoController.php

    class DemoController extends Controller
    {
        public function fancyAction($name, $color)
        {
            // utworzenie jakiegoś obiektu, na podstawie zmiennej $color
            $object = ...;

            return $this->render('AcmeDemoBundle:Demo:fancy.html.twig', array(
                'name' => $name,
                'object' => $object,
            ));
        }

        // ...
    }

Tworzenie odnośników pomiędzy stronami
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Skoro mowa o aplikacjach internetowych, to tworzenie odnośników pomiędzy stronami
jest koniecznością. Zamiast umieszczania w szablonach sztywnych adresów URL,
można zastosować funkcję ``path``, która wie jak wygenerować adres URL na podstawie
konfiguracji trasowania. W ten sposób wszystkie adresy URL mogą być łatwo aktualizowane
tylko przez zmianę konfiguracji:

.. code-block:: html+jinja

    <a href="{{ path('_demo_hello', { 'name': 'Thomas' }) }}">Greet Thomas!</a>

Funkcja ``path`` pobiera jako argumenty nazwę trasy i tablicę parametrów. Nazwa
trasy jest głównym kluczem do którego odnoszone są trasy, a parametry są wartościami
wieloznaczników określonymi w ścieżce trasy::

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    /**
     * @Route("/hello/{name}", name="_demo_hello")
     * @Template()
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

.. tip::

    Funkcja ``url`` generuje bezwzględny adres URL: ``{{ url('_demo_hello', {
    'name': 'Thomas' }) }}``.

Dołączanie zasobów: obrazów, skryptów JavaScript i arkuszy stylów
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Co to byłby za Internet bez zdjęć, skryptów JavaScript i arkuszy stylów? Symfony2
oferuje funkcję ``asset`` radzącą sobie łatwo z tym zagadnieniem:

.. code-block:: html+jinja

    <link href="{{ asset('css/blog.css') }}" rel="stylesheet" type="text/css" />

    <img src="{{ asset('images/logo.png') }}" />

Głównym zadaniem funkcji ``asset`` jest umożliwienie lepszej przenośności aplikacji.
Dzięki tej funkcji, możesz przenieść główny katalog aplikacji w dowolne miejsce bez
konieczności dokonywania zmian w kodzie szablonu.

Zabezpieczenie zmiennych
------------------------

Twig jest skonfigurowany tak aby zabezpieczać wszystkie dane wyjściowe.
Przeczytaj `dokumentację`_ systemu Twig aby dowiedzieć się więcej na temat
zabezpieczenia danych wyjściowych oraz rozszerzenia *Escaper*.

Podsumowanie
------------

Twig jest prosty ale skuteczny. Dzięki możliwości stosowania formatek (*ang. layout*),
bloków, dziedziczenia szablonów i akcjom, bardzo łatwo można zorganizować swój
szablon, w sposób logiczny i rozszerzalny. Jeśli jednak nie odpowiada Ci Twig,
to zawsze, bez żadnych problemów, możesz użyć w Symfony zwykłych szablonów PHP.

Pracujesz z Symfony2 od około 20 minut, ale już teraz możesz zrobić z nim
sporo niesamowitych rzeczy. To jest siła Symfony2. Nauka podstaw jest bardzo
prosta. Już niedługo odkryjesz, że prostota jest ukryta pod bardzo elastyczną
architekturą.

Ale coraz bardziej odbiegam od tematu. Po pierwsze, musisz dowiedzieć się więcej
o kontrolerach i to jest tematem :doc:`kolejnej części przewodnika <the_controller>`.
Gotowy na kolejne 10 minut z Symfony2?

.. _Twig:          http://twig.sensiolabs.org/
.. _dokumentacji: http://twig.sensiolabs.org/documentation
.. _dokumentację: http://twig.sensiolabs.org/documentation
