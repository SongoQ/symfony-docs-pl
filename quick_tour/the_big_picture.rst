.. highlight:: php
   :linenothreshold: 2

Obraz ogólny
============

Zacznij korzystać z Symfony2 w 10 minut! Ten rozdział poprowadzi Cię przez kilka
z najważniejszych koncepcji Symfony2 i wyjaśni, jak można rozpocząć
szybko pracę, pokazując prosty przykład projektu w działaniu.

Jeśli kiedykolwiek używałeś frameworka aplikacji internetowej, to z Symfony2 powinieneś
czuć się jak w domu.
Jeśli nie, to zapraszamy do poznania zupełnie nowego sposobu tworzenia aplikacji internetowych.

.. tip::

    Chcesz dowiedzieć się, dlaczego i kiedy należy używać frameworka? Przeczytaj dokument
    "`Symfony w 5 minut`_" (*tekst angielski*).
    
Pobieranie Symfony2
-------------------

Po pierwsze, sprawdź czy masz zainstalowany i skonfigurowany serwer WWW (np.
Apache) wraz z PHP 5.3.3 lub ​​wersją wyższą.

.. tip::

    Jeśli masz PHP 5.4, to możesz użyć wbudowanego serwera internetowego. Wbudowany
    serwer powinien być używany tylko przy pracach programistycznych, ale może pomóc
    szybko i łatwo rozpocząć swój projekt.

    Aby uruchomić serwer wystarczy użyć tego polecenia:

    .. code-block:: bash

        $ php -S localhost:80 -t /path/to/www

    gdzie "/path/to/www" jest ścieżką do jakiegoś katalogu na Twoim komputerze,
    który będzie wyodrębniał Symfony w taki sposób, że ewentualny adres URL do
    aplikacji będzie miał postać "http://localhost/Symfony/app_dev.php". Możesz
    również najpierw wyodrębnić Symfony a następnie uruchomić serwer internetowy
    w katalogu "web" Symfony. Jeśli to zrobisz, to adres do aplikacji będzie miał
    postać "http://localhost/app_dev.php".


Gotowy? Pobierz "`Symfony2 Standard Edition`_", jest to :term:`dystrybucja` Symfony,
która jest skonfigurowana do najczęstszych zastosowań i zawiera także przykładowy kod
pokazujący, jak używać Symfony2 (archiwum z *vendors*).

Po rozpakowaniu archiwum, struktura katalogów ``Symfony/`` wygląda następująco:

.. code-block:: text
   :linenos:

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

    Jeśli jesteś zaznajomiony z programem Composer, to zamiast pobierać archiwum
    instalacyjne, możesz uruchomić następujące polecenie:

    .. code-block:: bash

        $ composer.phar create-project symfony/framework-standard-edition path/to/install 2.2.0

        # remove the Git history
        $ rm -rf .git
    
    Aby zainstalować konkretną wersję, zamień `2.2.0` na symbol najnowszej wersji
    Symfony (np. 2.2.1). Poszczegóły sięgnij na `stronę instalacyjną Symfony`_

.. tip::

    Jeśli masz zainstalowane PHP 5.4, to również mozesz użyć wbudowanego serwera
    internetowego:

    .. code-block:: bash

        # sprawdzenie konfiguracji PHP CLI
        $ php ./app/check.php

        # uruchomienie serwera wbudowanego
        $ php ./app/console server:run

    Adres URL Twojej aplikacji będzie miał postać "http://localhost:8000/app_dev.php"


Sprawdzanie konfiguracji
------------------------

Symfony2 posiada wizualny tester konfiguracji serwera, który pomaga uniknąć
problemów, które pochodzą z serwera lub błędu w konfiguracji samego PHP. Wywołaj
następujący adres URL, aby uruchomić diagnostykę dokonanej instalacji.

.. code-block:: text

    http://localhost/config.php

.. note::

    Wszystkie podane w przykładach adresy URL zakładają, że katalogiem głównym
    serwera internetowego na Twoim komputerze jest katalog
    `ścieżka/do/instalacji/Symfony/web`. Jeśli postępowałeś
    zgodnie z podaną wyżej instrukcją i rozpakowałeś pliki w katalogu `Symfony`,
    zlokalizowanym w katalogu głównym serwera internetowego, to w adresie URL,
    bezpośrednio po `localhost` dodaj `/Symfony/web` we wszystkich adresach URL
    podawanych tu w przykładach, tak jak to:

    .. code-block:: text

        http://localhost/Symfony/web/config.php

.. note::
    
    Aby uzyskać ładne i krótkie adresy URL trzeba wskazać katalog
    `ścieżka/do/instalacji/Symfony/web` jako katalog główny serwera internetowego
    lub wirtualnego hosta. W takim przypadku adresy URL aplikacji będą wyglądać tak
    jak ``http://localhost/config.php`` lub ``http://site.local/config.php``, gdy
    utworzysz wirtualny host w lokalnej domenie ``site.local``.    


Jeżeli zostały wykazane jakieś niezgodności, to napraw je. Można również dostosować
konfigurację w sposób podany niżej. Jak wszystko będzie dobrze, kliknij na odnośnik
*Bypass configuration and go to the Welcome page*, aby zażądać swoją pierwszą
"rzeczywistą" stronę Symfony2:

.. code-block:: text

    http://localhost/Symfony/web/app_dev.php/

Symfony2 przywita Cię i pogratuluje udanej instalacji.

.. image:: /images/quick_tour/welcome.jpg
   :align: center

Zrozumieć podstawy
------------------

Jednym z głównych celów frameworka jest dostosowanie się do tzw.
`zasady seperacji zagadnień (ang. separation of concerns) <http://en.wikipedia.org/wiki/Separation_of_concerns>`_.
Dzięki temu kod jest dobrze zorganizowany, unikając mieszania zapytań do bazy danych,
znaczników HTML i logiki biznesowej w tym samym skrypcie, umożliwiając tym swobodny
rozwój aplikacji w czasie. Aby osiągnąć ten cel z Symfony, musisz najpierw nauczyć
się kilku podstawowych koncepcji i terminów.

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

* ``app_dev.php``: Jest to :term:`kontroler wejścia` - unikalny punkt wejścia aplikacji, który odpowiedzialny za wszystkie żądania użytkownika;

* ``/demo/hello/Fabien``: Jest to *wirtualna ścieżka* do zasobu jaki chce uzyskać użytkownik.

Twoim zadaniem, jako programisty jest napisanie takiego kodu, który odwzorowuje
*żądanie* (``/demo/hello/Fabien``) użytkownika na *zasób* z nim związany (``Hello Fabien!``).

Trasowanie
~~~~~~~~~~

System trasowania (*ang. routing*), nazywany też polskiej literaturze "systemem przekierowań",
w Symfony2 obsługuje żądania klienta, dopasowując ścieżkę dostępu (zawartą w adresie URL)
do skonfigurowanych wzorców tras i przekazaniu sterowania właściwemu kontrolerowi.
Domyślnie wzorce te są zdefiniowane w pliku ``app/config/routing.yml``. Kiedy jest
się w :ref:`środowisku<quick-tour-big-picture-environments>` programistycznym
(wskazanym przez ``app_dev.php``) kontroler wejścia ładuje konfigurację z pliku
``app/config/routing_dev.yml``. W Standard Edition, trasy stron “demo” są umieszczane
w pliku w ten sposób:


.. code-block:: yaml
   :linenos:

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
wcześniej). Żądanie wywoła kontroler ``AcmeDemoBundle:Welcome:index``. 
W kolejnym rozdziale dowiesz się dokładnie co to oznacza.

.. tip::

    Symfony2 Standard Edition używa `YAML`_ dla swoich plików konfiguracyjnych,
    oprócz tego obsługuje XML, PHP i natywne adnotacje.
    Wszystkie typy formatów są kompatybilne i mogą być używane zamiennie w
    aplikacji. Wydajność aplikacji nie zależy od wybranego formatu konfiguracji, 
    bo wszystko jest buforowane przy pierwszym żądaniu.

Kontrolery
~~~~~~~~~~

Kontroler jest to jakaś funkcja lub metoda PHP obsługująca przychodzące
*żądania* i zwracająca *odpowiedzi* (często kod HTML). Zamiast wykorzystywać
zmienne globalne PHP i funkcje (np. ``$_GET`` lub ``header()``) do zarządzania
komunikatami HTTP, Symfony używa obiekty :class:`Symfony\\Component\\HttpFoundation\\Request`
i :class:`Symfony\\Component\\HttpFoundation\\Response`. Możliwie najprostszy 
kontroler można utworzyć ręcznie, na podstawie żądania::

    use Symfony\Component\HttpFoundation\Response;

    $name = $request->query->get('name');

    return new Response('Hello '.$name, 200, array('Content-Type' => 'text/plain'));

.. note::

    Symfony2 uwzglednia specyfikację HTTP, której reguły rządzą całą komunikacją w sieci. 
    Przeczytaj rozdział podręcznika ":doc:`/book/http_fundamentals`", aby dowiedzieć 
    się o tym więcej.

Symfony2 wybiera kontroler na podstawie wartości ``_controller`` z 
konfiguracji trasowania: ``AcmeDemoBundle:Welcome:index``. Ten ciąg znaków jest
*logiczną nazwą* kontrolera i odwołuje się do metody ``indexAction`` z
:class:`Acme\DemoBundle\Controller\WelcomeController`::

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

Klasa ``WelcomeController`` rozszerza wbudowaną klasę :class:`Controller`,
która dostarcza użytecznych skrótowych metod, takich jak metoda
`render() <http://api.symfony.com/2.0/Symfony/Bundle/FrameworkBundle/Controller/Controller.html#render()>`_
ładująca i renderująca szablon (``AcmeDemoBundle:Welcome:index.html.twig``).
Zwracaną wartością jest obiekt ``Response`` wypełniony zrenderowaną zawartością strony.
Jeżeli wystąpi taka potrzeba, to obiekt ``Response`` może zostać zmodyfikowany przed
przesłaniem go do przeglądarki::

   public function indexAction()
   {
      $response = $this->render('AcmeDemoBundle:Welcome:index.txt.twig');
      $response->headers->set('Content-Type', 'text/plain');
      
      return $response;
   }

Nie ważne jak to jest robione, ostatecznym celem kontrolera jest zawsze zwrócenie
obiektu ``Response``, który następnie powinien być dostarczony do użytkownika.
Ten obiekt może być wypełniony kodem HTML, reprezentować przekierowanie klienta
lub nawet zwracać zawartość obrazu JPG nagłówka z ``Content-Type image/jpg``.

.. tip::
   
   Korzystanie z rozszerzenia klasy ``Controller`` jest opcjonalne. W rzeczywistości
   kontroler moze być zwykłą funkcją PHP lub nawet domknięciem PHP.
   Rozdział ":doc:`/book/controler`" książki wyjaśnia wszystko o kontrolerach Symfony2.
   
Nazwa szablonu ``AcmeDemoBundle:Welcome:index.html.twig``, to logiczna nazwa
odwołująca się do pliku ``Resources/views/Welcome/index.html.twig`` wewnątrz
``AcmeDemoBundle` (umieszczonego w ``src/Acme/DemoBundle``). Niżej zawarty rozdział
o pakietach wyjaśnia, dlaczego jest to takie użyteczne.

Teraz ponownie zajrzyj do konfiguracji tras i znajdź klucz ``_demo``:

.. code-block:: yaml
   :linenos:
   
   # app/config/routing_dev.yml
   _demo:
      resource: "@AcmeDemoBundle/Controller/DemoController.php"
      type:     annotatio
      prefix:   /demo
   
Symfony2 może czytać (importować) informację o trasach z różnych plików napisanych
w formatach YAML, XML, PHP lub nawet adnotacji osadzonych w PHP. Tutaj nazwa logiczna
pliku, to ``@AcmeDemoBundle/Controller/DemoController.php`` i odnosi się ona do pliku
``src/Acme/DemoBundle/Controller/DemoController.php``. W pliku tym trasy są określone
jako adnotacje o metodach działania::

   // src/Acme/DemoBundle/Controller/DemoController.php
   use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
   use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
   
   class DemoController extends Controller
   {
      /**
      * @Route("/hello/{name}", name="_demo_hello")
      * @Template()
      */
      public function helloAction($name)
      {
         return array('name' => $name);
      }
      
      // ...
   }

Adnotacja @Route() określa nową ścieżkę dostępu ze wzorca ``/hello/{name}`` który
po dopasowaniu wykonuje metodę ``helloAction``. Łańcuch ujęty w nawiasy klamrowe,
taki jak ``{name}`` nosi nazwę wieloznacznika (symbolu zastępczego). Jak widać,
jego wartość można zastąpić argumentem metody ``$name``.

.. note::
   
   Jeśli nawet adnotacje nie są natywnie obsługiwane przez PHP, korzystanie z nich
   w Symfony2 jest wygodnym sposobem konfigurowania zachowania się frameworka
   i utrzymania konfiguracji poza kodem.

Jeżeli przyjrzeć się dokładniej kodowi kontrolera, to można zauważyć, ze zamiast
renderowania szablonu i zwrócenia obiektu ``Response``, jak poprzednio, zwracana
teraz jest tablica parametrów. Adnotacja ``@Template()`` powiadamia Symfony aby
renderował szablon, przechodząc w każdej z tych zmiennych z tablicy do szablonu.
Nazwa renderowanego szablonu występuje po nazwie kontrolera. Tak więc w tym przykładzie,
renderowany jest szablon ``AcmeDemoBundle:Demo:hello.html.twig`` (znajduje się w
``src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig``).

.. tip::
   
   Adnotacje ``@Route()`` i ``@Template()`` są bardziej skomplikowane, niż pokazano
   to w tym przewodniku. Więcej dowiesz się o "adnotacjach w kontrolerach" w
   :doc:`oficjalnej dokumentacji adnotacji </bundles/SensioFrameworkExtraBundle>`.
   
Szablony
~~~~~~~~

Kontroler renderuje szablon ``src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig``
(lub ``AcmeDemoBundle:Demo:hello.html.twig`` jeśli używa się logicznej nazwy):

.. code-block:: html + jinja
   :linenos:
      
   {# src/Acme/DemoBundle/Resources/views/Demo/hello.html.twig #}
   {% extends "AcmeDemoBundle::layout.html.twig" %}
   
   {% block title "Hello " ~ name %}
   
   {% block content %}
      <h1>Hello {{ name }}!</h1>
   {% endblock %}

Symfony2 stosuje domyślnie silnik szablonów `Twig`_,
ale można również korzystać z tradycyjnych szablonów PHP. Natępny rozdział przedstawia
jak działają szablony w Symfony2.

Pakiety
~~~~~~~

Może zastanawiałeś się, do czego odnosi się słowo :term:`pakiet`(*ang. bundle*),
które już kilkakrotnie zostało użyte wcześniej? Cały kod tworzony dla jakiejś aplikacji
jest zorganizowany w pakiety. W Symfony2 mówi się, że pakiet, to uporządkowany zestaw plików
(plików PHP, arkuszy stylów, skryptów JavaScript, obrazów, ...), które implementują
pojedyńczą funkcjonalność (blog, forum, ...) i które mogą być łatwo udostępniane
innym programistom. Dotąd manipulowaliśmy jednym pakietem - ``AcmeDemoBundle``.
Dowiesz się więcej na temat pakietów w ostatnim rozdziale tego przewodnika.

.. _quick-tour-big-picture-environments:

Praca ze środowiskami
---------------------

Teraz, gdy już lepiej rozumiemy działanie Symfony2, przyjrzymy sie bliżej stopce
renderowanej na każdej stronie Symfony2. Możesz tam zauważyć mały pasek z logo Symfony2.
Jest on nazywany "paskiem debugowania" (*ang. "Web Debug Toolbar"*) i jest to najlepszy
przyjaciel programisty.

.. image:: /images/quick_tour/web_debug_toolbar.png
   :align: center
   
Ale to, co widzisz na początku jest tylko wierzchołkiem góry lodowej. Kliknij na
dziwne liczby szesnastkowe, aby odsłonić kolejne bardzo przydatne narzędzie do
debugowania Symfony2: profiler.

.. image:: /images/quick_tour/profiler.png
   :align: center

Oczywiście nie będziesz chciał pokazywać tych narzędzi w środowisku produkcyjnym witryny.
Dlatego znajdziesz w katalogu ``web/`` inny kontroler wejściowy (``app.php``), który
jest zoptymalizowany dla środowiska produkcyjnego:

.. code-block:: text
   
   http://localhost/Symfony/web/app.php/demo/hello/Fabien

Jeśli używa się Apache z włączoną opcją mod_rewrite, to można pominąć w adresie
URL część ``app.php``:

.. code-block:: text
   
   http://localhost/Symfony/web/demo/hello/Fabien
   
Co nie mniej ważne, na serwerach produkcyjnych powinno się wskazać katalog główny
serwera WWW na katalog web/ w celu zabezpieczenia swojej instalacji i aby mieć lepszy
adres URL, wyglądający tak:

.. code-block:: text
   
   http://localhost/demo/hello/Fabien

Symfony2 utrzymuje pamięć podręczną w katalogu ``app/cache/`` aby aplikacja reagowała
szybciej. W środowisku programistycznym (``app_dev.php``) pamięć podręczna jest
opróżniana automatycznie, gdy tylko zostaną wprowadzone zmiany kodu lub konfiguracji.
Lecz w przypadku środowiska produkcyjnego (``app.php``), gdzie jakość ma kluczowe
znaczenie, tak się nie dzieje. Dlatego przy programowaniu aplikacji należy zawsze
używać środowiska programistycznego.


Różne :term:`środowiska <środowisko>` danej aplikacji różnią się tylko swoją konfiguracją.
W rzeczywistości konfigurację można dziedziczyć z innej konfiguracji:

.. code-block:: yaml
   :linenos:
   
   # app/config/config_dev.yml
   imports:
      - { resource: config.yml }
   
   web_profiler:
      toolbar: true
      intercept_redirects: false

W tym przykładzie, środowisko programistyczne (ktore ładuje plik konfiguracyjny
``config_dev.yml``) importuje globalny plik ``config.yml`` i go modyfikuje, udostępniając
pasek debugowania.


Podsumowanie
------------

Gratulacje! Miałaś przedsmak kodowania Symfony2. To nie było tak trudne, prawda?
Jest dużo więcej do odkrycia, ale teraz trzeba zobaczyć, jak Symfony2 sprawia,
że ​​naprawdę łatwo jest wdrożyć strony internetowe. Jeśli chcesz się dowiedzieć
więcej o Symfony2, zacznij lekturę następnej część przewodnika: ":doc:`the_view`.


.. _`Symfony2 Standard Edition`:      http://symfony.com/download
.. _`Symfony w 5 minut`:              http://symfony.com/symfony-in-five-minutes
.. _`YAML`:                           http://www.yaml.org/
.. _`Adnotacje w kontrolerach`:       http://symfony.com/doc/current/bundles/SensioFrameworkExtraBundle/index.html#annotations-for-controllers
.. _`Twig`:                           http://www.twig-project.org/
.. _`stronę instalacyjną Symfony`:    http://symfony.com/download