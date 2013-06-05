.. highlight:: php
   :linenothreshold: 2

.. index::
   single: komponenty wewnętrzne

Elementy wewnętrze
==================

Dotychczasowe sekcje miały na celu wytłumaczenie tego, jak działa Symfony2
i jak się go rozszerza. Ten rozdział opisuje wewnętrzne elementy Symfony2.

.. note::

    Rozdział ten należy czytać tylko gdy chce sie zrozumieć, jak Symfony2 działa
    w tle lub jeśli chce się rozszerzać ten system.

Wprowadzenie
------------

Kod Symfony2 składa się z kilku niezależnych warstw. Każda warstwa jest zbudowana
na poprzedniej.

.. tip::

    Automatyczne ładowanie nie jest zarządzane bezpośrednio przez framework.
    Jest to realizowane za pomocą autoloadera Composer (``vendor/autoload.php``),
    który jest dołączony w pliku ``app/autoload.php``.

Komponent ``HttpFoundation``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Najgłębszą warstwą jest komponent :namespace:`Symfony\\Component\\HttpFoundation`.
HttpFoundation dostarcza główne obiekty niezbędne do uporania się z HTTP. Jest to
obiektowo zorientowana abstrakcja pewnych natywnych funkcji PHP i zmiennych:

* Klasa :class:`Symfony\\Component\\HttpFoundation\\Request` uabstrakcyjnia główne
  zmienne globalne PHP, takie jak ``$_GET``, ``$_POST``, ``$_COOKIE``,
  ``$_FILES`` i ``$_SERVER``;

* Klasa :class:`Symfony\\Component\\HttpFoundation\\Response` uabstrakcyjnia niektóre
  funkcje PHP, takie jak ``header()``, ``setcookie()`` i ``echo``;

* Klasa :class:`Symfony\\Component\\HttpFoundation\\Session` i interfejs
  :class:`Symfony\\Component\\HttpFoundation\\SessionStorage\\SessionStorageInterface`
  uabstrakcyjniają funkcje zarządzania sesjami ``session_*()``.

Komponent ``HttpKernel``
~~~~~~~~~~~~~~~~~~~~~~~~

Nad HttpFoundation znajduje się komponent :namespace:`Symfony\\Component\\HttpKernel`.
HttpKernel obsługuje dynamiczną część HTTP. Jest cienką otoczką klas Request i Response
w celu ujednolicenia sposobu w jaki obsługiwane są żądania. Zapewnia również punkty
rozszerzeń i narzędzia, które czynią go doskonałym punktem wyjścia do stworzenia
frameworka aplikacji internetowych bez dużego narzutu.

Ewentualnie tu też jest dodawana konfigurowalność i rozszerzalność, dzięki
`komponentowi wstrzykiwania zależności Symfony2`_ i potężnemu systemowi wtyczek (pakietów).

.. seealso::

    Czytaj więcej o :doc:`wstrzykiwaniu zależności</book/service_container>` i
    :doc:`pakietach</cookbook/bundles/best_practices>`.

Pakiet ``FrameworkBundle``
~~~~~~~~~~~~~~~~~~~~~~~~~~

Pakiet :namespace:`Symfony\\Bundle\\FrameworkBundle` jest pakietem, który łączy
razem główne komponenty i biblioteki w celu zrealizowania lekkiego i szybkiego
frameworka MVC. Dostarczany jest z sensowną domyślną konfiguracją, łagodzącą
ścieżkę uczenia się.

.. index::
   single: elementy wewnętrzne; kernel
   single: kernel

.. _book-internals-kernel:

Kernel
------

Klasa :class:`Symfony\\Component\\HttpKernel\\HttpKernel` jest centralną klasą
Symfony2 i jest odpowiedzialna za obsługę żądań . Jej głównym celem jest "konwersja"
obiektu :class:`Symfony\\Component\\HttpFoundation\\Request` do obiektu
:class:`Symfony\\Component\\HttpFoundation\\Response`.

Każdy kernel Symfony2 implementuje
:class:`Symfony\\Component\\HttpKernel\\HttpKernelInterface`::

    function handle(Request $request, $type = self::MASTER_REQUEST, $catch = true)

.. index::
   single: elementy wewnętrzne; interfejs rozpoznawania kontrolera

Kontrolery
~~~~~~~~~~

Aby przekształcić żądanie w odpowiedź, kernel wykorzystuje "kontrtoler".
Kontrolerem może być każde prawidłowe wywołanie kodu PHP.

Kernel zleca wybór tego co kontroler powinien wykonać do implementacji
:class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface`::

    public function getController(Request $request);

    public function getArguments(Request $request, $controller);

Metoda
:method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getController`
zwraca kontroler (wywoływalny kod PHP) związany z danym żądaniem.
Domyślna implementacja interfejsu rozpoznawania kontrolera (resolwer)
(:class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolver`)
wyszukuje atrybut ``_controller``obiektu Request , który reprezentuje nazwę kontrolera
(łańcuch "class::method", jak na przykład ``Bundle\BlogBundle\PostController:indexAction``).

.. tip::

    Domyślna implementacja wykorzystuje
    :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\RouterListener`
    do zdefiniowania atrybutu ``_controller`` obiektu Request (zobacza :ref:`kernel-core-request`).

Metoda
:method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getArguments`
Zwraca tablicę argumentów w celu przekazania jej do wywołania kontrolera. Domyślna
implementacja automatycznie rozwiązuje argumenty metody, opierając się na atrybutach
obiektu Request.

.. sidebar:: Dopasowywanie argumentów metody kontrolera do atrybutów obiektu Request

   Dla każdego argumentu metody, Symfony2 próbuje pobrać wartość atrybutu obiektu
   Request o tej samej nazwie. Jeśli nie jest to określone, zostaje użyta domyślna
   wartość argumentum, jeśli jest określona::

        // Symfony2 will look for an 'id' attribute (mandatory)
        // and an 'admin' one (optional)
        public function showAction($id, $admin = true)
        {
            // ...
        }

.. index::
   single: elementy wewnętrzne; obsługa żądania

Obsługa żądań
~~~~~~~~~~~~~

Metoda :method:`Symfony\\Component\\HttpKernel\\HttpKernel::handle` pobiera
``Request`` i zawsze zwraca ``Response``. Aby przekształcić ``Request``,  metoda
``handle()`` wykorzystuje resolwer i uporządkowany łańcuch powiadamiania o zdarzeniach
(zobacz do następnego rozdziału w celu uzyskania więcej informacji o poszczególnych
zdarzeniach):

#. Zanim cokolwiek zostanie zrobione, zgłaszane jest zdarzenie ``kernel.request``.
   Jeśli jeden z odbiorników (*ang. listeners*) zwraca ``Response``, to interpreter
   przechodzi bezpośrednio do kroku ósmego;

#. Wywoływany jest resolwer w celu określenia kontrolera jaki ma być wykonany;

#. Odbiorniki zdarzenia ``kernel.controller`` mogą teraz manipulować wywoływalnym,
   kontrolerem w sposób jaki się che (zmieniać go, opakowywać go itd.);

#. Kernel sprawdza, czy kontroler jest rzeczywiście prawidłowym wywołaniem PHP;

#. Wywoływany jest resolwer w celu ustalenia argumentów do przekazania do kontrolera;

#. Kernel wywołuje kontroler;

#. Jeśli kontroler nie zwraca ``Response``, odbiorniki zdarzenia ``kernel.view``
   mogą konwertować kontroler zwracając wartość do ``Response``;

#. Odbiorniki zdarzenia ``kernel.response`` mogą manipulować ``Response``
   (zawartość i nagłówki);

#. Zwracana jest odpowiedź HTTP.

Jeśli podczas przetwarzania zostanie zgłoszony wyjątek, wyzwalane jest zdarzenie
``kernel.exception`` i odbiorniki otrzymują możliwość konwersji wyjątku na odpowiedź.
Jeśli wszystko działa poprawnie, to wyzwalane jest zdarzenie ``kernel.response``,
jeśli nie, to ponownie zgłaszany jest wyjątek.

Jeśli nie chce się aby wyjątek obsługiwany (na przykład dla osadzonych odpowiedzi),
trzeba wyłączyć zdarzenie ``kernel.exception`` przekazując ``false`` jako trzeci
argument metody ``handle()``.


.. index::
   single: elementy wewnętrzne; żądania wewnetrzne

Żądania wewnętrzne
~~~~~~~~~~~~~~~~~~

W każdej chwili podczas obsługi żądania ('głównego'), mogą być obsłużone pod-żądania.
Można przekazać typ żądania do metody ``handle()`` (jako drugi argument):

* ``HttpKernelInterface::MASTER_REQUEST``;
* ``HttpKernelInterface::SUB_REQUEST``.

Ten typ jest przekazywany do wszystkich zdarzeń i odbiorników, które mogą podjąć
odpowiednie działanie (niektóre procesy są wyzwalane tylko dla żądania głównego).

.. index::
   pair: kernel; zdarzenie

Zdarzenia
~~~~~~~~~

Każde zdarzenie zrzucane przez kernel jest podklasą
:class:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent`. Oznacza to, że każde
zdarzenie ma dostęp do tej samej podstawowej informacji:

* :method:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent::getRequestType` 
  - zwraca *typ* żądania  (``HttpKernelInterface::MASTER_REQUEST`` lub 
  ``HttpKernelInterface::SUB_REQUEST``);

* :method:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent::getKernel` 
  - zwraca kernel obsługujący żądanie;

* :method:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent::getRequest` 
  - zwraca bieżacy obslugiwany ``Request``.

Zdarzenie ``getRequestType()``
..............................

Metoda ``getRequestType()`` pozwala detektorom nasłuchującym rozpoznawać rodzaj żądania.
Na przykład, jeśli detektor musi być aktywny dla żądań głównych, to trzeba dodać następujący
kod na początku metody detektora::

    use Symfony\Component\HttpKernel\HttpKernelInterface;

    if (HttpKernelInterface::MASTER_REQUEST !== $event->getRequestType()) {
        // return immediately
        return;
    }

.. tip::

    Jeśli nie jesteś jeszcze zaznajomiony z Event Dispatcher dla Symfony2, przeczytaj najpierw
    :doc:`dokumentację komponentu Event Dispatcher </components/event_dispatcher/introduction>`.

.. index::
   single: zdarzenie; kernel.request

.. _kernel-core-request:

Zdarzenie ``kernel.request``
............................

*Klasa zdarzenia*: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseEvent`

Celem tego zdarzenia jest albo natychmiastowe bezpośrednie zwrócenie obiektu ``Response`` albo
ustawienie zmiennych tak, aby kontroler mógł być wywoływany po tym zdarzeniu. Dowolny detektor może
zwrócić obiekt ``Response`` poprzez metodę ``setResponse()`` wywoływaną na zdarzeniu.
W tym przypadku, wszystkie inne detektory nie zostaną wywołane.

To zdarzenie jest używane przez pakiet ``FrameworkBundle`` do wypełniania atrybutu ``_controller``
obiektu ``Request`` za pośrednictwem obiektu
:class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\RouterListener`. RequestListener używa
obiektu :class:`Symfony\\Component\\Routing\\RouterInterface` do dopasowywania ``Request``
i określania nazwy kontrolera (przechowywanego w atrybucie ``_controller`` obiektu ``Request``).

.. seealso::

    Więcej informacji można znaleźć w rozdziale ":ref:`Zdarzenie kernel.request <component-http-kernel-kernel-request>`".


.. index::
   single: zdarzenie; kernel.controller

Zdarzenie ``kernel.controller``
...............................

*Klasa zdarzenia*: :class:`Symfony\\Component\\HttpKernel\\Event\\FilterControllerEvent`

Zdarzenie to nie jest używane przez pakiet ``FrameworkBundle``, ale może być punktem wyjścia do
modyfikacji kontrolera, który może być wykonany::

    use Symfony\Component\HttpKernel\Event\FilterControllerEvent;

    public function onKernelController(FilterControllerEvent $event)
    {
        $controller = $event->getController();
        // ...

        // the controller can be changed to any PHP callable
        $event->setController($controller);
    }

.. seealso::

    Więcej informacji można znaleźć w rozdziale ":ref:`Zdarzenie kernel.controller <component-http-kernel-kernel-controller>`".



.. index::
   single: zdarzenie; kernel.view

Zdarzenie ``kernel.view``
.........................

*Klasa zdarzenia*: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForControllerResultEvent`

Zdarzenie to nie jest używane przez ``FrameworkBundle``, ale może być użyte do implementacji
podsystemu widoków. Zdarzenie to jest wywoływane tylko gdy kontroler nie zwraca obiektu ``Response``. Celem zdarzenia jest umożliwienie zwrócenie wartości, która zostanie przekształcona w obiekt ``Response``.

Wartość zwracana przez kontroler jest dostępna przez metodę ``getControllerResult``::

    use Symfony\Component\HttpKernel\Event\GetResponseForControllerResultEvent;
    use Symfony\Component\HttpFoundation\Response;

    public function onKernelView(GetResponseForControllerResultEvent $event)
    {
        $val = $event->getControllerResult();
        $response = new Response();

        // ... some how customize the Response from the return value

        $event->setResponse($response);
    }

.. seealso::

    Więcej informacji można znależć w rodziale ":ref:`Zdarzenie kernel.view <component-http-kernel-kernel-view>`".


.. index::
   single: zdarzenie; kernel.response

Zdarzenie ``kernel.response``
.............................

*Klasa zdarzenia*: :class:`Symfony\\Component\\HttpKernel\\Event\\FilterResponseEvent`

Celem zdarzenia jest umożliwienie innym systemom modyfikację lub wymienienie obiektu ``Response``
po jego utworzeniu::

    public function onKernelResponse(FilterResponseEvent $event)
    {
        $response = $event->getResponse();

        // ... modify the response object
    }

Pakiet ``FrameworkBundle`` rejestruje kilka detektorów nasłuchujących:

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`:
  zbiera dane dla bieżącego żądania;

* :class:`Symfony\\Bundle\\WebProfilerBundle\\EventListener\\WebDebugToolbarListener`:
  wstrzykuje Web Debug Toolbar;

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\ResponseListener`: ustala
``Content-Type`` odpowiedzi w oparciu o format żądania;

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\EsiListener`: dodaje nagłówek HTTP
``Surrogate-Control`` gdy odpowiedź musi być parsowana dla znaczników ESI.

.. seealso::

    Więcej informacji można znaleźć w rozdziale ":ref:`Zdarzenie kernel.response <component-http-kernel-kernel-response>`".


.. index::
   single: zdarzenie; kernel.terminate

Zdarzenie ``kernel.terminate``
..............................

Celem zdarzenia jest wykonanie "cięższych" zadań po tym, jak odpowiedź już została
zaserwowana klientowi.

.. seealso::

    Więcej informacji można znaleźć w rozdziale ":ref:`Zadanie kernel.terminate <component-http-kernel-kernel-terminate>`.


.. index::
   single: zdarzenie; kernel.exception

.. _kernel-kernel.exception:

Zdarzenie ``kernel.exception``
..............................

*Klasa zdarzenia*: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForExceptionEvent`

Pakiet ``FrameworkBundle`` rejestruje obiekt
:class:`Symfony\\Component\\HttpKernel\\EventListener\\ExceptionListener`, który przekazuje obiekt
``Request`` do danego kontrolera (wartość parametru ``exception_listener.controller`` - musi być
w notacji ``class::method``).

Detektor nasłuchujący dla tego zdarzenia może tworzyć i ustawiać obiekt ``Response``, tworząc
i ustawiając nowy obiekt ``Exception`` lub nie robić nic::

    use Symfony\Component\HttpKernel\Event\GetResponseForExceptionEvent;
    use Symfony\Component\HttpFoundation\Response;

    public function onKernelException(GetResponseForExceptionEvent $event)
    {
        $exception = $event->getException();
        $response = new Response();
        // ustawienie obiektu Response w oparciu o wychwycony wyjętek
        $event->setResponse($response);

        // mozna alternatywnie ustawić nowy wyjątek
        // $exception = new \Exception('Some special exception');
        // $event->setException($exception);
    }

.. note::

    Ponieważ Symfony zapewnia, że kod statusu odpowiedzi jest ustawiony najbardziej odpowiednio
    w zależności od wyjątku, to ustawiania statusu na odpowiedzi nie będzie działać. Jeśli chcesz
    zastąpić kod statusu (co jednak musi mieć bardzo ważny powód), ustaw nagłówek ``X-Status-Code``::

        return new Response('Error', 404 /* ignored */, array('X-Status-Code' => 200));

.. index::
   single: dyspozytor zdarzeń

Dyspozytor zdarzeń
------------------

Dyspozytor zdarzeń jest samodzielnym komponentem, który jest odpowiedzialny za wiele
rzeczy związanych z logiką i przetwarzaniem żądania w Symfony. Więcej informacji
na ten temat można znaleźć w
":doc:`Event Dispatcher Component Documentation</components/event_dispatcher/introduction>`".

.. seealso::

    Więcej informacji można znaleźć  w rozdziale ":ref:`kernel.exception event <component-http-kernel-kernel-exception>`".


.. index::
   single: profiler

.. _internals-profiler:

Profiler
--------

Symfony profiler, gdy jest włączony, zbiera przydatne informacje o każdym żądaniu
wykonanym do aplikacji i przechowuje je w celu późniejszych analiz. Używaj profilera
w środowisku programistycznym w celu w celu ułatwienia sobie debugowania kodu i
zwiększenia wydajności aplikacji. W środowisku produkcyjnym używa się profilera
do badania problemów po ich wystąpieniu.

Programista bardzo rzadko wykorzystuje profiler bezpośredni, ponieważ Symfony2
udostępnia wizualne narzędzia, takie jak Web Debug Toolbar i Web Profiler. Jeśli
używa się Symfony2 Standard Edition, to profiler, pasek narzędziowy debugowania
i profiler internetowy są już skonfigurowane w sensowny sposób.

.. note::

    Profiler zbiera informacje o wszystkich żądaniach (prostych żądaniach, przekierowaniach,
    wyjątkach, żądaniach Ajax, żądaniach ESI oraz dla wszystkich metod HTTP i wszystkich
    formatach). Oznacza to, że dla pojedynczego adresu URL można mieć kilka powiązanych danych
    (po jednym na każdą zewnętrzną parę żądanie-odpowiedź).

.. index::
   single: profiler; wizualizacja

Wizualizaja danych profilowania
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Stosowanie Web Debug Toolbar
............................

W środowisku programistycznym pasek narzędziowy debugowania dostępny jest na dole
wszystkich stron. Pokazuje on dobre podsumowanie danych profilowania, zapewniając
natychmiastowy dostęp do wielu przydatnych informacji, gdy coś nie działa jak powinno.

Jeśli dostarczone w Web Debug Toolbar podsumowanie nie jest wystarczające, to należy
kliknąć na odnośnik tokenu (łańcuch z 13 losowymi znakami) aby uzyskać dostęp do
programu Web Profiler.

.. note::

    Jeśli token nie jest interaktywny, oznacza to, trasy profilera nie są zarejestrowane
    (zobacz do poniższej informacji konfiguracyjnej).

Analizowanie danych profilowania z użyciem Web Profiler
.......................................................

Web Profiler jest wizualnym narzędziem do profilowania danych, które można wykorzystać
w programowaniu do debugowania swojego kodu i zwiększenia wydajności aplikacji.
Może być oni również wykorzystane do rozwiązywania problemów w środowisku produkcyjnym.
Narzędzie to prezentuje wszystkie informacje zebrane przez profiler w interfejsie internetowym.

.. index::
   single: profiler; usługa profilera

Dostęp do informacji profilowania
.................................

Nie musi się korzystać z domyślnej wizualizacji aby uzyskać dostęp do informacji
profilowania. Ale w jaki sposób można uzyskać informacje profilowania dla określonego
żądania po fakcie? Gdy profiler przechowuje dane o żądaniu, ale również związanych
z tokenem, to token ten jest dostępny w nagłówku HTTP ``X-Debug-Token`` tego żądania::

    $profile = $container->get('profiler')->loadProfileFromResponse($response);

    $profile = $container->get('profiler')->loadProfile($token);

.. tip::

    Gdy włączony jest profiler, ale wyłączony jest pasek narzędziowy debugowania
    lub gdy chce się uzyskać token dla żądania Ajax, to aby pobrać wartość nagłówka
    HTTP ``X-Debug-Token``, trzeba użyć narzędzia takiego jak Firebug

Użyj metodę :method:`Symfony\\Component\\HttpKernel\\Profiler\\Profiler::find` 
aby uzyskać tokeny na podstawie jakichś kryteriów::

    // get the latest 10 tokens
    $tokens = $container->get('profiler')->find('', '', 10);

    // get the latest 10 tokens for all URL containing /admin/
    $tokens = $container->get('profiler')->find('', '/admin/', 10);

    // get the latest 10 tokens for local requests
    $tokens = $container->get('profiler')->find('127.0.0.1', '', 10);

Jeśli chce się manipulować danymi profilowania na innych komputerach niż ten,
w którym zostały wygenerowane te informacje, to można użyć metod 
:method:`Symfony\\Component\\HttpKernel\\Profiler\\Profiler::export` i 
:method:`Symfony\\Component\\HttpKernel\\Profiler\\Profiler::import`::

    // on the production machine
    $profile = $container->get('profiler')->loadProfile($token);
    $data = $profiler->export($profile);

    // on the development machine
    $profiler->import($data);

.. index::
   single: profiler; wizualizacja

Konfiguracja
............

Symfony2 dostarczany jest z sensowną, domyślną konfiguracją profilera,
paska narzędziowego debugowania i profilera internetowego. Oto, na przykład, konfiguracja środowiska
programistycznego:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # załadowanie profilera
        framework:
            profiler: { only_exceptions: false }

        # udostępnienie profilera internetowego
        web_profiler:
            toolbar: true
            intercept_redirects: true

    .. code-block:: xml
       :linenos:

        <!-- xmlns:webprofiler="http://symfony.com/schema/dic/webprofiler" -->
        <!-- xsi:schemaLocation="http://symfony.com/schema/dic/webprofiler http://symfony.com/schema/dic/webprofiler/webprofiler-1.0.xsd"> -->

        <!-- załadowanie profiler -->
        <framework:config>
            <framework:profiler only-exceptions="false" />
        </framework:config>

        <!-- udostępnienie profilera internetowego -->
        <webprofiler:config
            toolbar="true"
            intercept-redirects="true"
            verbose="true"
        />

    .. code-block:: php
       :linenos:
        // załadowanie profilera
        $container->loadFromExtension('framework', array(
            'profiler' => array('only-exceptions' => false),
        ));

        // udostępnienie profilera internetowego
        $container->loadFromExtension('web_profiler', array(
            'toolbar'             => true,
            'intercept-redirects' => true,
            'verbose'             => true,
        ));

Gdy opcja ``only-exceptions`` jest ustawiona na ``true``, to Web Profiler będzie
zbierał dane tylko gdy w aplikacji zostanie zgłoszony wyjątek.

Gdy opcja ``intercept-redirects`` jest ustawiona na ``true``, Web Profiler
przechwytuje przekierowania i umożliwia oglądanie zebranych danych przed przekierowaniem.

Jeśli włączy się Web Profiler, to również trzeba zamontować trasy profilera:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        _profiler:
            resource: @WebProfilerBundle/Resources/config/routing/profiler.xml
            prefix:   /_profiler

    .. code-block:: xml
       :linenos:

        <import resource="@WebProfilerBundle/Resources/config/routing/profiler.xml" prefix="/_profiler" />

    .. code-block:: php
       :linenos:

        $collection->addCollection($loader->import("@WebProfilerBundle/Resources/config/routing/profiler.xml"), '/_profiler');

Ponieważ profiler dodaje trochę obciążenia, można go włączać w środowisku produkcyjnym
tylko w pewnych okolicznościach. Opcja ``only-exceptions`` ogranicza profilowanie
do 500 stron, ale co zrobić jeśli chce się pobierać informacje dla klienta z określonego
adresu IP lub ograniczonych części witryny? Można użyć odpowiednika żądania:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # enables the profiler only for request coming for the 192.168.0.0 network
        framework:
            profiler:
                matcher: { ip: 192.168.0.0/24 }

        # enables the profiler only for the /admin URLs
        framework:
            profiler:
                matcher: { path: "^/admin/" }

        # combine rules
        framework:
            profiler:
                matcher: { ip: 192.168.0.0/24, path: "^/admin/" }

        # use a custom matcher instance defined in the "custom_matcher" service
        framework:
            profiler:
                matcher: { service: custom_matcher }

    .. code-block:: xml
       :linenos:

        <!-- enables the profiler only for request coming for the 192.168.0.0 network -->
        <framework:config>
            <framework:profiler>
                <framework:matcher ip="192.168.0.0/24" />
            </framework:profiler>
        </framework:config>

        <!-- enables the profiler only for the /admin URLs -->
        <framework:config>
            <framework:profiler>
                <framework:matcher path="^/admin/" />
            </framework:profiler>
        </framework:config>

        <!-- combine rules -->
        <framework:config>
            <framework:profiler>
                <framework:matcher ip="192.168.0.0/24" path="^/admin/" />
            </framework:profiler>
        </framework:config>

        <!-- use a custom matcher instance defined in the "custom_matcher" service -->
        <framework:config>
            <framework:profiler>
                <framework:matcher service="custom_matcher" />
            </framework:profiler>
        </framework:config>

    .. code-block:: php
       :linenos:

        // enables the profiler only for request coming for the 192.168.0.0 network
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('ip' => '192.168.0.0/24'),
            ),
        ));

        // enables the profiler only for the /admin URLs
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('path' => '^/admin/'),
            ),
        ));

        // combine rules
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('ip' => '192.168.0.0/24', 'path' => '^/admin/'),
            ),
        ));

        # use a custom matcher instance defined in the "custom_matcher" service
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('service' => 'custom_matcher'),
            ),
        ));

Dalsza lektura
--------------

* :doc:`/cookbook/testing/profiling`
* :doc:`/cookbook/profiler/data_collector`
* :doc:`/cookbook/event_dispatcher/class_extension`
* :doc:`/cookbook/event_dispatcher/method_behavior`

.. _`komponentowi wstrzykiwania zależności Symfony2`: https://github.com/symfony/DependencyInjection

