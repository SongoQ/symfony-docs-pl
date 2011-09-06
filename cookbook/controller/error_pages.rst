Jak dostosować Strony Błędów
============================

Gdy wyrzucany jest jakikolwiek wyjątek w Symfony2, wyjątek ten łapany jest wewnątrz 
klasy ``Kernel`` oraz ewentualnie przekierowany do specjalnego kontrolera,
``TwigBundle:Exception:show``. Ten kontroler, który żyje w ciele ``TwigBundle``,
określa, który szablon błedu zostanie użyty do wyświetlenia błędu,
oraz jaki kod statusu powinien zostać ustawiony dla danego wyjątku.

Strony błędów mogą być dostosowane na dwa różne sposoby, w zależności od tego 
jak dużej kontroli potrzebujesz:

1. Dostosowanie szablonów błędów dla różnych stron błędów (patrz niżej);

2. Zamiana domyślnego kontrolera wyjątku ``TwigBundle::Exception:show``
   na Twój własny kontroler i obsługa błędów w taki sposób jak chcesz 
   (zobacz :ref:`exception_controller in the Twig reference<config-twig-exception-controller>`);

.. tip::

    Dostosowanie obsługi wyjątków w rzeczywistości daje dużo więcej możliwości 
    niżeli zostało to opisane tutaj. Wywoływane jest wewnętrzne zdarzenie (event), ``kernel.exception``,
    które umożliwia kompletną kontrolę nad obsługą wyjątku. W celu uzyskania większej ilośći
    informacji, zobacz :ref:`kernel-kernel.exception`.

Wszystkie szablony błędów umieszczone są wewnątrz ``TwigBundle``.
Aby nadpisać szablony, wystarczy że będziemy polegać na standardowej metodzie
nadpisywania szablonów znajdujących się w bundlu. W celu uzyskania większej ilości informacji,
zobacz :ref:`overriding-bundle-templates`.

Dla przykładu, aby nadpisać domyślny szablon błędu który pokazuje się końcowemu użytkownikowi, 
stwórz nowy szablon w ``app/Resources/TwigBundle/views/Exception/error.html.twig``:

.. code-block:: html+jinja

    <!DOCTYPE html>
    <html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>An Error Occurred: {{ status_text }}</title>
    </head>
    <body>
        <h1>Oops! An Error Occurred</h1>
        <h2>The server returned a "{{ status_code }} {{ status_text }}".</h2>
    </body>
    </html>

.. tip::

    Nie martw się jeśli nie jesteś zaznajomiony z Twig. Twig jest prostym, efektywnym
    oraz opcjonalnym systemem szablonów który integruje się z ``Symfony2``.
    W celu uzyskania większej ilości informacji o Twig zobacz :doc:`/book/templating`.

Oprócz standardowych HTMLowych stron błędu, Symfony obsługuje także domyślne strony
błędów dla wielu popularnych formatów odpowiedzi (Response), w tym JSON
(``error.json.twig``), XML, (``error.xml.twig``), a nawet Javascript (``error.js.twig``),
to tylko kilka z nich.
Aby nadpisać któryś z szablonów, wystarczy stworzyć plik z taką samą nazwą w katalogu
``app/Resources/TwigBundle/views/Exception``. Jest to standardowy sposób na nadpisanie każdego
szablonu znajdującego się w bundlu.

.. _cookbook-error-pages-by-status-code:

Dostosowywanie Strony 404 oraz innych Stron Błędów
--------------------------------------------------

Możesz także dostosować konkretne szablony błędów w zależności od kodu błędu HTTP.
Dla przykładu, utwórz szablon ``app/Resources/TwigBundle/views/Exception/error404.html.twig``
dla wyświetlenia specjalnej strony dla błędu 404 (nie znaleziono strony).

Symfony używa następującego algorytmu do określenia którego szablonu użyć:

* Po pierwsze, szuka on szablonu dla danego formatu oraz kodu błędu (``error404.json.twig``);

* Jeśli taki nie istnieje, wyszukiwany jest szablon dla danego formatu (``error.json.twig``);

* Jeśli taki nie istnieje, to wraca do szablonu HTML (``error.html.twig``).

.. tip::

    Aby zobaczyć pełną listę dostępnych szablonów błędów, zobacz folder
    ``Resources/views/Exception`` w ``TwigBundle``. W standardowej instalacji
    Symfony2, ``TwigBundle`` znajduje się w katalogu ``vendor/symfony/src/Symfony/Bundle/TwigBundle``.
    Często, najprostszym sposobem na dostosowanie strony błędu jest skopiowanie jej z ``TwigBundle``
    do ``app/Resources/TwigBundle/views/Exception`` i zmodyfikowanie jej.

.. note::

    Przyjazne strony wyjątków (debug-friendly) dla dewelopera także mogą być dostosowane
    w taki sam sposób poprzez tworzenie szablonów takich jak ``exception.html.twig``
    dla standarowej strony błędu HTML lub ``exception.json.twig`` dla strony błędu
    formatu JSON.
