Widok (View)
============

Po przeczytaniu pierwszej części tego kursu, zdecydowałeś że możesz poświęcić
kolejne 10 minut dla Symfony2. Wspaniały wybór! W drugiej części, nauczysz się
więcej na temat silnika szablonów Symfony2, `Twig`_. Twig jest elastycznym,
szybkim, oraz bezpiecznym systemem szablonów PHP. To sprawia że Twoje szablony
są bardziej czytelne oraz zwięzłe; to czyni je także bardziej przyjazne dla
projektantów stron www.

.. note::

    Zamiast używać Twig, możesz użyć :doc:`PHP </cookbook/templating/PHP>`
    w swoich szablonach. Obydwa silniki są wspierane przez Symfony2.

Zapoznanie się z Twig
---------------------

.. tip::

    Jeśli chcesz się nauczyć Twig, zalecamy przeczytanie oficjalnej
    `dokumentacji`_. Ta sekcja jest tylko szybkim przeglądem głównych
    koncepcji.

Szablon Twig jest tekstowym plikiem który może generować każdy rodzaj
treści (HTML, XML, CSV, LaTeX, ...). Twig definiuje dwa rodzaje
ograniczników:

* ``{{ ... }}``: Wypisuje zmienną lub też wynik wyrażenia;

* ``{% ... %}``: Kontroluje logikę szablonu; jest używany do pętli ``for``
  oraz wyrażeń ``if``.

Poniżej znajduje się minimalny szablon który ilustruje kilka podstawowych rzeczy,
użyte zostały dwie zmienne ``page_title`` oraz ``navigation``, które zostały
przekazane do szablonu:

.. code-block:: html+jinja

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

Aby wyrenderować szablony w Symfony, użyj metody ``render`` z poziomu kontrolera (controller)
i przekaż wszystkie zmienne potrzebne w szablonie::

    $this->render('AcmeDemoBundle:Demo:hello.html.twig', array(
        'name' => $name,
    ));

Zmienne przekazane do szablonu mogą być ciągami znaków, tablicami, lub nawet obiektami.
Twig rozróżnia różnice między nimi i umożliwia Ci łatwy dostęp do "atrybutów" zmiennej
poprzez notację kropki (``.``):

.. code-block:: jinja

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
    instrukcją drukowania. Jeśli wyświetlasz zmienne wewnątrz tagów
    nie umieszczaj nawiasów klamrowych wokół zmiennej.

Dekorowanie Szablonów
---------------------

Przeważnie szablony w projekcie posiadają te same elementy, jak dobrze
znane "topy" oraz "stopki". W Symfony2 patrzymy na ten problem inaczej:
szablon może być udekorowany przez inny szablon. Działa to w ten sam
sposób jak klasy PHP: dziedziczenie w szablonach umożliwia Ci zbudowanie
bazowego "szablonu" który posiada wszystkie wspólne elementy Twojej strony
oraz definiuje "bloki" które mogą być nadpisane przez szablony-dzieci.

Szablon ``hello.html.twig`` dziedziczy po szablonie ``layout.html.twig``,
dzięki tagowi ``extends``:

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig #}
    {% extends "AcmeDemoBundle::layout.html.twig" %}

    {% block title "Hello " ~ name %}

    {% block content %}
        <h1>Hello {{ name }}!</h1>
    {% endblock %}

Zapis ``AcmeDemoBundle::layout.html.twig`` brzmi znajomo? prawda?
Jest to ta sama notacja użyta do określenia regularnego szablonu. Część
``::`` po prostu oznacza że kontroler jest pusty, więc odpowiedni plik
znajduje się bezpośrednio w folderze ``Resources/views/``.

Rzućmy okiem na uproszczoną wersję ``layout.html.twig``:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/layout.html.twig #}
    <div class="symfony-content">
        {% block content %}
        {% endblock %}
    </div>

Tagi ``{% block %}`` definiują bloki które szablony-dzieci mogą wypełnić.
Tag block mówi silnikowi szablonów że szablon-dziecko może nadpisać tą
część szablonu.

W tym przykładzie, szablon ``hello.html.twig`` nadpisuje blok ``content``,
co oznacza że tekst "Hello Fabien" jest renderowany w środku elementu
``div.symfony-content``.

Używanie Tagów, Filtrów oraz Funkcji
------------------------------------

Jedną z najlepszych funkcji Twig jest jego rozszerzalność za pomocą tagów,
filtrów, oraz funkcji. Symfony2 posiada wiele wbudowanych elementów
w celu ułatwienia pracy w projektowaniu szablonu.

Dołączenie innego Szablonu
~~~~~~~~~~~~~~~~~~~~~~~~~~

Najlepszym sposobem, aby podzielić się fragmentem kodu pomiędzy różnymi
szablonami jest stworzenie nowego szablonu który może zostać dołączony
przez inne szablony.

Stwórz szablon ``embedded.html.twig``:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/embedded.html.twig #}
    Hello {{ name }}

Oraz zmień szablon ``index.html.twig`` aby go dołączał:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig #}
    {% extends "AcmeDemoBundle::layout.html.twig" %}

    {# override the body block from embedded.html.twig #}
    {% block content %}
        {% include "AcmeDemoBundle:Demo:embedded.html.twig" %}
    {% endblock %}

Osadzanie innych Kontrolerów
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A co jeśli chcesz osadzić wynik kontrollera w szablonie?
Jest to bardzo pomocne gdy pracujesz z Ajaxem, lub też osadzony
szablon potrzebuje zmiennych niedostępnych w głównym szablonie.

Przypuśćmy że stworzyłeś akcję ``fancy``, i chcesz dołączyć ją
w szablonie ``index``. Aby to zrobić, użyj tagu ``render``:

.. code-block:: jinja

    {# src/Acme/DemoBundle/Resources/views/Demo/index.html.twig #}
    {% render "AcmeDemoBundle:Demo:fancy" with { 'name': name, 'color': 'green' } %}

``AcmeDemoBundle:Demo:fancy`` odwołuje się do akcji ``fancy`` z kontrolera ``Demo``.
Argumenty (``name`` oraz ``color``) zachowują się jakby były zmiennymi zapytania (request)
(tak jakby ``fancyAction`` obsługiwał całe zapytanie) oraz są dostępne dla kontrolera::

.. code-block:: php

    // src/Acme/DemoBundle/Controller/DemoController.php

    class DemoController extends Controller
    {
        public function fancyAction($name, $color)
        {
            // create some object, based on the $color variable
            $object = ...;

            return $this->render('AcmeDemoBundle:Demo:fancy.html.twig', array('name' => $name, 'object' => $object));
        }

        // ...
    }

Tworzenie Linków pomiędzy Stronami
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jeśli mówimy o aplikacjach webowych, tworzenie linków pomiędzy stronami jest koniecznością.
Zamiast na sztywno wpisywać URLe w szablonach, funkcja ``path`` wie jak wygenerować
URLe bazując na konfiguracji routingu. Tym sposobem, każdy z Twoich linków może być
łatwo zmieniony tylko przy użyciu konfiguracji:

.. code-block:: html+jinja

    <a href="{{ path('_demo_hello', { 'name': 'Thomas' }) }}">Greet Thomas!</a>

Funkcja ``path`` bierze nazwę routingu oraz tablicę parametrów jako argumetny.
Nazwa routingu jest głównym kluczem pod którym zapisana jest ścieżka routingu
a parametry są wartościami które zamieniają zmienne w wzorcu routingu::

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

    Funkcja ``url`` generuje absolutny URL: ``{{ url('_demo_hello', {
    'name': 'Thomas' }) }}``.

Dołączanie zasobów: obrazki, JavaScript, arkusze stylów
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jak wyglądał by internet bez obrazków, JavaScript czy też arkuszy stylów?
Symfony2 posiada funkcję ``asset`` która w łatwy sposób sobie z nimi radzi:

.. code-block:: jinja

    <link href="{{ asset('css/blog.css') }}" rel="stylesheet" type="text/css" />

    <img src="{{ asset('images/logo.png') }}" />

Głównym zadaniem funkcji ``asset`` jest umożliwienie lepszej przenoszalności
Twojej aplikacji. Dzięki tej funkcji, możesz przenieść główny katalog aplikacji
w dowolne miejsce bez konieczności zmian czegokolwiek w kodzie szablonu.

Zamienianie Zmiennych
---------------------

Twig jest skonfigurowany tak aby zamieniać wszystkie dane wyjściowe.
Przeczytaj `dokumentację`_ Twig aby dowiedzieć się więcej na temat
zamieniania danych wyjściowych oraz rozszerzenia Escaper.

Podsumowanie
------------

Twig jest prosty ale skuteczny. Dzięki layoutom, blokom, szablonom oraz 
akcjom włącznie, możesz w łatwy sposób zorganizować swoje szablony w logiczny
oraz rozszerzalny sposób. Jeśli jednak nie czujesz się komfortowo z Twig, możesz
zawsze użyć szablonów PHP w Symfony bez jakichkolwiek problemów.

Pracujesz z Symfony2 od około 20 minut, ale już teraz możesz zrobić z nim
sporo niesamowitych rzeczy. To jest siła Symfony2. Nauka podstaw jest bardzo
prosta, już niedługo nauczysz się że prostota jest ukryta pod bardzo elastyczną
architekturą.

Ale coraz bardziej odbiegam od tematu. Po pierwsze, musisz dowiedzieć się więcej
o kontrolerach i to jest tematem :doc:`kolejnej części kursu<the_controller>`.
Gotowy na kolejne 10 minut z Symfony2?

.. _Twig:          http://twig.sensiolabs.org/
.. _dokumentacji: http://twig.sensiolabs.org/documentation
.. _dokumentację: http://twig.sensiolabs.org/documentation
