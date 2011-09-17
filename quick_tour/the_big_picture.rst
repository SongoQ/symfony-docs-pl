Zarys całości
===============

Zacznij korzystać z Symfony2 w 10 minut! Ten rozdział poprowadzi Cię przez kilka
z najważniejszych koncepcji Symfony2 i wyjaśni, jak można rozpocząć
szybko pracę, pokazując prosty przykład projektu w działaniu.

Jeśli kiedykolwiek używałeś frameworka webowego to z Symfony2 powinieneś czuć się jak w domu.
Jeśli nie, to zapraszamy do poznania zupełnie nowego sposobu tworzenia aplikacji internetowych.

.. tip::

    Chcesz dowiedzieć się, dlaczego i kiedy należy używać frameworka? Przeczytaj dokument "`Symfony
    w ciągu 5 minut`_".
    
Pobieranie Symfony2
--------------------

Po pierwsze, sprawdź czy masz zainstalowany i skonfigurowany serwer WWW (np.
Apache) wraz z PHP 5.3.2 lub ​​wyższym.

Gotowy? Pobierz "`Symfony2 Standard Edition`_", jest to :term:`dystrybucja` Symfony, która 
jest skonfigurowana do najczęstszych przypadków użycia i zawiera także kod, który 
pokazuje, jak używać Symfony2 (archiwum z *vendors*).

Po rozpakowaniu archiwum, struktura katalogów ``Symfony/`` wygląda następująco:

.. code-block:: text

    www/ <- Twój głowny katalog www
        Symfony/ <- rozpakowane archiwum
            app/
                cache/
                config/
                logs/
                Resources/
            bin/
            src/
                Acme/
                    DemoBundle/
                        Controller/
                        Resources/
                        ...
            vendor/
                symfony/
                doctrine/
                ...
            web/
                app.php
                ...

.. note::

    Jeśli pobrałeś wersje Standard Edition *bez vendors*, uruchom poniższe 
    polecenie, aby pobrać wszystkie biblioteki.

    .. code-block:: bash

        php bin/vendors install

Sprawdzanie konfiguracji
--------------------------

Symfony2 posiada wizualny tester konfiguracji serwera, który pomaga uniknąć
problemów, które pochodzą z serwera lub błędu w konfiguracji samego PHP. Wywołaj następujący
adres URL, aby zobaczyć diagnostykę Twojego komputera.

.. code-block:: text

    http://localhost/Symfony/web/config.php

Odczytaj uważnie dane wyjściowe skryptu i napraw problemy, które zostały 
znalezione. Teraz, otwórz swoją pierwszą "prawdziwą" stronę opartą na Symfony2::

.. code-block:: text

    http://localhost/Symfony/web/app_dev.php/

Symfony2 powinno powitać Cię i pogratulować za ciężka pracę w dotarciu tak daleko!

.. image:: /images/quick_tour/welcome.jpg
   :align: center

Zrozumieć podstawy
------------------------------

Jednym z głównych celów frameworka jest zapewnienie `Separacji zagadnień`_.
Dzięki temu Twoja organizacja kodu pozwala aplikacji łatwo się rozrastać
w czasie, unikając mieszania wywoływań zapytań bazy danych, HTML i logiki 
biznesowej w tym samym skrypcie. Aby osiągnąć ten cel z Symfony, będziesz na początku
musiał nauczyć się kilku podstawowych pojęć i terminów.

.. tip::

    Chcesz dowodu na to, że używanie frameworka jest lepsze niż mieszanie wszystkiego
    w tym samym skrypcie? Przeczytaj rozdział z podręcznika ":doc:`/book/from_flat_php_to_symfony2`".

Standardowa dystrybucja wyposażona jest w przykładowy kod, który można użyć, aby dowiedzieć się 
więcej o głównych koncepcjach Symfony2. Przejdź do następującego adresu URL, zostaniesz powitany
przez Symfony2 (zamiast *Fabien* wpisz swoje imię):

.. code-block:: text

    http://localhost/Symfony/web/app_dev.php/demo/hello/Fabien

.. image:: /images/quick_tour/hello_fabien.png
   :align: center

Co się tutaj dzieje? Spróbujmy przeanalizować adres URL:

* ``app_dev.php``: Jest to :term:`front controller` - wyjątkowy punkt wejścia
aplikacji, który odpowiada na wszystkie zapytania użytkownika;

* ``/demo/hello/Fabien``: Jest to *wirtualna ścieżka* do zasobów jaką chce uzyskać
użytkownik.

Twoim zadaniem, jako developera jest napisanie takiego kodu, który odwzorowuje *zapytania* (``/demo/hello/Fabien``) 
użytkownika do *zasobu* z nim związanego (``Hello Fabien!``).

Routing
~~~~~~~

Routing w Symfony2 obsługuje zapytania użytkownika, dopasowując
żądanie danego adresu URL na podstawie skonfigurowanych wzorców. Domyślnie te wzorce
(tzw. trasy) są zdefiniowane w pliku ``app/config/routing.yml``. 
Kiedy jesteś w ``dev``:ref:`środowisku<quick-tour-big-picture-environments>`-
wskazany przez **app_dev**.php front kontroler ładuje konfiguracje z pliku ``app/config/routing_dev.yml``
W Standard Edition, trasy do stron "demo" są umieszczane w tym pliku:

.. code-block:: yaml

    # app/config/routing_dev.yml
    _welcome:
        pattern:  /
        defaults: { _controller: AcmeDemoBundle:Welcome:index }

    _demo:
        resource: "@AcmeDemoBundle/Controller/DemoController.php"
        type:     annotation
        prefix:   /demo

    # ...

Pierwsze trzy linie (po komentarzu) określają kod, który jest wykonywany
gdy użytkownik zażąda zasobu "``/``" (tj. strony powitalnej, którą widziałeś
wcześniej). Zapytanie wywoła kontroler ``AcmeDemoBundle:Welcome:index``. 
W kolejnym rozdziale dowiesz się dokładnie co to oznacza.

.. tip::

    Symfony2 Standard Edition używa `YAML`_ dla swoich plików konfiguracyjnych,
    oprócz tego obsługuje XML, PHP i adnotacje natywnie.
    Wszystkie typy formatów są kompatybilne i mogą być używane zamiennie w
    aplikacji. Wydajność aplikacji nie zależy od konfiguracji wybranego formatu, 
    bo wszystko jest buforowane podczas pierwszego zapytania.

Kontrolery
~~~~~~~~~~~

Kontroler jest to nazwa wymyślona dla funkcji PHP lub metody, która obsługuje przychodzące
*zapytania* i zwraca *odpowiedzi* (często w postaci HTML). Zamiast wykorzystywać
zmienne globalne PHP i funkcje (np. ``$_GET`` lub ``header()``) do zarządzania
komunikatami HTTP, Symfony wykorzystuje obiekty: :class:`Symfony\\Component\\HttpFoundation\\Request`
i: :class:`Symfony\\Component\\HttpFoundation\\Response`. Najprostszy z możliwych
kontrolerów to odpowiedź zwracana ręcznie, na podstawie zapytania::

    use Symfony\Component\HttpFoundation\Response;

    $name = $request->query->get('name');

    return new Response('Hello '.$name, 200, array('Content-Type' => 'text/plain'));

.. note::

    Symfony2 obejmuje specyfikację HTTP, której reguły rządzą całą komunikacją w sieci. 
    Przeczytaj rozdział podręcznika ":doc:`/book/http_fundamentals`", aby dowiedzieć 
    się więcej o tym i co z tego wynika.

Symfony2 wybiera kontroler bazujący na ``_controller`` wartości z 
konfiguracji routingu: ``AcmeDemoBundle:Welcome:index``. Ten ciąg znaków jest
*logiczne nazwany* i odwołuje się do metody ``indexAction`` z
``Acme\DemoBundle\Controller\WelcomeController`` class::

    // src/Acme/DemoBundle/Controller/WelcomeController.php
    namespace Acme\DemoBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class WelcomeController extends Controller
    {
        public function indexAction()
        {
            return $this->render('AcmeDemoBundle:Welcome:index.html.twig');
        }
    }

.. tip::

    Można używać pełnej nazwy klasy i metody - 
    ``Acme\DemoBundle\Controller\WelcomeController::indexAction`` dla wartości ``_controller``
    Jeśli jendnak chcesz wykorzystywać proste konwencje, używaj nazwy logicznej,
    która jest krótsza i pozwala na większą elastyczność.

Końcowe myśli
--------------

Gratulacje! Dostałeś pierwszy kąsek kodu Symfony2. To nie było takie
trudne, prawda? Jest jeszcze dużo więcej do odkrycia, ale już teraz możesz zobaczyć, jak
Symfony2 jest proste i szybkie w implementacji stron internetowych. Jeśli
chcesz dowiedzieć się więcej o Symfony2, zanurz się w następnej sekcji:
":doc:`The View<the_view>`".

.. _Symfony2 Standard Edition:      http://symfony.com/download
.. _Symfony w 5 minut:              http://symfony.com/symfony-in-five-minutes
.. _Separacja zagadnień:            http://en.wikipedia.org/wiki/Separation_of_concerns
.. _YAML:                           http://www.yaml.org/
.. _Adnotacje w kontrolerach:       http://symfony.com/doc/current/bundles/SensioFrameworkExtraBundle/index.html#annotations-for-controllers
.. _Twig:                           http://www.twig-project.org/
