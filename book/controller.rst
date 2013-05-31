.. highlight:: php
   :linenothreshold: 2

.. index::
   single: kontroler

Kontroler
=========

Kontroler to funkcja PHP, którą tworzy się, aby pobierała informacje z żądania
HTTP, a następnie konstruowała i zwracała odpowiedź HTTP (jako obiekt
``Response`` Symfony2). Odpowiedź może być stroną HTML, dokumentem XML,
serializowaną tablicą JSON, obrazem, przekierowaniem, stroną błędu 404
lub czymkolwiek innym. Kontroler może zawierać dowolną logikę wymagana do
wyrenderować zawartości strony.

Aby zobaczyć, jakie to proste, spójrzmy jak działa kontroler Symfony2.
Poniższy kontroler wygeneruje stronę, która wyświetlającą ``Hello world!``::

    use Symfony\Component\HttpFoundation\Response;

    public function helloAction()
    {
        return new Response('Hello world!');
    }

Zadanie kontrolera jest zawsze takie samo: stworzyć i zwrócić obiekt ``Response``.
Po drodze może odczytywać informacje z żądania, wczytywać dane z bazy danych,
wysłać email, czy zapisać informacje w sesji użytkownika. Jednakże w każdym przypadku
kontroler ostatecznie zwróci obiekt ``Response``, który będzie dostarczony z powrotem
do użytkownika.

Nie ma tutaj żadnej magii czy innych wymagań, o które trzeba się martwić. Oto kilka
najczęstszych przypadków:

* *Kontroler A* przygotowuje obiekt ``Response`` reprezentujący zawartość strony głównej
  witryny.

* *Kontroler B* odczytuje parametr ``slug`` z żądania, aby pobrać wpis blogu
  z bazy danych i utworzyć obiekt ``Response``, który wyświetli tego blogu. Jeśli
  ``slug`` nie zostanie znaleziony w bazie danych, kontroler utworzy obiekt ``Response``
  zawierający kod błędu 404.

* *Kontroler C* obsługuje formularz kontaktowy. Odczytuje dane formularza z żądania HTTP,
  zapisuje dane kontaktowe do bazy danych i wysyła email do administratora. W końcu tworzy
  obiekt ``Response``, który przekieruje przeglądarkę klienta do strony z podziękowaniami.

.. index::
   single: kontroler; cykl żądanie-kontroler-odpowiedź

Cykl żądanie-kontroler-odpowiedź
--------------------------------

Każde żądanie obsługiwane przez projekt Symfony2 przechodzi ten sam cykl.
Framework zajmuje się powtarzającymi się czynnościami i w końcu wykonuje kontroler,
który przechowuje indywidualny kod aplikacji:

#. Każde żądanie jest obsługiwane przez pojedynczy plik kontrolera wejścia
   (np. ``app.php`` czy ``app_dev.php``), który inicjuje aplikację;

#. ``Router`` odczytuje informacje z żądania (np. URI), znajduje trasę, która
    pasuje do tej informacji oraz odczytuje parametr ``_controller`` dopasowanej
    trasy;

#. Wykonywany jest kontroler z dopasowana trasą i kod w nim zawarty
   tworzy i zwraca obiekt ``Response``;

#. Odsyłane są do klienta nagłówki HTTP i zawartość obiektu ``Response`.

Tworzenie strony sprowadza się do utworzenie kontrolera (#3) i wykonania trasy,
która odwzorowuje adres URL do tego kontrolera (#2).

.. note::

    Pomimo podobnej nazwy, "kontroler wejścia" zupełnie różni się od "kontrolerów",
    o których mówimy w tym rozdziale. :term:`Kontroler wejścia<kontroler wejscia>`
    to krótki plik PHP, który znajduje się w katalogu `web` aplikacji i kierowane
    są doniego wszystkie przechodzące żądania. Typowa aplikacja posiada
    kontroler wejścia dla środowiska produkcyjnego (np. ``app.php``) i kontroler
    wejścia dla środowiska programistycznego (np. ``app_dev.php``).
    Prawdopodobnie nigdy nie będziesz musiał edytować, przeglądać czy martwić się
    o kontrolery wejścia swojej aplikacji.

.. index::
   single: kontroler; prosty przykład

Prosty kontroler
----------------

Ogólnie rzecz biorąc, kontrolerem może być dowolne wywołanie kodu PHP (funkcja,
metoda obiektu czy domknięcie), to jednak w Symfony2 :term:`kontroler` jest zazwyczaj
pojedynczą metodą w obiekcie kontrolera. Kontrolery są też nazywane *akcjami*.

.. code-block:: php
    :linenos:

    // src/Acme/HelloBundle/Controller/HelloController.php

    namespace Acme\HelloBundle\Controller;
    use Symfony\Component\HttpFoundation\Response;

    class HelloController
    {
        public function indexAction($name)
        {
          return new Response('<html><body>Hello '.$name.'!</body></html>');
        }
    }

.. tip::

    Prosze zauważyć, że *kontrolerem* jest metoda ``indexAction``, która zawarta
    jest wewnatrz *klasy kontrolera* (``HelloController``). Nie należy sugerowac się
    nazewnictwem. Klasa kontrolera to po prostu wygodny sposób na grupowanie kilku
    kontrolerów (akcji). Zazwyczaj klasa kontrolera przechowuje kilka kontrolerów
    (akcji) (np. ``updateAction``, ``deleteAction`` itd.)

Kontroler jest bardzo prosty, ale przeanalizujmy go:

* *linia 3*: Symfony2 korzysta z funkcjonalności przestrzeni nazw PHP 5.3, aby
  nazwać całą klasę kontrolera. Słowo kluczowe ``use`` importuje klasę ``Response``,
  którą nasz kontroler musi zwrócić.

* *linia 6*: Nazwa klasy to połączenie nazwy kontrolera (np. ``Hello``) i słowa
  ``Controller``. Jest to konwencja zapewniająca zgodność nazewniczą kontrolerów
  i pozwalająca na odwoływanie się do nich wyłącznie przez pierwszą część ich nazwy
  (np. ``Hello``) w konfiguracji trasowania.

* *linia 8*: Każda nazwa akcji w klasie kontrolera posiada przyrostek ``Action``
  i odwołuje się do konfiguracji trasowania poprzez nazwę akcji (``index``).
  W następnym rozdziale utworzymy trasę, która będzie odwzorowywac adres URL do
  akcji. Nauczysz się jak wieloznaczniki (*ang. placeholders*) trasy (``{name}``)
  stają się argumentami metody akcji (``$name``).

* *linia 10*: Kontroler tworzy i zwraca obiekt ``Response``.

.. index::
   single: kontroler; trasa

Odwzorowanie adresu URL do kontrolera
-------------------------------------

Nowy kontroler zwraca prostą stronę HTML. Aby móc zobaczyć tą stronę w przeglądarce,
trzeba utworzyć trasę (*ang. route*) odwzorowującą wzorzec ścieżki URL do kontrolera:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/routing.yml
        hello:
            path:      /hello/{name}
            defaults:     { _controller: AcmeHelloBundle:Hello:index }

    .. code-block:: xml
       :linenos:

        <!-- app/config/routing.xml -->
        <route id="hello" path="/hello/{name}">
            <default key="_controller">AcmeHelloBundle:Hello:index</default>
        </route>

    .. code-block:: php
       :linenos:

        // app/config/routing.php
        $collection->add('hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
        )));

Teraz , po wprowdzeniu ścieżki ``/hello/ryan`` wykonany zostanie kontroler
``HelloController::indexAction()`` i zostanie przekazana wartość ``ryan`` do zmiennej
``$name``. Tworzenie "strony" to tworzenie metody kontrolera i powiązanie jej z trasą.

Proszę zwrócić uwagę na składnię użytą w odwołaniu się do kontrolera ``AcmeHelloBundle:Hello:index``.
W celu odwoływania się do różnych kontrolerów Symfony2 uzywa ciągów znakowych o
elastycznej notacji. Jest to najczęściej używana składnia, która wskazuje Symfony2,
aby szukał klasy kontrolera o nazwie ``HelloController`` wewnątrz pakietu ``AcmeHelloBundle``.
Następnie wykonywana jest metoda ``indexAction()``.

Więcej informacji o formacie łańcucha odwoływania się do różnych kontrolerów można
znaleźć w rozdziale :ref:`Wzorzec nazewniczy kontrolera<controller-string-syntax>`.

.. note::

    W tym przykładzie konfiguracja trasowania znajduje się bezpośrednio w katalogu
    ``app/config/``. Lepszym rozwiazaniem  organizacji trasowania jest umieszczenie
    każdej trasy w pakiecie, do którego ona należy. Więcej informacji na ten temat
    można znaleźć w rozdziale :ref:`routing-include-external-resources`.

.. tip::

    Możesz dowiedzieć się więcej o systemie trasowania w rozdziale
    :doc:`routing`.

.. index::
   single: kontroler; argumenty kontrolera

.. _route-parameters-controller-arguments:

Parametry trasy jako argumenty kontrolera
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Już wiemy, że parametr ``_controller`` w kontrolerze ``AcmeHelloBundle:Hello:index``
odnosi się do metody ``HelloController::indexAction()`` znajdujacej się w pakiecie
``AcmeHelloBundle``. Co ciekwsze, jest to też argument przekazywany do tej metody::

    // src/Acme/HelloBundle/Controller/HelloController.php

    namespace Acme\HelloBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
          // ...
        }
    }

Kontroler ma pojedynczy argument ``$name``, który odpowiada parametrowi ``{name}``
z dopasowanej trasy (w naszym przykładzie ma on wartość ``ryan``). W rzeczywistości
podczas wykonywania kontrolera Symfony2 dopasowuje każdy argument kontrolera
do parametru trasy. Rozważmy następujący przykład:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/routing.yml
        hello:
            path:      /hello/{first_name}/{last_name}
            defaults:  { _controller: AcmeHelloBundle:Hello:index, color: green }

    .. code-block:: xml
       :linenos:

        <!-- app/config/routing.xml -->
        <route id="hello" path="/hello/{first_name}/{last_name}">
            <default key="_controller">AcmeHelloBundle:Hello:index</default>
            <default key="color">green</default>
        </route>

    .. code-block:: php
       :linenos:

        // app/config/routing.php
        $collection->add('hello', new Route('/hello/{first_name}/{last_name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
            'color'       => 'green',
        )));

W tym przykładzie kontroler może przyjąć kilka argumentów::

    public function indexAction($first_name, $last_name, $color)
    {
        // ...
    }

Prosze zwrócić uwagę, że obie zmienne (``{first_name}`` i ``{last_name}``)
oraz domyślny parametr ``color`` są dostępne w kontrolerze jako jego argumenty.
Kiedy trasa jest dopasowywana, parametry trasy oraz ``wartości domyślne`` są łączone
w jedną tablicę, która jest dostępna dla kontrolera.

Odwzorowanie parametrów trasy na argumenty kontrolera jest łatwe i elastyczne.
Należy pamiętać o następujących wskazówkach:

* **Kolejność argumentów kontrolera nie ma znaczenia**

    Symfony potrafi dopasować nazwy parametrów z trasy do nazw zmiennych z sygnatury
    metody kontrolera. Innymi słowy, Symfony rozumie, że parametr ``{last_name}``
    pasuje do argumentu ``$last_name``. Argumenty kontrolera mogą być kompletnie
    pomieszane i nadal będą działać poprawnie::

        public function indexAction($last_name, $color, $first_name)
        {
            // ..
        }

* **Każdy wymagany argument kontrolera musi pasować do parametru trasowania**

    Poniższy kod zgłosi wyjątek ``RuntimeException``, ponieważ parametr ``foo``
    nie został określony w trasie::

        public function indexAction($first_name, $last_name, $color, $foo)
        {
            // ..
        }

    Rozwiązaniem problemu może być przypisanie wartości domyślnej do argumentu.
    Poniższy przykład nie zgłosi wyjątku::

        public function indexAction($first_name, $last_name, $color, $foo = 'bar')
        {
            // ..
        }

* **Nie wszystkie parametry trasowania muszą być argumentami kontrolera**

    Jeśli, na przykład, ``last_name`` nie jest istotny dla kontrolera,
    można go całkowicie pominąć::

        public function indexAction($first_name, $color)
        {
            // ..
        }

.. tip::

    Każda trasa posiada również specjalny parametr ``_route``, który przyjmuje
    wartość nazwy dopasowanej trasy (np. ``hello``). Parametr ten dostępny jest
    jako argument kontrolera, ale jest mało przydatny.

.. _book-controller-request-argument:

Obiekt Request jako argument kontrolera
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Można również przekazać do Symfony obiekt ``Request`` jako argument kontrolera.
Jest to sczególnie wygodne podczas pracy z formularzem, na przykład::

    use Symfony\Component\HttpFoundation\Request;

    public function updateAction(Request $request)
    {
        $form = $this->createForm(...);

        $form->handleRequest($request);
        // ...
    }

.. index::
   single: kontroler; strony statyczne

Tworzenie stron statycznych
---------------------------

Można tworzyć strony statyczne nawet bez użycia kontrolera (potrzebna jest tylko
trasa i szablon).

Używaj tego! Zobacz :doc:`/cookbook/templating/render_without_controller`.


.. index::
   single: kontroler; podstawowa klasa kontrolera

Podstawowa klasa kontrolera
---------------------------

Symfony2 udostępnia klasę ``Controller`` będącą klasą podstawową (bazową) dla kontrolerów
aplikacji. Pomaga ona w najbardziej typowych zadaniach kontrolera i daje klasie
kontrolera dostęp do każdego potrzebnego zasobu. Rozszerzając klasę ``Controller``
można skorzystać z kilku metod pomocniczych (helperów).

Dodajmy instrukcję ``use`` na początku pliku kontrolera, a później zmodyfikujmy
``HelloController`` tak, aby dziedziczył z klasy ``Controller``:

.. code-block:: php
   :linenos:

    // src/Acme/HelloBundle/Controller/HelloController.php

    namespace Acme\HelloBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\HttpFoundation\Response;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
          return new Response('<html><body>Hello '.$name.'!</body></html>');
        }
    }

W rzeczywistości niczego to nie zmienia w sposobie działania kontrolera.
W następnym rozdziale dowiesz się o metodach pomocniczych (helperach), które są
udostępnione przez klasę kontrolera podstawowego. Te metody to po prostu skróty
do rdzennych funkcji Symfony2, które są dla dostępne niezależnie od tego, czy używa
się klasy ``Controller``, czy nie. Dobrym sposobem na zapoznanie się z klasą
:class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller` jest zobaczenie
jak ona działa.
.

.. tip::

    W Symfonii rozszerzanie klasy podstawowej ``Controler`` jest opcjonalne - zawiera
    ona pomocne skróty ale nic obowiązkowego. Można również rozszerzyć
    :class:`Symfony\Component\DependencyInjection\ContainerAware`. Stanie się wówczas
    dostępny  obiekt *kontenera usługi* poprzez właściwość ``container``.

.. note::

    Można również zdefiniować własne
    :doc:`kontrolery jako usługi</cookbook/controller/service>`.

.. index::
   single: kontroler; typowe zadania

Typowe zadania kontrolera
-------------------------

Choć kontroler może praktycznie wykonywać prawie wszystko, większość kontrolerów
będzie wykonywać te same podstawowe zadania w kółko. Zadania takie jak jak przekierowania, forwardowanie,
przetwrzanie szablonów i udostępnianie rdzennych usług są w Symfony2 bardzo
łatwe w użyciu.

.. index::
   single: kontroler; przekierowania

Przekierowania
~~~~~~~~~~~~~~

Jeśli chce się przekierować użytkownika do innej strony, należy użyć metody ``redirect()``::

    public function indexAction()
    {
        return $this->redirect($this->generateUrl('homepage'));
    }

Metoda ``generateUrl`` jest helperem generującym ścieżkę URL dla danej trasy.
Więcej informacji można znaleźć w rozdziale :doc:`routing`.

Domyślnie metoda ``redirect()`` realizuje przekierowanie 302 (tymczasowe, *ang. temporary*).
Aby wykonać przekierowanie 301 (trwałe, *ang. permanent*), należy podać drugi argument::

    public function indexAction()
    {
        return $this->redirect($this->generateUrl('homepage'), 301);
    }

.. tip::

    Metoda ``redirect()`` jest skrótem tworzącym obiekt ``Response``,
    którego zadaniem jest przekierowanie użytkownika. Jest to równoznaczne z::

      use Symfony\Component\HttpFoundation\RedirectResponse;
      
      return new RedirectResponse($this->generateUrl('homepage'));


.. index::
   single: kontroler; przekazywania

Przekazywanie (forwarding)
~~~~~~~~~~~~~~~~~~~~~~~~~~

Można również łatwo dokonać przekazania do innego wewnętrznego kontrolera przy
użyciu metody ``forward()``. Metoda ta sprawia, że zamiast przekierowywać przegladarkę
użytkownika wykonywane jest wewnętrzne podżądanie i wywoływany jest określony kontroler.
Metoda ``forward()`` zwraca obiekt ``Response``, który jest zwracany przez ten kontroler::

    public function indexAction($name)
    {
        $response = $this->forward('AcmeHelloBundle:Hello:fancy', array(
            'name'  => $name,
            'color' => 'green'
        ));

        // ... zmodyfikowanie odpowiedzi lub zwrócenie jej bezpośrednio

        return $response;
    }

Należy zwrócić uwagę, że metoda ``forward()`` wykorzystuje tą samą reprezentację
znakową kontrolera jaka jest używana w konfiguracji trasowania. W powyższym przykładzie
klasa docelowego kontrolera ``HelloController`` będzie się znajdować wewnątrz pakietu
``AcmeHelloBundle``. Tablica przekazana do metody staje sie argumentami wynikowego
kontrolera. Taki sam interfejs jest stosowany podczas osadzania kontrolerów w szablonach
(zobacz do rozdziału :ref:`templating-embedding-controller`). Metoda docelowego
kontrolera musi wyglądać następująco::

    public function fancyAction($name, $color)
    {
        // ... utworzenie i zwrócenie obiektu Response
    }

Kolejność argumentów ``fancyAction`` nie ma znaczenia, podobnie jak w przypadku
tworzenia kontrolera dla trasy. Symfony2 dopasowuje nazwy indeksów (np. ``name``)
do nazw argumentów metody (np. ``$name``). Jeśli zmieni się kolejność argumentów,
Symfony2 wciąż będzie w stanie przekazywać właściwą wartości do każdej zmiennej.

.. tip::

    Podobnie jak inne metody podstawowej klasy ``Controller``, metoda ``forward``
    jest skrótem do rdzennej funkcjonalności Symfony2. Przekazanie może być też
    dokonane bezpośrednio przez usługę ``http_kernel`` zawracajac obiekt
    ``Response``::

        $httpKernel = $this->container->get('http_kernel');
        $response = $httpKernel->forward('AcmeHelloBundle:Hello:fancy', array(
            'name'  => $name,
            'color' => 'green',
        ));

.. index::
   single: kontroler; renderowanie szablonów

.. _controller-rendering-templates:

Renderowanie szablonów
~~~~~~~~~~~~~~~~~~~~~~

Chociaż nie jest to wymagane, większość kontrolerów ostatecznie renderuje szablon,
który jest odpowiedzialny za generowanie kodu HTML (lub w innym formacie).
Metoda ``renderView()`` renderuje szablon i zwraca jego zawartość. Zawartość
szablonu może być użyta do utworzenia obiektu ``Response``::

    use Symfony\Component\HttpFoundation\Response;

    $content = $this->renderView(
        'AcmeHelloBundle:Hello:index.html.twig',
        array('name' => $name)
    );

    return new Response($content);

Może to być nawet wykonane w jednym kroku przy użyciu metody ``render``, która
zwraca obiekt ``Response`` zawierający zawartość szablonu::

    return $this->render(
        'AcmeHelloBundle:Hello:index.html.twig',
        array('name' => $name)
    );

W obu przypadkach, wyrenderowany zostanie szablon ``Resources/views/Hello/index.html.twig``
z pakietu ``AcmeHelloBundle``.

Silnik szablonów Symfony jest szczegółowo wyjaśniony w rozdziale
:doc:`templating`.

.. tip::

    Można nawet uniknąć wywoływania metody ``render`` stosujac adnotację ``@Template``.
    Zobacz do dokumentacji :doc:`FrameworkExtraBundle</bundles/SensioFrameworkExtraBundle/annotations/view>`
    w celu poznania szczegółów.
    

.. tip::

    Metoda ``renderView`` jest skrótem usługi ``templating``.
    Usługa ``templating`` może być również użyta bezpośrednio::

        $templating = $this->get('templating');
        $content = $templating->render(
            'AcmeHelloBundle:Hello:index.html.twig',
            array('name' => $name)
        );

.. note::

    Możliwe jest także renderowanie szablonów znajdujących się w głębszych podkatalogach,
    jednak należy uważać, aby nie wpaść w pułapkę nadmiernie rozbudowanej struktury
    katalogów:::

        $templating->render(
            'AcmeHelloBundle:Hello/Greetings:index.html.twig',
            array('name' => $name)
        );
        // renderowany jest index.html.twig znajdujacy się w Resources/views/Hello/Greetings.


.. index::
   single: kontroler; dostęp do usług

Dostęp do innych usług
~~~~~~~~~~~~~~~~~~~~~~

Rozszerzając klasę kontrolera podstaowego, można uzyskać dostęp do
każdej usługi Symfony2 poprzez metodę ``get()``. Poniżej znajduje się kilka
popularnych usług, jakie mogą być potrzebne::

    $request = $this->getRequest();

    $templating = $this->get('templating');

    $router = $this->get('router');

    $mailer = $this->get('mailer');

Istnieje wiele dostępnych usług i zachęca się do tworzenia własnych.
Aby wyświetlić listę wszstkich dostępnych usług, nalezy użyć polecenia konsoli ``container:debug``:

.. code-block:: bash

    $ php app/console container:debug

Aby dowiedzieć się więcej, zobacz rozdział :doc:`service_container`.

.. index::
   single: kontroler; zarządzanie stronami błędów
   single: kontroler; strona 404

Zarządzanie stronami błędów i strona 404
----------------------------------------

Gdy zasób nie może być znaleziony, to protokół HTTP zwraca odpowiedź 404. Aby to
obsłużyć trzeba zrzucić specjalny wyjątek. Jeśli rozszerza się klasę kontrolera
podstawiwego, można postąpić następująco::

    public function indexAction()
    {
        $product = // pobieramy obiekt z bazy danych
        if (!$product) {
            throw $this->createNotFoundException('Produkt nie istnieje');
        }

        return $this->render(...);
    }

Metoda ``createNotFoundException()`` tworzy specjalny obiet ``NotFoundHttpException``,
który w efekcie końcowym wyzwala odpowiedź HTTP z kodem statusu 404.

Oczywiście w kontrolerze można zrzucić dowolną klasę ``Exception`` - Symfony2 będzie
wówczas automatycznie zwracać kod odpowiedzi HTTP 500, który interpretowany jest
jako wewnętrzny, niezidentyfikowany błąd serwera.

.. code-block:: php

    throw new \Exception('Coś poszło źle!');

W każdym przypadku użytkownikowi końcowemu jest wyświetlana wystylizowana strona
błędu a programiście strona pełnego raportu z debugowania (gdy strona jest wyswietlana
w trybie debugowania). Obie te strony błędu mogą być dostosowane do indywidualnych
potrzeb. Więcej szczegółów można znaleźć w artykule
":doc:`/cookbook/controller/error_pages`".

.. index::
   pair: kontroler; sesja

Zarządzanie sesją
-----------------

Symfony2 zapewnia świetny obiekt sesji, który można użyć do przechowywania informacji
o użytkowniku między poszczególnymio żądaniami (zarówno prawdziwej osoby używającej
przeglądarki, jak i użytkownika w postacji serwisu web). Domyślnie Symfony2 zapamiętuje
atrybuty w pliku cookie, używając natywnych sesji PHP.

Przechowywanie i pobieranie informacji z sesji może być łatwo osiągnięte z dowolnego
kontrolera::

    $session = $this->getRequest()->getSession();

    // zapisanie atrybutu do odczytania w kolejnym żądaniu
    $session->set('foo', 'bar');

    // w innym kontrolerze i innym żądaniu
    $foo = $session->get('foo');

    // użycie domyślnej wartości, jeśli nie istnieje klucz
    $filters = $session->get('filters', array());

Atrybuty te pozostają przypisane użytkownikowi przez pozostałą część sesji.

.. index::
   single sesja; wiadomości fleszowe

Komunikaty fleszowe
~~~~~~~~~~~~~~~~~~~

W sesji uzytkownika można również przechowywać małe komunikaty dla dokładnie
jednego dodatkowego żądania. Jest to przydatne w przetwarzaniu formularzy:
gdy chce się przekierować stronę i mieć specjalny komunikat wyświetlający
następne żądanie. Tego typu komunikaty nazywane są "fleszowymi".

Na przykład, wyobraźmy sobie, że przetwarzane jest zgłoszenie formularza::

    public function updateAction()
    {
        $form = $this->createForm(...);

        $form->handleRequest($this->getRequest());
        
        if ($form->isValid()) {
            // obsługa formularza

            $this->get('session')->getFlashBag()->add('notice', 'Zmiany zostały zapisane!');

            return $this->redirect($this->generateUrl(...));
        }

        return $this->render(...);
    }

Po obsłużeniu żądania, kontroler ustawia komunikat fleszowy ``notice``, a następnie
wykonuje przekierowanie. Nazwa (``notice``) nie ma znaczenia - używa sie ją tylko
do zidentyfikowania typu komunikatu.

W szablonie następnej akcji poniższy kod jest użyty do wyrenderowania
komunikatu ``notice``:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        {% if app.session.started %}
            {% for flashMessage in app.session.flashbag.get('notice') %}
                <div class="flash-notice">
                    {{ flashMessage }}
                </div>
            {% endfor %}
        {% endif %}

    .. code-block:: html+php
       :linenos:

        <?php if ($view['session']->isStarted()): ?>
            <?php foreach ($view['session']->getFlashBag()->get('notice') as $message): ?>
                <div class="flash-notice">
                    <?php echo "<div class='flash-error'>$message</div>" ?>
                </div>
            <?php endforeach; ?>
        <?php endif; ?>

Zgodnie z założeniem, komunikaty fleszowe sa przeznaczone do użycia dokładnie
przy jednym żądaniu (są one wyświetlane natychmiast). Zostały zaprojektowane tak,
aby stosować przekierowania tak, jak zrobiliśmy to w tym przykładzie.

.. index::
   single: kontroler; obiekt Response

Obiekt Response
---------------

Jedyny wymóg dla kontrolera to zwrócić obiekt ``Response``. Klasa
:class:`Symfony\\Component\\HttpFoundation\\Response` to abstrakcja PHP dla
odpowiedzi HTTP - tekstowa wiadomość zawierająca nagłówki HTTP i treść, która
jest zwracana klientowi::

    use Symfony\Component\HttpFoundation\Response;

    // utworzenie prostego obiektu Response z kodem statusu 200 (domyślnie)
    $response = new Response('Hello '.$name, 200);

    // utworzenie odpowiedzi JSON ze kodem statusu 200
    $response = new Response(json_encode(array('name' => $name)));
    $response->headers->set('Content-Type', 'application/json');

.. tip::

    Właściwość ``headers`` to obiekt :class:`Symfony\\Component\\HttpFoundation\\HeaderBag`
    zawierający kilka użytecznych metod służących do odczytywania i modyfikowania
    nagłówków ``Response``. Nazwy nagłówków są znormalizowane, dzięki czemu
    ``Content-Type`` jest tym samym, co ``content-type``, czy nawet ``content_type``.

.. tip::

    Istnieją również specjalne klasy ułatwiające wykonanie kodu dla niektórych rodzajów odpowiedzi:
    
    
    - dla JSON jest to klasa :class:`Symfony\\Component\\HttpFoundation\\JsonResponse`.
      Zobacz :ref:`component-http-foundation-json-response`.
    - dla plików jest to klasa :class:`Symfony\\Component\\HttpFoundation\\BinaryFileResponse`.
      Zobacz :ref:`component-http-foundation-serving-files`.

.. index::
   single: kontroler; obiekt Request

Obiekt Request
--------------

Rozszerzając podstawową klasę ``Controller``, kontroler uzyskuje również dostęp
do obiektu``Request``::

    $request = $this->getRequest();

    $request->isXmlHttpRequest(); // żądanie Ajax?

    $request->getPreferredLanguage(array('en', 'fr'));

    $request->query->get('page'); // pobieramy parametr $_GET

    $request->request->get('page'); // pobieramy parametr $_POST

Podobnie jak w przypadku obiektu ``Response``, nagłówki żądania są przechowywane w
obiekcie ``HeaderBag`` i są równie łatwo dostępne.

Wnioski końcowe
---------------

Za każdym razem, kiedy tworzy sie stronę, musi się napisać kod, który zaiwera logikę
tej strony. W Symfony nazywa się ten kod kontrolerem i jest to funkcja PHP, która
może robić wszystko co jest potrzebne, aby w efekcie końcowym został zwrócony
obiekt ``Response``, który zostaje wysłany do użytkownika.

Aby ułatwić sobie życie, możesz rozszerzyć podstawową klasę ``Controller``,
która zawiera skrótowe metody wielu typowych zadań kontrolera. Na przykład,
jeśli nie chce się umieszczać kodu HTML w swoim kontrolerze, można użyć metody
``render()``, aby wyrenderować zawartość szablonu.

W kolejnych rozdziałach zobaczysz jak kontroler może być wykorzystany do umieszczania
i pobierania obiektów z bazy danych, przetwarzania formularzy, wykorzystywania pamieci
podręcznej i wiele więcej.

Dalsza lektura
--------------

* :doc:`/cookbook/controller/error_pages`
* :doc:`/cookbook/controller/service`