.. highlight:: php
   :linenothreshold: 2

.. index::
   single: Symfony2; podstawy
   single: HTTP; podstawy

Podstawy Symfony2 i HTTP
========================

Ucząc się Symfony2, znalazłeś się na dobrej drodze aby być bardziej produktywnym
twórcą nowoczesnych aplikacji internetowych. Symfony2 powstał po to, aby programista
otrzymał narzędzia pozwalające na szybsze programowanie i budowanie bardziej
niezawodnych aplikacji internetowych, w swobodny sposób. Symfony wykorzystuje
najlepsze pomysły z wielu technologii. Narzędzia i koncepcje, których się nauczysz
reprezentują wysiłek tysięcy ludzi w ciągu wielu lat. Innymi słowami, nie jest to
tylko nauka "Symfony" - nauczysz się podstaw internetu, najlepszych praktyk
programistycznych oraz tego, jak stosować wiele wyśmienitych nowoczesnych bibliotek
PHP, wewnątrz Symfony2 lub poza nim.

By być wiernym filozofii Symfony2, ten rozdział rozpoczyna się od wyjaśnienia
podstawowego pojęcia z zakresu programowania aplikacji internetowych: protokołu HTTP.
Niezależnie od stopnia zaawansowania oraz preferowanego języka rozdział ten jest
**lekturą obowiązkową dla każdego**.

HTTP jest prosty
----------------

HTTP (dla maniaków: Hypertext Transfer Protocol) jest tekstowym językiem
umożliwiającym, aby dwa komputery mogły się ze sobą komunikować. To jest to! Dla
przykładu, podczas sprawdzania najnowszego komiksu `xkcd`_ ma miejsce (w uproszczeniu)
następująca rozmowa:

.. image:: /images/http-xkcd.png
   :align: center

Chociaż w tej „rozmowie” rzeczywiście używany język jest nieco bardziej formalny,
to jest nadal dziecinnie prosty. HTTP jest terminem używanym do opisania prostego
języka tekstowego i jest niezależny od języka programowania aplikacji internetowych.
Celem każdego serwera internetowego jest zawsze zrozumienie prostego tekstu żądania
i zwrócenie tekstu odpowiedzi.

Symfony2 jest zbudowany wokół tej rzeczywistości. Czy jesteś tego świadomy, czy
też nie, HTTP jest czymś, co codziennie używasz serwując w internecie. Z Symfony2
nauczysz się jak mistrzowsko to opanować.

.. index::
   single: HTTP; paradygmat żądanie-odpowiedź

Krok 1: klient wysyła żądanie
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Każda rozmowa w internecie rozpoczyna się od wysłania żądania. Żądanie jest
komunikatem tekstowym utworzonym przez klienta (np. przeglądarkę, aplikację
iPhone itd.) w specjalnym formacie zwanym **protokołem HTTP**. Klient wysyła
żądanie do serwera i czeka na odpowiedź.

Spójrz na pierwsza część interakcji pomiędzy przeglądarką a serwerem internetowym
xkcd (żądanie):

.. image:: /images/http-xkcd-request.png
   :align: center

W „języku HTTP”, to żądanie HTTP będzie wyglądać następująco:

.. code-block:: text
   :linenos:

    GET / HTTP/1.1
    Host: xkcd.com
    Accept: text/html
    User-Agent: Mozilla/5.0 (Macintosh)

Ten prosty komunikat przekazuje wszystkie niezbędne informacje dokładnie określające
żądany przez klienta zasób. Pierwsza linia żądania HTTP jest najważniejsza i zawiera
dwie rzeczy: adres URI i metodę HTTP.

URI (np. ``/``, ``/contact`` itd.) jest unikalnym adresem lub lokalizacją
identyfikującą żądany przez klienta zasób. Metoda HTTP (np. ``GET``) określa co
chce się zrobić z tym zasobem. Metody HTTP są czasownikami żądania i określają
kilka typowych sposobów oddziaływania na zasób:

+----------+-------------------------------+
| *GET*    | Pobierz zasób z serwera       |
+----------+-------------------------------+
| *POST*   | Utwórz zasób na serwerze      |
+----------+-------------------------------+
| *PUT*    | Zaktualizuj zasób na serwerze |
+----------+-------------------------------+
| *DELETE* | Usuń zasób z serwera          |
+----------+-------------------------------+

Mając to na uwadzę, można sobie wyobrazić jak może wyglądać żądanie HTTP na przykład
dla usunięcia wpisu bloga:

.. code-block:: text

    DELETE /blog/15 HTTP/1.1

.. note::

    W rzeczywistości istnieje dziewięć metod HTTP zdefiniowane
    w specyfikacji HTTP, ale wielu z nich nie jest powszechnie
    wykorzystywanych lub obsługiwanych przez przeglądarki.
    Większość współczesnych przeglądarek nie obsługują na przykład
    metody PUT i DELETE.

Żądanie HTTP, oprócz pierwszej linii, zawiera też zawsze inne linie informacji
zwane nagłówkami żądania. Nagłówki mogą dostarczać szeroki wachlarz informacji,
takie jak żądany host (``Host``), akceptowane przez klienta formaty odpowiedzi
(``Accept``) czy nazwę aplikacji stosowanej przez klienta do wykonania żądania
(``User-Agent``). Istnieje wiele nagłówków i można się z nimi zapoznać w artykule
`List of HTTP header fields`_ na stronach Wikipedii.

Krok 2: Server zwraca odpowiedź
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Gdy serwer otrzyma żądanie, wie dokładnie jaki zasób jest potrzebny klientowi
(poprzez analizę URI) i co chce klient zrobić z tym zasobem (poprzez analizę metody).
Na przykład, w przypadku żądania GET serwer przygotowuje zasób i zwraca go w
odpowiedzi HTTP. Rozważmy odpowiedź z serwera internetowego xkcd:

.. image:: /images/http-xkcd.png
   :align: center

Odpowiedź przesłana z powrotem do przeglądarki, przetłumaczona na HTTP, będzie
wyglądać podobnie do tego:

.. code-block:: text
   :linenos:

    HTTP/1.1 200 OK
    Date: Sat, 02 Apr 2011 21:05:05 GMT
    Server: lighttpd/1.4.19
    Content-Type: text/html

    <html>
      <!-- ... HTML for the xkcd comic -->
    </html>

Odpowiedź HTTP zawiera żądany zasób (w tym przypadku treść HTML), a także inne
informacje dotyczące odpowiedzi. Pierwsza linia jest szczególnie ważna i zawiera
kod stanu odpowiedzi HTTP (w tym przypadku 200). Kod stanu informuje o ogólnym
wyniku wywołania zwrotnego przesyłanego klientowi. Czy żądanie odniosło sukces?
Czy wystąpił błąd? Istnieją różne kody stanu wskazujące na sukces, błąd lub na
konieczność wykonania czegoś przez klienta (np. przekierowania do innej strony).
Z pełną litą kodów stanu odpowiedzi HTTP można się zapoznać w artykule
`List of HTTP status codes`_ na stronach Wikipedii.

Podobnie jak żądanie, odpowiedź HTTP zawiera porcję dodatkowej informacji nazywanej
*nagłówkami HTTP*. Na przykład, jednym z ważniejszych nagłówków odpowiedzi HTTP
jest ``Content-Type``. Samo ciało odpowiedzi może zostać zwrócone w wielu różnych
formatach, takich jak HTML, XML lub JSON a nagłówek ``Content-Type`` wykorzystuje
internetowe typy mediów, takie jak ``text/html``, aby poinformować klienta, jaki
format jest zwracany w odpowiedzi. Listę popularnych typów mediów można znaleźć w
artykule `List of common media types`_ na stronach Wikipedii.

Używa się wiele nagłówków, niektóre z nich są bardzo użyteczne. Na przykład,
niektóre nagłówki mogą być używane do tworzenia wydajnego systemu buforowania.

Żądanie, odpowiedź a tworzenie aplikacji internetowej
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Konwersacja żądanie-odpowiedź jest podstawowym procesem napędzającym całą komunikację
w internecie. Pomimo, że jest to proces tak ważny i zaawansowany, to jest on również
bardzo prosty.

Najważniejsze jest to, że niezależnie od używanego języka, rodzaju aplikacji
(web, mobile, JSON API) lub przyjetej filozofii tworzenia aplikacji, ostatecznym
celem aplikacji jest **zawsze** przeanalizowanie każdego żądania i zwrócenie
odpowiedniej odpowiedzi.

Symfony jest zaprojektowany tak, aby dopasować sie do tej rzeczywistości.

.. tip::

    Aby dowiedzieć się więcej o specyfikacji HTTP przeczytaj dokument `HTTP 1.1 RFC`_
    lub `HTTP Bis`_, które wyjaśniają oryginalna specyfikację tego protokołu.
    Doskonałym narzędziem do sprawdzania nagłówków żądań i odpowiedzi podczas
    przeglądania jest rozszerzenie `Live HTTP Headers`_ do Firefox.

.. index::
   single: Symfony2; żądanie i odpowiedź

Żądanie i odpowiedź w PHP
-------------------------

Jak więc można oddziaływać na "żądanie" i tworzyć "odpowiedzi" przy użyciu PHP?
W rzeczywistości PHP zwalnia Cię po części z takiej konieczności::

    $uri = $_SERVER['REQUEST_URI'];
    $foo = $_GET['foo'];

    header('Content-type: text/html');
    echo 'The URI requested is: '.$uri;
    echo 'The value of the "foo" parameter is: '.$foo;

Choć brzmi to dziwnie, ta mała aplikacja jest rzeczywistości pobiera informację z
żądania HTTP i używa ją do utworzenia odpowiedzi HTTP. Zamiast parsować surowy
komunikat żądania HTTP, PHP przygotowuje super globalne zmienne, takie jak
``$_SERVER`` i ``$_GET``, które zawierają wszystkie informacje o żądaniu.
Podobnie, zamiast zwracać odpowiedź tekstem formatowanym w HTTP, można użyć
funkcję ``header()`` do utworzenia nagłówków odpowiedzi i po prostu wydrukowania
rzeczywistej treści, która będzie porcją zawartości komunikatu odpowiedzi.
PHP utworzy prawdziwą odpowiedź HTTP i zwróci ją klientowi:

.. code-block:: text

    HTTP/1.1 200 OK
    Date: Sat, 03 Apr 2011 02:14:33 GMT
    Server: Apache/2.2.17 (Unix)
    Content-Type: text/html

    The URI requested is: /testing?foo=symfony
    The value of the "foo" parameter is: symfony


Żądanie i odpowiedź w Symfony
-----------------------------

Symfony stanowi alternatywę dla surowego podejścia PHP, wykorzystując dwie klasy
pozwalające na interakcje z żądaniem HTTP i odpowiedzią w łatwy sposób.
Klasa :class:`Symfony\\Component\\HttpFoundation\\Request` jest prostą, obiektowo
zorientowaną reprezentacją komunikatu żądania HTTP. Dzięki niej ma się wszystkie
informacje o żądaniu pod ręką::

    use Symfony\Component\HttpFoundation\Request;

    $request = Request::createFromGlobals();

    // the URI being requested (e.g. /about) minus any query parameters
    $request->getPathInfo();

    // retrieve GET and POST variables respectively
    $request->query->get('foo');
    $request->request->get('bar', 'default value if bar does not exist');

    // retrieve SERVER variables
    $request->server->get('HTTP_HOST');

    // retrieves an instance of UploadedFile identified by foo
    $request->files->get('foo');

    // retrieve a COOKIE value
    $request->cookies->get('PHPSESSID');

    // retrieve an HTTP request header, with normalized, lowercase keys
    $request->headers->get('host');
    $request->headers->get('content_type');

    $request->getMethod();          // GET, POST, PUT, DELETE, HEAD
    $request->getLanguages();       // an array of languages the client accepts


Klasa ``Request`` wykonuje dużo pracy w tle, zwalniając programistę z konieczności
pisania rozwlekłego kodu. Na przykład, metoda ``isSecure()`` sprawdza trzy różne
wartości w PHP wskazujące na to, czy użytkownik wykorzystuje bezpieczne połączenie
(np. ``https``).

.. sidebar:: atrybuty ParameterBags i Request

    Jak wyżej widać, zmienne ``$_GET`` i ``$_POST`` są dostępne poprzez publiczne
    właściwości, odpowiedznio ``query`` i ``request``. Każdy z tych obiektów jest
    obiektem klasy :class:`Symfony\\Component\\HttpFoundation\\ParameterBag`, który
    ma metody takie jak
    :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::get`,
    :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::has`,
    :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::all` i więcej.
    W rzeczywistości każda publiczna właściwość użyta w poprzednim przykładzie
    jest instancją klasy ParameterBag.
    
    .. _book-fundamentals-attributes:
      
    Klasa Request ma również publiczną właściwość attributes, która przechowuje
    specjalne dane dotyczące tego, jak aplikacja działa wewnętrznie.
    We frameworku Symfony2 właściwość ``attributes`` przechowuje wartości zwracane
    przez dopasowaną trasę, takie jak ``_controller``, ``id`` (jeżeli ma się
    wieloznacznik ``{id})`` a nawet nazwę dopasowanej trasy (``_route``).
    Właściwość ``attributes`` istnieje wyłącznie po to, aby być miejscem, gdzie
    można przygotować i przechowywać informacje o żądaniu, specyficzne dla kontekstu.

Symfony również udostępnia klasę ``Response`` – prostą reprezentację PHP komunikatu
odpowiedzi HTTP. Umożliwia ona aplikacji wykorzystanie obiektowo zorientowanego
interfejsu do tworzenia odpowiedzi, jakie mają być zwracane klientowi::

    use Symfony\Component\HttpFoundation\Response;
    $response = new Response();

    $response->setContent('<html><body><h1>Hello world!</h1></body></html>');
    $response->setStatusCode(200);
    $response->headers->set('Content-Type', 'text/html');

    // prints the HTTP headers followed by the content
    $response->send();

Gdyby Symfony nie oferował nic ponadto, to miałbyś już narzędzie do łatwego
uzyskiwania dostępu do informacji żądania i obiektowo zorientowany interfejs do
tworzenia odpowiedzi. Nawet jak nauczysz się wykorzystywać wiele zaawansowanych
możliwości Symfony, to pamiętaj, że celem aplikacji jest zawsze *interpretacja
żądania i utworzenie odpowiedzi w oparciu o logikę aplikacji*.

.. tip::

    Klasy ``Request`` i ``Response` są częścią niezależnego komponentu włączonego
    do Symfony o nazwie ``HttpFoundation``. Jest to komponent niezależny i może
    być stosowany poza Symfony, dostarczając klas dla obsługi sesji i wysyłania plików.
    

Podróż od żądania do odpowiedzi
-------------------------------

Obiekty ``Request`` i ``Response`` są bardzo proste, podobnie jak HTTP.
Najtrudniejszym w tworzeniu aplikacji jest to, co trzeba napisać w środku. Innymi
słowami, prawdziwy trud napotyka się przy pisaniu kodu interpretującego informację
żądania i tworzącego odpowiedź.

Twoja aplikacja będzie przypuszczalnie robiła wiele rzeczy, takie jak wysyłanie
wiadomości e-mail, obsługa zgłoszeń formularzy, zapisywanie danych do bazy danych,
generowanie stron HTML i zabezpieczanie zawartości przez system bezpieczeństwa.
Jak zarządzać tym wszystkim i nadal mieć kod zorganizowany i łatwy w utrzymaniu?

Symfony został stworzony, aby rozwiązać wszystkie te problemy za Ciebie.

Kontroler wejściowy
~~~~~~~~~~~~~~~~~~~

Zwykle, aplikacje są budowane tak, aby każda "strona" witryny była fizycznym plikiem:

.. code-block:: text
   :linenos:

    index.php
    contact.php
    blog.php

Istnieje kilka problemów związanych z takim podejściem, włączając w to brak
elastyczności w adresowaniu URL (co jeśli chce się zmienić ``blog.php`` na
``news.php`` bez zerwania wszystkich linków?) i fakt, że każdy plik musi ręcznie
dołączać pewien zbiór plików rdzenia, tak aby bezpieczeństwo, połączenia z bazą
danych i wyszukiwanie mogły być spójne.

Znacznie lepszym rozwiązaniem jest użycie :term:`kontrolera wejsciowego` –
pojedynczego pliku PHP obsługującego każde żądanie kierowane do aplikacji.
Na przykład:

+------------------------+------------------------+
| ``/index.php``         | wykonuje ``index.php`` |
+------------------------+------------------------+
| ``/index.php/contact`` | wykonuje ``index.php`` |
+------------------------+------------------------+
| ``/index.php/blog``    | wykonuje ``index.php`` |
+------------------------+------------------------+

.. tip::

    Wykorzystując ``moduł mod_rewrite` Apache (lub równoważny dla innych serwerów
    internetowych), można używać tzw. przyjaznych adresów URL, takich jak ``/``,
    ``/contact`` czy ``/blog``.
    
Teraz każde żądanie jest obsługowane dokładnie w taki sam sposób. Zamiast
pojedynczych adresów URL wykonujących różne pliki PHP, jest *zawsze* wykonywany
kontroler wejścia a trasowanie różnych adresów URL do różnych części aplikacji
wykonywane jest wewnętrznie. Rozwiązuje to obydwa problemy wynikające z pierwotnego
rozwiązania. Prawie wszystkie współczesne aplikacje internetowe tak robią – włączając
w to WordPress.


Bądź zorganizowany
~~~~~~~~~~~~~~~~~~

Ale jak wiedzieć, która strona powinna być wygenerowana przez kontroler i jak można
wykonać generowanie każdej strony w sposób jasny? Tak czy owak, trzeba sprawdzić
przychodzące adresy URI i wykonać różne części kodu, w zależności od tej wartości.
Można to zrobić szybko i brzydko::

    // index.php
    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\HttpFoundation\Response;
    $request = Request::createFromGlobals();
    $path = $request->getPathInfo(); // the URI path being requested

    if (in_array($path, array('', '/'))) {
        $response = new Response('Welcome to the homepage.');
    } elseif ($path == '/contact') {
        $response = new Response('Contact us');
    } else {
        $response = new Response('Page not found.', 404);
    }
    $response->send();

Rozwiązanie tego problemu może być trudne. Na szczęście jest to dokładnie
zaprojektowane w Symfony.

Przetwarzanie w aplikacji Symfony
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Kiedy zdecydujesz się powierzyć Symfony obsługę każdego żądania, to życie może
stać się łatwiejsze. Symfony stosuje taki sam prosty wzorzec dla każdego żądanie:

.. _request-flow-figure:

.. figure:: /images/request-flow.png
   :align: center
   :alt: Symfony2 request flow

Przychodzące żądania są interpretowane przez trasowanie (ang. routing) i przekazywane
są do funkcji (metody) kontrolera, który zwraca obiekt Response.

Każda "strona" witryny jest zdefiniowana w pliku konfiguracji trasowania, który 
odwzorowuje adresy URL na funkcje PHP. Zadaniem każdej takiej funkcji
PHP, nazywanej :term:`kontrolerem`, jest wykorzystanie informacji z żądania
(wraz z wielu innymi narzędziami udostępnionymi w Symfony) dla utworzenia i
zwrócenia obiektu ``Response``. Innymi słowami, kontroler jest tą częścią kodu,
która interpretuje żądanie oraz tworzy i zwraca odpowiedź.

Jest to takie proste. Przyjrzyjmy się temu:

* Każde żądanie przetwarzane jest przez kontroler wejściowy;

* System trasowania, w oparciu o informacje z żądania i konfigurację trasowania,
  określa jakie mają zostać wykonane funkcje PHP;

* Wykonywana jest właściwa funkcja PHP, tworząc i zwracając odpowiedni obiekt ``Response``.

Żądanie Symfony w akcji
~~~~~~~~~~~~~~~~~~~~~~~

Przyglądnijmy się temu procesowi, bez zagłębiania się w szczegóły.
Załóżmy, że chcesz dodać stronę ``/contact`` do swojej aplikacji Symfony.
W pierwszej kolejności dodaj wpis dla ``/contact`` do pliku konfiguracji trasowania:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/routing.yml
        contact:
            path:     /contact
            defaults: { _controller: AcmeDemoBundle:Main:contact }

    .. code-block:: xml
       :linenos:

        <route id="contact" path="/contact">
            <default key="_controller">AcmeBlogBundle:Main:contact</default>
        </route>

    .. code-block:: php
       :linenos:

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('contact', new Route('/contact', array(
            '_controller' => 'AcmeBlogBundle:Main:contact',
        )));

        return $collection;

.. note::

   W tym przykładzie do określenia konfiguracji trasowania zastosowano format YAML.
   Konfiguracja trasowania może być również napisana w innych formatach, takich
   jak XML lub PHP.

Kiedy ktoś odwiedza stronę ``/contact``, to dopasowywana jest trasa i wykonywany
jest określony kontroler. Jak można się dowiedzieć w rozdziale :doc:`routing chapter</book/routing>`,
łańcuch ``AcmeDemoBundle:Main:contact`` jest skróconą składnią wskazującą metodę
``contactAction`` wewnątrz klasy o nazwie ``MainController``::

    // src/Acme/DemoBundle/Controller/MainController.php
    use Symfony\Component\HttpFoundation\Response;

    class MainController
    {
        public function contactAction()
        {
            return new Response('<h1>Contact us!</h1>');
        }
    }


W tym prostym przykładzie kontroler tworzy obiekt :class:`Symfony\\Component\\HttpFoundation\\Response`
z kodem HTML ``<h1>Contact us!</h1>``.
W rozdziale :doc:`controller chapter</book/controller>`, dowiesz się jak kontroler
może przetwarzać szablony, umożliwiając by kod „warstwy prezentacji” (czyli cokolwiek,
co napisane jest w HTML) był zapisany w oddzielnym pliku. Odciąża to kontroler,
pozostawiając mu trudniejsze zadania: interakcję z bazą danych, obsługę przekazywanych
danych lub wysyłanie wiadomości e-mail.

Symfony2: Buduj swoja aplikacje a nie swoje narzędzia
-----------------------------------------------------

Teraz już wiesz, że celem każdej aplikacji jest zinterpretowanie przychodzącego
żądania HTTP i utworzenie odpowiedniej odpowiedzi. Gdy aplikacja jest rozbudowywana,
staje się coraz trudniejszym utrzymanie kodu w dobrej organizacji. Niezmiennie
wykonywane są w kółko te same złożone zadania: utrzymywanie zapisów w bazie danych,
generowanie i ponowne wykorzystywanie szablonów, obsługa zgłoszeń z formularzy,
wysyłanie wiadomości e-mail, walidacja danych wprowadzanych przez użytkownika
i obsługa bezpieczeństwa.

Dobrą wiadomością jest to, że żadne z tych zadań nie jest wyjątkowe. Symfony oferuje
pełny framework narzędzi, które pozwalają zbudować aplikację, a nie własne narzędzia.
W Symfony2 nic nie jest narzucone programiście: ma on pełną swobodę w wykorzystaniu
frameworka, tylko jakiejś jego części albo całości.



.. index::
   single: Symfony2; komponenty


Standalone Tools: The Symfony2 *Components*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Więc czym jest Symfony2? Po pierwsze, Symfony2 jest zbiorem ponad dwudziestu
niezależnych bibliotek, które mogą być wykorzystane w jakimkolwiek projekcie PHP.
Biblioteki te, o nazwie Symfony2 Components, zawierają pożyteczny kod dla niemal
każdego rozwiązania, niezależnie od tego jak projekt jest tworzony. Oto kilka z nich:

* :doc:`HttpFoundation</components/http_foundation/introduction>` -  zawiera klasy
   ``Request`` i ``Response``, jak również klasy do obsługi sesji i pobierania plików;

* :doc:`Routing</components/routing/introduction>` - zaawansowany i szybki system
   trasowania pozwalający odwzorować konkretny adres URI (np. ``/contact``) na
   informację o tym jak żądanie powinno zostać obsłużone (np. poprzez wykonanie
   metody ``contactAction()``);

* `Form`_ - w pełni funkcjonalna biblioteka do tworzenia formularzy i obsługi
   zgłoszeń formularza;

* `Validator`_ system do tworzenia reguł dotyczących danych i sprawdzanie danych
   pod kątem spełniania tych reguł;;

* :doc:`ClassLoader</components/class_loader>` biblioteka automatycznego ładowania
   klas PHP, bez konieczności wczytywanie plików klas przez funkcj PHP (``require`` itp.);;

* :doc:`Templating</components/templating>` zestaw narzędzi do przetwarzania szablonów,
   obsługi dziedziczenia szablonów (czyli szablon jest kształtowany na bazie układów - *ang. layouts*)
   oraz do wykonywania innych zadań szablonu;

* `Security`_ - bardzo zaawansowana biblioteka do obsługi wszystkich aspektów bezpieczeństwa
   wewnątrz aplikacji;

* `Translation`_ zbiór bibliotek do tłumaczenia łańcuchów tekstowych w aplikacji.

Każdy z tych komponentów jest samodzielny i może być wykorzystany oddzielnie w
dowolnym projekcie PHP, niezależnie od tego, czy używa się frameworka Symfony2,
czy też nie. Każda część jest zrobiona po to, aby być wykorzystana jeżeli zachodzi
taka potrzeba

Pełne rozwiązanie: *framework* Symfony2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Więc czym jest ten framework Symfony2? Framework Symfony2 jest biblioteką PHP
realizujący dwa oddzielne zadania:

#. Zapewnienia wybór komponentów (czyli *Symfony2 Components*) i dodatkowych
   bibliotek (np. `Swiftmailer`_ dla wysyłania wiadomości e-mail);

#. Zapewnienia sensowną konfigurację i "sklejenie" wszystkich bibliotek w całość.

Celem frameworka jest zintegrowanie wielu niezależnych narzędzi w jeden spójny
interfejs programistyczny. Nawet sam framework jest pakietem (ang. bundle)
(czyli wtyczką) mogącą zostać skonfigurowaną i całkowicie zmienioną.

Symfony2 dostarcza potężny zestaw narzędzi do szybkiego tworzenia aplikacji
internetowych, bez narzucania programiście rozwiązań w zakresie funkcjonalności
aplikacji. Zwykły użytkownik może szybko rozpocząć programowanie, stosując okreśłoną
dystrybucje Symfony2, która dostarcza framework z sensownymi domyślnymi ustawieniami.
Dla bardziej zaawansowanych użytkowników praktycznie nie ma ograniczeń.

.. _`xkcd`: http://xkcd.com/
.. _`HTTP 1.1 RFC`: http://www.w3.org/Protocols/rfc2616/rfc2616.html
.. _`HTTP Bis`: http://datatracker.ietf.org/wg/httpbis/
.. _`Live HTTP Headers`: https://addons.mozilla.org/en-US/firefox/addon/live-http-headers/
.. _`List of HTTP status codes`: http://en.wikipedia.org/wiki/List_of_HTTP_status_codes
.. _`List of HTTP header fields`: http://en.wikipedia.org/wiki/List_of_HTTP_header_fields
.. _`List of common media types`: http://en.wikipedia.org/wiki/Internet_media_type#List_of_common_media_types
.. _`Form`: https://github.com/symfony/Form
.. _`Validator`: https://github.com/symfony/Validator
.. _`Security`: https://github.com/symfony/Security
.. _`Translation`: https://github.com/symfony/Translation
.. _`Swiftmailer`: http://swiftmailer.org/
